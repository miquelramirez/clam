#include "AdjustTickWRTSwing.hxx"
#include "IOIHistogram.hxx"
#include <list>

namespace CLAM
{
	namespace RhythmDescription
	{
		void AdjustTickWRTSwingConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
		}

		AdjustTickWRTSwing::AdjustTickWRTSwing()
		{
		}

		const char* AdjustTickWRTSwing::GetClassName() const
		{
			return "AdjustTickWRTSwing";
		}

		const ProcessingConfig& AdjustTickWRTSwing::GetConfig() const
		{
			return mConfig;
		}

		bool AdjustTickWRTSwing::ConcreteConfigure( const ProcessingConfig& cfg )
		{
			CopyAsConcreteConfig( mConfig, cfg );

			mUpperBound = mConfig.GetSampleRate()*60.0/ mConfig.GetTempoLimSup();
			mLowerBound = mConfig.GetSampleRate()*60.0/ mConfig.GetTempoLimInf();		

			return true;
		}

		bool AdjustTickWRTSwing::Do()
		{
			return false;
		}

		bool AdjustTickWRTSwing::Do( const IOIHistogram& hist, const TimeSeriesSeed& prevTick,
					     TimeSeriesSeed& refinedTick )
		{
			//quarter-note is either = tick, 2 ticks, 3 ticks or 4 ticks
			TData quarternote = prevTick.GetInterval();
			TData max = 0.0;
		
			std::list<TData> candidates;
			typedef std::list<TData>::iterator LI;
		
			TData* arr = hist.GetBins().GetPtr();		
			TData tmpCand= prevTick.GetInterval();

			for(int i=0;i<3;i++) 
			{
				if( (tmpCand>mUpperBound) 
				    && (tmpCand<mLowerBound) ) 
				{
					candidates.push_back(tmpCand);
				}
			
				tmpCand += prevTick.GetInterval();
			}
		
			for( LI i=candidates.begin();
			     i != candidates.end(); i++) 
			{
				if (arr[(int)(*i)] > max) 
				{
					max = arr[(int)*i];
					quarternote = *i;	
				} 
			}
		

			refinedTick.SetInterval( (unsigned)quarternote );	

			return true;
		}

	}
}
