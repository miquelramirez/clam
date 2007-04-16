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
#ifndef _CLEANTRACKS__
#define _CLEANTRACKS__

#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "DataTypes.hxx"
#include "Processing.hxx"
#include "Array.hxx"
#include "SearchArray.hxx"
#include "CleanTracksConfig.hxx"

namespace CLAM {

	class SpectralPeakArray;
	class Segment;
	class ProcessingConfig;

	/* CleanTracksConfig moved to CleanTracksConfig.hxx */

	typedef struct STrajectory
	{
		TIndex id;
		int beginPos;
		int length;
		TData initialFreq;
		TData finalFreq;
		TData initialMag;
		TData finalMag;
		int continuedAtId;
		friend bool operator >=(const struct STrajectory& first,const struct STrajectory& second)
		{
			return(first.id>=second.id);
		}
		friend bool operator <(const struct STrajectory& first,const struct STrajectory& second)
		{
			return(first.id<second.id);
		}
	}TTrajectory;



	class CleanTracks: public Processing
	{
		mutable CleanTracksConfig mConfig;

		const char *GetClassName() const {return "CleanTracks";}

		/** Config change method */
		virtual bool ConcreteConfigure(const ProcessingConfig&);

	public:

		CleanTracks();
		CleanTracks(const CleanTracksConfig &c);
		~CleanTracks();

		//Configuration accessor
		const ProcessingConfig &GetConfig() const { return mConfig;};

		//Peak Continuation for one frame
		bool Do(Array<SpectralPeakArray*>& peakArrayArray);
		bool Do(Segment& segment);
        bool Do(void);

		int GetnCleanedTracks() const {return mTrajectoryArray.Size();};

	private:
		void LoadTracks(Array<SpectralPeakArray*>& peakArrayArray);
		void AddTrajectory(TTrajectory& trajectory);
		void FindContinuations(void);
		void JoinContinuations(Array<SpectralPeakArray*>& peakArrayArray);
		void Clean(Array<SpectralPeakArray*>& peakArrayArray);
		void UpdateTrackIds(Array<SpectralPeakArray*>& peakArrayArray);
		void InterpolatePeaks(TTrajectory& trajectory, Array<SpectralPeakArray*>& peakArrayArray);

		TIndex FindTrajectoryPosition(TIndex id);

		TData mSamplingRate;
		TData mSpecSize;
		TSize mMaxDropOut;
		TSize mMinLength;
		TData mFreqDev;
		Array<TTrajectory> mTrajectoryArray;
		SearchArray<TTrajectory> mSearchTrajectories;




	};

};//namespace

#endif

