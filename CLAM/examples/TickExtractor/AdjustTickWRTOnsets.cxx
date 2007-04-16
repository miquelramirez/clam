/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "AdjustTickWRTOnsets.hxx"

namespace CLAM
{
	namespace RhythmDescription
	{
		void AdjustTickWRTOnsetsConfig::DefaultInit()
		{
			AddAll();
			UpdateData();
		}

		AdjustTickWRTOnsets::AdjustTickWRTOnsets()
			: mTSF_OffsetMin( "TSF/OffsetMin", this ),
			  mTSF_OffsetStep( "TSF/OffsetStep", this ),
			  mTSF_IntervalMin( "TSF/IntervalMin", this ),
			  mTSF_IntervalMax( "TSF/IntervalMax", this ),
			  mTSF_IntervalStep( "TSF/IntervalStep", this ),
			  mTSF_OverSubdivisionPenalty( "TSF/OverSubdivisionPenalty", this),
			  mFirstTransientPosition( "FirstTransientPosition", this ),
			  mLastTransientPosition( "LastTransientPosition", this )
		{
			mGridGenerator.SetParent( this );
			mTimeSeriesFinder.SetParent( this );
		}

		const char* AdjustTickWRTOnsets::GetClassName() const
		{
			return "AdjustTickWRTOnsets";
		}

		const ProcessingConfig& AdjustTickWRTOnsets::GetConfig() const
		{
			return mConfig;
		}

		bool AdjustTickWRTOnsets::ConcreteConfigure( const ProcessingConfig& cfg )
		{
			CopyAsConcreteConfig( mConfig, cfg );
			
			TimeSeriesFinderConfig tsfConfig;

			tsfConfig.SetDeviationPenalty( mConfig.GetDeviationPenalty() );
			tsfConfig.SetOverSubdivisionPenalty( mConfig.GetOverSubdivisionPenalty() );

			mTimeSeriesFinder.Configure( tsfConfig );

			PulseGridGeneratorConfig pulseGridConf;

			mGridGenerator.Configure( pulseGridConf );

			return true;
		}

		bool AdjustTickWRTOnsets::Do()
		{
			return false;
		}

		bool AdjustTickWRTOnsets::Do( const Array<TimeIndex>& transients,
					      const TimeSeriesSeed& previousSeed,
					      Array<TimeIndex>& ticks,
					      TimeSeriesSeed& refinedSeed )
		{
			
			unsigned int scope = (unsigned int)std::min(TData(mConfig.GetScope()*mConfig.GetSampleRate()),
								    TData(previousSeed.GetInterval()*0.5));
			
			mTimeSeriesFinder.GetInControl("OffsetMin").DoControl( 0 );
			mTimeSeriesFinder.GetInControl("OffsetStep").DoControl( 50 );
			mTimeSeriesFinder.GetInControl("IntervalMin").DoControl(
				std::max(TData(previousSeed.GetInterval()-scope*0.5),TData(mConfig.GetTickLimSup())) );
			mTimeSeriesFinder.GetInControl("IntervalMax").DoControl(previousSeed.GetInterval()+scope/2);
			mTimeSeriesFinder.GetInControl("IntervalStep").DoControl( 10 );
			mTimeSeriesFinder.GetInControl("OverSubdivisionPenalty").DoControl( 0 );

			
			int firstTransientPosition = (int) mFirstTransientPosition.GetLastValue();
			int lastTransientPosition = (int) mLastTransientPosition.GetLastValue();


			mTimeSeriesFinder.Do( transients, refinedSeed );

			TData start = (firstTransientPosition+refinedSeed.GetInterval())/mConfig.GetSampleRate();
			TData gap = refinedSeed.GetInterval()/mConfig.GetSampleRate();
			TData end = lastTransientPosition/mConfig.GetSampleRate();

			int nUnits = 1 + (end-start)/gap;

			ticks.Resize( nUnits );
			ticks.SetSize( nUnits );

			mGridGenerator.GetInControl( "Start" ).DoControl( start );
			mGridGenerator.GetInControl( "Gap" ).DoControl( gap );
			mGridGenerator.GetInControl( "End" ).DoControl( end );

			mGridGenerator.Do( ticks );

			return true;
		}
	}
	
}

