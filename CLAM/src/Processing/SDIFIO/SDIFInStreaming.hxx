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

#ifndef _SDIFInStreaming_
#define _SDIFInStreaming_

#include "Segment.hxx"
#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "OutPort.hxx"
#include "SDIFInConfig.hxx"
#include "Frame.hxx"
#include "SDIFFileReader.hxx"

namespace CLAM
{

class SDIFInStreaming : public Processing
{
public:

	SDIFInStreaming(const SDIFInConfig& c);
	SDIFInStreaming();

	virtual ~SDIFInStreaming();

	const char * GetClassName() const {return "SDIFInStreaming";}

	bool Do(void);

	const ProcessingConfig &GetConfig() const;

	OutPort<Fundamental> mFundamentalOutput;
	OutPort<Spectrum> mResidualSpectrumOutput;
	OutPort<SpectralPeakArray> mSpectralPeakArrayOutput;

private:

	bool ConcreteConfigure(const ProcessingConfig& c);

	SDIFInConfig mConfig;

// member variables
	SDIFFileReader mSDIFReader;
};


};//CLAM
#endif

