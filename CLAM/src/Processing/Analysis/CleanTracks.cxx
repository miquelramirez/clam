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
#include "CleanTracks.hxx"
#include "ErrProcessingObj.hxx"
#include <iostream.h>


namespace CLAM {

  
	void CleanTracksConfig::DefaultInit()
	{
		
		AddAll();
		UpdateData();

		//Default values
		SetMaxDropOut(3);
		SetMinLength(3);
		SetFreqDev(200);
		SetSamplingRate(44100);
		SetSpecSize(22050);
	}
	
	CleanTracks::CleanTracks():mSearchTrajectories(mTrajectoryArray)
	{
		Configure(CleanTracksConfig());
	}

	CleanTracks::CleanTracks(const CleanTracksConfig &c ):mSearchTrajectories(mTrajectoryArray)
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
		mSamplingRate= mConfig.GetSamplingRate();
		mSpecSize= mConfig.GetSpecSize();
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
					InterpolatePeaks(mTrajectoryArray[i], peakArrayArray);
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
				int id=peakArrayArray[i]->GetIndex(z-nDeleted);
				int trajectoryPosition=FindTrajectoryPosition(id);
				if (trajectoryPosition!=-1)
				{
					if(mTrajectoryArray[trajectoryPosition].length<mMinLength)
					{
						//modified
						peakArrayArray[i]->DeleteSpectralPeak(z-nDeleted);
						peakArrayArray[i]->SetIsIndexUpToDate(true);
						peakArrayArray[i]->DeleteIndex(id);
						mTrajectoryArray[trajectoryPosition].length--;//update length
						if(mTrajectoryArray[trajectoryPosition].length==0)
							DeleteTrajectory(id);
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
				newTrackid=FindTrajectoryPosition(currentTrackid);
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
			pos=FindTrajectoryPosition(trajectory.id);
		}
		if(pos!=-1)//if found, length and last data are updated
		{
			mTrajectoryArray[pos].length++;
			mTrajectoryArray[pos].finalFreq=trajectory.finalFreq;
			mTrajectoryArray[pos].finalMag=trajectory.finalMag;
		}
		else  //trajectory added
		{
			mTrajectoryArray.AddElem(trajectory);
		}
	}


	void CleanTracks::DeleteTrajectory(int id)
	{
		int pos;
		
		pos=FindTrajectoryPosition(id);
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

	void  CleanTracks::InterpolatePeaks(TTrajectory& fromTrajectory, Array<SpectralPeakArray*>& peakArrayArray)
	{
		int newTrajPos=FindTrajectoryPosition(fromTrajectory.continuedAtId);
		CLAM_ASSERT(newTrajPos>-1,"CleanTracks::InterpolatePeaks:Negative Index for track");
		TTrajectory toTrajectory=mTrajectoryArray[newTrajPos];
		int gap=toTrajectory.beginPos-(fromTrajectory.beginPos+fromTrajectory.length);
		TData freqSlope=(toTrajectory.initialFreq-fromTrajectory.finalFreq)/(gap+1);
		TData magSlope=(toTrajectory.initialMag-fromTrajectory.finalMag)/(gap+1);
		TData currentFreq=fromTrajectory.finalFreq;
		TData currentMag=fromTrajectory.finalMag;
		TData currentBinPos;
		int currentBinWidth;
		TData lastBinPos=0;
		int z;
		for(z=fromTrajectory.beginPos+fromTrajectory.length;z<toTrajectory.beginPos;z++)
		{
			currentFreq+=freqSlope;
			currentMag+=magSlope;
			currentBinPos=2*currentFreq*mSpecSize/mSamplingRate;
			currentBinWidth=currentBinPos-lastBinPos;
			lastBinPos=currentBinPos;
			SpectralPeak tmpPeak;
			tmpPeak.AddPhase();
			tmpPeak.AddBinWidth();
			tmpPeak.AddBinPos();
			tmpPeak.UpdateData();
			tmpPeak.SetFreq(currentFreq);
			tmpPeak.SetMag(currentMag);
			tmpPeak.SetScale(EScale(EScale::eLog));
			tmpPeak.SetBinPos(currentBinPos);
			tmpPeak.SetBinWidth(currentBinWidth);
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

	
TIndex CleanTracks::FindTrajectoryPosition(TIndex id)
{
	TTrajectory tmpTrajectory;
	int trajectoryPosition=-1;
	tmpTrajectory.id=id;
	//we have to check whether it is first or last track
	if(tmpTrajectory.id==mTrajectoryArray[0].id) 
		trajectoryPosition=0;
	else if (tmpTrajectory.id==mTrajectoryArray[mTrajectoryArray.Size()-1].id) 
		trajectoryPosition=mTrajectoryArray.Size()-1;
	else
	{
		trajectoryPosition=mSearchTrajectories.Find(tmpTrajectory);
		//note that Find returns the closest index and that does not guarantee that is the exact one
		if(trajectoryPosition!=-1&&mTrajectoryArray[trajectoryPosition].id!=id) trajectoryPosition=-1;
	}
	return trajectoryPosition;
}

};//namespace