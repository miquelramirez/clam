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

