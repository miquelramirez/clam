/*
 * Author: fabien gouyon 
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
#ifndef _TIME_DIFFERENCE_H_
#define _TIME_DIFFERENCE_H_

/**
 * @class CLAM::TimeDifference
 * A processing object that computes the histogram of position
 * differences between each pair in a weigthed dirac list
 * (Inter onset interval histogram).
 * The histogram is smoothed by adding a gaussian window
 * weighted by the minimum weight of each dirac pair.
 * 
 * If there is any uniform rhythmic pattern, the histogram is 
 * expected to have equidistant maximums over the multiples of
 * the basic rhythm interval.
 * @todo choosing the weigthing: Min (current), Max, Mean, Product...
 * @see TimeDifferenceConfig
 */

#include <CLAM/ProcessingComposite.hxx>
#include <CLAM/Array.hxx>
#include "TimeIndex.hxx"
#include <CLAM/WindowGenerator.hxx>
#include "IOIHistogram.hxx"

namespace CLAM 
{
	namespace RhythmDescription
	{
		
		class TimeDifferenceConfig: public ProcessingConfig
		{
		public:
			DYNAMIC_TYPE_USING_INTERFACE (TimeDifferenceConfig, 1,ProcessingConfig);
			DYN_ATTRIBUTE (0,public, TSize, GaussianSize);
		protected:
			void DefaultInit();

		};

		class TimeDifference: public Processing 
		{
			TimeDifferenceConfig mConfig;
			WindowGenerator      mWindowGen;
			DataArray            mWindow;

			const char *GetClassName() const;
		
			bool ConcreteConfigure(const ProcessingConfig&);

		public:
			TimeDifference();
			TimeDifference(const TimeDifferenceConfig &c);
			~TimeDifference();

			const ProcessingConfig &GetConfig() const { return mConfig;}

			bool Do(void);
			bool Do(const Array<TimeIndex>& input, IOIHistogram& out);
		};

	}

} // namespace CLAM

#endif // _TIME_DIFFERENCE_H_

