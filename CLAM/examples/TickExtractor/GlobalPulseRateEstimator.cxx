#include "GlobalPulseRateEstimator.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{
		void GlobalPulseRateEstimatorConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
		}

		GlobalPulseRateEstimator::GlobalPulseRateEstimator()
			: mRateLowerBound( "RateLowerBound", this )
		{
			mPulseGenerator.SetParent( this );
			mPeakDetector.SetParent( this );
		}

		const char* GlobalPulseRateEstimator::GetClassName() const
		{
			return "GlobalPulseRateEstimator";
		}

		const ProcessingConfig& GlobalPulseRateEstimator::GetConfig() const
		{
			return mConfig;
		}

		bool GlobalPulseRateEstimator::ConcreteConfigure( const ProcessingConfig& cfg )
		{
			CopyAsConcreteConfig( mConfig, cfg );

			GlobalPulseGeneratorConfig gpConf;
			
			gpConf.SetGaussianSize( (TSize)mConfig.GetGaussianSize() );

			mPulseGenerator.Configure( gpConf );

			IOIHistPeakDetectorConfig peakDetCfg;

			peakDetCfg.SetThreshold( 0.0 );
			peakDetCfg.SetSampleRate( mConfig.GetSampleRate() );

			mPeakDetector.Configure( peakDetCfg );

			
			return true;
		}

		bool GlobalPulseRateEstimator::Do()
		{
			return false;
		}

		bool GlobalPulseRateEstimator::Do( const CLAM::Array<TData>& pulses, Pulse& pulseDesc )
		{
			int pulseLimSup = (int)mRateLowerBound.GetLastValue();
			

			mPulseHist.GetBins().Resize( pulseLimSup + 10000 );
			mPulseHist.GetBins().SetSize( pulseLimSup + 10000 );

			mPulseGenerator.Do( pulses, mPulseHist );
			
			mPeakDetector.Do( mPulseHist, mPulseHistPeaks );
			
			CLAM_ASSERT( mPulseHistPeaks.Size() != 0,
				     "There should be some peak in the histogram!!!!" );
			
			TData max = 0;
			int index = 0;
			for (int i=1;i < mPulseHistPeaks.Size() ;i++) //starts at 1 because there is a peak at 0 (with 0 weight)
				if (mPulseHistPeaks[i].GetWeight() > max)
				{
					max =  mPulseHistPeaks[i].GetWeight();
					index = i;
				}
			

			pulseDesc.SetRate( 60.0 / (mPulseHistPeaks[index].GetPosition() / mConfig.GetSampleRate() ) );	

			return true;
		}
	}
}
