#include "Complex.hxx"
#include "FundFreqDetect.hxx"
#include "ErrProcessingObj.hxx"
// Emilia: debug facilities
#include <stdio.h>

#define CLASS "FundFreqDetect"

#define MIN(a,b) ((a<=b)?(a):(b))
#define MAX(a,b) ((a>=b)?(a):(b))
#define INFINITE_MAGNITUD 1000000


namespace CLAM {

  /* The  Configuration object has at least to have a name */

	void FundFreqDetectConfig::DefaultInit()
	{
		/* the dynamic type takes care if we add an existing attr .. */

		AddAll();

		/* All Attributes are added */
		UpdateData();
		DefaultValues();
	}
	
	void FundFreqDetectConfig::DefaultValues()
	{
		
		/* Default values for attributes*/
		SetreferenceFundFreq(1000);
		SetlowestFundFreq(35);
		SethighestFundFreq(2000);
		SetcentralBandLF(1000);
		SetcentralBandHF(4000);
		SetmaxPeakMagDiff(60);
		SetmaxCandMagDiff(30);
		SetminBinWidth(2);
		SetmaxFundFreqError(10); // ??
		SetnInt(5);
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
		SetnMaxCandidates(60);
	}

  FundFreqDetect::FundFreqDetect()
	{
		Configure(FundFreqDetectConfig());
	}

	FundFreqDetect::FundFreqDetect(const FundFreqDetectConfig &c )
	{
		Configure(c);
	}

	FundFreqDetect::~FundFreqDetect()
	{}


  /* Configure the Processing Object according to the Config object */


	bool FundFreqDetect::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
	{
		mConfig = dynamic_cast<const FundFreqDetectConfig&>(c);	    

		mReferenceFundFreq = mConfig.GetreferenceFundFreq();
		mLowestFundFreq    = mConfig.GetlowestFundFreq();
		mHighestFundFreq   = mConfig.GethighestFundFreq();
		mCentralBandLF     = mConfig.GetcentralBandLF();
		mCentralBandHF     = mConfig.GetcentralBandHF();
		mMaxPeakMagDiff    = mConfig.GetmaxPeakMagDiff();
		mMaxCandMagDiff    = mConfig.GetmaxCandMagDiff();
		mMinBinWidth       = mConfig.GetminBinWidth();
		mMaxFundFreqError  = mConfig.GetmaxFundFreqError(); // ??
		mnInt              = mConfig.GetnInt();
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
		mnMaxCandidates= TData(mConfig.GetnMaxCandidates());
		
		return true;
	}

  const ProcessingConfig &FundFreqDetect::GetConfig() const
	{
		
		mConfig.SetcentralBandLF(mCentralBandLF);
		mConfig.SetcentralBandHF(mCentralBandHF);
		mConfig.SetmaxPeakMagDiff(mMaxPeakMagDiff);
		mConfig.SetmaxCandMagDiff(mMaxCandMagDiff);
	  mConfig.SetminBinWidth(mMinBinWidth);
		mConfig.SetmaxFundFreqError(mMaxFundFreqError); // ??
		mConfig.SetnInt(mnInt);
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
		mConfig.SetnMaxCandidates(TSize(mnMaxCandidates));
		return mConfig;
	}


  /* Setting Prototypes for faster processing */

	bool FundFreqDetect::SetPrototypes(const SpectralPeakArray& in)
	{
		return false;}
  
	bool FundFreqDetect::SetPrototypes()
	{
		return false;}

	bool FundFreqDetect::UnsetPrototypes()
	{
	  return false;
	}

  /* The supervised Do() function */

	bool  FundFreqDetect::Do(void) 
	{
		throw(ErrProcessingObj(CLASS"::Do(): Supervised mode not implemented"),this);
		return false;
	}
  
  /* The  unsupervised Do() function */

	bool  FundFreqDetect::Do(SpectralPeakArray& peaks,Fundamental& outFreq)
	{
	  // Check Number of Candidates required
	  if (outFreq.GetnMaxCandidates() <= 0)
	    throw Err("FundFreqDet::Detection: negative number of candidates wanted");
	  
	  // See if the number of best candidates to be calculated is less than the maximum permitted
	  if(outFreq.GetnMaxCandidates() > mnMaxCandidates)
	    throw Err("FundFreqDet::Detection:Number of candidates wanted bigger than the maximum configured on the algorithm");
 
	  
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
	  
	  // 1.- SELECT PEAKS: minimun bin width and a minimun magnitude (dBs)
	  //     peaks LowestFundFreq < f_peak < mCentralBandLF with a minimun binwidth
	  //     && all peaks mCentralBandLF < f_peak < mCentralBandHF
	  
	  // Add an index to the PeakArray
	  peaks.AddIndexArray();
	  peaks.AddIsIndexUpToDate();
	  peaks.UpdateData();
	  
	  
	  // Reset indices in the peak array
	  peaks.ResetIndices();

		// Recta de la forma y=a*x + b
		// a: pendiente de la recta
		// Before the maximum magnitude peak
		 TData x,y,a,b;
		TData spectralRes = peaks.GetFreq(nMaxMagPeak)/peaks.GetBinPos(nMaxMagPeak);
		a =  - spectralRes/TData(200.0);  
		b = maxMag - 20 - a*(double)peaks.GetBinPos(nMaxMagPeak);
		DataArray& peakMagBuffer=peaks.GetMagBuffer();
		DataArray& peakBinPosBuffer=peaks.GetBinPosBuffer();
		for(i=0; i<nMaxMagPeak; i++) {
			y = peakMagBuffer[i];
			x = peakBinPosBuffer[i];
			if( y < (a*x+b)) {
				peaks.DeleteIndex(i);
			}
			
		}
		// After the maximum magnitude peak
		// 1 dB each 1000 Hz	
		a = - spectralRes/TData(400.0);
		b = maxMag - 50 - a*(double)peaks.GetBinPos(nMaxMagPeak);
		DataArray& peakMagBuffer2=peaks.GetMagBuffer();
		DataArray& peakBinPosBuffer2=peaks.GetBinPosBuffer();
		for(i=nMaxMagPeak+1; i<peaks.GetnPeaks();i++) {
			y = peakMagBuffer2[i];
			x = peakBinPosBuffer2[i];
			if(y < (a*x+b)) {
				peaks.DeleteIndex(i);
			}
		}		

	  /*
			for(i=0; i<nMaxMagPeak; i++)
			if (peaks.GetMag(i) < (maxMag-mMaxPeakMagDiff/2) )  // magnitude
			peaks.DeleteIndex(i);
			for(i=nMaxMagPeak+1; i<peaks.GetnPeaks();i++) // 1 dB each 400 Hz por bin
			if(peaks.GetMag(i) <(maxMag-60-((peaks.GetFreq(i)/peaks.GetBinPos(i))/400)*(i-nMaxMagPeak)))
			peaks.DeleteIndex(i);
			
			  for (i=0; i<(peaks.GetnPeaks()); i++) 
						{ 
						if (peaks.GetMag(i) < (maxMag - mMaxPeakMagDiff) ) { // magnitude
						peaks.DeleteIndex(i);
						}
						else {                                             // frequency 
						f = peaks.GetFreq(i);
						if ( (f > mCentralBandHF) || (f < mLowestFundFreq) ) { // above the central band or below the minfundfreq
						peaks.DeleteIndex(i); 
						}
						else {
						if (f < mCentralBandLF ) // below the central band and above the minfundfreq
						if ((peaks.GetBinWidth(i) < mMinBinWidth) && (peaks.GetBinWidth(i)!=0)) { 
						// 0: not initialized
						peaks.DeleteIndex(i);
						}
						}
						}
						}
			*/
	  
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
	      // if ( fabs(peaks.GetThruIndexMag(i)- maxMag ) < mMaxCandMagDiff)
				if (IsGoodCandidate(freq))
					tmpFreq.AddElem(freq);
	      i++;
	    }
	  
	  // 2.4.- Frequency offset between peaks
	  i=0;
	  while( (i<peaks.GetIndexArray().Size()) && (tmpFreq.GetnCandidates()<mnMaxCandidates) )
	    {
	      if (i!=nMaxMagPeak) {  // 2.2
					j = i+1;
					while( (j<peaks.GetIndexArray().Size()) &&  (tmpFreq.GetnCandidates()<mnMaxCandidates)  )
						{
							freq = peaks.GetThruIndexFreq(j) - peaks.GetThruIndexFreq(i);
							if (IsGoodCandidate(freq))
								tmpFreq.AddElem(freq);
							j++;
						}
	      }
	      i++;
	    }
	  
	  // 2.5.- Frequencies related to peaks by integer ratios (before: except for the 3 maximun peaks. not now)
	  // (Not implemented in SMS)
	  i=0;
	  while( (i<peaks.GetIndexArray().Size()) && (tmpFreq.GetnCandidates()<mnMaxCandidates) )
	    {
	      j=1;
	      while( (j <= mnInt) &&  (tmpFreq.GetnCandidates()<mnMaxCandidates)  )
					{
						freq = peaks.GetThruIndexFreq(i)/j;
						if (IsGoodCandidate(freq))
							tmpFreq.AddElem(freq);
						j++;
					}
	      i++;
	    }
	  
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

		for (i=0;i<tmpFreq.GetnCandidates();i++)
	    if (i>0){
				int j; 
				bool addedNearOne=false;
				for(j=0; j<outFreq.GetnCandidates(); j++)
					if((tmpFreq.GetFreq(i)>0.95*outFreq.GetFreq(j)) && (tmpFreq.GetFreq(i)<1.1*outFreq.GetFreq(j))) {
						addedNearOne = true;
						if(outFreq.GetErr(j)> tmpFreq.GetErr(i)) {
							outFreq.SetFreq(j,tmpFreq.GetFreq(i));
							outFreq.SetErr(j,tmpFreq.GetErr(i));
						}
					}
				if((!addedNearOne)&&(outFreq.GetnCandidates()<outFreq.GetnMaxCandidates()))
					outFreq.AddElem(tmpFreq.GetFreq(i),tmpFreq.GetErr(i));
				if(outFreq.GetnCandidates()>=outFreq.GetnMaxCandidates()) 
					break;
			}				
			else
				if(outFreq.GetnCandidates()<outFreq.GetnMaxCandidates()) 
					outFreq.AddElem(tmpFreq.GetFreq(i),tmpFreq.GetErr(i));
	  
		// 5.- SEARCH AROUND FOR A RELATIVE MINIMUM
		for(i=0; i<outFreq.GetnCandidates(); i++)
			{
        TData Low  = outFreq.GetFreq(i)*TData(.9);
        TData High = outFreq.GetFreq(i)*TData(1.1);
        TData Incr = MAX(1.0,outFreq.GetFreq(i)*TData(.008)); 
        TData FinalPitch = outFreq.GetFreq(i);
        TData FinalError = outFreq.GetErr(i);
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
        outFreq.SetFreq(i,FinalPitch);
				outFreq.SetErr(i,FinalError);
			} 		

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
					Tmp = FreqDistance /sqrt(Harmonic); // if PMp is 0.5
					TData MagFactor = TData(MAX(0,MaxMag - Mag + 20));
					MagFactor = TData(1.0) - MagFactor/TData(75.0);
					if (MagFactor < 0)
						MagFactor = 0;
					ErrorPM += (Tmp +MagFactor * (mPMq * Tmp - mPMr));//*MagFactor;
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
				Mag = TData(peaks.GetThruIndexMag(Peak));
				Harmonic = TData(GetClosestHarmonic(Freq,freq));
				FreqDistance = fabs(Freq - Harmonic);
				//Tmp = FreqDistance * pow(Freq, -MPp);
				Tmp = FreqDistance / sqrt(Freq); // if 			MPp is 0.5
				TData MagFactor = TData(MAX(0,MaxMag - Mag + 20));
				MagFactor = TData(1.0) - MagFactor/TData(75.0);
				if (MagFactor < 0)
					MagFactor = 0;
				ErrorMP += (Tmp + MagFactor * (mMPq * Tmp - mMPr))*MagFactor*MagFactor;
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
		  
  /* Weight Candidate, returns the error for this candidate 
     using peak information 
		 double FundFreqDetect::WeightCandidate(double freq, double maxMag, SpectralPeakArray& peaks) const
		 {
		 int peak=0, i;
		 
		 // Predicted to Measured mismatch error (PM)
		 double errorPM = 0;
		 double harmonic = freq;
		 TSize maxNPM = MIN(mPMnPeaks,peaks.GetIndexArray().Size());
		 TSize nPM = maxNPM;
		 double fDistance, tmp, magFactor;
		 
		 for (i=0; i<maxNPM; i++)
		 {
		 if (harmonic > peaks.GetThruIndexFreq(peaks.GetIndexArray().Size()-1)) // if harmonic > highest peak
		 {
		 nPM = i+1;	// maxNPM+1
		 break;        // end of process
		 }
		 
		 peak = GetClosestPeak(harmonic, peak, peaks);
		 
		 fDistance = fabs(peaks.GetThruIndexFreq(peak) - harmonic);
		 tmp = fDistance * pow(harmonic, -mPMp);
		 magFactor = MAX(0, maxMag - (peaks.GetThruIndexMag(peak)) - 20);
		 magFactor = MAX(0, 1.0 - magFactor/75.0);
		 errorPM += (tmp + magFactor * (mPMq * tmp - mPMr))*magFactor; //! *magFactor; comented in SMS code
		 harmonic += freq; // predicted harmonic components
		 }
		 
		 // Measured to Predicted mismatch error (MP) 
		 double errorMP = 0;
		 TSize maxNMP = MIN(mMPnPeaks,peaks.GetIndexArray().Size());
		 harmonic = freq;
		 TSize nMP = peaks.GetIndexArray().Size();
		 for (peak=0; peak<nMP; peak++)
		 {
		 harmonic = GetClosestHarmonic(peaks.GetThruIndexFreq(peak), freq);
		 fDistance = fabs(peaks.GetThruIndexFreq(peak) - harmonic);
		 tmp = fDistance * pow(peaks.GetThruIndexFreq(peak), -mMPp);
		 magFactor = MAX(0,maxMag - (peaks.GetThruIndexMag(peak)) - 20);
		 magFactor = MAX(0, 1.0 - magFactor/75.0);
		 errorMP += (tmp + magFactor * (mMPq * tmp - mMPr))*magFactor;
		 if (peaks.GetThruIndexFreq(peak) > freq * 10) // peak > 10*f
		 if (peak > maxNMP)
		 {
		 nMP = peak+1;
		 break;
		 }
		 }
		 // total error
		 return(mPMCont * errorPM/nPM + mMPCont * errorMP/nMP);
		 }
	*/


/* Get the closest peak to a given frequency 
   and returns the number of the closest peak 
   there's another parameter, peak, that contains the last peak taken   */
int FundFreqDetect::GetClosestPeak(double freq, int peak,SpectralPeakArray& peaks) const
{
  int bestpeak = peak;
  bool found = false;
  double distance = INFINITE_MAGNITUD, nextdistance = 0;
  int size=peaks.GetIndexArray().Size();
  while ( (peak < size /*GetnPeaks()*/) && (!found) )
    {
      nextdistance = fabs(freq-peaks.GetThruIndexFreq(peak));
      if (nextdistance >= distance)
	{ 
	  bestpeak = peak-1;
	  found = true;
	}
      else {
	bestpeak = peak; // line added in the case when it never enter into if(nextDistance>=distance)
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
    return 0;
  else 
    return 1;
}


}; // namespace CLAM
