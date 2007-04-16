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

#ifndef _SDIFOut_
#define _SDIFOut_

#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "Segment.hxx"
#include "Filename.hxx"


namespace SDIF {class File;};//forward declaration

namespace CLAM{

class SDIFOutConfig:public ProcessingConfig
{
public:

	DYNAMIC_TYPE_USING_INTERFACE (SDIFOutConfig, 9, ProcessingConfig);
	DYN_ATTRIBUTE(0,public, TData, SpectralRange);
	DYN_ATTRIBUTE(1,public, TIndex, MaxNumPeaks);
	DYN_ATTRIBUTE(2,public, bool,EnableResidual);
	DYN_ATTRIBUTE(3,public, bool,EnablePeakArray);
	DYN_ATTRIBUTE(4,public, bool,EnableFundFreq);
	DYN_ATTRIBUTE(5,public, Filename,FileName);
	DYN_ATTRIBUTE(6,public, TData, SamplingRate);
	DYN_ATTRIBUTE(7,public, TData, FrameSize);
	DYN_ATTRIBUTE(8,public, TData, SpectrumSize);

	void DefaultInit();
};

class SDIFOut: public Processing
{
public:

	SDIFOut(const SDIFOutConfig& c);
	SDIFOut();
	
	virtual ~SDIFOut();
	const char * GetClassName() const {return "SDIFOut";}
	
	bool GetEnableResidual()        {return mConfig.GetEnableResidual();}
	bool GetEnablePeakArray()       {return mConfig.GetEnablePeakArray();}
	bool GetEnableFundFreq()        {return mConfig.GetEnableFundFreq();}
	
	bool Do(void)
	{
		CLAM_ASSERT(false,"Not implemented");
		return false;
	}
	bool Do(const Frame& frame);

	const ProcessingConfig &GetConfig() const;

	SDIF::File* mpFile;
	
protected:

	bool ConcreteStart();

	bool ConcreteStop();

private:
	
	bool ConcreteConfigure(const ProcessingConfig& c);
	
	SDIFOutConfig mConfig;

// member variables
	IndexArray mPrevIndexArray;
		
};



};//CLAM
#endif

