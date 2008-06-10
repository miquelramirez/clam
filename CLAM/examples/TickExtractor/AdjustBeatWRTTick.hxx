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

#ifndef __ADJUSTBEATWRTTICK__
#define __ADJUSTBEATWRTTICK__


#include <CLAM/Processing.hxx>
#include <CLAM/DataTypes.hxx>
#include "RD_TimeSeriesSeed.hxx"

namespace CLAM
{

	namespace RhythmDescription
	{
		class AdjustBeatWRTTickConfig : public ProcessingConfig
		{
			DYNAMIC_TYPE_USING_INTERFACE( AdjustBeatWRTTickConfig, 3, ProcessingConfig );
			DYN_ATTRIBUTE( 0, public, TData, SampleRate );
			DYN_ATTRIBUTE( 1, public, TData, TempoLimSup );
			DYN_ATTRIBUTE( 2, public, TData, TempoLimInf );

		protected:

			void DefaultInit();
		};

		class AdjustBeatWRTTick : public Processing
		{
		public:
			AdjustBeatWRTTick();

			const char* GetClassName() const;

			const ProcessingConfig& GetConfig() const;

			bool Do();
			bool Do( const TimeSeriesSeed& tickParams,
				 const TimeSeriesSeed& oldBeatParams,
				 TimeSeriesSeed& refinedBeatParams );

		protected:

			bool ConcreteConfigure( const ProcessingConfig& cfg );

		private:

			AdjustBeatWRTTickConfig   mConfig;
		};
	}

}


#endif // AdjustBeatWRTTick.hxx

