#include "CleanTracks.hxx"
#include "ErrProcessingObj.hxx"
#include <iostream.h>


namespace CLAM {

  
	void CleanTracksConfig::DefaultInit()
	{
		
		AddName();
		AddMaxDropOut();
		AddMinLength();
		AddFreqDev();

		UpdateData();

		//Default values
		SetMaxDropOut(3);
		SetMinLength(3);
		SetFreqDev(200);



	}
	
	CleanTracks::CleanTracks()
	{
		Configure(CleanTracksConfig());
	}

	CleanTracks::CleanTracks(const CleanTracksConfig &c )
	{
		Configure(c);
	}

	CleanTracks::~CleanTracks()
	{}




/* Configure the Processing Object according to the Config object */

	bool CleanTracks::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
	{	    

		mConfig = dynamic_cast<const CleanTracksConfig&>(c);	    

		mMaxDropOut = mConfig.GetMaxDropOut();

		mMinLength= mConfig.GetMinLength();

		mFreqDev= mConfig.GetFreqDev();

		//initialize trajectories

		return true;
	}




//Process

	
	//Supervised mode
	bool  CleanTracks::Do(void) 
	{
		throw(ErrProcessingObj("CleanTracks::Do(): Supervised mode not implemented"),this);
		return false;
	}  

	bool CleanTracks::Do(Array<SpectralPeakArray*>& peakArrayArray)
	{

		Update(peakArrayArray);

		Continue(peakArrayArray);
		Clean(peakArrayArray);
		UpdateTrackIds(peakArrayArray);
		return true;
	}

	bool CleanTracks::Do(Segment& segment)
	{
		int i;
		int nFrames=segment.GetnFrames();
		Array<SpectralPeakArray*> spectralPeakArrayArray;
		for(i=0;i<nFrames;i++)
		{
			spectralPeakArrayArray.AddElem(&segment.GetFrame(i).GetSpectralPeakArray());
		}
		return Do(spectralPeakArrayArray);

	}

	void CleanTracks::Update(Array<SpectralPeakArray*>& peakArrayArray)
	{
		int i,z;
		for(i=0;i<peakArrayArray.Size();i++)
		{
			for(z=0;z<peakArrayArray[i]->GetnIndexedPeaks();z++)
			{
				TTrajectory tmpTrajectory;
				tmpTrajectory.id=peakArrayArray[i]->GetIndex(z);
				tmpTrajectory.beginPos=i;
				tmpTrajectory.initialFreq=tmpTrajectory.finalFreq=peakArrayArray[i]->GetFreq(z);//At the beginning, initial=final
				tmpTrajectory.initialMag=tmpTrajectory.finalMag=peakArrayArray[i]->GetMag(z);
				tmpTrajectory.length=1;
				tmpTrajectory.continuedAtId=-1;
				AddTrajectory(tmpTrajectory);
			}
		}
		ContinuedAt();
	}

	void CleanTracks::Continue(Array<SpectralPeakArray*>& peakArrayArray)
	{
		int i, id, contAt, begPos,lastfreq;
		for(i=0;i<mTrajectoryArray.Size();i++)
		{
			id=mTrajectoryArray[i].id;
			contAt=mTrajectoryArray[i].continuedAtId;
			begPos=mTrajectoryArray[i].beginPos;
			lastfreq=int(mTrajectoryArray[i].finalFreq);
			if(mTrajectoryArray[i].continuedAtId!=-1)
			{
				while(mTrajectoryArray[i].continuedAtId!=-1)
				{
					contAt=mTrajectoryArray[i].continuedAtId;
					interpolatePeaks(mTrajectoryArray[i], peakArrayArray);
				}
			}
		}
	}
	
	
	
	void CleanTracks::Clean(Array<SpectralPeakArray*>& peakArrayArray)
	{
		int i,z;
		for(i=0;i<peakArrayArray.Size();i++)
		{
			int nDeleted=0;
			for(z=0;z<peakArrayArray[i]->GetnIndexedPeaks();z++)
			{
				TTrajectory tmpTrajectory;
				tmpTrajectory.id=peakArrayArray[i]->GetIndex(z-nDeleted);
				//int trajectoryPosition=mSearchTrajectories.Find(tmpTrajectory);
				
				int j;
				int trajectoryPosition;
				for(j=0; j<mTrajectoryArray.Size(); j++)
				{
					if(mTrajectoryArray[j].id==tmpTrajectory.id)
					{
						trajectoryPosition=j;
						break;
					}
					else trajectoryPosition=-1;
				}
				///////////////////////////////////
				if (trajectoryPosition!=-1)
				{
					if(mTrajectoryArray[trajectoryPosition].length<mMinLength)
					{
						//modified
						peakArrayArray[i]->DeleteSpectralPeak(z-nDeleted);
						peakArrayArray[i]->SetIsIndexUpToDate(true);
						peakArrayArray[i]->DeleteIndex(tmpTrajectory.id);
						mTrajectoryArray[trajectoryPosition].length--;//update length
						if(mTrajectoryArray[trajectoryPosition].length==0)
							DeleteTrajectory(tmpTrajectory.id);
						nDeleted++;
					}
				}
			}
		}
	}


	void CleanTracks::UpdateTrackIds(Array<SpectralPeakArray*>& peakArrayArray)
	{
		int i,z;
		int nMissingTracks=0,greatestTrackid=0;
		int currentTrackid;
		int newTrackid;
		for(i=0;i<peakArrayArray.Size();i++)
		{
			for(z=0;z<peakArrayArray[i]->GetnIndexedPeaks();z++)
			{
				currentTrackid=peakArrayArray[i]->GetIndex(z);
				TTrajectory tmpTrajectory;
				tmpTrajectory.id=currentTrackid;
				//newTrackid=mSearchTrajectories.Find(tmpTrajectory);
				int j;
				for(j=0; j<mTrajectoryArray.Size(); j++)
				{
					if(mTrajectoryArray[j].id==tmpTrajectory.id)
					{
						newTrackid=j;
						break;
					}
					else newTrackid=-1;
				}

				if(newTrackid!=currentTrackid)
				{
					peakArrayArray[i]->SetIndex(z,newTrackid);
					peakArrayArray[i]->SetIsIndexUpToDate(true);//needed?
				}
			}
		}
	}

				

	void CleanTracks::AddTrajectory(TTrajectory& trajectory)
	{
		int pos=-1;
		//if the trajectory exists, pos=id?
		TSize nTrajectories=mTrajectoryArray.Size();
		if(nTrajectories>0)
		{
			//would be faster using searcharray.find
			//pos=mSearchTrajectories.Find(trajectory);
			int i;
			for(i=0; i<nTrajectories; i++)
			{
				if(mTrajectoryArray[i].id==trajectory.id)
				{
					pos=i;
					break;
				}
				else pos=-1;
			}
			//if(trajectory.id!=mTrajectoryArray[pos].id)
			//	pos=-1;
						
		}
		if(pos!=-1)//if found, length and last data are updated
		{
			mTrajectoryArray[pos].length++;
			mTrajectoryArray[pos].finalFreq=trajectory.finalFreq;
			mTrajectoryArray[pos].finalMag=trajectory.finalMag;
			//mTrajectoryArray[pos].finalPhase=trajectory.finalPhase;
		}
		else  //trajectory added
		{
			mTrajectoryArray.AddElem(trajectory);
		}
	}


	void CleanTracks::DeleteTrajectory(int id)
	{
		TTrajectory tmpTrajectory;
		tmpTrajectory.id=id;
		int i, pos;
		TSize nTrajectories=mTrajectoryArray.Size();
		for(i=0; i<nTrajectories; i++)
			{
				if(mTrajectoryArray[i].id==id)
				{
					pos=i;
					break;
				}
				else pos=-1;
			}

		mTrajectoryArray.DeleteElem(pos);
	}


	
	void  CleanTracks::ContinuedAt()
	{		
		int i, j, k;
		TSize dropOut;
		int bestCandidate;
		bool ThereIsCandidate;

		for(i=0; i<mTrajectoryArray.Size(); i++)
		{
			ThereIsCandidate=false;
			TData bestFreqDif=mFreqDev;
			
			//GetBestCandidate
			for(k=0; k<mTrajectoryArray.Size(); k++)
			{
				dropOut=mTrajectoryArray[i].beginPos-(mTrajectoryArray[k].beginPos+mTrajectoryArray[k].length);
				
				if((dropOut>0)&&(dropOut<=mMaxDropOut)&&
					(Abs(mTrajectoryArray[i].initialFreq-mTrajectoryArray[k].finalFreq)<bestFreqDif))				
				{
					bestFreqDif=Abs(mTrajectoryArray[i].initialFreq-mTrajectoryArray[k].finalFreq);
					bestCandidate=k;
					ThereIsCandidate=true;
				}
			}

			if(ThereIsCandidate)
			{
				//IsBestCandidateInTheOtherWay
				bool IsBestCandidate=true;
				for(j=0; j<mTrajectoryArray.Size(); j++)
				{
					dropOut=mTrajectoryArray[j].beginPos-(mTrajectoryArray[bestCandidate].beginPos+mTrajectoryArray[bestCandidate].length);
					if((dropOut>0)&&(dropOut<=mMaxDropOut)&&
						(Abs(mTrajectoryArray[j].initialFreq-mTrajectoryArray[bestCandidate].finalFreq)<bestFreqDif))
						//(Abs(mTrajectoryArray[j].initialFreq-mTrajectoryArray[bestCandidate].finalFreq)/(dropOut+1)<bestFreqDif))
						IsBestCandidate=false;
				}
				if(IsBestCandidate==true) mTrajectoryArray[bestCandidate].continuedAtId=mTrajectoryArray[i].id;

			}
		}
	}

	void  CleanTracks::interpolatePeaks(TTrajectory& fromTrajectory, Array<SpectralPeakArray*>& peakArrayArray)
	{
		TTrajectory tmpTrajectory;
		tmpTrajectory.id=fromTrajectory.continuedAtId;
		//int newTrajPos=mSearchTrajectories.Find(tmpTrajectory);
		int j, newTrajPos, z;
		for(j=0; j<mTrajectoryArray.Size(); j++)
		{
			if(mTrajectoryArray[j].id==tmpTrajectory.id)
			{
				newTrajPos=j;
				break;
			}
			//else newTrajPos=-1;
		}
		TTrajectory toTrajectory=mTrajectoryArray[newTrajPos];


		int gap=toTrajectory.beginPos-(fromTrajectory.beginPos+fromTrajectory.length);
		TData freqSlope=(toTrajectory.initialFreq-fromTrajectory.finalFreq)/(gap+1);
		TData magSlope=(toTrajectory.initialMag-fromTrajectory.finalMag)/(gap+1);
		TData currentFreq=fromTrajectory.finalFreq;
		TData currentMag=fromTrajectory.finalMag;
		for(z=fromTrajectory.beginPos+fromTrajectory.length;z<toTrajectory.beginPos;z++)
		{
			currentFreq+=freqSlope;
			currentMag+=magSlope;
			SpectralPeak tmpPeak;
			tmpPeak.AddPhase();
			tmpPeak.AddBinWidth();
			tmpPeak.AddBinPos();
			tmpPeak.UpdateData();
			tmpPeak.SetFreq(currentFreq);
			tmpPeak.SetMag(currentMag);
			tmpPeak.SetScale(EScale(EScale::eLog));
			peakArrayArray[z]->AddSpectralPeak(tmpPeak, true, fromTrajectory.id);
			//Peaks should be sorted in the frames where added !!!!!
		}
		//Need to only update index in the next frames
		for(z=toTrajectory.beginPos;z<toTrajectory.beginPos+toTrajectory.length;z++)
		{
			peakArrayArray[z]->SetIndex(peakArrayArray[z]->GetPositionFromIndex(toTrajectory.id),fromTrajectory.id);
		}
		fromTrajectory.length+=(gap+toTrajectory.length);
		fromTrajectory.continuedAtId=mTrajectoryArray[newTrajPos].continuedAtId;
		fromTrajectory.finalFreq=mTrajectoryArray[newTrajPos].finalFreq;
		mTrajectoryArray.DeleteElem(newTrajPos);
		
	}

	

};//namespace
