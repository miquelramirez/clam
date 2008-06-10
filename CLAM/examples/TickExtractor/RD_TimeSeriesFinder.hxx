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
#ifndef _TEMPORAL_SERIES_FINDER_
#define _TEMPORAL_SERIES_FINDER_

/**
 * @class CLAM::TemporalSeriesFinder
 * A processing object that takes a dirac list and returns
 * a temporal serie (offset + interval) within the given
 * bounds so that minimizes the 
 *
 * - By selecting the search bounds for the rhythm interval you can
 * address higher or lower level rhythm units (beat, ticks...)
 * - By selecting the search step you can balance between speed
 * and precission. A two steps approach 
 * 
 * @todo documenting the units for position and output 
 * @todo choosing wether to build the histogram by adding 
 *       deltas or gaussian distributions for each distance.
 * @todo choosing the weigthing: Min (current), Max, Mean, Product...
 * @see CLAM::TemporalSeriesFinderConfig
 */

#include <CLAM/Processing.hxx>
#include <CLAM/Array.hxx>
#include <CLAM/InControl.hxx>
#include "TimeIndex.hxx"


namespace CLAM
{

namespace RhythmDescription
{

class TimeSeriesSeed;

class TimeSeriesFinderConfig : public ProcessingConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (TimeSeriesFinderConfig, 9,ProcessingConfig);
	DYN_ATTRIBUTE (0, public,unsigned int , OffsetMin);
	DYN_ATTRIBUTE (1, public,unsigned int , OffsetMax);
	DYN_ATTRIBUTE (2, public,unsigned int , OffsetStep);
	DYN_ATTRIBUTE (3, public,unsigned int , IntervalMin);
	DYN_ATTRIBUTE (4, public,unsigned int , IntervalMax);
	DYN_ATTRIBUTE (5, public,unsigned int , IntervalStep);
	DYN_ATTRIBUTE (6, public,TData, DeviationPenalty);
	DYN_ATTRIBUTE (7, public,TData, OverSubdivisionPenalty);
	DYN_ATTRIBUTE (8, public,bool, UseOffsetMax);
protected:
	void DefaultInit();	
};

class TimeSeriesFinder : public Processing
{
	TimeSeriesFinderConfig mConfig;

protected:
	bool ConcreteConfigure(const ProcessingConfig&);

public:
	TimeSeriesFinder();

	~TimeSeriesFinder();

	const ProcessingConfig &GetConfig() const { return mConfig;}

	bool Do(void);
	bool Do(const Array<TimeIndex>& indexes, TimeSeriesSeed& tss);
	const char* GetClassName() const;

private:
	InControl mOffsetMin;
	InControl mOffsetMax;
	InControl mOffsetStep;
	InControl mIntervalMin;
	InControl mIntervalMax;
	InControl mIntervalStep;
	InControl mOverSubdivisionPenalty;

	static const unsigned long mInfinite;

};

}

} // namespace CLAM

#endif //_TEMPORAL_SERIES_FINDER_

