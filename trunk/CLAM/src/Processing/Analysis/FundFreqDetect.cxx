#include "Complex.hxx"
#include "FundFreqDetect.hxx"
#include "ErrProcessingObj.hxx"

#define CLASS "FundFreqDetect"

#define INFINITE_MAGNITUD 1000000

namespace CLAM {

  /* The  Configuration object has at least to have a name */

	void FundFreqDetectConfig::DefaultInit()
	{
		AddAll();
		/* All Attributes are added */
		UpdateData();
		/* Set default values  */
		DefaultValues();
	}
	
	void FundFreqDetectConfig::DefaultValues()
	{		
		SetReferenceFundFreq(1000);
		SetLowestFundFreq(35);
		SetHighestFundFreq(2000);
		SetMaxCandMagDiff(30);
		SetMaxFundFreqError(10); 
		SetNInt(5);
		SetPMp(0.5);
		SetPMq(TData(1.4));
		SetPMr(TData(0.5));
		SetMPp(0.5);
		SetMPq(TData(1.4));
		SetMPr(0.5);
		SetPMnPeaks(10);  
		SetMPnPeaks(10);
		SetPMCont(1.0);
		SetMPCont(1.0);  
		SetNMaxCandidates(60);
	}

	FundFreqDetect::FundFreqDetect()
	{
		Configure(FundFreqDetectConfig());
	}

	FundFreqDetect::FundFreqDetect(const FundFreqDetectConfig &c )
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
	bool  FundFreqDetect::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
  
  /* The  unsupervised Do() function */
	bool  FundFreqDetect::Do(SpectralPeakArray& peaks,Fundamental& outFreq)
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
		if (peaks.GetnPeaks() <= 0) return false;

		// Calculate Maximun Magnitude Peak
		TIndex nMaxMagPeak = peaks.GetMaxMagPos();
		double maxMag      = peaks.GetMag(nMaxMagPeak);
		int i, j;

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
		while((z<peaks.GetnPeaks()) && (peakBinPosBuffer[z]<lowestFundFreqBinPos)) {
			peaks.DeleteIndex(z);
			z++;
		}

		// Before the maximum magnitude peak
		for(i=z; i<nMaxMagPeak; i++) {
			if(peakMagBuffer[i] < maxMag - 30)
				peaks.DeleteIndex(i);
		}

		// Delete peaks above 3000
		z = peaks.GetnPeaks()-1;
		TData peaklimitBinPos = 3000.0/spectralRes;
		while ((z > nMaxMagPeak) && (peakBinPosBuffer[z] > peaklimitBinPos)) {
			peaks.DeleteIndex(z);
			z--;
		}
			
		// After the maximum magnitude peak
		TData x,y,a,b;
		a = - 10*spectralRes/TData(1000.0);
		b = maxMag - 50 - a*(double)peakBinPosBuffer[nMaxMagPeak];
		for(i=nMaxMagPeak+1; i<z; i++) {
			y = peakMagBuffer[i];
			x = peakBinPosBuffer[i];
			if(y < (a*x+b)) {
				peaks.DeleteIndex(i);
			}
		}		

	  // If there no valid peaks for calculate a fundamental frequency
	  if (peaks.GetIndexArray().Size() <= 0)
	    return false;
	  
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
	  i=0;
	  while ( (i < nMaxMagPeak) && (tmpFreq.GetnCandidates() < mnMaxCandidates) ) // be careful not to exceed the maximun permitted
	  {
	      if((i!=nMaxMagPeak2) && (i!=nMaxMagPeak3))
					if (peaks.GetThruIndexMag(i) > (maxMag - mMaxCandMagDiff) )
						if ( IsGoodCandidate(peaks.GetThruIndexFreq(i)) )
							tmpFreq.AddElem(peaks.GetThruIndexFreq(i));
	      i++;
	  }
	  
	  // 2.3.- Frequency offset between peaks above the maximun magnitude peak and the maximun magnitude peak
	  double freq;
	  i=nMaxMagPeak+1;
	  while ( (i<peaks.GetIndexArray().Size()) && (tmpFreq.GetnCandidates()<mnMaxCandidates) )
	  {
	      freq = peaks.GetThruIndexFreq(i) - peaks.GetThruIndexFreq(nMaxMagPeak);
	      if (freq < peaks.GetThruIndexFreq(nMaxMagPeak)*1.1)
					if (IsGoodCandidate(freq))
						tmpFreq.AddElem(freq);
	      i++;
	  }
	  
	  // 2.4.- Frequency offset between peaks
	  i=0;
	  while( (i<peaks.GetIndexArray().Size()) && (tmpFreq.GetnCandidates()<mnMaxCandidates) )
	  {
	      if (i!=nMaxMagPeak) {  
					j = i+1;
					while( (j<peaks.GetIndexArray().Size()) &&  (tmpFreq.GetnCandidates()<mnMaxCandidates)  )
						{
							freq = peaks.GetThruIndexFreq(j) - peaks.GetThruIndexFreq(i);
							if (freq < peaks.GetThruIndexFreq(nMaxMagPeak)*1.1)
								if (IsGoodCandidate(freq))
									tmpFreq.AddElem(freq);
							j++;
						}
	      }
	      i++;
	  }
	  
	  // 2.5.- Frequencies related to peaks by integer ratios (before: except for the 3 maximun peaks. not now)
	  i=0;
	  while( (i<peaks.GetIndexArray().Size()) && (tmpFreq.GetnCandidates()<mnMaxCandidates) )
	  {
	      j=1;
	      while( (j <= mnInt) &&  (tmpFreq.GetnCandidates()<mnMaxCandidates)  )
					{
						freq = peaks.GetThruIndexFreq(i)/j;
						if (freq < peaks.GetThruIndexFreq(nMaxMagPeak)*1.1)
							if (IsGoodCandidate(freq))
								tmpFreq.AddElem(freq);
						j++;
					}
	      i++;
	  }

		if(tmpFreq.GetnCandidates() <= 0)
			return false;

	  // 3.- CALCULATE ERRORS (TMW procedure)
    double myf=0, mye=0; 
	  for (i=0; i<tmpFreq.GetnCandidates(); i++)
	    {
	      myf = tmpFreq.GetFreq(i);
	      mye  = WeightCandidate(myf,maxMag,peaks);
	      tmpFreq.SetErr(i,TData(mye));
	    }
		
	  // 4.- CHOOSE THE BEST CANDIDATES: choose the FundFreq.NCandidates() candidates with the smallest error
	  for (i=0; i<tmpFreq.GetnCandidates(); i++) // Ordering
	    for (j=i+1; j<tmpFreq.GetnCandidates(); j++)
	      if (tmpFreq.GetErr(i) > tmpFreq.GetErr(j))
					tmpFreq.Exchange(i,j);
		
		Fundamental tmpFreq2;
		tmpFreq2.SetnMaxCandidates(tmpFreq.GetnCandidates());

		for (i=0;i<tmpFreq.GetnCandidates();i++)
	    if (i>0){
				int j; 
				bool addedNearOne=false;
				for(j=0; j<tmpFreq2.GetnCandidates(); j++)
					if((tmpFreq.GetFreq(i)>0.95*tmpFreq2.GetFreq(j)) && (tmpFreq.GetFreq(i)<1.1*tmpFreq2.GetFreq(j))) {
						addedNearOne = true;
						if(tmpFreq2.GetErr(j)> tmpFreq.GetErr(i)) {
							tmpFreq2.SetFreq(j,tmpFreq.GetFreq(i));
							tmpFreq2.SetErr(j,tmpFreq.GetErr(i));
						}
					}
				if(!addedNearOne)
					tmpFreq2.AddElem(tmpFreq.GetFreq(i),tmpFreq.GetErr(i));
			}		
			else
				tmpFreq2.AddElem(tmpFreq.GetFreq(i),tmpFreq.GetErr(i));
	  
		// 5.- SEARCH AROUND FOR A RELATIVE MINIMUM
		TData nMinimum = MIN(3,tmpFreq2.GetnCandidates());
		for(i=0; i<nMinimum; i++)
			{
        TData Low  = tmpFreq2.GetFreq(i)*TData(.9);
        TData High = tmpFreq2.GetFreq(i)*TData(1.1);
        TData Incr = MAX(1.0,tmpFreq2.GetFreq(i)*TData(.008)); 
        TData FinalPitch = tmpFreq2.GetFreq(i);
        TData FinalError = tmpFreq2.GetErr(i);
        TData lPitch,lErr;
				for(lPitch = Low; lPitch <=High; lPitch += Incr)
						{
							lErr = WeightCandidate(double(lPitch),maxMag,peaks);
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
		for (i=0; i<tmpFreq2.GetnCandidates(); i++) 
	    for (j=i+1; j<tmpFreq2.GetnCandidates(); j++)
	      if (tmpFreq2.GetErr(i) > tmpFreq2.GetErr(j))
					tmpFreq2.Exchange(i,j);

		TIndex nCandidates = MIN(outFreq.GetnMaxCandidates(),tmpFreq2.GetnCandidates());
		for(i=0; i<nCandidates; i++)
			if(tmpFreq2.GetErr(i) <= mMaxFundFreqError)
				outFreq.AddElem(tmpFreq2.GetFreq(i), tmpFreq2.GetErr(i));

		if(outFreq.GetnCandidates() == 0)
			return false;

	  // Added to get into account fundamental frequency for consecutive frames
	  // Set Reference fundFreq to last FundFreq
	  mReferenceFundFreq = outFreq.GetFreq(0);
	  return true;
	}
	
	double FundFreqDetect::WeightCandidate(double freq, double MaxMag, SpectralPeakArray& peaks) const
	{
		
		TData Mag, Freq, FreqDistance, Tmp;
		int Peak=0, i;
		int nPeaks = peaks.GetIndexArray().Size();
		
		// predicted to measured mismatch error
		TData ErrorPM = 0;
		int MaxNPM = 10;
		if (nPeaks  > 4)
			MaxNPM = MIN(mPMnPeaks,nPeaks);
		
		TData Harmonic = TData(freq);
		TSize nPM = MaxNPM;
		TData lastFreq=TData(peaks.GetThruIndexFreq(nPeaks-1));
		if (nPeaks > 0)
			for (i=0; i<MaxNPM; i++)
				{
					if (Harmonic > lastFreq)
					{
						nPM = i+1;               
						break;	
					}
					Peak = GetClosestPeak(Harmonic,Peak,peaks);
					Freq = TData(peaks.GetThruIndexFreq(Peak));
					Mag  = TData(peaks.GetThruIndexMag(Peak));
					FreqDistance = fabs(Freq - Harmonic);
					Tmp = FreqDistance *	pow(Harmonic, -mPMp);
					TData MagFactor = TData(MAX(0,MaxMag - Mag + 20));
					MagFactor = TData(1.0) - MagFactor/TData(75.0);
					if (MagFactor < 0)
						MagFactor = 0;
					ErrorPM += (Tmp +MagFactor * (mPMq * Tmp - mPMr));
					Harmonic +=TData(freq);
				}
 
		// measured to predicted mismatch error 
		TData ErrorMP = 0;
		int MaxNMP = MIN(mMPnPeaks,nPeaks);
		Harmonic = TData(freq);
		TSize nMP = nPeaks;
		for (Peak=0; Peak<nPeaks; Peak++)
			{
				Freq = TData(peaks.GetThruIndexFreq(Peak));
				// For high frequency candidates, not get into account too-low peaks
				if ( (freq > 500) && (Freq < 100))
					continue;

				Mag = TData(peaks.GetThruIndexMag(Peak));
				Harmonic = TData(GetClosestHarmonic(Freq,freq));
				FreqDistance = fabs(Freq - Harmonic);
				Tmp = FreqDistance * pow(Freq, -mMPp);
				TData MagFactor = TData(MAX(0,MaxMag - Mag + 20));
				MagFactor = TData(1.0) - MagFactor/TData(75.0);
				if (MagFactor < 0)
					MagFactor = 0;
				ErrorMP += (Tmp + MagFactor * (mMPq * Tmp - mMPr))*MagFactor*MagFactor*MagFactor;
				if (Freq > freq * 10)
					if (Peak > MaxNMP)
						{
							nMP =	Peak+1;
							break;
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
int FundFreqDetect::GetClosestPeak(double freq, int peak,SpectralPeakArray& peaks) const
{
	int bestpeak = peak;
	bool found = false;
	double distance = INFINITE_MAGNITUD, nextdistance = 0;
	int size=peaks.GetIndexArray().Size();
	while ( (peak < size) && (!found) )
	{
		nextdistance = fabs(freq-peaks.GetThruIndexFreq(peak));
		if (nextdistance >= distance)
		{ 
			bestpeak = peak-1;
			found = true;
		}
		else {
			bestpeak = peak; 
			distance=nextdistance;
			peak++;
		}
	}
	return bestpeak;
}

/* Get Closest Harmonic */
double FundFreqDetect::GetClosestHarmonic(double peak, double fundfreq) const
{
  double bestHarmonic = fundfreq, distance = INFINITE_MAGNITUD, nextdistance = 0;
  bool found = false;

  while(!found)
    {
		nextdistance = fabs(bestHarmonic - peak);
		if (nextdistance >= distance)
		{
			bestHarmonic -= fundfreq;
			found = true; 
		}
		else {
				distance = nextdistance;
				bestHarmonic += fundfreq;      
		}
    }
  return bestHarmonic;
}

bool FundFreqDetect::IsGoodCandidate(double freq) const
{
  if ( (freq < mLowestFundFreq) | (freq > mHighestFundFreq) )
    return false;
  else 
    return true;
}

}; // namespace CLAM
