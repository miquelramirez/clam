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
		: mInput( "Input", this),
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

	/* Setting Prototypes for faster processing */
	bool FundFreqDetect::SetPrototypes(const SpectralPeakArray& in)
	{return false;}

	bool FundFreqDetect::SetPrototypes()
	{return false;}

	bool FundFreqDetect::UnsetPrototypes()
	{return false;}

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

		Fundamental tmpFreq; // this will be used throughout the algorithm to allocate new candidates
		tmpFreq.AddCandidatesFreq();
		tmpFreq.AddCandidatesErr();
		tmpFreq.UpdateData();
		tmpFreq.SetnCandidates(0);
		tmpFreq.SetnMaxCandidates(int(mnMaxCandidates));

		// not enough peak information available for fundamental frequency detection");
		if (peaks.GetnPeaks() <= 0)
		{
			mFundFreqValue.SendControl(0.0f);
			return false;
		}

		// Calculate Maximun Magnitude Peak
		TIndex nMaxMagPeak = peaks.GetMaxMagPos();
		double maxMag      = peaks.GetMag(nMaxMagPeak);
		int j;

		// 1.- SELECT PEAKS
		// Add an index to the PeakArray
		if(!peaks.HasIndexArray())
		{
			peaks.AddIndexArray();
			peaks.AddIsIndexUpToDate();
			peaks.UpdateData();
			peaks.SetnMaxPeaks(peaks.GetnMaxPeaks());
		}

		// Reset indices in the peak array
		peaks.ResetIndices();

		// Delete peaks below the lowest pitch
		TData spectralRes = peaks.GetFreq(nMaxMagPeak)/peaks.GetBinPos(nMaxMagPeak);
		DataArray& peakMagBuffer=peaks.GetMagBuffer();
		DataArray& peakBinPosBuffer=peaks.GetBinPosBuffer();
		
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
			if(peakMagBuffer[i] < maxMag - 30)
				peaks.DeleteIndex(i);
		}

		// Delete peaks above 3000
		z = peaks.GetnPeaks()-1;
		TData peaklimitBinPos = 3000.0/spectralRes;
		while ((z > nMaxMagPeak) && (peakBinPosBuffer[z] > peaklimitBinPos)) 
		{
			peaks.DeleteIndex(z);
			z--;
		}
			
		// After the maximum magnitude peak
		TData x,y,a,b;
		a = - 10*spectralRes/TData(1000.0);
		b = maxMag - 50 - a*(double)peakBinPosBuffer[nMaxMagPeak];
		for(int i=nMaxMagPeak+1; i<z; i++) 
		{
			y = peakMagBuffer[i];
			x = peakBinPosBuffer[i];
			if(y < (a*x+b)) 
			{
				peaks.DeleteIndex(i);
			}
		}		

		// If there no valid peaks for calculate a fundamental frequency
		if (peaks.GetIndexArray().Size() <= 0)
		{
			mFundFreqValue.SendControl(0.0f);
			return false;
		}
	  
		// Find maximun magnitude peak from the selected ones
		nMaxMagPeak = peaks.GetMaxMagIndex(); // only indexed peaks
		maxMag      = peaks.GetThruIndexMag(nMaxMagPeak);

		// 2.- FIND mnMaxCandidates CANDIDATES
	  
		// 2.0.- Reference Fundamental Frequency
		if( IsGoodCandidate(mReferenceFundFreq) ) 
			tmpFreq.AddElem(mReferenceFundFreq);
	  
		// 2.1.- Three maximum magnitude peaks and its integer ratios
		TIndex nMaxMagPeak2 = nMaxMagPeak;
		TIndex nMaxMagPeak3 = nMaxMagPeak;
		if(peaks.GetIndexArray().Size() >= 2)
		{
	  		// find second max magnitude peak
		  	peaks.SetThruIndexMag(nMaxMagPeak,-2000);
		      	nMaxMagPeak2 = peaks.GetMaxMagIndex();
			if(peaks.GetIndexArray().Size() >= 3)
			{
				double aux;
				// find third max magnitude peak
				aux = peaks.GetThruIndexMag(nMaxMagPeak2);
				peaks.SetThruIndexMag(nMaxMagPeak2,-2000);
				nMaxMagPeak3 = peaks.GetMaxMagIndex();
				// add candidate
				if ( IsGoodCandidate(peaks.GetThruIndexFreq(nMaxMagPeak3)) )
					tmpFreq.AddElem(peaks.GetThruIndexFreq(nMaxMagPeak3));
				// restore second peak information
				peaks.SetThruIndexMag(nMaxMagPeak2,aux);
			}
			// add candidate
			if ( IsGoodCandidate(peaks.GetThruIndexFreq(nMaxMagPeak2)) )
				tmpFreq.AddElem(peaks.GetThruIndexFreq(nMaxMagPeak2));
			
			// restore first peak information
			peaks.SetThruIndexMag(nMaxMagPeak,maxMag);
		}	  
		// Add peaks as candidates
		if ( IsGoodCandidate(peaks.GetThruIndexFreq(nMaxMagPeak)) )
			tmpFreq.AddElem(peaks.GetThruIndexFreq(nMaxMagPeak));	
	  
		// 2.2.- Peaks below the maximum magnitude peak (except for the 3 max peaks)
		for (int i=0; (i < nMaxMagPeak) && (tmpFreq.GetnCandidates() < mnMaxCandidates); i++ ) // be careful not to exceed the maximun permitted
		{
			if(i==nMaxMagPeak2) continue;
			if (i==nMaxMagPeak3) continue;
			if (peaks.GetThruIndexMag(i) <= maxMag - mMaxCandMagDiff ) continue;
			if (! IsGoodCandidate(peaks.GetThruIndexFreq(i)) ) continue;
			tmpFreq.AddElem(peaks.GetThruIndexFreq(i));
		}
	  	
		// 2.3.- Frequency offset between peaks above the maximun magnitude peak and the maximun magnitude peak
		double freq;
		for (int i = nMaxMagPeak+1; (i<peaks.GetIndexArray().Size()) && (tmpFreq.GetnCandidates()<mnMaxCandidates); i++)
		{
			freq = peaks.GetThruIndexFreq(i) - peaks.GetThruIndexFreq(nMaxMagPeak);
			if (freq >= peaks.GetThruIndexFreq(nMaxMagPeak)*1.1) continue;
			if (!IsGoodCandidate(freq)) continue;
			tmpFreq.AddElem(freq);
		}
		
		// 2.4.- Frequency offset between peaks
		for (int i = 0; (i<peaks.GetIndexArray().Size()) && (tmpFreq.GetnCandidates()<mnMaxCandidates); i++ )
		{
			if (i==nMaxMagPeak) continue;
			j = i+1;
			while( (j<peaks.GetIndexArray().Size()) && (tmpFreq.GetnCandidates()<mnMaxCandidates) )
			{
				freq = peaks.GetThruIndexFreq(j) - peaks.GetThruIndexFreq(i);
				if (freq < peaks.GetThruIndexFreq(nMaxMagPeak)*1.1)
					if (IsGoodCandidate(freq))
						tmpFreq.AddElem(freq);
				j++;
			}
		}
	  
		// 2.5.- Frequencies related to peaks by integer ratios (before: except for the 3 maximun peaks. not now)	
		for (int i=0; (i<peaks.GetIndexArray().Size()) && (tmpFreq.GetnCandidates()<mnMaxCandidates); i++ )
		{
			j=1;
			while( (j <= mnInt) && (tmpFreq.GetnCandidates()<mnMaxCandidates) )
			{
				freq = peaks.GetThruIndexFreq(i)/j;
				if (freq < peaks.GetThruIndexFreq(nMaxMagPeak)*1.1)
					if (IsGoodCandidate(freq))
						tmpFreq.AddElem(freq);
				j++;
			}
		}
	
		if(tmpFreq.GetnCandidates() <= 0)
		{
			mFundFreqValue.SendControl(0.0f);
			return false;
		}

		// 3.- CALCULATE ERRORS (TMW procedure)
		double myf=0, mye=0; 
		for (int i=0; i<tmpFreq.GetnCandidates(); i++)
		{
			myf = tmpFreq.GetFreq(i);
			mye = WeightCandidate(myf,maxMag,peaks);
			tmpFreq.SetErr(i,TData(mye));
		}
		
		// 4.- CHOOSE THE BEST CANDIDATES: choose the FundFreq.NCandidates() candidates with the smallest error
		for (int i=0; i<tmpFreq.GetnCandidates(); i++) // Ordering
			for (j=i+1; j<tmpFreq.GetnCandidates(); j++)
				if (tmpFreq.GetErr(i) > tmpFreq.GetErr(j))
					tmpFreq.Exchange(i,j);
		Fundamental tmpFreq2;
		tmpFreq2.SetnMaxCandidates(tmpFreq.GetnCandidates());

		for (int i=0;i<tmpFreq.GetnCandidates();i++)
		{
			if (i<=0)
			{
				tmpFreq2.AddElem(tmpFreq.GetFreq(i),tmpFreq.GetErr(i));
				continue;
			}
			bool addedNearOne=false;
			for(int j=0; j<tmpFreq2.GetnCandidates(); j++)
			{
				if (tmpFreq.GetFreq(i)<=0.95*tmpFreq2.GetFreq(j)) continue;
				if (tmpFreq.GetFreq(i)>=1.1*tmpFreq2.GetFreq(j)) continue;
				addedNearOne = true;
				if(tmpFreq2.GetErr(j)> tmpFreq.GetErr(i))
				{
					tmpFreq2.SetFreq(j,tmpFreq.GetFreq(i));
					tmpFreq2.SetErr(j,tmpFreq.GetErr(i));
				}
			}
			if(!addedNearOne)
				tmpFreq2.AddElem(tmpFreq.GetFreq(i),tmpFreq.GetErr(i));
		}
		
		// 5.- SEARCH AROUND FOR A RELATIVE MINIMUM
		TData nMinimum = std::min(3,tmpFreq2.GetnCandidates());
		for(int i=0; i<nMinimum; i++)
		{
			TData Low  = tmpFreq2.GetFreq(i)*TData(.9);
			TData High = tmpFreq2.GetFreq(i)*TData(1.1);
			TData Incr = std::max(TData(1.0),tmpFreq2.GetFreq(i)*TData(.008)); 
			TData FinalPitch = tmpFreq2.GetFreq(i);
			TData FinalError = tmpFreq2.GetErr(i);
			for(TData lPitch = Low; lPitch <=High; lPitch += Incr)
			{
				TData lErr = WeightCandidate(double(lPitch),maxMag,peaks);
				if (lPitch > peaks.GetThruIndexFreq(nMaxMagPeak)*1.1)
					lErr +=10;
				if (lErr < FinalError)
				{
					FinalPitch = lPitch;
					FinalError = lErr;
				}
			}
			tmpFreq2.SetFreq(i,FinalPitch);
			tmpFreq2.SetErr(i,FinalError);
		} 		

		// Ordering the minimum
		for (int i=0; i<tmpFreq2.GetnCandidates(); i++) 
			for (j=i+1; j<tmpFreq2.GetnCandidates(); j++)
				if (tmpFreq2.GetErr(i) > tmpFreq2.GetErr(j))
					tmpFreq2.Exchange(i,j);

		TIndex nCandidates = std::min(outFreq.GetnMaxCandidates(),tmpFreq2.GetnCandidates());
		for(int i=0; i<nCandidates; i++)
			if(tmpFreq2.GetErr(i) <= mMaxFundFreqError)
				outFreq.AddElem(tmpFreq2.GetFreq(i), tmpFreq2.GetErr(i));

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
	
	double FundFreqDetect::WeightCandidate(double freq, double MaxMag, SpectralPeakArray& peaks) const
	{
		TData Tmp;
		const int nPeaks = peaks.GetIndexArray().Size();
		
		// predicted to measured mismatch error
		TData ErrorPM = 0;
		int MaxNPM = 10;
		if (nPeaks > 4)
			MaxNPM = std::min(mPMnPeaks,nPeaks);
		TData Harmonic = TData(freq);
		TSize nPM = MaxNPM;
		int Peak =0;
		TData lastFreq=TData(peaks.GetThruIndexFreq(nPeaks-1));
		if (nPeaks > 0)
			for (int i=0; i<MaxNPM; i++)
			{
				if (Harmonic > lastFreq)
				{
					nPM = i+1;
					break;	
				}
				Peak = GetClosestPeak(Harmonic,Peak,peaks);
				TData Freq = TData(peaks.GetThruIndexFreq(Peak));
				TData Mag  = TData(peaks.GetThruIndexMag(Peak));
				TData FreqDistance = fabs(Freq - Harmonic);
				Tmp = FreqDistance *	pow(Harmonic, -mPMp);
				TData MagFactor = TData(std::max(0.0,MaxMag - Mag + 20.0));
				MagFactor = TData(1.0) - MagFactor/TData(75.0);
				if (MagFactor < 0)
					MagFactor = 0;
				ErrorPM += (Tmp +MagFactor * (mPMq * Tmp - mPMr));
				Harmonic +=TData(freq);
			}

		// measured to predicted mismatch error 
		TData ErrorMP = 0;
		int MaxNMP = std::min(mMPnPeaks,nPeaks);
		Harmonic = TData(freq);
		TSize nMP = nPeaks;
		for (int Peak=0; Peak<nPeaks; Peak++)
		{
			TData Freq = TData(peaks.GetThruIndexFreq(Peak));
			// For high frequency candidates, not get into account too-low peaks
			if ( (freq > 500) && (Freq < 100))
				continue;

			TData Mag = TData(peaks.GetThruIndexMag(Peak));
			Harmonic = TData(GetClosestHarmonic(Freq,freq));
			TData FreqDistance = fabs(Freq - Harmonic);
			Tmp = FreqDistance * pow(Freq, -mMPp);
			TData MagFactor = TData(std::max(0.0,MaxMag - Mag + 20.0));
			MagFactor = TData(1.0) - MagFactor/TData(75.0);
			if (MagFactor < 0)
				MagFactor = 0;
			ErrorMP += (Tmp + MagFactor * (mMPq * Tmp - mMPr))*MagFactor*MagFactor*MagFactor;
			if (Freq <= freq * 10) continue;
			if (Peak <= MaxNMP) continue;
			nMP =	Peak+1;
			break;
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
int FundFreqDetect::GetClosestPeak(double freq, int firstPeak, SpectralPeakArray& peaks) const
{
	const int size=peaks.GetIndexArray().Size();
	int bestpeak = firstPeak;
	double distance = INFINITE_MAGNITUD;
	for (int peak=firstPeak; peak < size; peak++)
	{
		const double nextdistance = fabs(freq - peaks.GetThruIndexFreq(peak));
		if (nextdistance >= distance)
			return peak-1;
		bestpeak = peak; 
		distance=nextdistance;
	}
	return bestpeak;
}

/* Get Closest Harmonic */
double FundFreqDetect::GetClosestHarmonic(double peak, double fundfreq) const
{
	if(peak<fundfreq)
		return fundfreq;
	return floor(peak/fundfreq+.5)*fundfreq;
}

bool FundFreqDetect::IsGoodCandidate(double freq) const
{
	return (freq >= mLowestFundFreq)  && (freq <= mHighestFundFreq);
}

} // namespace CLAM

