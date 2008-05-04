/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Complex.hxx"
#include "FundFreqDetect.hxx"
#include "Fundamental.hxx"
#include "SpectralPeakArray.hxx"
#include <cmath>

#define INFINITE_MAGNITUD 1000000

namespace CLAM 
{

	FundFreqDetect::FundFreqDetect()
		: mInput( "Input", this),
		  mOutput( "Output", this ),
		  mFundFreqValue( "Fund Freq Value", this )
	{
		Configure(FundFreqDetectConfig());
	}

	FundFreqDetect::FundFreqDetect(const FundFreqDetectConfig &c )
		: mInput( "Input", this ),
		  mOutput( "Output", this ),
		  mFundFreqValue( "Fund Freq Value", this )
	{
		Configure(c);
	}

	FundFreqDetect::~FundFreqDetect()	{}

	/* Configure the Processing Object according to the Config object */
	bool FundFreqDetect::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);

		mReferenceFundFreq = mConfig.GetReferenceFundFreq();
		mLowestFundFreq    = mConfig.GetLowestFundFreq();
		mHighestFundFreq   = mConfig.GetHighestFundFreq();
		mMaxCandMagDiff    = mConfig.GetMaxCandMagDiff();
		mMaxFundFreqError  = mConfig.GetMaxFundFreqError(); 
		mnInt              = mConfig.GetNInt();
		mPMp = mConfig.GetPMp();
		mPMq = mConfig.GetPMq();
		mPMr = mConfig.GetPMr();
		mMPp = mConfig.GetMPp();
		mMPq = mConfig.GetMPq();
		mMPr = mConfig.GetMPr();
		mPMnPeaks = mConfig.GetPMnPeaks();
		mMPnPeaks = mConfig.GetMPnPeaks();
		mPMCont = mConfig.GetPMCont();
		mMPCont = mConfig.GetMPCont();
		mnMaxCandidates= TData(mConfig.GetNMaxCandidates());
		
		return true;
	}

	const ProcessingConfig &FundFreqDetect::GetConfig() const
	{		
		mConfig.SetMaxCandMagDiff(mMaxCandMagDiff);
		mConfig.SetMaxFundFreqError(mMaxFundFreqError); 
		mConfig.SetNInt(mnInt);
		mConfig.SetPMp(mPMp);
		mConfig.SetPMq(mPMq);
		mConfig.SetPMr(mPMr);
		mConfig.SetMPp(mMPp);
		mConfig.SetMPq(mMPq);
		mConfig.SetMPr(mMPr);
		mConfig.SetPMnPeaks(mPMnPeaks);
		mConfig.SetMPnPeaks(mMPnPeaks);
		mConfig.SetPMCont(mPMCont);
		mConfig.SetMPCont(mMPCont);
		mConfig.SetNMaxCandidates(TSize(mnMaxCandidates));
		return mConfig;
	}

	/* The supervised Do() function */
	bool FundFreqDetect::Do(void)
	{
		mOutput.GetData().SetnMaxCandidates(1);

		bool result = Do( mInput.GetData(), mOutput.GetData() );
		mInput.Consume();
		mOutput.Produce();

		return result;
	}

	/* The unsupervised Do() function */
	bool FundFreqDetect::Do(SpectralPeakArray& peaks,Fundamental& outFreq)
	{
		outFreq.Init();

		// Check Number of Candidates required
		CLAM_ASSERT (outFreq.GetnMaxCandidates() > 0, 
			"FundFreqDet::Detection: negative number of candidates wanted");

		// See if the number of best candidates to be calculated is less than the maximum permitted
		CLAM_ASSERT (outFreq.GetnMaxCandidates() <= mnMaxCandidates,
			"FundFreqDet::Detection:Number of candidates wanted bigger "
			"than the maximum configured on the algorithm"); 

		// not enough peak information available for fundamental frequency detection");
		if (peaks.GetnPeaks() <= 0)
		{
			mFundFreqValue.SendControl(0.0f);
			return false;
		}

		// Calculate Maximun Magnitude Peak
		TIndex nMaxMagPeak = peaks.GetMaxMagPos();
		TData maxMag       = peaks.GetMag(nMaxMagPeak);

		// 1.- SELECT PEAKS
		// Add an index to the PeakArray
		if(!peaks.HasIndexArray())
		{
			peaks.AddIndexArray();
			peaks.UpdateData();
			peaks.SetnMaxPeaks(peaks.GetnMaxPeaks());
		}

		// Reset indices in the peak array
		peaks.ResetIndices();

		// Delete peaks below the lowest pitch
		DataArray& peakMagnitudes=peaks.GetMagBuffer();
		DataArray& peakFrequencies=peaks.GetFreqBuffer();
		DataArray& peakBinPosBuffer=peaks.GetBinPosBuffer();

		const TData spectralRes = peakFrequencies[nMaxMagPeak]/peakBinPosBuffer[nMaxMagPeak];
		
		TData lowestFundFreqBinPos = mLowestFundFreq/spectralRes;
		TIndex z=0;
		while((z<peaks.GetnPeaks()) && (peakBinPosBuffer[z]<lowestFundFreqBinPos)) 
		{
			peaks.DeleteIndex(z);
			z++;
		}

		// Before the maximum magnitude peak
		for(int i=z; i<nMaxMagPeak; i++) 
		{
			if(peakMagnitudes[i] < maxMag - 30)
				peaks.DeleteIndex(i);
		}

		// Delete peaks above 3000
		TData peaklimitBinPos = 3000.0/spectralRes;
		for (int i=peaks.GetnPeaks()-1; i > nMaxMagPeak; i--)
		{
			if (peakBinPosBuffer[i] <= peaklimitBinPos) break;
			peaks.DeleteIndex(i);
		}
			
		// After the maximum magnitude peak
		TData x,y,a,b;
		a = - 10*spectralRes/TData(1000.0);
		b = maxMag - 50 - a*(TData)peakBinPosBuffer[nMaxMagPeak];
		for(int i=nMaxMagPeak+1; i<z; i++) 
		{
			y = peakMagnitudes[i];
			x = peakBinPosBuffer[i];
			if(y < (a*x+b)) 
			{
				peaks.DeleteIndex(i);
			}
		}		

		const IndexArray & peakIndexes = peaks.GetIndexArray();
		// If there no valid peaks for calculate a fundamental frequency
		if (peakIndexes.Size() <= 0)
		{
			mFundFreqValue.SendControl(0.0f);
			return false;
		}
	  
		// Find maximun magnitude peak from the selected ones
		nMaxMagPeak = peaks.GetMaxMagIndex(); // only indexed peaks
		maxMag      = peakMagnitudes[peakIndexes[nMaxMagPeak]];

		// 2.- FIND mnMaxCandidates CANDIDATES
	  
		Fundamental tmpFreq; // this will be used throughout the algorithm to allocate new candidates
		tmpFreq.AddCandidatesFreq();
		tmpFreq.AddCandidatesErr();
		tmpFreq.UpdateData();
		tmpFreq.SetnCandidates(0);
		tmpFreq.SetnMaxCandidates(int(mnMaxCandidates));

		DataArray & candidatesFrequency = tmpFreq.GetCandidatesFreq();
		DataArray & candidatesError = tmpFreq.GetCandidatesErr();

		// 2.0.- Reference Fundamental Frequency
		if( IsGoodCandidate(mReferenceFundFreq) ) 
			tmpFreq.AddElem(mReferenceFundFreq);
	  
		// 2.1.- Three maximum magnitude peaks and its integer ratios
		TIndex nMaxMagPeak2 = nMaxMagPeak;
		TIndex nMaxMagPeak3 = nMaxMagPeak;
		if(peakIndexes.Size() >= 2)
		{
	  		// find second max magnitude peak
		  	peakMagnitudes[peakIndexes[nMaxMagPeak]]=-2000;
		      	nMaxMagPeak2 = peaks.GetMaxMagIndex();
			if(peakIndexes.Size() >= 3)
			{
				TData aux;
				// find third max magnitude peak
				aux = peakMagnitudes[peakIndexes[nMaxMagPeak2]];
				peakMagnitudes[peakIndexes[nMaxMagPeak2]]=-2000;
				nMaxMagPeak3 = peaks.GetMaxMagIndex();
				// add candidate
				if ( IsGoodCandidate(peakFrequencies[peakIndexes[nMaxMagPeak3]]) )
					tmpFreq.AddElem(peakFrequencies[peakIndexes[nMaxMagPeak3]]);
				// restore second peak information
				peakMagnitudes[peakIndexes[nMaxMagPeak2]]=aux;
			}
			// add candidate
			if ( IsGoodCandidate(peakFrequencies[peakIndexes[nMaxMagPeak2]]) )
				tmpFreq.AddElem(peakFrequencies[peakIndexes[nMaxMagPeak2]]);
			
			// restore first peak information
			peakMagnitudes[peakIndexes[nMaxMagPeak]]=maxMag;
		}	  
		// Add peaks as candidates
		if ( IsGoodCandidate(peakFrequencies[peakIndexes[nMaxMagPeak]]) )
			tmpFreq.AddElem(peakFrequencies[peakIndexes[nMaxMagPeak]]);	
	  
		// 2.2.- Peaks below the maximum magnitude peak (except for the 3 max peaks)
		for (int i=0; i < nMaxMagPeak; i++ )
		{ 
			// be careful not to exceed the maximun permitted
			if (candidatesFrequency.Size() >= mnMaxCandidates) break;
			if (i==nMaxMagPeak2) continue;
			if (i==nMaxMagPeak3) continue;
			if (peakMagnitudes[peakIndexes[i]] <= maxMag - mMaxCandMagDiff ) continue;
			if (! IsGoodCandidate(peakFrequencies[peakIndexes[i]]) ) continue;
			tmpFreq.AddElem(peakFrequencies[peakIndexes[i]]);
		}
	  	
		// 2.3.- Frequency offset between peaks above the maximun magnitude peak and the maximun magnitude peak
		TData freq;
		for (int i = nMaxMagPeak+1; i<peakIndexes.Size(); i++)
		{
			// be careful not to exceed the maximun permitted
			if (candidatesFrequency.Size() >= mnMaxCandidates) break;
			freq = peakFrequencies[peakIndexes[i]] - peakFrequencies[peakIndexes[nMaxMagPeak]];
			if (freq >= peakFrequencies[peakIndexes[nMaxMagPeak]]*1.1) continue;
			if (!IsGoodCandidate(freq)) continue;
			tmpFreq.AddElem(freq);
		}
		
		// 2.4.- Frequency offset between peaks
		for (int i = 0; i<peakIndexes.Size(); i++ )
		{
			if (i==nMaxMagPeak) continue;
			for (int j = i+1; j<peakIndexes.Size(); j++)
			{
				// be careful not to exceed the maximun permitted
				if (candidatesFrequency.Size() >= mnMaxCandidates) break;
				freq = peakFrequencies[peakIndexes[j]] - peakFrequencies[peakIndexes[i]];
				if (freq < peakFrequencies[peakIndexes[nMaxMagPeak]]*1.1)
					if (IsGoodCandidate(freq))
						tmpFreq.AddElem(freq);
			}
		}
	  
		// 2.5.- Frequencies related to peaks by integer ratios (before: except for the 3 maximun peaks. not now)	
		for (int i=0; i<peakIndexes.Size(); i++ )
		{
			for (int j=1; j <= mnInt; j++)
			{
				// be careful not to exceed the maximun permitted
				if (candidatesFrequency.Size() >= mnMaxCandidates) break;
				freq = peakFrequencies[peakIndexes[i]]/j;
				if (freq < peakFrequencies[peakIndexes[nMaxMagPeak]]*1.1)
					if (IsGoodCandidate(freq))
						tmpFreq.AddElem(freq);
			}
		}
	
		if(candidatesFrequency.Size() <= 0)
		{
			mFundFreqValue.SendControl(0.0f);
			return false;
		}

		// 3.- CALCULATE ERRORS (TMW procedure)
		TData oneOverSeventyFive = 0.013333333333f;
		
		const int nPeaks = peaks.GetIndexArray().Size();
		
		int i;
		
		//These are all needed by the WeightCandidate and it is much better to compute just once				
		DataArray magFactor(nPeaks);
		DataArray magFactor3(nPeaks);
		DataArray magFactor4(nPeaks);
		DataArray magFactor4r(nPeaks);
		DataArray magFactor4q(nPeaks);
		DataArray magFactor34q(nPeaks);
				
		for (i=0; i<nPeaks; i++)
		{
			TData Mag = peakMagnitudes[peakIndexes[i]];
			TData tmpMagFactor = TData(CLAM_max(0.0,maxMag - Mag + 20.0));
			tmpMagFactor = TData(1.0) - tmpMagFactor*oneOverSeventyFive;
			if (tmpMagFactor < 0)
				tmpMagFactor = 0;
			magFactor[i] = tmpMagFactor;
			magFactor3[i] = tmpMagFactor*tmpMagFactor*tmpMagFactor;
			magFactor4[i] = magFactor3[i]*tmpMagFactor;
			magFactor4r[i] = magFactor4[i]*mMPr;
			magFactor4q[i] = magFactor4[i]*mMPq;
			magFactor34q[i] = magFactor3[i] + magFactor4q[i];
		}
		
		int maxNMP = CLAM_min(mMPnPeaks,nPeaks);
		// predicted to measured mismatch error
		int maxNPM = 10;
		//xamat: does not depend on variable data: compute out of the loop!!
		if (nPeaks > 4)
			maxNPM = CLAM_min(mPMnPeaks,nPeaks);
			
		for (int i=0; i<candidatesFrequency.Size(); i++)
		{
			TData myFrequency = candidatesFrequency[i];
			TData myError = WeightCandidate(myFrequency,peaks, magFactor, magFactor34q, magFactor4r, maxNMP, maxNPM);
			candidatesError[i] = myError;
		}
		
		// 4.- CHOOSE THE BEST CANDIDATES: choose the FundFreq.NCandidates() candidates with the smallest error
		
		tmpFreq.SortByError();
		Fundamental tmpFreq2;
		tmpFreq2.SetnMaxCandidates(candidatesFrequency.Size());
		DataArray & candidates2Frequency = tmpFreq2.GetCandidatesFreq();
		DataArray & candidates2Error = tmpFreq2.GetCandidatesErr();

		for (int i=0;i<candidatesFrequency.Size();i++)
		{
			if (i<=0)
			{
				tmpFreq2.AddElem(candidatesFrequency[i],candidatesError[i]);
				continue;
			}
			bool addedNearOne=false;
			for(int j=0; j<candidates2Frequency.Size(); j++)
			{
				if (candidatesFrequency[i]<=0.95*candidates2Frequency[j]) continue;
				if (candidatesFrequency[i]>=1.1*candidates2Frequency[j]) continue;
				addedNearOne = true;
				if(candidates2Error[j] > candidatesError[i])
				{
					candidates2Frequency[j] = candidatesFrequency[i];
					candidates2Error[j] = candidatesError[i];
				}
			}
			if(!addedNearOne)
				tmpFreq2.AddElem(candidatesFrequency[i],candidatesError[i]);
		}
		
		// 5.- SEARCH AROUND FOR A RELATIVE MINIMUM
		TData nMinimum = std::min(3,candidates2Frequency.Size());
		for(int i=0; i<nMinimum; i++)
		{
			TData & myFrequency = candidates2Frequency[i];
			TData Low  = myFrequency*TData(.9);
			TData High = myFrequency*TData(1.1);
			TData Incr = std::max(TData(1.0), myFrequency*TData(.008)); 

			TData FinalPitch = candidates2Frequency[i];
			TData FinalError = candidates2Error[i];
			for(TData lPitch = Low; lPitch <=High; lPitch += Incr)
			{
				TData lErr = WeightCandidate(lPitch,peaks, magFactor, magFactor34q, magFactor4r, maxNMP, maxNPM);
				if (lPitch > peakFrequencies[peakIndexes[nMaxMagPeak]]*1.1)
					lErr +=10;
				if (lErr < FinalError)
				{
					FinalPitch = lPitch;
					FinalError = lErr;
				}
			}
			candidates2Frequency[i] = FinalPitch;
			candidates2Error[i] = FinalError;
		}

		// Ordering the minimum
		tmpFreq2.SortByError();

		TIndex nCandidates = std::min(outFreq.GetnMaxCandidates(),candidates2Frequency.Size());
		for(int i=0; i<nCandidates; i++)
			if(candidates2Error[i] <= mMaxFundFreqError)
				outFreq.AddElem(candidates2Frequency[i], candidates2Error[i]);

		if(outFreq.GetnCandidates() == 0)
		{
			mFundFreqValue.SendControl(0.0f);
			return false;
		}

		// Added to get into account fundamental frequency for consecutive frames
		// Set Reference fundFreq to last FundFreq
		mReferenceFundFreq = outFreq.GetFreq(0);
		mFundFreqValue.SendControl( mReferenceFundFreq );

		return true;
	}
	
	TData FundFreqDetect::WeightCandidate(TData freq, const SpectralPeakArray& peaks, const DataArray& magFactor, const DataArray& magFactor34q, 
		const DataArray& magFactor4r, int maxNMP, int maxNPM) const
	{
			TData Tmp;
			const int nPeaks = peaks.GetIndexArray().Size();
			const IndexArray & peakIndexes = peaks.GetIndexArray();
			DataArray& peakFrequencies=peaks.GetFreqBuffer();
			//DataArray& peakMagnitudes=peaks.GetMagBuffer();
		
			TData ErrorPM = 0;
			TData ErrorMP = 0;

			TData HarmonicPm = freq;
			TSize nPM = maxNPM;
			TData lastFreq=peakFrequencies[peakIndexes[nPeaks-1]];
			
			// measured to predicted mismatch error 
			TData HarmonicMp = freq;
			TSize nMP = nPeaks;
			
			int Peak =0;
			int i;
			
			bool finishedPM = false;
			bool finishedMP = false;
			
			TData tenTimesFreq = freq*10.f;
			bool isFreqHigh = (freq>500.);
			TData oneOverFundFreq = 1./freq;
			
			for (i=0; i<nPeaks; i++)
			{
				if(!finishedPM)
				{
					if(i<maxNPM)
					{
						if (HarmonicPm > lastFreq)
						{
							nPM = i+1;
							finishedPM = true;
							if (finishedMP) break;
						}
						else
						{
							Peak = GetClosestPeak(HarmonicPm,Peak, peakIndexes, peakFrequencies);
							//xamat: does not depend on variable data: compute out of the loop!!
							TData Freq = peakFrequencies[peakIndexes[Peak]];
							//TData Mag  = peakMagnitudes[peakIndexes[Peak]];
							
							TData FreqDistance = Abs(Freq - HarmonicPm);
							//xamat: note that default value for mPMp is 0.5 thus yielding a sqrt
#ifdef CLAM_OPTIMIZE
							Tmp = FreqDistance / CLAM_sqrt(HarmonicPm);
#else							
							Tmp = FreqDistance * CLAM_pow(HarmonicPm, -mPMp);
#endif
							ErrorPM += (Tmp +magFactor[Peak] * (mPMq * Tmp - mPMr));
							HarmonicPm += freq;
						}
					}
				}
			
				if(!finishedMP)
				{
					TData Freq = TData(peakFrequencies[peakIndexes[i]]);
					// For high frequency candidates, not get into account too-low peaks
					if ( (isFreqHigh) && (Freq < 100))
						continue;
					
					HarmonicMp = GetClosestHarmonic(Freq,freq, oneOverFundFreq);
					TData FreqDistance = Abs(Freq - HarmonicMp);
					//xamat: note that default value for mMPp is 0.5 thus yielding a sqrt							
#ifdef CLAM_OPTIMIZE
					Tmp = FreqDistance / CLAM_sqrt(Freq);
#else					
					Tmp = FreqDistance * CLAM_pow(Freq, -mMPp);
#endif
					ErrorMP += magFactor34q[i] * Tmp - magFactor4r[i];
					if (Freq > tenTimesFreq){
						if (i > maxNMP)
						{
							nMP =	i+1;
							finishedMP = true;
							if (finishedPM) break;
						}
					}
				}
			}
	
		
		// total error
		if (ErrorPM > 20)
			ErrorPM = 20 + (ErrorPM-20)*(ErrorPM-20);
		if (ErrorMP > 20)
			ErrorMP = 20 + (ErrorMP-20)*(ErrorMP-20);

		return (mPMCont * ErrorPM/nPM + mMPCont * ErrorMP/nMP);
	}

	/* Get the closest peak to a given frequency 
	   and returns the number of the closest peak 
	   there's another parameter, peak, that contains the last peak taken   */
	int FundFreqDetect::GetClosestPeak(TData freq, int firstPeak, const IndexArray& peakIndexes, const DataArray & peakFrequencies) const
	{
		const int size = peakIndexes.Size();
		int bestpeak = firstPeak;
		TData distance = INFINITE_MAGNITUD;
		for (int peak=firstPeak; peak < size; peak++)
		{
			TData nextdistance = Abs(freq - peakFrequencies[peakIndexes[peak]]);
			if (nextdistance >= distance)
				return peak-1;
			bestpeak = peak; 
			distance=nextdistance;
		}
		return bestpeak;
	}

	/* Get Closest Harmonic */
	TData FundFreqDetect::GetClosestHarmonic(TData peak, TData fundfreq, TData oneOverFundfreq) const
	{
		if(peak<fundfreq)
			return fundfreq;
		//xamat: this should be optimized!
		return floor(peak*oneOverFundfreq+.5)*fundfreq;
	}

	bool FundFreqDetect::IsGoodCandidate(TData freq) const
	{
		return (freq >= mLowestFundFreq)  && (freq <= mHighestFundFreq);
	}

} // namespace CLAM

