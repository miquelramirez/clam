#ifndef __GLOBALPULSERATEESTIMATOR__
#define __GLOBALPULSERATEESTIMATOR__

#include "ProcessingComposite.hxx"
#include "ProcessingConfig.hxx"
#include "RD_GlobalPulse.hxx"
#include "IOIHistPeakDetector.hxx"
#include "IOIHistogram.hxx"
#include "DataTypes.hxx"
#include "Pulse.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{
		class GlobalPulseRateEstimatorConfig : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( GlobalPulseRateEstimatorConfig, 3, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, TData, SampleRate );
			DYN_ATTRIBUTE( 1, public, TData, RateLowerBound );
			DYN_ATTRIBUTE( 2, public, TData, GaussianSize );

		protected:

			void DefaultInit();
		};

		class GlobalPulseRateEstimator : public ProcessingComposite
		{
		public:
			GlobalPulseRateEstimator();

			const char* GetClassName() const;
			
			const ProcessingConfig& GetConfig() const;

			bool Do();
			bool Do( const Array<TData>& pulses,
				 Pulse& pulseRate );
		protected:

			bool ConcreteConfigure( const ProcessingConfig& cfg );
			
		private:

			GlobalPulseRateEstimatorConfig mConfig;

			GlobalPulseGenerator           mPulseGenerator;
			IOIHistPeakDetector            mPeakDetector;

			IOIHistogram                   mPulseHist;
			Array<TimeIndex>               mPulseHistPeaks;

			InControl                      mRateLowerBound;
			
		};
	}
}

#endif // GlobalPulseRateEstimator.hxx
