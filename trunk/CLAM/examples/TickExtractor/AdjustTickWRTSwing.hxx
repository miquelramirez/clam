#ifndef __ADJUST_TICK_WRT_SWING__
#define __ADJUST_TICK_WRT_SWING__

#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "DataTypes.hxx"
#include "RD_TimeSeriesSeed.hxx"

namespace CLAM
{

	namespace RhythmDescription
	{
		class IOIHistogram;

		class AdjustTickWRTSwingConfig : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( AdjustTickWRTSwingConfig, 3, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, TData, SampleRate );
			DYN_ATTRIBUTE( 1, public, TData, TempoLimSup );
			DYN_ATTRIBUTE( 2, public, TData, TempoLimInf );

		protected:

			void DefaultInit();
		};


		class AdjustTickWRTSwing : public Processing
		{
		public:

			AdjustTickWRTSwing();
			
			const char*             GetClassName() const;
			const ProcessingConfig& GetConfig() const;
			
			bool Do();

			bool Do( const IOIHistogram& hist, const TimeSeriesSeed& prevTick, TimeSeriesSeed& newTick );

		protected:

			bool ConcreteConfigure( const ProcessingConfig& cfg );

		private:

			AdjustTickWRTSwingConfig  mConfig;
			TData                     mUpperBound;
			TData                     mLowerBound;
		};
		
	}

}

#endif // AdjustTickWRTSwing.hxx
