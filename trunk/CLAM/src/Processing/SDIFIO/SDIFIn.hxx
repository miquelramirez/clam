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

#ifndef _SDIFIn_
#define _SDIFIn_

#include "Segment.hxx"
#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "OutPort.hxx"
#include "SDIFInConfig.hxx"

namespace SDIF { class File; } //forward declaration

namespace CLAM
{

class SDIFIn: public Processing
{
public:

	SDIFIn(const SDIFInConfig& c);
	SDIFIn();
	
	virtual ~SDIFIn();

	const char * GetClassName() const {return "SDIFIn";}
	
	bool GetEnableResidual()        {return mConfig.GetEnableResidual();}
	bool GetEnablePeakArray()       {return mConfig.GetEnablePeakArray();}
	bool GetEnableFundFreq()        {return mConfig.GetEnableFundFreq();}
	
	bool Do(void);

	bool Do( CLAM::Segment& segment );

	const ProcessingConfig &GetConfig() const;

	SDIF::File* mpFile;
	OutPort<Segment> mOutput;
protected:

	bool ConcreteStart();

	bool ConcreteStop();

	bool LoadSDIFDataIntoSegment( CLAM::Segment& s );

private:
	
	bool ConcreteConfigure(const ProcessingConfig& c);
	
	SDIFInConfig mConfig;

// member variables
	TTime mLastCenterTime;
	IndexArray mPrevIndexArray;
	
};


};//CLAM
#endif

