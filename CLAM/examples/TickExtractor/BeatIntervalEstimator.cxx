#include "BeatIntervalEstimator.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{
		void BeatIntervalEstimatorConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
		}

		BeatIntervalEstimator::BeatIntervalEstimator()
		{
		}
		
		const char* BeatIntervalEstimator::GetClassName() const
		{
			return "BeatIntervalEstimator";
		}

		const ProcessingConfig& BeatIntervalEstimator::GetConfig() const
		{
			return mConfig;
		}

		bool BeatIntervalEstimator::ConcreteConfigure( const ProcessingConfig& cfg )
		{
			CopyAsConcreteConfig( mConfig, cfg );

			return true;
		}

		bool BeatIntervalEstimator::Do()
		{
			return false;
		}

		bool BeatIntervalEstimator::Do( const Array<TimeIndex>& peaks,
						TimeSeriesSeed& beatParams )
		{
			

			TData maxForTempo=0.0;
			int indexForTempo=0;
		
			for (int i=1;i < peaks.Size() ;i++) //starts at 1 because there is a peak at 0 (with 0 weight)
			{
				//Tempo is between tempoLimInf and tempoLimSup BPM
				if ((peaks[i].GetPosition() > mConfig.GetSampleRate()*60.0/mConfig.GetTempoLimSup())
				    && (peaks[i].GetPosition() < mConfig.GetSampleRate()*60.0/mConfig.GetTempoLimInf())
				    && (peaks[i].GetWeight() > maxForTempo))
				{
					maxForTempo = peaks[i].GetWeight();
					indexForTempo = i;
				}
			}

			if (indexForTempo==0)			
				beatParams.SetInterval( 60.0*mConfig.GetSampleRate()/((mConfig.GetTempoLimSup()+mConfig.GetTempoLimInf())/2) );
			else
				beatParams.SetInterval( peaks[indexForTempo].GetPosition() );

			return true;
		}
		
	}
}
