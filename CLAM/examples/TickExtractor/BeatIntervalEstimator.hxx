#ifndef __BEATINTERVALESTIMATOR__
#define __BEATINTERVALESTIMATOR__

#include "ProcessingConfig.hxx"
#include "Processing.hxx"
#include "RD_TimeSeriesSeed.hxx"
#include "Array.hxx"
#include "TimeIndex.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{
		class BeatIntervalEstimatorConfig : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( BeatIntervalEstimatorConfig, 3, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, TData, SampleRate );
			DYN_ATTRIBUTE( 1, public, TData, TempoLimSup );
			DYN_ATTRIBUTE( 2, public, TData, TempoLimInf );

		protected:

			void DefaultInit();
		};

		class BeatIntervalEstimator : public Processing
		{
		public:
			BeatIntervalEstimator();
			
			const char* GetClassName() const;

			const ProcessingConfig& GetConfig() const;

			bool Do();

			bool Do( const Array<TimeIndex>& peaks,
				 TimeSeriesSeed& beatParams );

		protected:

			bool ConcreteConfigure( const ProcessingConfig& cfg );

		private:

			BeatIntervalEstimatorConfig mConfig;

		};
	}
}

#endif // BeatIntervalEstimator.hxx
