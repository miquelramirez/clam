#include "AdjustBeatWRTTick.hxx"

namespace CLAM
{

	namespace RhythmDescription
	{
		void AdjustBeatWRTTickConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
		}

		AdjustBeatWRTTick::AdjustBeatWRTTick()
		{
		}

		const char* AdjustBeatWRTTick::GetClassName() const
		{
			return "AdjustBeatWRTTick";
		}

		const ProcessingConfig& AdjustBeatWRTTick::GetConfig() const
		{
			return mConfig;
		}

		bool AdjustBeatWRTTick::ConcreteConfigure( const ProcessingConfig& cfg )
		{
			CopyAsConcreteConfig( mConfig, cfg );

			return true;			
		}

		bool AdjustBeatWRTTick::Do()
		{
			return false;
		}

		bool AdjustBeatWRTTick::Do( const TimeSeriesSeed& tickParams,
					    const TimeSeriesSeed& oldBeatParams,
					    TimeSeriesSeed& refinedBeatParams )
		{
			const TData sampleRate = mConfig.GetSampleRate();
			const TData tempoLimSup = mConfig.GetTempoLimSup();
			const TData tempoLimInf = mConfig.GetTempoLimInf();

			TData newBeatInterval = TData( oldBeatParams.GetInterval()) + TData(tickParams.GetInterval())/2.0;
			newBeatInterval /= TData(tickParams.GetInterval());
			newBeatInterval *= TData(tickParams.GetInterval());

			while ( newBeatInterval < sampleRate*60.0 / tempoLimSup )
				newBeatInterval += TData(tickParams.GetInterval());

			while ( newBeatInterval > sampleRate*60.0/tempoLimInf )
				newBeatInterval -= TData(tickParams.GetInterval());

			if ( newBeatInterval <= 1e-7 )
				newBeatInterval = tickParams.GetInterval();

			refinedBeatParams.SetInterval( unsigned( newBeatInterval ) );

			return true;
		}
		
	}

}
