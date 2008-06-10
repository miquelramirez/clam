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

#include "DownSampling.hxx"

namespace CLAM
{
	void DownSamplingConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetDownSamplingRate(0);
	}

	DownSampling::DownSampling():
	mDownSamplingRate(0)
	{
		DownSamplingConfig cfg;

		AttachChildren();
		Configure( cfg );
	}

	DownSampling::DownSampling( DownSamplingConfig &cfg ):
	mDownSamplingRate(0)
	{
		AttachChildren();
		Configure( cfg );
	}

	DownSampling::~DownSampling()
	{}

	bool DownSampling::ConcreteConfigure( const ProcessingConfig& cfg )
	{
		bool res = true;
		CopyAsConcreteConfig(mConfig, cfg);
		try{
			if( !mConfig.HasDownSamplingRate() ) return true;
			mDownSamplingRate = mConfig.GetDownSamplingRate();

			if( !ConfigureChildren() )
			{
				AddConfigErrorMessage( "Failed to configure children: ");
				res = false;
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

	bool DownSampling::ConcreteStart(void)
	{
		ProcessingComposite::ConcreteStart();

		return true;
	}

	bool DownSampling::ConcreteStop(void)
	{
		ProcessingComposite::ConcreteStop();

		return true;
	}

	void DownSampling::AttachChildren()
	{
		mTDFilterGenerator.SetParent(this);
		mTDFilter.SetParent(this);
	}

	bool DownSampling::ConfigureChildren()
	{
		DataArray A, B;

		A.Resize( 11 );
		A.SetSize( 11 );

		B.Resize( 11 );
		B.SetSize( 11 );

	/* Cutoff frequency 5000 */ 
/*		B[10] = 1;		A[10] =   0.0092341067;
		B[9]= 10;     	A[9] =    -0.1343381478;
		B[8] = 45;     	A[8] = 0.8925614801;
		B[7] = 120;     A[7] =  -3.5717077136;
		B[6] = 210;     A[6] = 9.5491733251 ;
		B[5] = 252;     A[5] = -17.8599406974;
		B[4] = 210;     A[4] = 23.7263412533;
		B[3] = 120;     A[3] = -22.1798128381;
		B[2] = 45;     	A[2] = 14.0254513805;
		B[1] = 10;     	A[1] = -5.4519604921;
		B[0] = 1;     	A[0] =  1.0;

*/
		/* Cutoff frequency 1500 */
/*		B[10] = 1;		A[10] = 0.7837776007;
		B[9]= 10;     	A[9] = -7.9378399334;
		B[8] = 45;     	A[8] = 36.2692306608;
		B[7] = 120;     A[7] = -98.4571949115;
		B[6] = 210;     A[6] = 175.8517737299  ;
		B[5] = 252;     A[5] = -215.9310970880;
		B[4] = 210;     A[4] = 184.6091852174;
		B[3] = 120;     A[3] = -108.5111053370;
		B[2] = 45;     	A[2] = 41.9674505919 ;
		B[1] = 10;     	A[1] = -9.6441805298;
		B[0] = 1;     	A[0] =  1.0;
*/
		/* Cutoff frequency 2000 */
/*		B[10] = 1;		A[10] = 0.7226563722;
		B[9]= 10;     	A[9] = -7.3116273336;
		B[8] = 45;     	A[8] = 33.4416384259;
		B[7] = 120;     A[7] = -91.0541741542;
		B[6] = 210;     A[6] = 163.4448773528  ;
		B[5] = 252;     A[5] = -202.1093907772;
		B[4] = 210;     A[4] = 174.3628657994;
		B[3] = 120;     A[3] = -103.6328778599;
		B[2] = 45;     	A[2] = 40.6130214770;
		B[1] = 10;     	A[1] = -9.4769892844;
		B[0] = 1;     	A[0] =  1.0;
*/
		/* Cutoff frequency 2500 */
/*		B[10] = 1;		A[10] = 0.6663120310;
		B[9]= 10;     	A[9] = -6.7170688026;
		B[8] = 45;     	A[8] = 30.6895217111;
		B[7] = 120;     A[7] = -83.6861929272;
		B[6] = 210;     A[6] = 150.8321024337  ;
		B[5] = 252;     A[5] = -187.7596142275;
		B[4] = 210;     A[4] = 163.4934351818;
		B[3] = 120;     A[3] = -98.3399747611;
		B[2] = 45;     	A[2] = 39.1078333273;
		B[1] = 10;     	A[1] = -9.2863538032;
		B[0] = 1;     	A[0] =  1.0;
*/
/* Cutoff frequency 3000 */
		B[10] = 1;		A[10] = 0.6143727357;
		B[9]= 10;     	A[9] = -6.1542833879;
		B[8] = 45;     	A[8] = 28.0290535204;
		B[7] = 120;     A[7] = -76.4293902538;
		B[6] = 210;     A[6] = 138.1834009801  ;
		B[5] = 252;     A[5] = -173.0994506938;
		B[4] = 210;     A[4] = 152.1665443806;
		B[3] = 120;     A[3] = -92.7035425542;
		B[2] = 45;     	A[2] = 37.4661143108;
		B[1] = 10;     	A[1] = -9.0728180564;
		B[0] = 1;     	A[0] =  1.0;

/* Cutoff frequency 3500 */
/*		B[10] = 1;		A[10] = 0.5664952296;
		B[9]= 10;     	A[9] = -5.6231140358;
		B[8] = 45;     	A[8] = 25.4737214600;
		B[7] = 120;     A[7] = -69.3515272860;
		B[6] = 210;     A[6] = 125.6560783800  ;
		B[5] = 252;     A[5] = -158.3375598500;
		B[4] = 210;     A[4] = 140.5466608100;
		B[3] = 120;     A[3] = -86.7968170780 ;
		B[2] = 45;     	A[2] = 35.7030128530;
		B[1] = 10;     	A[1] = -8.8369460160;
		B[0] = 1;     	A[0] =  1.0;
*/
/* Cutoff frequency 3750 */
/*		B[10] = 1;		A[10] = 0.5439796619;
		B[9]= 10;     	A[9] = -5.3692716356;
		B[8] = 45;     	A[8] = 24.2390011317;
		B[7] = 120;     A[7] = -65.8985179940;
		B[6] = 210;     A[6] = 119.4831603103  ;
		B[5] = 252;     A[5] = -150.9803459916;
		B[4] = 210;     A[4] = 134.6772064560;
		B[3] = 120;     A[3] = -83.7652911158 ;
		B[2] = 45;     	A[2] = 34.7809029799;
		B[1] = 10;     	A[1] = -8.7108150132;
		B[0] = 1;     	A[0] =  1.0;

*/
/* Cutoff frequency 4000 */
/*		B[10] = 1;		A[10] = 0.5223626848;
		B[9]= 10;     	A[9] = -5.1231711762;
		B[8] = 45;     	A[8] = 23.0344875030;
		B[7] = 120;     A[7] = -62.5117383930;
		B[6] = 210;     A[6] = 113.3929048267  ;
		B[5] = 252;     A[5] = -143.6701239476;
		B[4] = 210;     A[4] = 128.7941882829;
		B[3] = 120;     A[3] = -80.6940651537;
		B[2] = 45;     	A[2] = 33.8344912241;
		B[1] = 10;     	A[1] = -8.5793192916;
		B[0] = 1;     	A[0] =  1.0;
*/

		TDFilterGenConfig GenCfg;
		GenCfg.SetName( "myGenerator" );
		GenCfg.SetType( ETDFilterType::eARMA );
		GenCfg.SetSpectralRange( mDownSamplingRate / 2. );
		GenCfg.SetACoefficients( A );
		GenCfg.SetBCoefficients( B );

		if( !mTDFilterGenerator.Configure( GenCfg ) )
		{
			AddConfigErrorMessage("mTDFilterGenerator: ");
			AddConfigErrorMessage( mTDFilterGenerator.GetConfigErrorMessage() );
			return false;
		}
		
		return true;
	}

	inline void DownSampling::CheckTypes( const Audio &in, const Audio &out )
	{
		CLAM_BEGIN_CHECK

		if( in.GetSampleRate() < mDownSamplingRate )
		{
			std::stringstream s;

			s << "DownSampling::Do: Wrong down sampling rate\n"
			  << "	Expected a down sampling rate greater than: " << mDownSamplingRate << "	, used " << in.GetSampleRate();
			CLAM_ASSERT( 0, s.str().c_str() );			
		}

		if (!in.HasBuffer())
			CLAM_ASSERT(0,"DownSampling Do: Float attribute required for input Audio object.");

		CLAM_END_CHECK
	}

	void DownSampling::Filter( const Audio &in, Audio &out)
	{

		TDFilterKernel myKernel;

		TDFilterConfig FilterCfg;
		FilterCfg.SetName( "myFilter" );
		FilterCfg.SetAudioSize( in.GetSize() );

		mTDFilter.Stop();
		mTDFilter.Configure( FilterCfg );

		mTDFilter.Start();

		mTDFilterGenerator.Do( myKernel );
		mTDFilter.Do( in, myKernel, out );
	}

	bool DownSampling::Do( const Audio &in, Audio &out )
	{
		TData downsampling_factor;
		TData *inBuffer, *outBuffer, *endOutBuffer;
		Audio filtered;

		if( !AbleToExecute() ) return true;
	
		CheckTypes( in, out );

		if( in.GetSampleRate() == mDownSamplingRate )
		{
			out = in;
			return true;
		}

		filtered.SetSize( in.GetSize() );
		filtered.SetSampleRate( in.GetSampleRate());

		Filter( in, filtered );

	/*	AudioFileConfig fcfg;
		fcfg.SetName("audio_file_out");
		fcfg.SetFilename("C:\\filt.wav");
		fcfg.SetFiletype( EAudioFileType::eWave );
		fcfg.SetChannels( 1 );

		AudioFileOut fileOut( fcfg );

		fileOut.Start();
		fileOut.Do( filtered );*/

		downsampling_factor = in.GetSampleRate() / mDownSamplingRate ;
		TSize down = TSize( floor(downsampling_factor) );

		out.SetSize( TSize( floor( in.GetSize() / down ) ) );
		out.SetSampleRate(in.GetSampleRate()/down);

		inBuffer = filtered.GetBuffer().GetPtr();
		outBuffer = out.GetBuffer().GetPtr();
		endOutBuffer = outBuffer + out.GetSize();

		(*outBuffer++) = (*inBuffer);

		while( outBuffer != endOutBuffer )
			(*outBuffer++) = *(inBuffer+=down);

		return true;
	}

} //namespace
