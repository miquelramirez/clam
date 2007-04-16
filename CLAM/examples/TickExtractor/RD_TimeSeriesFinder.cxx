/*
 * Author: fabien gouyon 
 *			David Garcia
 * http://www.iua.upf.es/~fgouyon
 * Description:
 *
 * Syntax: C++
 *
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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
#include "RD_TimeSeriesFinder.hxx"
#include "RD_TimeSeriesSeed.hxx"
#include "CLAM_Math.hxx"

namespace CLAM 
{

	namespace RhythmDescription
	{

		void TimeSeriesFinderConfig::DefaultInit()
		{
			AddAll();
			// All Attributes are added 
			UpdateData();
			//default values
			SetDeviationPenalty(0.5);
			SetOverSubdivisionPenalty(0.5);
			SetUseOffsetMax(false);
		}

		const unsigned long TimeSeriesFinder::mInfinite = 0xFFFFFFFF;

		TimeSeriesFinder::TimeSeriesFinder() :
			mOffsetMin("OffsetMin",this),
			mOffsetMax("OffsetMax",this),
			mOffsetStep("OffsetStep",this),
			mIntervalMin("IntervalMin",this),
			mIntervalMax("IntervalMax",this),
			mIntervalStep("IntervalStep",this),
			mOverSubdivisionPenalty("OverSubdivisionPenalty",this)
		{

		}

		TimeSeriesFinder::~TimeSeriesFinder()
		{
		}

		// Configure the Processing Object according to the Config object
		bool TimeSeriesFinder::ConcreteConfigure(const ProcessingConfig& c)
		{
			CopyAsConcreteConfig( mConfig, c );
			mOffsetMin.DoControl(mConfig.GetOffsetMin());
			mOffsetMax.DoControl(mConfig.GetOffsetMax());
			mOffsetStep.DoControl(mConfig.GetOffsetStep());
			mIntervalMin.DoControl(mConfig.GetIntervalMin());
			mIntervalMax.DoControl(mConfig.GetIntervalMax());
			mIntervalStep.DoControl(mConfig.GetIntervalStep());
			mOverSubdivisionPenalty.DoControl(mConfig.GetOverSubdivisionPenalty());
			
			return true;
		}

		const char* TimeSeriesFinder::GetClassName() const
		{
			return "TimeSeriesFinder";
		}

		bool  TimeSeriesFinder::Do(void)
		{
			return false;
		}

		bool TimeSeriesFinder::Do(const Array<TimeIndex>& diracs, TimeSeriesSeed& tss)
		{
			const unsigned int offsetMin = (unsigned int)(mOffsetMin.GetLastValue()+0.5);
			unsigned int offsetMax = (unsigned int) (mOffsetMax.GetLastValue()+0.5);
			const unsigned int offsetStep = (unsigned int)(mOffsetStep.GetLastValue()+0.5);
			const unsigned int intervalMin = (unsigned int)(mIntervalMin.GetLastValue()+0.5);
			const unsigned int intervalMax = (unsigned int)(mIntervalMax.GetLastValue()+0.5);
			const unsigned int intervalStep = (unsigned int)(mIntervalStep.GetLastValue()+0.5);
			const TData deviationPenalty = mConfig.GetDeviationPenalty();
			const TData overSubdivisionPenalty = mOverSubdivisionPenalty.GetLastValue();

			CLAM_ASSERT(offsetStep>0, "TimeSeriesFinder::Do() : No valid offset step");
			CLAM_ASSERT(intervalMin<intervalMax, "TimeSeries::Do() : No valid interval range");
			//TODO: Add some other restriction?

			const unsigned int nDiracs = diracs.Size();

			CLAM_BEGIN_DEBUG_CHECK
				unsigned lastDiracPosition=0;
			for (int j=0; j < nDiracs; j++) {
				unsigned currentDiracPosition= (unsigned)diracs[j].GetPosition();
				CLAM_DEBUG_ASSERT(currentDiracPosition>=lastDiracPosition,
						  "Dirac are not sorted");
				lastDiracPosition=currentDiracPosition;
			}
			CLAM_END_DEBUG_CHECK;

			unsigned int selectedOffset=0;
			unsigned int selectedInterval=intervalMin;
			TData selectedPenalty = mInfinite;

			//version 1
			const bool useOM = mConfig.GetUseOffsetMax();
			
			for(unsigned int interval=intervalMin; interval<intervalMax; interval+=intervalStep)
			{
				const unsigned int halfInterval=interval>>1;

				if (!useOM)
					offsetMax = interval;
				else 
					offsetMax = (unsigned int)(mOffsetMax.GetLastValue()+0.5);
				
				CLAM_ASSERT(offsetMin < offsetMax, "TimeSeriesFinder::Do() : No valid interval range");
				
				for( unsigned int offset=offsetMin; 
				     offset < offsetMax; 
				     offset+=offsetStep )
				{

					TData diracDeviation=0;
					unsigned int nUnusedIntervals=0;
					signed int lastInterval=0;

					for (unsigned int diracIndex=0; diracIndex<nDiracs; diracIndex++)
					{
						const unsigned diracPosition = (unsigned)(diracs[diracIndex].GetPosition());
						const TData diracWeight = diracs[diracIndex].GetWeight();
						const int relativePosition
							= signed(diracPosition+halfInterval)-offset;
						const int currentInterval=(relativePosition)/signed(interval);

						CLAM_DEBUG_ASSERT(currentInterval>=lastInterval, "Dirac are not sorted");

						// In the following we substract 1 in order not to penalize
						// diracs corresponding to successive intervals and penalize
						// with Abs(-1)=1 diracs corresponding to the same interval
						const signed decision =(signed(currentInterval-lastInterval)-1);

						if ( decision > 0)
							nUnusedIntervals+=std::abs(decision);
						//nUnusedIntervals+=Abs(signed(currentInterval-lastInterval)-1);

						lastInterval=currentInterval;

						diracDeviation += diracWeight * std::abs(signed(relativePosition%interval-halfInterval));
					}

					const TData overallPenalty = deviationPenalty*diracDeviation * nDiracs / 20
						+overSubdivisionPenalty*nUnusedIntervals* halfInterval;

					//+overSubdivisionPenalty*nUnusedIntervals * halfInterval;//no use of the 2d error function

					if (selectedPenalty>=overallPenalty)
					{
						selectedOffset = offset;
						selectedInterval = interval;
						selectedPenalty = overallPenalty;
					}
				}
			}
			
			tss.SetOffset(selectedOffset);
			tss.SetInterval(selectedInterval);
			return true;
		}
		
	}
	
} // namespace CLAM

			/*
			//version 2
			for(unsigned int offset=offsetMin; offset<intervalMax; offset+=offsetStep)
			{
			DataArray diracDev;diracDev.Init();
			DataArray unusedIntval;unusedIntval.Init();
			for(unsigned int interval=intervalMin; interval<intervalMax;
			interval+=intervalStep)
			{
			const unsigned int halfInterval=interval>>1;
			unsigned int diracDeviation=0;
			unsigned int nUnusedIntervals=0;
			signed int lastInterval=0;
			for (unsigned int diracIndex=0; diracIndex<nDiracs; diracIndex++)
			{
			const unsigned diracPosition = diracs[diracIndex].GetPosition();
			const int relativePosition
			= signed(diracPosition+halfInterval)-offset;
			const int currentInterval=(relativePosition)/signed(interval);
			CLAM_DEBUG_ASSERT(currentInterval>=lastInterval, "Dirac are not sorted");
			const signed toto=(signed(currentInterval-lastInterval)-1);
			if (toto>0)
			nUnusedIntervals+=Abs(toto);
			//					nUnusedIntervals+=Abs(signed(currentInterval-lastInterval)-1);
			lastInterval=currentInterval;
			diracDeviation+=Abs(signed(relativePosition%interval-halfInterval));
	}
	
	const TData overallPenalty =
		deviationPenalty*diracDeviation * nDiracs / 30
		+overSubdivisionPenalty*nUnusedIntervals * halfInterval;
		
	if (selectedPenalty>=overallPenalty)
	{
		selectedOffset = offset;
		selectedInterval = interval;
		selectedPenalty = overallPenalty;
	}
	cout<<"\tdirdev "<<deviationPenalty*diracDeviation * nDiracs / 30<<endl;
	cout<<"\tunusedInt "<<overSubdivisionPenalty*nUnusedIntervals* halfInterval<<endl;
	diracDev.AddElem(deviationPenalty*diracDeviation * nDiracs / 30);
	unusedIntval.AddElem(overSubdivisionPenalty*nUnusedIntervals* halfInterval);
	}
	cout<<"offset "<<offset<<endl;
	CLAM::ShowSnapshot(diracDev,"diracDeviation");
	CLAM::ShowSnapshot(unusedIntval,"nUnusedIntervals");
	}
*/

