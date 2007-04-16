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

