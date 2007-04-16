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
#include "CleanTracks.hxx"
#include <iostream>


namespace CLAM {


	void CleanTracksConfig::DefaultInit()
	{

		AddAll();
		UpdateData();

		//Default values
		SetMaxDropOut(3);
		SetMinLength(3);
		SetFreqDev(20);
		SetSamplingRate(44100);
		SetSpecSize(22050);
	}

	CleanTracks::CleanTracks():mTrajectoryArray(100,100),mSearchTrajectories(mTrajectoryArray)
	{
		Configure(CleanTracksConfig());
	}

	CleanTracks::CleanTracks(const CleanTracksConfig &c ):mTrajectoryArray(100,100),mSearchTrajectories(mTrajectoryArray)
	{
		Configure(c);
	}

	CleanTracks::~CleanTracks()
	{}




/* Configure the Processing Object according to the Config object */

	bool CleanTracks::ConcreteConfigure(const ProcessingConfig& c)
	{

		CopyAsConcreteConfig(mConfig, c);

		mMaxDropOut = mConfig.GetMaxDropOut();
		mMinLength= mConfig.GetMinLength();
		mFreqDev= 1.0/mConfig.GetFreqDev();
		mSamplingRate= mConfig.GetSamplingRate();
		mSpecSize= mConfig.GetSpecSize();
		return true;
	}




//Process


	//Supervised mode
	bool  CleanTracks::Do(void)
	{
		CLAM_ASSERT(false,"CleanTracks::Do(): Supervised mode not implemented");
		return false;
	}

	bool CleanTracks::Do(Array<SpectralPeakArray*>& peakArrayArray)
	{

		LoadTracks(peakArrayArray);
		FindContinuations();
		JoinContinuations(peakArrayArray);
		Clean(peakArrayArray);
		UpdateTrackIds(peakArrayArray);
		return true;
	}

	bool CleanTracks::Do(Segment& segment)
	{
		const int nFrames=segment.GetnFrames();
		Array<SpectralPeakArray*> spectralPeakArrayArray;
		spectralPeakArrayArray.Resize(nFrames);
		for(int i=0;i<nFrames;i++)
		{
			spectralPeakArrayArray.AddElem(&segment.GetFrame(i).GetSpectralPeakArray());
		}
		return Do(spectralPeakArrayArray);

	}

	void CleanTracks::LoadTracks(Array<SpectralPeakArray*>& peakArrayArray)
	{
		for(int i=0;i<peakArrayArray.Size();i++)
		{
			for(int z=0;z<peakArrayArray[i]->GetnIndexedPeaks();z++)
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
	}

	void CleanTracks::FindContinuations()
	{
		int firstCandidatable = 0;

		for(int i=0; i<mTrajectoryArray.Size(); i++)
		{
			const TTrajectory & toBeAppended = mTrajectoryArray[i];
/*
			// Non-candidatables now won't be candidatables never
			while (firstCandidatable<i && toBeAppended.beginPos <
				mTrajectoryArray[firstCandidatable].beginPos+
				mTrajectoryArray[firstCandidatable].length+mMaxDropOut)
				firstCandidatable++;
*/
			bool thereIsCandidate=false;
			// MRJ: Transition of Frequency deviation from absolute hertz to
			// a %. We multiply this factor by the currently considered trajectory
			// final frequency
			TData bestFreqDif=mFreqDev * mTrajectoryArray[i].finalFreq ;
			int bestCandidate=0;

			// Get the best 'candidate' to be followed by the track 'toBeAppended'
			for(int k=firstCandidatable; k<i; k++)
			{
				const TTrajectory & candidate = mTrajectoryArray[k];

				// Suposing that they are ordered by start frame,
				// there is a point from which all the candidates are invalid
				if (candidate.beginPos>=toBeAppended.beginPos) break;

				const TSize dropOut=
					toBeAppended.beginPos-
						(candidate.beginPos+candidate.length);

				// 'candidate' should end before 'toBeAppended' starts
				if (dropOut<=0) continue;
				// ...but not too much
				if (dropOut>mMaxDropOut) continue;

				// ...and the frequency distance should be the better one
				const TData frequencyDistance =
					Abs(toBeAppended.initialFreq-candidate.finalFreq);
				if (frequencyDistance >= bestFreqDif) continue;

				bestFreqDif=frequencyDistance;
				bestCandidate=k;
				thereIsCandidate=true;
			}

			// If there is no candidate, toBeAppended is not appended, next...
			if (!thereIsCandidate) continue;

			TTrajectory & candidateTrajectory = mTrajectoryArray[bestCandidate];

			// Check that the best candidate for 'toBeAppended'
			// is not the best one to another

			// Unused variable: const TSize candidateEnd = candidateTrajectory.beginPos+candidateTrajectory.length;

			TSize previousFollowerPosition = candidateTrajectory.continuedAtId;
			// Candidate has already has been attached?
			if (previousFollowerPosition != -1)
			{
				TTrajectory & previousFollower = mTrajectoryArray[previousFollowerPosition];
				const TData frequencyDistance =
					Abs(previousFollower.initialFreq-candidateTrajectory.finalFreq);
				if (frequencyDistance <= bestFreqDif) continue;
			}

			candidateTrajectory.continuedAtId=toBeAppended.id;

		}
	}

	void CleanTracks::JoinContinuations(Array<SpectralPeakArray*>& peakArrayArray)
	{
		for(int i=0;i<mTrajectoryArray.Size();i++)
		{
			// Unused variable: const int id     = mTrajectoryArray[i].id;
			int contAt = mTrajectoryArray[i].continuedAtId;
			// Unused variable: const int begPos = mTrajectoryArray[i].beginPos;
			// Unused variable: const int lastfreq=int(mTrajectoryArray[i].finalFreq);
			while(mTrajectoryArray[i].continuedAtId!=-1)
			{
				contAt=mTrajectoryArray[i].continuedAtId;
				InterpolatePeaks(mTrajectoryArray[i], peakArrayArray);
			}
		}
	}


	void CleanTracks::Clean(Array<SpectralPeakArray*>& peakArrayArray)
	{
		for(int i=0;i<peakArrayArray.Size();i++)
		{
			int nDeleted=0;
			for(int z=0;z<peakArrayArray[i]->GetnIndexedPeaks();z++)
			{
				const int id=peakArrayArray[i]->GetIndex(z-nDeleted);
				const int trajectoryPosition=FindTrajectoryPosition(id);

				if (trajectoryPosition==-1) continue;
				if (mTrajectoryArray[trajectoryPosition].length>=mMinLength) continue;

				//modified
				peakArrayArray[i]->DeleteSpectralPeak(z-nDeleted);
				peakArrayArray[i]->SetIsIndexUpToDate(true);
				peakArrayArray[i]->DeleteIndex(id);
				mTrajectoryArray[trajectoryPosition].length--;//update length
				if(mTrajectoryArray[trajectoryPosition].length==0)
					mTrajectoryArray.DeleteElem(trajectoryPosition);
				nDeleted++;
			}
		}
	}


	void CleanTracks::UpdateTrackIds(Array<SpectralPeakArray*>& peakArrayArray)
	{
		for(int i=0;i<peakArrayArray.Size();i++)
		{
			for(int z=0;z<peakArrayArray[i]->GetnIndexedPeaks();z++)
			{
				const int currentTrackid=peakArrayArray[i]->GetIndex(z);
				const int newTrackid=FindTrajectoryPosition(currentTrackid);
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
		// would be faster using searcharray.find?
		const int pos = FindTrajectoryPosition(trajectory.id);
		if(pos==-1)
		{
			// not found, new id, add it
			mTrajectoryArray.AddElem(trajectory);
		}
		else
		{
			// if found, length and last data are updated
			mTrajectoryArray[pos].length++;
			mTrajectoryArray[pos].finalFreq=trajectory.finalFreq;
			mTrajectoryArray[pos].finalMag=trajectory.finalMag;
		}
	}

	void CleanTracks::InterpolatePeaks(TTrajectory& fromTrajectory, Array<SpectralPeakArray*>& peakArrayArray)
	{
		const int newTrajPos=FindTrajectoryPosition(fromTrajectory.continuedAtId);
		CLAM_ASSERT(newTrajPos>-1,"CleanTracks::InterpolatePeaks:Negative Index for track");
		const TTrajectory & toTrajectory = mTrajectoryArray[newTrajPos];
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
			currentBinWidth=int(currentBinPos-lastBinPos);
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
		// For Empty arrays return not found
		if (mTrajectoryArray.Size()==0)
			return -1;
		//we have to check whether it is first or last track
		if (id == mTrajectoryArray[0].id)
			return 0;
		if (id == mTrajectoryArray[mTrajectoryArray.Size()-1].id)
			return mTrajectoryArray.Size()-1;

		TTrajectory tmpTrajectory;
		tmpTrajectory.id=id;
		TIndex trajectoryPosition = mSearchTrajectories.Find(tmpTrajectory);

		//note that Find returns the closest index and that does not guarantee that is the exact one
		if (trajectoryPosition!=-1)
			if (mTrajectoryArray[trajectoryPosition].id!=id)
				return -1;
		return trajectoryPosition;
	}

};//namespace

