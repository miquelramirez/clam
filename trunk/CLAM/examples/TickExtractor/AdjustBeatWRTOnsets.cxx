#include "AdjustBeatWRTOnsets.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{
		void AdjustBeatWRTOnsetsConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
		}

		AdjustBeatWRTOnsets::AdjustBeatWRTOnsets()
			: mFirstTransientPosition( "FirstTransientPosition", this ),
			  mLastTransientPosition( "LastTransientPosition", this )
		{
			mGridGenerator.SetParent( this );
			mSeriesFinder.SetParent( this );
		}

		const char* AdjustBeatWRTOnsets::GetClassName() const
		{
			return "AdjustBeatWRTOnsets";
		}

		const ProcessingConfig& AdjustBeatWRTOnsets::GetConfig() const
		{
			return mConfig;
		}

		bool AdjustBeatWRTOnsets::ConcreteConfigure( const ProcessingConfig& cfg )
		{
			CopyAsConcreteConfig( mConfig, cfg );
			
			TimeSeriesFinderConfig tsfConfig;

			tsfConfig.SetDeviationPenalty( mConfig.GetDeviationPenalty() );
			tsfConfig.SetOverSubdivisionPenalty( mConfig.GetOverSubdivisionPenalty() );

			mSeriesFinder.Configure( tsfConfig );

			PulseGridGeneratorConfig pulseGridConf;

			mGridGenerator.Configure( pulseGridConf );

			return true;
		}

		bool AdjustBeatWRTOnsets::Do()
		{
			return false;
		}

		bool AdjustBeatWRTOnsets::Do( const Array<TimeIndex>& transients,
					      const TimeSeriesSeed& tickParams,
					      const TimeSeriesSeed& oldBeatParams,
					      Array<TimeIndex>& beats,
					      TimeSeriesSeed& refinedBeats )
		{
			//get the best phase
			// Computing best beat phase
			mSeriesFinder.GetInControl("OffsetMin").DoControl(tickParams.GetOffset());			
			mSeriesFinder.GetInControl("OffsetStep").DoControl(tickParams.GetInterval());
			mSeriesFinder.GetInControl("IntervalMin").DoControl(oldBeatParams.GetInterval());
			mSeriesFinder.GetInControl("IntervalMax").DoControl(oldBeatParams.GetInterval()+1);
			mSeriesFinder.GetInControl("IntervalStep").DoControl(2);
			
			//NB: Use of transients instead of transientsForHist
			// i.e. making use of transient weights
			mSeriesFinder.Do(transients, refinedBeats);
			
			int firstTransPos = (int) mFirstTransientPosition.GetLastValue();
			int lastTransPos = (int) mLastTransientPosition.GetLastValue();

			TData start = (firstTransPos + refinedBeats.GetOffset())/mConfig.GetSampleRate();
			TData gap = refinedBeats.GetInterval()/mConfig.GetSampleRate();
			TData end = lastTransPos/mConfig.GetSampleRate();

			int nUnits = 1 + int((end - start)/gap);

			beats.Resize( nUnits );
			beats.SetSize( nUnits );

			mGridGenerator.GetInControl( "Start" ).DoControl( start );
			mGridGenerator.GetInControl( "Gap" ).DoControl( gap );
			mGridGenerator.GetInControl( "End" ).DoControl( end );
			
			mGridGenerator.Do( beats );
						
			return true;
		}
	}
	
}
