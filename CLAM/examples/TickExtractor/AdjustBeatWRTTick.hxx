#ifndef __ADJUSTBEATWRTTICK__
#define __ADJUSTBEATWRTTICK__


#include "Processing.hxx"
#include "DataTypes.hxx"
#include "RD_TimeSeriesSeed.hxx"

namespace CLAM
{

	namespace RhythmDescription
	{
		class AdjustBeatWRTTickConfig : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( AdjustBeatWRTTickConfig, 3, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, TData, SampleRate );
			DYN_ATTRIBUTE( 1, public, TData, TempoLimSup );
			DYN_ATTRIBUTE( 2, public, TData, TempoLimInf );

		protected:

			void DefaultInit();
		};

		class AdjustBeatWRTTick : public Processing
		{
		public:
			AdjustBeatWRTTick();

			const char* GetClassName() const;

			const ProcessingConfig& GetConfig() const;

			bool Do();
			bool Do( const TimeSeriesSeed& tickParams,
				 const TimeSeriesSeed& oldBeatParams,
				 TimeSeriesSeed& refinedBeatParams );

		protected:

			bool ConcreteConfigure( const ProcessingConfig& cfg );

		private:

			AdjustBeatWRTTickConfig   mConfig;
		};
	}

}


#endif // AdjustBeatWRTTick.hxx
