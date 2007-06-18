/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "FormantExtractor.hxx"
#include <cmath>
#include <fstream>

namespace CLAM
{
	void FormantExtractorConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetNCandidates(1);
		SetWinSize(256);
		SetSamplingRate(44100);
	}

	FormantExtractor::FormantExtractor() :
		mNCandidates( 0 ), mNFrames( 0 ), mWinSize( 0 )
	{
		FormantExtractorConfig cfg;

		AttachChildren();
		Configure( cfg );
	}

	FormantExtractor::FormantExtractor( FormantExtractorConfig& cfg ) :
		mNCandidates( 0 ), mNFrames( 0 ), mWinSize( 0 )
	{
		AttachChildren();
		Configure( cfg );
	}

	FormantExtractor::~FormantExtractor()
	{}

	bool FormantExtractor::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		bool res = true;

		CopyAsConcreteConfig(mConfig,cfg);

		try{

			if( mConfig.HasNCandidates() )
			{
				if( mConfig.GetNCandidates() < 0 )
				throw( ErrProcessingObj( "Wrong (negative) number of required candidates in FormantExtractor configuration",
										  this ) );

				if( mConfig.HasNCandidates() )
					mNCandidates = mConfig.GetNCandidates();
				if( mConfig.HasHopSize() )
					mHopSize = mConfig.GetHopSize();
				if( mConfig.HasWinSize() )
					mWinSize = mConfig.GetWinSize();
				if( mConfig.HasSamplingRate() )
					mSamplingRate = mConfig.GetSamplingRate();
				if( mConfig.HasNFrames() )
					mNFrames = mConfig.GetNFrames();

				if( !ConfigureChildren() )
				{
					AddConfigErrorMessage("\nFailed to configure children: ");
					res = false;			
				}

				ConfigureData();
			}
		}
		catch( std::exception &e )
		{
			AddConfigErrorMessage("Failed to configure children: ");
			AddConfigErrorMessage( e.what() );
			return false;			
		}

		return res;
	}

	bool FormantExtractor::ConcreteStart(void)
	{
		ProcessingComposite::ConcreteStart();

		return true;
	}

	bool FormantExtractor::ConcreteStop()
	{
		ProcessingComposite::ConcreteStop();
		
		return true;
	}

	void FormantExtractor::AttachChildren(void)
	{
		mLPC.SetParent(this);
		mDownSampling.SetParent(this);
		mTDFilterGen.SetParent(this);
		mWinGenerator.SetParent(this);
		mAProduct.SetParent(this);
	}

	void FormantExtractor::ConfigureData(void)
	{
		mWindow.SetSize( mWinSize );
		mWinGenerator.Do( mWindow );
	}

	bool FormantExtractor::ConfigureChildren(void)
	{
		LPCConfig LPC_cfg;

		LPC_cfg.SetOrder( mNCandidates * 2 + 1 );

		if( !mLPC.Configure( LPC_cfg ) )
		{
			AddConfigErrorMessage("mLPC: ");
			AddConfigErrorMessage( mLPC.GetConfigErrorMessage() );
			return false;
		}

		DownSamplingConfig	downSampling_cfg;

		downSampling_cfg.SetDownSamplingRate( mConfig.GetFmax() * 2 );

		if( !mDownSampling.Configure( downSampling_cfg ) )
		{
			AddConfigErrorMessage("mDownSampling: ");
			AddConfigErrorMessage( mDownSampling.GetConfigErrorMessage() );
			return false;
		}

		
		WindowGeneratorConfig wCfg;


		wCfg.SetSize( mWinSize );
		wCfg.SetType( mConfig.GetWindowType() );

		if( !mWinGenerator.Configure( wCfg ) )
		{
			AddConfigErrorMessage("mWinGenerator: ");
			AddConfigErrorMessage( mWinGenerator.GetConfigErrorMessage() );
			return false;
		}


		return true;
	}

	inline void FormantExtractor::CheckTypes( const Audio& in/*, const Candidates& out*/ ) const
	{
		CLAM_BEGIN_CHECK

		if( !in.HasBuffer() )
		{
			CLAM_ASSERT( 0, "FormantExtractor::Do: Float attribute required for Audio in object. " );		
		}
		

	/*	if( out.GetNumColumns() != mNFrames )
		{
			std::stringstream s;

			s << "FormantExtractor::Do: Wrong columns size in matrix of formants\n"
			  << "	Expected: " << mNFrames << "	, used " << out.GetNumColumns();
			CLAM_ASSERT( 0, s.str().c_str() );
		}

		if( out.GetNumRows() != mNCandidates )
		{
			std::stringstream s;

			s << "FormantExtractor::Do: Wrong rows size in matrix of formants\n"
			  << "	Expected: " << mNCandidates << "	, used " << out.GetNumRows();
			CLAM_ASSERT( 0, s.str().c_str() );		
		}*/

		CLAM_END_CHECK
	}

	void FormantExtractor::ConfigureTDFilterGen( const DataArray& a )
	{
		TDFilterGenConfig cfg;


		cfg.SetType( ETDFilterType::eAutoregressive );
		cfg.SetSpectralRange( mDownSampling.GetDownSamplinRate() );
		cfg.SetSamplingRate( mSamplingRate );
		cfg.SetACoefficients( a );

		if( mTDFilterGen.GetExecState() == Running )	mTDFilterGen.Stop();

		mTDFilterGen.Configure( cfg ) ;	
	}

	bool FormantExtractor::Do( const Audio& in, Candidates &out )
	{	
		Audio downData, tempAudio;
		TSize downSize;
		TData downFactor;
		DataArray A, K;
		TData E;
		TDFilterKernel	F;
		int f, li, ui, nCand, ncoeff;
		int i,j;

		if( !AbleToExecute() ) return true;

		ncoeff = mNCandidates * 2 + 1;
		A.Resize( ncoeff );
		A.SetSize( ncoeff );

		K.Resize( ncoeff );
		K.SetSize( ncoeff );

		downFactor = floor(in.GetSampleRate() / mDownSampling.GetDownSamplinRate());
		downSize = floor( in.GetSize() / downFactor );
		downData.SetSize( downSize);

		mDownSampling.Do( in, downData );

		CheckTypes( downData );
	    
		mSamplingRate = downData.GetSampleRate();

		out.Allocate(mNCandidates, mNFrames );
	
/*		std::ofstream to, fr;

		fr.open("C:\\frames.txt");
		to.open("C:\\formants.txt");

		to << mNFrames << " "<< mNCandidates << " ";
		fr << mNFrames << " "<< mWinSize << " " << mHopSize<<" "<< downSize <<" ";
		fr << mSamplingRate << " " << mNCandidates << " ";*/


		for( f = 0; f < mNFrames; f++ )
		{
			li= f*mHopSize;
			ui=li+mWinSize-1;
			
			if (ui>downSize)
			{
				mNFrames=f-1;
				break;
			}
    
			downData.GetAudioChunk( li, ui, tempAudio ); 
			mAProduct.Do( tempAudio, mWindow, tempAudio );
//			TData *buff = tempAudio.GetBuffer().GetPtr();

/*			for(unsigned k=0; k<tempAudio.GetSize(); k++)
				fr << buff[k] << " ";
*/
			mLPC.Do( tempAudio, A, K, E );
			ConfigureTDFilterGen( A );
			mTDFilterGen.Do( F );

			out.GetFreqMatrix()( 0, f ) = F.GetFreqPoles()[0];
			out.GetMagMatrix()( 0, f ) = F.GetAmpPoles()[0];
			out.GetNCandPerFrame()[ f ] = F.GetNPoles();

			F.GetFreqPoles()[0] = 0;
			F.GetAmpPoles()[0] = 0;

			if( F.GetNPoles() > 0 )
			{

				for( nCand = 1; nCand < F.GetNPoles() ; nCand++  )
				{
					out.GetFreqMatrix()( nCand, f ) = F.GetFreqPoles()[nCand];
					out.GetMagMatrix()( nCand, f ) = F.GetAmpPoles()[nCand];
					F.GetFreqPoles()[nCand] = 0;
					F.GetAmpPoles()[nCand] = 0;
				}
				

				for ( i=0;i< F.GetNPoles()-1; i++)
				 for (j=F.GetNPoles()-1;j>i; j--)
					if ( out.GetFreqMatrix()(j-1,f) > out.GetFreqMatrix()(j,f) )
					{
						TData tmp=out.GetFreqMatrix()(j-1,f);
						tmp=out.GetFreqMatrix()(j,f);
						std::swap( out.GetFreqMatrix()(j-1,f), out.GetFreqMatrix()(j,f));
						std::swap( out.GetMagMatrix()(j-1,f), out.GetMagMatrix()(j,f));
					}
			}
		
/*			to << F.GetNPoles() << " ";
			for( unsigned c=0; c<F.GetNPoles(); c++ )
			{
				to << A[c] << " ";
			}
*/
		/*	for( unsigned c=0; c<F.GetNPoles(); c++ )
			{
				to << out.GetFreqMatrix()( c, f ) << " ";
				std::cout << out.GetFreqMatrix()( c, f ) << " ";
			}*/
	/*		std::cout << std::endl;
			for( unsigned c=0; c<F.GetNPoles(); c++ )
			{
				to << DB(out.GetMagMatrix()( c, f )) << " ";
				std::cout << DB(out.GetMagMatrix()( c, f )) << " ";
			}*/
			
		//	std::cout << std::endl << std::endl;
			F.SetNPoles(0);

		}

		return true;
	}

} // namespace
