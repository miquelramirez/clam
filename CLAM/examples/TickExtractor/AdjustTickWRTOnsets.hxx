#ifndef __ADJUSTTICKWRTONSETS__
#define __ADJUSTTICKWRTONSETS__

#include "ProcessingComposite.hxx"
#include "RD_TimeSeriesFinder.hxx"
#include "RD_TimeSeriesSeed.hxx"
#include "RD_GridGen.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{
		class AdjustTickWRTOnsetsConfig : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( AdjustTickWRTOnsetsConfig, 5, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, TData, SampleRate );
			DYN_ATTRIBUTE( 1, public, TData, DeviationPenalty );
			DYN_ATTRIBUTE( 2, public, TData, OverSubdivisionPenalty );
			DYN_ATTRIBUTE( 3, public, TData, TickLimSup );
			DYN_ATTRIBUTE( 4, public, TData, Scope );

		protected:

			void DefaultInit();
		};
		
		class AdjustTickWRTOnsets : public ProcessingComposite
		{
		public:

			AdjustTickWRTOnsets();

			const char* GetClassName() const;

			bool Do();
			bool Do( const Array<TimeIndex>& onsets,
				 const TimeSeriesSeed& previousSeed,
				 Array<TimeIndex>& ticks,
				 TimeSeriesSeed& refinedSeed );

			const ProcessingConfig& GetConfig() const;

		protected:

			bool ConcreteConfigure( const ProcessingConfig& cfg );

		private:

			AdjustTickWRTOnsetsConfig mConfig;
			InControl        mTSF_OffsetMin;
			InControl        mTSF_OffsetStep;
			InControl        mTSF_IntervalMin;
			InControl        mTSF_IntervalMax;
			InControl        mTSF_IntervalStep;
			InControl        mTSF_OverSubdivisionPenalty;
			InControl        mFirstTransientPosition;
			InControl        mLastTransientPosition;

			PulseGridGenerator mGridGenerator;
			TimeSeriesFinder   mTimeSeriesFinder;
		};
	}
}


#endif // AdjustTickWRTOnsets.hxx
