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

