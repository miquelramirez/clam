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

#include "SinTracking.hxx"
#include "SearchArray.hxx"


namespace CLAM
{


SinTracking::SinTracking()
	: mInput("Input", this ),
	  mOutput("Output", this ),
	  mFundFreqValue("Fund Freq Value", this )
{
	Configure(SinTrackingConfig());
}

SinTracking::SinTracking(const SinTrackingConfig &c )
	: mInput("Input", this ),
	  mOutput("Output", this ),
	  mFundFreqValue("Fund Freq Value", this )

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
	bool result = Do( mInput.GetData(), mOutput.GetData() );
	mInput.Consume();
	mOutput.Produce();
	return result;
}

bool SinTracking::Do(const SpectralPeakArray& iPeakArray,SpectralPeakArray& oPeakArray)
{
	//oPeakArray initialization		
	oPeakArray.AddIndexArray();
	oPeakArray.AddPhaseBuffer();
	oPeakArray.AddBinWidthBuffer();
	oPeakArray.AddBinPosBuffer();
	oPeakArray.UpdateData();

	TData fn = mFundFreqValue.GetLastValue();
	if(mHarmonic && fn>0)
	{
		mLastHarmonic=true;
		return DoHarmonic(iPeakArray,oPeakArray,fn);
	}
	else
	{
		if(mLastHarmonic) KillAll();
		mLastHarmonic=false;
		return DoInharmonic(iPeakArray,oPeakArray);
	}
}


void SinTracking::AddNewTrack(int peakPosition, const SpectralPeak& currentPeak,SpectralPeakArray& oPeakArray) const
{
  for(int i=0;i<mnMaxSines;i++)
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
	const DataArray & previousFreqBuffer = mPreviousPeakArray.GetFreqBuffer();
	const IndexArray & previousIndexBuffer = mPreviousPeakArray.GetIndexArray();
	TData currentPeakFreq = previousFreqBuffer[processedPeakPos];

	const DataArray& iFreqBuffer=iPeakArray.GetFreqBuffer();

	if(!ThereIsCandidate(currentPeakFreq,iPeakArray,oPeakArray))
	{
		KillTrack(previousIndexBuffer[processedPeakPos]);
		return;
	}

	TData distance;
	int candidatePos=GetCandidate(currentPeakFreq,iPeakArray,distance);
	if(candidatePos==-1) return;
	TData candidatePeakFreq = iFreqBuffer[candidatePos];

	if( candidatePos>=oPeakArray.GetnPeaks()
	    || (!IsBestCandidate(candidatePeakFreq,currentPeakFreq))
	    || (oPeakArray.GetIndex(candidatePos)!=-1) )
	{
		KillTrack(previousIndexBuffer[processedPeakPos]);
		return;
	}

	// TODO: Optimize this by accessing directly to the buffers
	// instead of calling Set/GetSpectralPeak
	const SpectralPeak candidatePeak = iPeakArray.GetSpectralPeak(candidatePos);
	TIndex trackId = previousIndexBuffer[processedPeakPos];
	oPeakArray.SetSpectralPeak(candidatePos,candidatePeak,trackId);
}



//true as soon as the distance between currentPeak and a Peak in iPeakArray is <mThreshold
bool SinTracking::ThereIsCandidate(TData currentPeakFreq, 
				   const SpectralPeakArray& iPeakArray,
				   SpectralPeakArray& oPeakArray) const
{
  TSize nInputPeaks=iPeakArray.GetnPeaks();
  if(nInputPeaks>mnMaxSines) nInputPeaks=mnMaxSines;
  DataArray& peakFreqBuffer=iPeakArray.GetFreqBuffer();	  
  TData factor=100/currentPeakFreq;
  IndexArray& outputIndexArray=oPeakArray.GetIndexArray();
  for (int i=0;i<nInputPeaks;i++)
  {
	int dist=int(Abs(peakFreqBuffer[i]-currentPeakFreq)*factor);
	if((dist< mThreshold)&&(outputIndexArray[i]==-1)) return true;
  }
  return false;
}


//Sets mGuideArray.isDead to true and mnActiveGuides--
void SinTracking::KillTrack(int trackId) const
{
	for(int i=0;i<mnMaxSines;i++)
	{
		if(mGuideArray[i].trackId!=trackId) continue;
		mGuideArray[i].isDead=true;
		mnActiveGuides--;
		return;
	}
}

//Return the position of the peak in iPeakArray which is the closest to currentPeak
TIndex SinTracking::GetCandidate(TData currentPeakFreq, 
									   const SpectralPeakArray& iPeakArray,
									   TData& distance) const
{
  //Can be optimized! XA
  distance=-1;
  int nPeaks=iPeakArray.GetnPeaks();
  DataArray& peakFreqBuffer=iPeakArray.GetFreqBuffer();
  TData factor=100./currentPeakFreq;
  
  //xamat: test!
  SearchArray<TData> mySearch(peakFreqBuffer);
  TIndex found=mySearch.Find(currentPeakFreq);
  if (found==-1) found = 0; //Pau: to avoid assert. Is this the correct behaviour?
  TIndex originalFound = found;
  distance = Abs(peakFreqBuffer[found]-currentPeakFreq);
  //make sure that the two surrounding peaks are not in fact closer
  TIndex newFound;
  TData nextDistance;
  if(originalFound<nPeaks-1)
  {
	for(newFound=found+1; newFound<nPeaks; newFound++)
	{
		nextDistance = Abs(peakFreqBuffer[newFound]-currentPeakFreq);
		if(nextDistance>distance) break;
		distance = nextDistance;
	}
	found = newFound-1;
  }
  if(originalFound>0)
  {
	for(newFound=found-1; newFound>-1; newFound--)
	{
		nextDistance = Abs(peakFreqBuffer[newFound]-currentPeakFreq);
		if(nextDistance>distance) break;
		distance = nextDistance;
		}
	found = newFound + 1;
  }
  distance *= factor;
  return found;
}



//true if there is no peak in previousPeakArray closer to candidate

bool SinTracking::IsBestCandidate(TData candidateFreq, TData currentFreq) const
{
	double nextDistance=Abs(currentFreq-candidateFreq);

	int nPeaks=mPreviousPeakArray.GetnPeaks();
	DataArray& peakFreqBuffer=mPreviousPeakArray.GetFreqBuffer();
	for(int i=0;i<nPeaks;i++)
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
TIndex SinTracking::GetFirstNonAssignedPeakPos(const SpectralPeakArray& oPeakArray, TIndex beginAt=0) const
{
	const TIndex nPeaks = oPeakArray.GetnPeaks();
	if (beginAt>=nPeaks) return -1;
	if (beginAt<0) return -1;

	int i=oPeakArray.GetFirstNonValidIndexPosition(beginAt);

	if(i==nPeaks) return -1;//All peaks have been matched

	return i;
}


//Initialization of the first output peak array: all peaks must be assigned a track
void SinTracking::Initialization(const SpectralPeakArray& iPeakArray, SpectralPeakArray& oPeakArray)
{
	TSize nPeaks=oPeakArray.GetnPeaks();
	for(int i=0; i<nPeaks; i++)
	{
		AddNewTrack(i, iPeakArray.GetSpectralPeak(i), oPeakArray);
	}
	mPreviousPeakArray=oPeakArray;
}

void SinTracking::KillAll()
{
	for (int i=0;i<mnMaxSines;i++)
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
	
	oPeakArray.SetIsIndexUpToDate(true);
	for(int i=0;i<mPreviousPeakArray.GetnPeaks();i++)
	{
		Tracking(iPeakArray,oPeakArray,i);
	}
	CheckForNewBornTracks(iPeakArray,oPeakArray);
	mPreviousPeakArray=oPeakArray;
	
	//xamat: testing not to keep inharmonic peaks
	//oPeakArray.SetnPeaks(0);
	//mPreviousPeakArray=oPeakArray;
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
	
	out.SetnPeaks(mnMaxSines);
	
	//DataArray& iFreqBuffer=in.GetFreqBuffer();
	DataArray& oFreqBuffer=out.GetFreqBuffer();
	DataArray& iMagBuffer=in.GetMagBuffer();
	DataArray& oMagBuffer=out.GetMagBuffer();
	DataArray& iPhaseBuffer=in.GetPhaseBuffer();
	DataArray& oPhaseBuffer=out.GetPhaseBuffer();

	
	int i;

	TSize nPeaks=mnMaxSines;
	i=0;
	int n;
	for(n=0; n<mnMaxSines;n++)
	{
		pos=GetCandidate(oFreqBuffer[i],in,d);
		if(d<funFreq/2 && pos>-1)
		{
			if(i==0 || iMagBuffer[pos]!=oMagBuffer[i-1])
			{
				oMagBuffer[i]=iMagBuffer[pos];
				oFreqBuffer[i]=oFreqBuffer[n];
				oPhaseBuffer[i]=iPhaseBuffer[pos];
				i++;
			}
		}
	}
	out.SetnPeaks(i);
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

} // namespace CLAM

