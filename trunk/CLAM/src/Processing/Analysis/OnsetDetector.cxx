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

#include "OnsetDetector.hxx"
#include "Point.hxx"
#include "OD_AudioDecimation.hxx"
#include "CLAM_Math.hxx"
#include <libresample.h>

namespace CLAM
{
	OnsetDetector::OnsetDetector()
	{
		AttachChildren();
		Configure(OnsetDetectorConfig());
	}

	OnsetDetector::OnsetDetector(OnsetDetectorConfig& c)
	{
		AttachChildren();
		Configure(c);
	}

	bool OnsetDetector::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig( mConfig, c );

		mnBands = mConfig.GetnBands();
		mLowFreq = mConfig.GetLowFreq();
		mHighFreq = mConfig.GetHighFreq();
		mBandThreshold = mConfig.GetBandThreshold();
		mMinPeakDist = mConfig.GetMinPeakDist();
		mGlobalThreshold = mConfig.GetGlobalThreshold();
		mNoiseThreshold = mConfig.GetNoiseThreshold();
		mComputeOffsets = mConfig.GetComputeOffsets();
		mOffsetThreshold = mConfig.GetOffsetThreshold();
		
		//Computes the smoothing filter coefficients
		mWinSize = mConfig.GetSmoothFiltSize();
		mWinCoef.Resize(mWinSize);
		mWinCoef.SetSize(mWinSize);

		double dPhi = ( 2.0 * M_PI ) / ( TData(mWinSize)*2.0 );
		double phi = 0.0;
		mWinCoefSummation = 0.0;

		for(int i=0; i<mWinSize; i++, phi += dPhi )
		{
			mWinCoef[i]=0.5+0.5*std::cos(phi);
			mWinCoefSummation += mWinCoef[i];
		}

		
		ConfigureChildren();
	
		return true;
	}


	bool OnsetDetector::ConfigureChildren()
	{
		//Filter Bank Configuration
		GT_FilterBankConfig bankcfg;
		bankcfg.SetnChannels(mnBands);
		bankcfg.SetLowFreq(mLowFreq);
		bankcfg.SetHighFreq(mHighFreq);
		bankcfg.SetSampleRate(22050);
		mFilterBank.Configure(bankcfg);

		return true;
	}


	void OnsetDetector::AttachChildren()
	{
		mFilterBank.SetParent(this);
	}

	bool OnsetDetector::Do(Segment &originalSegment, Array<TimeIndex>& out)
	{
		CLAM_ASSERT( int(originalSegment.GetAudio().GetSampleRate()) == 44100,
			     "This onset detection algorithm only works for signals sampled at a 44.1kHz rate" );

		//Downsampling 1:2 factor

		TSize originalAudioSize = originalSegment.GetAudio().GetSize();
		TSize downAudioSize = (originalSegment.GetAudio().GetSize()/2);

		mAudio.SetSize( downAudioSize + 1000 );
		mAudio.SetSampleRate(originalSegment.GetAudio().GetSampleRate()/2);

		void* handle = resample_open( 1, 0.5, 0.5 );
		int srcused;

		resample_process( handle, 0.5,
				  originalSegment.GetAudio().GetBuffer().GetPtr(),
				  originalAudioSize,
				  1, &srcused,
				  mAudio.GetBuffer().GetPtr(), downAudioSize+1000 );
				  
		resample_close( handle );

		mAudio.SetSize( downAudioSize );

		//Filter bank output computation
		mFilterBankOutputs.Resize(mnBands);
		mFilterBankOutputs.SetSize(mnBands);
		
		CalcFilterBankOutput( );
		mnSamples = mFilterBankOutputs[0].Size();

		//Onset detection	
		Algorithm( originalSegment, out );

		return true;

	}


	///////////////
	//FILTER BANK//
	///////////////
	bool OnsetDetector::CalcFilterBankOutput()
	{
		//Filter Bank


		const TSize bandSize=mAudio.GetSize();

		for ( int band = 0; band < mnBands; band++ )
		{
			mFilterBankOutputs[band].Resize((bandSize/90) + 1000);
			mFilterBankOutputs[band].SetSize(( bandSize/90) + 1000);			
		}

		mFilterBank.Do( mAudio, mFilterBankOutputs );

		mSampleRate = 245;
	
		return true;
	}


////////////////////////////////////////
////////SEGMENTATION ALGORITHM//////////
////////////////////////////////////////
	void OnsetDetector::Algorithm( Segment& s , Array<TimeIndex>& finalOnsets )
	{
		Array< Array<float> > smoothedInput(mnBands);	//Smoothed Band Energy Array
		smoothedInput.SetSize(mnBands);

		for ( int i = 0; i < mnBands; i++ )
		{
			smoothedInput[i].Resize( mnSamples );
			smoothedInput[i].SetSize( mnSamples );
		}

		Array< Array<float> > bandOnsetDetectData(mnBands);	//Data on which detection is performed 
		bandOnsetDetectData.SetSize(mnBands);

		for ( int i = 0; i < mnBands; i++ )
		{
			bandOnsetDetectData[i].Resize( mnSamples );
			bandOnsetDetectData[i].SetSize( mnSamples );
		}


		std::list< TimeIndex > candidates;
	
		TimeIndex tempOnset;
		Array< Array<TimeIndex> > bandOnsets(mnBands);
		bandOnsets.SetSize(mnBands);
	

		DataArray onsetsArray(mnSamples);		//sum of cleaned onsets detected in each bands (Data)
		onsetsArray.SetSize(mnSamples);	

		std::map< TIndex, double > onsetsDetected;


		mRevSmoothedEnergy.Resize( mnSamples );
		mRevSmoothedEnergy.SetSize( mnSamples );

		//////////////////////////
		//ONSET DETECTION PER BAND	
		for (int band=0 ; band<mnBands ; band++)
		{
		
			//smoothing
			Smoothing( mFilterBankOutputs[band] , smoothedInput[band] );

						
			//Detection Function Calculation
			DetectPosition( smoothedInput[band] , bandOnsetDetectData[band]);		

		
			//Candidates Detection
			DetectCandidates( bandOnsetDetectData[band] , smoothedInput[band], mBandThreshold , candidates);


			//weak onsets deletion
			RemoveTooNearOnsetsFromCandidatesList( candidates );


			//sums up band results
			for ( LI j = candidates.begin();
			      j != candidates.end();
			      j++ )
				if ( onsetsDetected.find( (TIndex)j->GetPosition() ) != onsetsDetected.end() )
					onsetsDetected[ (TIndex)j->GetPosition() ] += j->GetWeight();
				else
					onsetsDetected[ (TIndex)j->GetPosition() ] = j->GetWeight();

			candidates.clear();
		}

		if ( onsetsDetected.empty() )
			return;


		TimeIndex tmp;
		for ( MI i = onsetsDetected.begin(); i != onsetsDetected.end();
		      i++ )
		{
			tmp.SetPosition( i->first );
			tmp.SetWeight( i->second );
			candidates.push_back( tmp );
		}

	

		//weak onsets deletion (weak onsets intensities are summed up
		//and added to the highest peak in a mMinPeakDist long window)
		RemoveTooWeakOnsetsFromCandidatesList( candidates );

		double maxWeight = 0.0;
         LI i = candidates.begin();
		 while( i != candidates.end() )
		 {
			if ( i->GetWeight() <= mGlobalThreshold )
			{
				i = candidates.erase( i );
				continue;
			}

			if ( i->GetWeight() >= maxWeight )
				maxWeight = i->GetWeight();
			++i;

		}

	
		if ( candidates.empty() )
			return;
		

		if ( mComputeOffsets )
			CheckOffset( s, candidates );

		

		finalOnsets.Resize( candidates.size() );
		finalOnsets.SetSize( candidates.size() );

		
		int k = 0;
		
		for ( LI i = candidates.begin(); 
		      i != candidates.end(); i++, k++ )
		{
			finalOnsets[k].SetPosition( i->GetPosition() / mSampleRate );			
			finalOnsets[k].SetWeight( i->GetWeight() / maxWeight );			
		}


	}


	void OnsetDetector::Smoothing(Array<float>& energy, Array<float>& smoothedEnergy )
	{
		int i, j, k;
		double temp;

		double inverseWCSum = 1.0 / mWinCoefSummation;

		CLAM_ASSERT( mnSamples > mWinSize, 
			     "OnsetDetector::Smoothing() : number of samples to process should be greater than mWinSize" );

		//convolution
		for ( i = 0; i < mWinSize; i++ )
		{
			temp = 0.0;
			for ( j = -(i-mWinSize)-1; j < mWinSize; j++ )
				temp+=energy[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];

			const double normConv = temp * inverseWCSum;

			smoothedEnergy[i] = ( normConv > mNoiseThreshold ) ? normConv : mNoiseThreshold;

			mRevSmoothedEnergy[mnSamples-i-1]=smoothedEnergy[i];
		}


		for(; i<mnSamples;i++)
		{	
			temp=0.0;

			for(j = 0; j<mWinSize; j++)
				temp+=energy[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];


			const double normConv = temp * inverseWCSum;

			smoothedEnergy[i] = ( normConv > mNoiseThreshold ) ? normConv : mNoiseThreshold;

			mRevSmoothedEnergy[mnSamples-i-1]=smoothedEnergy[i];
		}

		//reverse convolution for zero-phase distortion
		for ( i = 0; i < mWinSize; i++ )
		{
			temp=0.0;

			for( j = -(i-mWinSize)-1; j<mWinSize; j++)
			{
				temp+=mRevSmoothedEnergy[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];
				
			}
			smoothedEnergy[mnSamples-i-1]=temp*inverseWCSum;		

		}

		for(; i<mnSamples;i++)
		{	
			temp=0.0;

			for(j=0; j<mWinSize; j++)
			{
				temp+=mRevSmoothedEnergy[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];
				
			}
			smoothedEnergy[mnSamples-i-1]=temp*inverseWCSum;		
		}

	}


	void OnsetDetector::DetectPosition(Array<float>& in, Array<float>& ret)
	{
		int i;

		//Sets first and last values to zero
		ret[0] = 0.0;
		ret[mnSamples-1] = 0.0;

		// MRJ: Original computation was a loop from 1:mnSamples-1 where
		// ret(i):=log(in[i+1])-log(in[i-1]).
		// This simple trick reduces the overall number of log()'s to
		// exactly mnSamples ( which is yet a lot of times ).

		double a = log( in[0] );
		double b = log( in[1] );
		double c = log( in[2] );

		ret[1] = c - a;

		for (i=2; i<mnSamples-1; i++) 
		{
			a = b; b = c; c = log( in[i+1] );
			
			ret[i] = c - a;

		}
	}


////////////////////////////////
///////CANDIDATE DETECTION//////
////////////////////////////////
	void OnsetDetector::DetectCandidates(Array<float>& in, Array<float>& weight, 
					     TData threshold , std::list<TimeIndex>& ret)
	{	
	
		//This function detect the positions and weights of candidates
		//The highest peak in the derivative of the log smoothed energy is taken as the position
		//(Instead, the position of the first peak or the highest peak in the derivative of the smoothed energy can be taken)
		//The weigth is the derivative of the smoothed energy at this position
		//(instead, the derivative of the log smoothed energy or the "dynamic" can be taken)	
	
	
		int i=1, maxLogPos, maxLinPos;
		TData maxLog, maxLin;	
		TimeIndex candidate;


		TSize maxPosition = in.Size() - 3;

		//takes peaks above the threshold
		while(i < maxPosition)
		{
			maxLog=maxLin=0;		

			if(in[i]>0 || in[i+1]>0 || in[i+2]>0 )
			{

				//while the derivative is positive (up to 2 negative values are allowed)
				do
				{
				

					//store the position and the value of the highest peak in the detection function (derivative of
					//the log smoothed energy)
					if(in[i]>maxLog)
					{
						maxLog = in[i];
						maxLogPos = i;
					}

					//store the position and the value of the highest peak in the derivative of
					//the smoothed energy
					double deltaEnergy = weight[i+1] - weight[i-1];
					if(deltaEnergy > maxLin)
					{
						maxLin = deltaEnergy;
						maxLinPos = i;
					}
				

					i++;
				}
				while(i < maxPosition  && (in[i]>0 || in[i+1]>0 || in[i+2]>0));
			
				if(maxLog>threshold)
				{
					candidate.SetPosition(maxLogPos);
					candidate.SetWeight(maxLin);
					ret.push_back( candidate );

				}
			}
			else 
				i++;
		}
	}

	void OnsetDetector::RemoveTooNearOnsetsFromCandidatesList( std::list<TimeIndex>& in )
	{
		TimeIndex newPeak;
		
		typedef std::list<TimeIndex>::iterator LI;

		LI theLastOne = in.end();
		theLastOne--;
		LI j = in.begin();
		LI k, knext, lastToRemove;


		while ( j != theLastOne )
		{
			k = j;

			knext = k; knext++;


			while( k != theLastOne
			       && ( knext->GetPosition() - k->GetPosition() ) < mMinPeakDist )
			{
				knext++;
				k++;
			}
				
			if ( j == k ) // No peaks were found to be "deleteable"
				j++;
			else
			{
				TimeIndex newPeak;
				PeakDeletion( j, knext, newPeak );
				*j = newPeak;
				LI first = j; first++;
				LI last = k; 
				if ( last != in.end() ) last++;
				in.erase( first, last ); 
				theLastOne = in.end();
				theLastOne--;

			}
			       
		}
	}

	void OnsetDetector::PeakDeletion( LI first, LI last, TimeIndex& newPeak )
	{
		double max = -1.0;

		for ( LI i = first; i != last; i++ )
			if ( i->GetWeight() > max )
			{
				max = i->GetWeight();
				newPeak.SetPosition( i->GetPosition() );
				newPeak.SetWeight( i->GetWeight() );
			}
	}

	void OnsetDetector::RemoveTooWeakOnsetsFromCandidatesList( std::list<TimeIndex>& in )
	{
		TimeIndex newPeak;

		
		typedef std::list<TimeIndex>::iterator LI;

		LI theLastOne = in.end();
		theLastOne--;
		LI j = in.begin();
		LI k, knext, lastToRemove;


		while ( j != theLastOne )
		{
			k = j;

			knext = k; knext++;


			while( k != theLastOne
			       && ( knext->GetPosition() - j->GetPosition() ) < mMinPeakDist )
			{
				knext++;
				k++;
			}
				
			if ( j == k ) // No peaks were found to be "deleteable"
				j++;
			else
			{
				TimeIndex newPeak;
				PeakSummation( j, knext, newPeak );
				*j = newPeak;
				LI first = j; first++;
				LI last = k; 
				if ( last != in.end() ) last++;
				in.erase( first, last ); 
				theLastOne = in.end();
				theLastOne--;
			}
			       
		}

	}

	void OnsetDetector::PeakSummation( LI first, LI last, TimeIndex& newPeak )
	{
		double max = -1.0, sum = 0.0;
		
		for ( LI i = first; i != last; i++ )
		{
			sum += i->GetWeight();
			
			if ( i->GetWeight() > max )
			{
				max = i->GetWeight();
				newPeak.SetPosition( i->GetPosition() );
			}
		}

		newPeak.SetWeight( sum );
	}


	//PEAK DELETION



	/////////////////////
	//CHECK  FOR OFFSETS//
	/////////////////////

	void OnsetDetector::DecimationForEnvelopeComputation( Array<float>& envelope )
	{
		TSize originalAudioSize = mAudio.GetSize();
		TSize downAudioSize = mAudio.GetSize() / 90;

		envelope.Resize( downAudioSize + 1000 );
		envelope.SetSize( downAudioSize + 1000 );

		DataArray& samples = mAudio.GetBuffer();

		for ( int i = 0; i < originalAudioSize; i++ )
			samples[i] = fabsf( samples[i] );

		// Decimation 
		double factor = 245.0 / 22050.0;

		void* handle = resample_open( 1, factor, factor );
		int srcused;

		resample_process( handle, factor,
				  samples.GetPtr(), originalAudioSize,
				  1, &srcused,
				  envelope.GetPtr(), downAudioSize+1000 );

		resample_close( handle );
		
		envelope.SetSize( downAudioSize );

	}

	void OnsetDetector::ComputeSmoothingFilterCoeffs( Array<float>& coeffs )
	{
		TSize winSize = TSize(0.05 * mSampleRate);
		coeffs.Resize( winSize );
		coeffs.SetSize( winSize );

		for ( int i = 0; i < winSize; i++ )
			coeffs[i] = 1.0;
	}

	void OnsetDetector::ExtractAudioEnvelope( Array<float>& envelope )
	{
		// Extraction of amplitude envelope

		Array<float> amplitude;

		DecimationForEnvelopeComputation( amplitude );

		Array<float> winCoef;

		ComputeSmoothingFilterCoeffs( winCoef );
		
		// rescaling factor
		double sum = winCoef.Size();

		//convolution
		envelope.Resize(amplitude.Size());
		envelope.SetSize(amplitude.Size());
		Array<float> revenvelope(amplitude.Size());
		revenvelope.SetSize(amplitude.Size());
		

		//convolution
		double inverseWCSum = 1.0 / sum;
		TSize  ampSize = amplitude.Size();
		double temp;

		int j = 0;
		int i = 0;
		
		for ( i = 0; i < mWinSize; i++ )
		{
			temp = 0.0;
			for ( j = -(i-mWinSize)-1; j < mWinSize; j++ )
				temp+=amplitude[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];

			envelope[i] = temp * inverseWCSum;

			revenvelope[ampSize-i-1]=envelope[i];
		}


		for(; i<ampSize;i++)
		{	
			temp=0.0;

			for(j = 0; j<mWinSize; j++)
				temp+=amplitude[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];


			envelope[i] = temp * inverseWCSum;

			revenvelope[ampSize-i-1]=envelope[i];
		}

		//reverse convolution for zero-phase distortion
		for ( i = 0; i < mWinSize; i++ )
		{
			temp=0.0;

			for( j = -(i-mWinSize)-1; j<mWinSize; j++)
			{
				temp+=revenvelope[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];
				
			}
			envelope[ampSize-i-1]=temp*inverseWCSum;		

		}

		for(; i<ampSize;i++)
		{	
			temp=0.0;

			for(j=0; j<mWinSize; j++)
			{
				temp+=revenvelope[i-mWinSize+1+j]*mWinCoef[mWinSize-1-j];
				
			}
			envelope[ampSize-i-1]=temp*inverseWCSum;		
		}


	}

	void OnsetDetector::CheckOffset( Segment& s, std::list<TimeIndex>& candidates )
	{
		int i, j;
		std::list< PointTmpl<int,int> > offsets;
		typedef std::list< PointTmpl<int,int> >::iterator PLI;

		Array<float> envelope;

		ExtractAudioEnvelope( envelope );

		// Actual Offset Checking

		PointTmpl<int,int> temp;
		double min = 0;
		LI prev, next;
		prev = candidates.begin();
		next = prev;
		next++;

		for ( ; next != candidates.end() ; prev++, next++)
		{
			//onset position
			i=int(next->GetPosition());
			
			temp.SetX(int(prev->GetPosition()));

			//looks for the first min before the onset
			min = envelope[i];
			
			do
			{
				if(envelope[i]<min)
					min = envelope[i];
				i--;
			} 
			while (i>0  && envelope[i]>envelope[i-1]);
			
			//finds the point (going backwards) where the amplitude rises above the threshold 
			
			if (min < mOffsetThreshold && i>0) 
			{
				while (envelope[i] < mOffsetThreshold && i>prev->GetPosition())
					i--;
				
				if(i>prev->GetPosition())
				{
					temp.SetY(i);
					offsets.push_back(temp);
				}
			}
		}
		
		//Checking for last offset
		i=int(candidates.back().GetPosition());
		
		temp.SetX(int(candidates.back().GetPosition()));
		
		bool found=false;
		
		do  
		{
			if (envelope[i]< mOffsetThreshold 
			    && envelope[i]>envelope[i+1] 
			    && envelope[i+1]>envelope[i+2] 
			    && envelope[i+2]>envelope[i+3] )
			{
				temp.SetY(i);
				offsets.push_back(temp);
				found=true;
			}
			i++;
		}
		while(i<envelope.Size()-3 && !found);

		// Onset/Offset information info
		// A segment start is always an onset
		// A segment end is either an offset ( if the amplitude goes below mOffsetThreshold )
		// or the next onset

		if ( candidates.empty() )
			return;

		bool hasOffset = false;
		int  offsetNumber = offsets.size();
		PLI  offsetIt = offsets.begin();
		LI   last = candidates.end();
		last--;

		for ( LI candidateIt = candidates.begin(); candidateIt != last; candidateIt++ )
		{
			Segment tmpSegment;
			int onset = int(candidateIt->GetPosition());

			if ( offsetIt != offsets.end() )
			{
				int offOnset = offsetIt->GetX();
				if ( onset == offOnset )
					hasOffset = true;
			}

			if ( hasOffset )
			{
				tmpSegment.SetBeginTime( (TTime)offsetIt->GetX()/(TTime)mSampleRate );
				tmpSegment.SetEndTime( (TTime)offsetIt->GetY()/(TTime)mSampleRate );
				tmpSegment.SetpParent( &s );
				s.GetChildren().AddElem( tmpSegment );
				offsetIt++;
				candidateIt++;
				hasOffset = false;
			}
			else
			{
				tmpSegment.SetBeginTime( (TTime)candidateIt->GetPosition()/ (TTime) mSampleRate );
				LI nextCandidateIt = candidateIt;
				nextCandidateIt++;
				tmpSegment.SetEndTime( (TTime)nextCandidateIt->GetPosition() / (TTime) mSampleRate );
				tmpSegment.SetpParent( &s );
				s.GetChildren().AddElem( tmpSegment );
			}

			
		}
			
		// Last boundaries
		Segment tmpSegment;
		int onset = int(candidates.back().GetPosition());
		
		if ( offsetIt != offsets.end() )
		{
			int offOnset = offsetIt->GetX();
			if ( onset == offOnset )
				hasOffset = true;
		}
		
		if (hasOffset)
		{
			tmpSegment.SetBeginTime((TTime)offsetIt->GetX()/(TTime)mSampleRate);
			tmpSegment.SetEndTime((TTime)offsetIt->GetY()/(TTime)mSampleRate);
			tmpSegment.SetpParent(&s);
			s.GetChildren().AddElem(tmpSegment);
		}
		else
		{
			tmpSegment.SetBeginTime((TTime)candidates.back().GetPosition() / (TTime)mSampleRate);
			tmpSegment.SetEndTime(s.GetEndTime());
			tmpSegment.SetpParent(&s);
			s.GetChildren().AddElem(tmpSegment);
		}


	}



}

