#ifndef __TICKSEQUENCETRACKER__
#define __TICKSEQUENCETRACKER__

#include "ProcessingComposite.hxx"
#include "Array.hxx"
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
