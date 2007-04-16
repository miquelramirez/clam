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

