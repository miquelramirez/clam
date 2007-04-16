/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "DataTypes.hxx"
#include "CLAM_Math.hxx"
#include "Normalization.hxx"
#include "Stats.hxx"



namespace CLAM
{


	void NormalizationConfig::DefaultInit()
	{
		AddType();
		AddFrameSize();
		UpdateData();
		SetType(1);
		SetFrameSize(2205);//0.05s at 44.1k

	}


	Normalization::Normalization()
		: mIsSilenceCtrl( "Silence", this )
	{
		Configure(NormalizationConfig());
	}

	Normalization::Normalization(NormalizationConfig& c)
		: mIsSilenceCtrl( "Silence", this )
	{
		Configure(c);
	}

	Normalization::~Normalization() {}


	bool Normalization::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig,c);

		mType=mConfig.GetType();
		mFrameSize=mConfig.GetFrameSize();
	
		return true;
	}

	bool Normalization::Do(void) 
	{
		return false;
	}
	
	bool Normalization::Do(Audio &in)
	{
		TData scaleFactor = 0;

		//Type #1: normalizes according to the max energy 
		//Type #2: normalizes according to the average energy
		//Type #3: normalizes according to the threshold under which lies percent% of
		//the energy values that are not silence

		if ( mType == 1 ) 
			scaleFactor = ComputeScaleFactorFromMaxEnergy( in.GetBuffer() );
		else if ( mType == 2 )
			scaleFactor = ComputeScaleFactorFromAvgEnergy( in.GetBuffer() );
		else if ( mType == 3 )
			scaleFactor = ComputeScaleFactorFromDominantEnergy( in.GetBuffer() );

		const TData invScaleFactor = 1.0 / scaleFactor;
		DataArray& inBufferSamples = in.GetBuffer();

		for (int n=0; n<in.GetSize(); n++)
			inBufferSamples[n]*=invScaleFactor;
		
		return true;
	}


	bool Normalization::Do(const Audio& unnorm, Audio& norm)
	{
		CLAM_ASSERT( unnorm.GetSize() == norm.GetSize(),
			     "Normalization::Do() : input and output audio sizes must match" );

		TData scaleFactor = 0;

		//Type #1: normalizes according to the max energy 
		//Type #2: normalizes according to the average energy
		//Type #3: normalizes according to the threshold under which lies percent% of
		//the energy values that are not silence

		if ( mType == 1 ) 
			scaleFactor = ComputeScaleFactorFromMaxEnergy( unnorm.GetBuffer() );
		else if ( mType == 2 )
			scaleFactor = ComputeScaleFactorFromAvgEnergy( unnorm.GetBuffer() );
		else if ( mType == 3 )
			scaleFactor = ComputeScaleFactorFromDominantEnergy( unnorm.GetBuffer() );

		const TData invScaleFactor = 1.0 / scaleFactor;
		DataArray& outBufferSamples = norm.GetBuffer();
		const DataArray& inBufferSamples = unnorm.GetBuffer();

		for (int n=0; n<norm.GetSize(); n++)
			outBufferSamples[n]=inBufferSamples[n]*invScaleFactor;
		
		return true;
	}


	TData Normalization::ComputeScaleFactorFromMaxEnergy( DataArray& inAudio )
	{
		TIndex    p = 0;
		const     TIndex end = inAudio.Size() - mFrameSize;
		DataArray chunk;
		TData     maxEnergy = 0.0;
		const     TData quantizationThreshold = 0.3 * TData( mFrameSize ) / TData( 4410 );

		do
		{
			chunk.SetPtr( inAudio.GetPtr()+p, mFrameSize );
			/* unused: TSize size = chunk.GetSize(); */
			DataArray moments(4);
			moments.SetSize(4);
			Stats myStats(&chunk);
			myStats.GetMoments(moments, FifthOrder);

			TData currentChunkEnergy = myStats.GetEnergy();

			//remove silence
			if ( currentChunkEnergy > quantizationThreshold ) //seems to be just above noise due to 8 bits quantization
			{
				if(maxEnergy<currentChunkEnergy) maxEnergy=currentChunkEnergy;
			}

			p += mFrameSize;
			
		} while (p <= end );

		// Enjoy the Silence...
		if ( maxEnergy <= 1e-7 )
		{
			mIsSilenceCtrl.SendControl( true );
			return 1.0;
		}

		mIsSilenceCtrl.SendControl(false);
			
		return CLAM_sqrt( maxEnergy / TData(mFrameSize ) );
		

	}
	
	TData Normalization::ComputeScaleFactorFromAvgEnergy( DataArray& inAudio )
	{

		TIndex    p = 0;
		const     TIndex end = inAudio.Size() - mFrameSize;
		DataArray chunk;
		TData     avgEnergy = 0.0;
		const     TData quantizationThreshold = 0.3 * TData( mFrameSize ) / TData( 4410 );

		do
		{
			chunk.SetPtr( inAudio.GetPtr()+p, mFrameSize );
			DataArray moments(4);
			moments.SetSize(4);
			Stats myStats(&chunk);
			myStats.GetMoments(moments, FifthOrder);

			TData currentChunkEnergy = myStats.GetEnergy();

			//remove silence
			if ( currentChunkEnergy > quantizationThreshold ) //seems to be just above noise due to 8 bits quantization
			{
				avgEnergy += currentChunkEnergy;
			}

			p += mFrameSize;
			
		} while (p <= end );

		avgEnergy /= TData( inAudio.Size() );

		// Enjoy the Silence...
		if ( avgEnergy <= 1e-7 )
		{
			mIsSilenceCtrl.SendControl( true );
			return 1.0;
		}

		mIsSilenceCtrl.SendControl(false);

		return CLAM_sqrt( avgEnergy );

	}

	TData Normalization::ComputeScaleFactorFromDominantEnergy( DataArray& inAudio )
	{
		TIndex    p = 0;
		const     TIndex end = inAudio.Size() - mFrameSize;
		DataArray chunk;
		DataArray chunksEnergies;
		const     TData quantizationThreshold = 0.3 * TData( mFrameSize ) / TData( 4410 );

		do
		{
			chunk.SetPtr( inAudio.GetPtr()+p, mFrameSize );
			DataArray moments(4);
			moments.SetSize(4);
			Stats myStats(&chunk);
			myStats.GetMoments(moments, FifthOrder);

			TData currentChunkEnergy = myStats.GetEnergy();

			//remove silence
			if ( currentChunkEnergy > quantizationThreshold ) //seems to be just above noise due to 8 bits quantization
			{
				chunksEnergies.AddElem( currentChunkEnergy );
			}

			p += mFrameSize;			
		} while (p <= end );

		// Enjoy the silence...
		if ( chunksEnergies.Size() == 0 )
		{
			mIsSilenceCtrl.SendControl( true );
			return 1.0;
		}

		std::sort( chunksEnergies.GetPtr(), chunksEnergies.GetPtr()+chunksEnergies.Size() );

		//find the threshold under which lies percent% of the energy values
		//that are not silence
		
		int percentage = 90;

		int i = ( chunksEnergies.Size()*percentage ) / 100;

		i = ( i == 0 ) ? i : i - 1;

		mIsSilenceCtrl.SendControl(false);

		return CLAM_sqrt( chunksEnergies[i]/TData(mFrameSize) );

	}
	
	void Normalization::CheckSilence( int size )
	{
		if (size==0) 
			mIsSilenceCtrl.SendControl(true);
		else
			mIsSilenceCtrl.SendControl(false);
	}


}

