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

#ifndef _SDIFFileReader_
#define _SDIFFileReader_

#include "IndexArray.hxx"
#include "Processing.hxx"
#include "Err.hxx"
#include "OutPort.hxx"
#include "Filename.hxx"
#include "OutPort.hxx"
#include "SDIFInConfig.hxx"
#include "SDIFFrame.hxx"
#include "Fundamental.hxx"
#include "SpectralPeakArray.hxx"
#include "Spectrum.hxx"

namespace SDIF { class File; } //forward declaration

namespace CLAM
{

class SDIFFileReader
{
public:

	//TODO maybe a normal constructor?
	SDIFFileReader(const SDIFInConfig& c);
	SDIFFileReader();

	virtual ~SDIFFileReader();

	bool GetEnableResidual()        { return mConfig.GetEnableResidual(); }
	bool GetEnablePeakArray()       { return mConfig.GetEnablePeakArray(); }
	bool GetEnableFundFreq()        { return mConfig.GetEnableFundFreq(); }

	bool ReadFrame(	CLAM::Fundamental& argFundamental,
					CLAM::SpectralPeakArray& argSpectralPeaks,
					CLAM::Spectrum& argResidual);

	bool ReadFrame(	CLAM::Fundamental& argFundamental,
					CLAM::SpectralPeakArray& argSpectralPeaks,
					CLAM::Spectrum& argResidual,
					TTime& argFrameCenterTime);

	int GetSamplingRate();

	int GetFrameCenterTime();

	const ProcessingConfig &GetConfig() const;

	bool Configure(const SDIFInConfig& c);

	SDIF::File* mpFile;

private:
	bool OpenFile();

	void CopyFramesDataObjects(	SDIF::Frame& tmpSDIFFrame,
								CLAM::Fundamental& argFundamental,
								CLAM::SpectralPeakArray& argSpectralPeaks,
								CLAM::Spectrum& argResidual);

	SDIFInConfig mConfig;

// member variables
	TTime mLastCenterTime;
	int mSamplingRate;
	IndexArray mPrevIndexArray;
	bool mIsFileOpened;
};


};//CLAM
#endif

