#ifndef __ADJUSTBEATWRTONSETS__
#define __ADJUSTBEATWRTONSETS__

#include "ProcessingComposite.hxx"
#include "ProcessingConfig.hxx"
#include "RD_TimeSeriesFinder.hxx"
#include "RD_TimeSeriesSeed.hxx"
#include "RD_GridGen.hxx"
#include "DataTypes.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{
		class AdjustBeatWRTOnsetsConfig : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( AdjustBeatWRTOnsetsConfig, 3, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, TData, SampleRate );
			DYN_ATTRIBUTE( 1, public, TData, DeviationPenalty );
			DYN_ATTRIBUTE( 2, public, TData, OverSubdivisionPenalty );


		protected:

			void DefaultInit();
		};
		
		class AdjustBeatWRTOnsets : public ProcessingComposite
		{
		public:

			AdjustBeatWRTOnsets();

			const char* GetClassName() const;

			bool Do();
			bool Do( const Array<TimeIndex>& onsets,
				 const TimeSeriesSeed& tickParams,
				 const TimeSeriesSeed& oldBeatParams,
				 Array<TimeIndex>& beats,
				 TimeSeriesSeed& refinedBeats );

			const ProcessingConfig& GetConfig() const;

		protected:

			bool ConcreteConfigure( const ProcessingConfig& cfg );

		private:

			AdjustBeatWRTOnsetsConfig mConfig;
			TimeSeriesFinder          mSeriesFinder;
			PulseGridGenerator        mGridGenerator;
			InControl                 mFirstTransientPosition;
			InControl                 mLastTransientPosition;
			
		};

	}
}


#endif // AdjustBeatWRTOnsets.hxx
