#include "SinTracking.hxx"
#include "ErrProcessingObj.hxx"

using namespace CLAM;

  
	void SinTrackingConfig::DefaultInit()
	{
		AddAll();
		UpdateData();

		DefaultValues();
		
	}

	void SinTrackingConfig::DefaultValues()
	{
		SetThreshold(20);//in percents
		SetnMaxSines(50);
		SetIsHarmonic(0);
	}
	
	SinTracking::SinTracking()
	{
		Configure(SinTrackingConfig());
	}

	SinTracking::SinTracking(const SinTrackingConfig &c )
	{
		Configure(c);
	}

	SinTracking::~SinTracking()
	{}




/* Configure the Processing Object according to the Config object */

	bool SinTracking::ConcreteConfigure(const ProcessingConfig& c)
	{	    

		CopyAsConcreteConfig(mConfig, c);

		mnMaxSines = mConfig.GetnMaxSines();

		mThreshold= mConfig.GetThreshold();

		mHarmonic= mConfig.GetIsHarmonic();

		mnActiveGuides=0;

		mNextTrackId=0;

		mInitialized=false;
		mLastHarmonic=false;
		
		int i;
		//initializes guide array
		mGuideArray.Resize(mnMaxSines);

		mGuideArray.SetSize(mnMaxSines);
		for(i=0;i<mnMaxSines;i++)
		{
			mGuideArray[i].isDead=true;
		}
		
		return true;
	}




//Process

	
	//Supervised mode
	bool  SinTracking::Do(void) 
	{
		throw(ErrProcessingObj("SinTracking::Do(): Supervised mode not implemented"),this);
		return false;
	}
  

	bool SinTracking::Do(const SpectralPeakArray& iPeakArray,SpectralPeakArray& oPeakArray,TData fn)
	{
		//oPeakArray initialization		
		oPeakArray.AddIndexArray();
		oPeakArray.AddPhaseBuffer();
		oPeakArray.AddBinWidthBuffer();
		oPeakArray.AddBinPosBuffer();
		oPeakArray.AddIsIndexUpToDate();
		oPeakArray.UpdateData();
		if(mHarmonic && fn>0){
			mLastHarmonic=true;
			return DoHarmonic(iPeakArray,oPeakArray,fn);}
		else{
			if(mLastHarmonic) KillAll();
			mLastHarmonic=false;
			return DoInharmonic(iPeakArray,oPeakArray);}
	}

	bool SinTracking::Do(const SpectralPeakArray& iPeakArray,SpectralPeakArray& oPeakArray)
	{
		//oPeakArray initialization		
		oPeakArray.AddIndexArray();
		oPeakArray.AddPhaseBuffer();
		oPeakArray.AddBinWidthBuffer();
		oPeakArray.AddBinPosBuffer();
		oPeakArray.AddIsIndexUpToDate();
		oPeakArray.UpdateData();
		return DoInharmonic(iPeakArray,oPeakArray);
	}


	void SinTracking::AddNewTrack(int peakPosition, const SpectralPeak& currentPeak,SpectralPeakArray& oPeakArray) const
	{
	  int i;
	  for(i=0;i<mnMaxSines;i++)
	  {
		if(mGuideArray[i].isDead==true)
		{
		  mGuideArray[i].isDead=false;
		  mGuideArray[i].trackId=mNextTrackId;
		  mGuideArray[i].freq=currentPeak.GetFreq();
		  mGuideArray[i].mag=currentPeak.GetMag();
		  oPeakArray.SetSpectralPeak(peakPosition,currentPeak, mNextTrackId);
		  mNextTrackId++;
		  mnActiveGuides++;
		  break;
		}
	  }
    
	}



	void SinTracking::Tracking(const SpectralPeakArray& iPeakArray,SpectralPeakArray& oPeakArray,TIndex processedPeakPos) const
	{
	  bool notFinished=true;
	  int candidatePos;
	  TData distance;
	  SpectralPeak currentPeak;		
	  currentPeak=mPreviousPeakArray.GetSpectralPeak(processedPeakPos);
	  if(!ThereIsCandidate(currentPeak,iPeakArray,oPeakArray))
	  {
		//Kill Track
		KillTrack(mPreviousPeakArray.GetIndex(processedPeakPos));
	  }
	  else
	  {
		candidatePos=GetCandidate(currentPeak,iPeakArray,distance);
		if(candidatePos==-1) return;
		SpectralPeak candidatePeak;
		candidatePeak=iPeakArray.GetSpectralPeak(candidatePos);
		//I added &&(candidatePeak.GetIndex()==-1) because it matched already matched peak
		if(candidatePos<mnMaxSines&&(IsBestCandidate(candidatePeak,processedPeakPos))&&(oPeakArray.GetIndex(candidatePos)==-1))
		{
		  //Match
		  Match(mPreviousPeakArray.GetIndex(processedPeakPos),candidatePos,candidatePeak,oPeakArray);
		}
		else
		{
		//seems useless. To be checked....
		/*candidatePos--;//Try with previous peak in array
		  if(candidatePos==-1)&&(candidatePeak.GetIndex()==-1)
		  {
			KillTrack(mPreviousPeakArray.GetIndex(processedPeakPos));
		  }
		  else
		  {
			candidatePeak=iPeakArray.GetSpectralPeak(candidatePos);
			if(IsBestCandidate(candidatePeak,processedPeakPos))
			{
			 //Match
			 Match(mPreviousPeakArray.GetIndex(processedPeakPos),
			   candidatePos,candidatePeak,oPeakArray);
			}
			else
			{
			 //Kill Track
			 KillTrack(mPreviousPeakArray.GetIndex(processedPeakPos));
			}
		  }*/
			KillTrack(mPreviousPeakArray.GetIndex(processedPeakPos));
		}
	  }

	}



//true as soon as the distance between currentPeak and a Peak in iPeakArray is <mThreshold
	bool SinTracking::ThereIsCandidate(const SpectralPeak& currentPeak,
										const SpectralPeakArray& iPeakArray,SpectralPeakArray& oPeakArray) const
	{
	  int i;
	  int dist;
	  TSize nInputPeaks=iPeakArray.GetnPeaks();
	  if(nInputPeaks>mnMaxSines) nInputPeaks=mnMaxSines;
	  DataArray& peakFreqBuffer=iPeakArray.GetFreqBuffer();	  
	  TData currentFreq=currentPeak.GetFreq();
	  TData factor=100/currentFreq;
	  IndexArray& outputIndexArray=oPeakArray.GetIndexArray();
	  for (i=0;i<nInputPeaks;i++)
	  {
		dist=int(Abs(peakFreqBuffer[i]-currentFreq)*factor);
		if((dist< mThreshold)&&(outputIndexArray[i]==-1)) return true;
	  }
	  return false;
	}


//Sets mGuideArray.isDead to true and mnActiveGuides--
	void SinTracking::KillTrack(int trackId) const
	{
	  int i;
	  for(i=0;i<mnMaxSines;i++)
	  {
		if(mGuideArray[i].trackId==trackId)
		{
		  mGuideArray[i].isDead=true;
		  mnActiveGuides--;
		  break;
		}
	  }
	}

//Return the position of the peak in iPeakArray which is the closest to currentPeak
	TIndex SinTracking::GetCandidate(const SpectralPeak& currentPeak, 
										   const SpectralPeakArray& iPeakArray,
										   TData& distance) const
	{
	  //Can be optimized! XA
	  TIndex i,bestCandidate=-1;
	  TData tmpDistance;
	  distance=-1;
	  int nPeaks=iPeakArray.GetnPeaks();
	  DataArray peakFreqBuffer=iPeakArray.GetFreqBuffer();
	  TData currentFreq=currentPeak.GetFreq();
	  TData factor=100/currentFreq;
	  for (i=0;i<nPeaks;i++)
	  {
		tmpDistance=Abs(peakFreqBuffer[i]-currentFreq)*factor;
		if((distance==-1)/*test: XA &&(tmpDistance<mThreshold)*/ || 
		  (tmpDistance < distance))
		{
		  distance=tmpDistance;
		  bestCandidate=i;
		}
	  }
	  //CLAM_ASSERT (bestCandidate!=-1,"SinTracking::GetCandidate: No candidate!");
	  return bestCandidate;

	}



//true if there is no peak in previousPeakArray closer to candidate

	bool SinTracking::IsBestCandidate(const SpectralPeak& candidate, 
									   int nMatchedPeaksInPreviousFrame) const
	{
	  int i;
	  SpectralPeak tmpPeak;
	  tmpPeak=mPreviousPeakArray.GetSpectralPeak(nMatchedPeaksInPreviousFrame);
	  double nextDistance=(tmpPeak|candidate);
	  int nPeaks=mPreviousPeakArray.GetnPeaks();
	  DataArray& peakFreqBuffer=mPreviousPeakArray.GetFreqBuffer();
	  TData candidateFreq=candidate.GetFreq();
	  for(i=0;i<nPeaks;i++)
	  {
		if(Abs(peakFreqBuffer[i]-candidateFreq)<nextDistance) return false;
	  }
	  return true;

	}


	
//Set the best candidate in oPeakArray at the same position as in iPeakArray
//with the same index as the peak it continues
	void SinTracking::Match(TIndex trackId, TIndex peakIndex,
							 const SpectralPeak& currentPeak, 
							 SpectralPeakArray& oPeakArray) const
	{
	  CLAM_ASSERT(peakIndex<oPeakArray.GetnPeaks(),"SinTracking::Match: Not a valid peak Index");
	  oPeakArray.SetSpectralPeak(peakIndex,currentPeak,trackId);
	  
	}



/*Function to check peaks in next frame that have not been matched to peaks in current
frame. These are then assigned to newborn tracks.*/
	void SinTracking::CheckForNewBornTracks(const SpectralPeakArray& iPeakArray,SpectralPeakArray& oPeakArray) const
	{
	  TIndex nonAssignedPeakIndex=0;
	  SpectralPeak tmpPeak;
	  bool notFinished=true;
	  nonAssignedPeakIndex=GetFirstNonAssignedPeakPos(oPeakArray,nonAssignedPeakIndex);
	  while(notFinished)
	  {
	   if(nonAssignedPeakIndex == -1)
	   {
		 notFinished=false;
		 break;
	   }
	   else
	   {
		 /*Note that when a new track is born the Id assigned is next index. This gives
		 us and idea of the number of total tracks that have been born and died up to
		 now, unique ID*/
		 AddNewTrack(nonAssignedPeakIndex,iPeakArray.GetSpectralPeak(nonAssignedPeakIndex),oPeakArray);
		 nonAssignedPeakIndex++;
		 nonAssignedPeakIndex=GetFirstNonAssignedPeakPos(oPeakArray,nonAssignedPeakIndex);
	   }
	  }

	}


/*Returns index of first peak that has not been assigned to a track (-1 if all have
already been assigned)*/
//beginAt=0? why not starting from the previous new track index?
	TIndex SinTracking::GetFirstNonAssignedPeakPos(const SpectralPeakArray& oPeakArray,
													  TIndex beginAt=0) const
	{
	  int i;
	  TIndex assigned=1;
	  if(beginAt>=oPeakArray.GetnPeaks()||beginAt<0) return -1;
	  i=oPeakArray.GetFirstNonValidIndexPosition(beginAt);
	  if(i==(oPeakArray.GetnPeaks())) i=-1;//All peaks have been matched
	  return i;
	}

	
//Initialization of the first output peak array: all peaks must be assigned a track
	void SinTracking::Initialization(const SpectralPeakArray& iPeakArray, SpectralPeakArray& oPeakArray)
	{
		int i;
		TSize nPeaks=oPeakArray.GetnPeaks();
		for(i=0; i<nPeaks; i++)
		{
			AddNewTrack(i, iPeakArray.GetSpectralPeak(i), oPeakArray);
		}
		mPreviousPeakArray=oPeakArray;
	}

	void SinTracking::KillAll()
	{
		int i;
		for (i=0;i<mnMaxSines;i++)
		{
			mGuideArray[i].isDead=true;
		}
		mnActiveGuides=0;
	}

	bool SinTracking::DoInharmonic(const SpectralPeakArray& iPeakArray,SpectralPeakArray& oPeakArray)
	{
		if(iPeakArray.GetnPeaks()<mnMaxSines)
			oPeakArray.SetnPeaks(iPeakArray.GetnPeaks());
		else
			oPeakArray.SetnPeaks(mnMaxSines);

		oPeakArray.ResetIndices();
		oPeakArray.InitIndices();
		oPeakArray.SetScale(EScale(EScale::eLog));
		
		if(!mInitialized)
		{
			Initialization(iPeakArray, oPeakArray);
			mInitialized=true;
			return true;
		}
		
		int i;
		oPeakArray.SetIsIndexUpToDate(true);
		for(i=0;i<mPreviousPeakArray.GetnPeaks();i++)
		{
			Tracking(iPeakArray,oPeakArray,i);
		}
		CheckForNewBornTracks(iPeakArray,oPeakArray);
		mPreviousPeakArray=oPeakArray;
		return true;
	}


/* Harmonic Peak Continuation */
	bool SinTracking::DoHarmonic(const SpectralPeakArray& in, SpectralPeakArray& out,TData funFreq)
	{
		out.SetnPeaks(mnMaxSines);

		out.ResetIndices();
		out.SetScale(EScale(EScale::eLog));
		
		InitHarmonicTracks(out,funFreq);
		out.SetIsIndexUpToDate(true);
		HarmonicTracking(in, out, funFreq);
		mPreviousPeakArray=out;
		return true;

	}

	void SinTracking::HarmonicTracking(const SpectralPeakArray& in,SpectralPeakArray& out,TData funFreq)
	{
		TData d;
		TIndex pos;
		DataArray& iFreqBuffer=in.GetFreqBuffer();
		DataArray& oFreqBuffer=out.GetFreqBuffer();
		DataArray& iMagBuffer=in.GetMagBuffer();
		DataArray& oMagBuffer=out.GetMagBuffer();
		DataArray& iPhaseBuffer=in.GetPhaseBuffer();
		DataArray& oPhaseBuffer=out.GetPhaseBuffer();

		
		int i;

		/*for(i=0;i<mnMaxSines;i++)
		{
			pos=GetCandidate(out.GetSpectralPeak(i),in,d);
			if(d<funFreq/2 && pos>-1)
			{
				oMagBuffer[i]=iMagBuffer[pos];
				oFreqBuffer[i]=iFreqBuffer[pos];
				oPhaseBuffer[i]=iPhaseBuffer[pos];

			}
		}*/

		//GILLES
		TSize nPeaks=mnMaxSines;
		i=0;
		do
		{
			pos=GetCandidate(out.GetSpectralPeak(i),in,d);
			if(d<funFreq/2 && pos>-1)
			{
				if(i==0 || iMagBuffer[pos]!=oMagBuffer[i-1])
				{
					oMagBuffer[i]=iMagBuffer[pos];
					//XA: oFreqBuffer[i]=iFreqBuffer[pos];
					oPhaseBuffer[i]=iPhaseBuffer[pos];
					i++;
				}
				else
				{
					out.DeleteSpectralPeak(i);
					nPeaks=nPeaks-1;
					out.SetnPeaks(nPeaks);
				}
			}
			else
			{
				out.DeleteSpectralPeak(i);
				nPeaks=nPeaks-1;
				out.SetnPeaks(nPeaks);
			}
		}while(i<out.GetnPeaks());
	}
	
	void SinTracking::InitHarmonicTracks(SpectralPeakArray& peaks, TData funFreq)
	{
		DataArray& freqBuffer=peaks.GetFreqBuffer();
		DataArray& magBuffer=peaks.GetMagBuffer();
		
		int i;
		
		TData currentFreq=funFreq;

		for(i=0;i<mnMaxSines;i++)
		{
			freqBuffer[i]=currentFreq;
			magBuffer[i]=-99;
			currentFreq+=funFreq;
		}
	}
