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

#ifndef _SDIFStreamingSource_
#define _SDIFStreamingSource_

#include "Segment.hxx"
#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "OutPort.hxx"
#include "Filename.hxx"
#include "OutPort.hxx" 
#include "SDIFInConfig.hxx"

namespace SDIF { class File; } //forward declaration

namespace CLAM
{

class SDIFStreamingSource: public Processing
{
public:

	SDIFStreamingSource(const SDIFInConfig& c);
	SDIFStreamingSource();
	
	virtual ~SDIFStreamingSource();

	const char * GetClassName() const {return "SDIFStreamingSource";}
	
	bool GetEnableResidual()        {return mConfig.GetEnableResidual();}
	bool GetEnablePeakArray()       {return mConfig.GetEnablePeakArray();}
	bool GetEnableFundFreq()        {return mConfig.GetEnableFundFreq();}
	
	bool Do(void);

	bool Do( CLAM::Fundamental& fundamental, CLAM::Spectrum& residualSpectrum, CLAM::SpectralPeakArray& spectralPeaks );

	const ProcessingConfig &GetConfig() const;

	SDIF::File* mpFile;
	OutPort<Fundamental> mFundamentalOutput;
	OutPort<Spectrum> mResidualSpectrumOutput;
	OutPort<SpectralPeakArray> mSpectralPeakArrayOutput;
    
protected:

	bool ConcreteStart();

	bool ConcreteStop();

	bool LoadFrameOfSDIFData( CLAM::Fundamental& fundamental, CLAM::Spectrum& residualSpectrum, CLAM::SpectralPeakArray& spectralPeaks );

private:
	
	bool ConcreteConfigure(const ProcessingConfig& c);
	
	SDIFInConfig mConfig;

// member variables
	TTime mLastCenterTime;
	IndexArray mPrevIndexArray;
	
};


};//CLAM
#endif

