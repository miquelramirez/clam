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

#include "SDIFIn.hxx"
#include "SpectrumConfig.hxx"
#include "ErrOpenFile.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "Fundamental.hxx"
#include "SDIFFile.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"
#include "Fundamental.hxx"
#include "Spectrum.hxx"
#include "SpectralPeakArray.hxx"

using std::iterator;

namespace CLAM
{

SDIFIn::SDIFIn():
	mOutput("Output",this),
	mSDIFReader(),
	mPrevIndexArray(0)
{
	mLastCenterTime=-1;

	Configure(SDIFInConfig());
}

SDIFIn::SDIFIn(const SDIFInConfig& c):
	mOutput("Output",this),
	mSDIFReader(c),
	mPrevIndexArray(0)
{
	mLastCenterTime=-1;

	Configure(c);
}

SDIFIn::~SDIFIn()
{
}

bool SDIFIn::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	mSDIFReader.Configure(mConfig);

	return true;
}

bool SDIFIn::ConcreteStart()
{
	return true;
}

bool SDIFIn::ConcreteStop()
{
	return true;
}

const ProcessingConfig& SDIFIn::GetConfig() const
{
	return mConfig;
}

bool SDIFIn::LoadSDIFDataIntoSegment( CLAM::Segment& segment )
{

//	CLAM::Fundamental fundamental;
//	CLAM::Spectrum residualSpectrum;
//	CLAM::SpectralPeakArray spectralPeaks;

	// TODO: should you be recreating these objects every time?  what would happen if you
	// used a single object?  would the data be overwritten in the Reader or would
	// the new data be merely appended to the old?
	Frame aFrame;
	aFrame.AddSpectralPeakArray();
	aFrame.AddResidualSpec();
	aFrame.AddFundamental();
	aFrame.UpdateData();
	TTime frameCenterTime;

//	Frame& tmpFrame=segment.GetFrame(segment.GetnFrames()-1);

	// although it might be better to do this in a while loop, historically this method
	// was called once for each frame of data to be loaded. so, for backwards compatability
	// we will continue to do it this way.
	bool response = mSDIFReader.ReadFrame(	aFrame.GetFundamental(), aFrame.GetSpectralPeakArray(),
											aFrame.GetResidualSpec(), frameCenterTime );

	if (response == true)
	{
		aFrame.SetCenterTime(frameCenterTime);

		segment.SetSamplingRate(mSDIFReader.GetSamplingRate());

		segment.AddFrame(aFrame);
	}

	return response;
}

bool SDIFIn::Do( CLAM::Segment& segment )
{
	//TODO uncomment: bool thereIsMoreData = false;

//TODO	while( ( thereIsMoreData = LoadSDIFDataIntoSegment( segment ) ) );
	return LoadSDIFDataIntoSegment( segment );

	return true;
}

bool SDIFIn::Do(void)
{
	bool result = LoadSDIFDataIntoSegment( mOutput.GetData() );
	//mOutput.Produce();
	return result;
}

} // namespace CLAM

