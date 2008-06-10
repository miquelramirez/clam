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

#ifndef __TICKSEQUENCETRACKER__
#define __TICKSEQUENCETRACKER__

#include <CLAM/ProcessingComposite.hxx>
#include <CLAM/Array.hxx>
#include "TimeIndex.hxx"
#include "TickSequenceTrackerConfig.hxx"
#include "IOIHistPeakDetector.hxx"
#include "IOIHistogram.hxx"
#include "Pulse.hxx"
#include "RD_TimeDifference.hxx"
#include "RD_TimeSeriesFinder.hxx"
#include "RD_TimeSeriesSeed.hxx"
#include "AdjustTickWRTSwing.hxx"
#include "AdjustTickWRTOnsets.hxx"
#include "AdjustBeatWRTTick.hxx"
#include "AdjustBeatWRTOnsets.hxx"
#include "BeatIntervalEstimator.hxx"
#include "GlobalPulseRateEstimator.hxx"

namespace CLAM
{

	class Audio;


	namespace RhythmDescription
	{

		class TickSequenceTracker : public ProcessingComposite
		{
			TickSequenceTrackerConfig mConfig;

		public:
			TickSequenceTracker();

			~TickSequenceTracker();

			const ProcessingConfig &GetConfig() const { return mConfig;}
			
			const char *GetClassName() const {return "TickSequenceTracker";}

			bool Do(void);

			///Method to use when the input is a list of note onsets
			///(onsets already computed, or MIDI)
			bool Do(const Array<TimeIndex>& transients, Pulse& tickSequence,
				Pulse& beatSequence, IOIHistogram& interOnsetHist );

		protected:

			bool ConcreteConfigure(const ProcessingConfig&);

			void StorePulseIndexes( const int nLoops, 
						const Array<TimeIndex>& pulsesArray,
						Array<TimeIndex>& mPulses );

				       

		private: // children Processing

			IOIHistPeakDetector     mPeakDetector;
			TimeDifference          mTemporalDiff;
			TimeSeriesFinder        mTimeSeriesFinder;
			TimeSeriesFinderConfig  mTSFConfig;
			AdjustTickWRTSwing      mTickSwingAdjuster;
			AdjustTickWRTOnsets     mTickOnsetsAdjuster;
			AdjustBeatWRTTick       mBeatTickAdjuster;
			AdjustBeatWRTOnsets     mBeatOnsetsAdjuster;
			BeatIntervalEstimator   mTempoEstimator;
			GlobalPulseRateEstimator mGlobalPREstimator;

		private: // Internal PD's
		
			TimeSeriesSeed    mTickFirstGuess;
			TimeSeriesSeed    mGoodTick;
			TimeSeriesSeed    mGoodTempo;
			Array<TimeIndex>  mTransientsForHist;

			// Internal Parameters
			TSize             mIOIHistMaxSize;

		};

	} // RhythmDescription

} // namespace CLAM


#endif // TickSequenceTracker.hxx

