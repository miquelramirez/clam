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

#include "SDIFInStreaming.hxx"
#include "SpectrumConfig.hxx"
#include "ErrOpenFile.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "SpectralPeakArray.hxx"
#include "Fundamental.hxx"
#include "SDIFFile.hxx"
#include "SDIFFrame.hxx"
#include "SDIFMatrix.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "SDIFInStreaming",
	//	"category", "SDIF I/O",
	//	"description", "SDIFInStreaming",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, SDIFInStreaming> regSDIFInStreaming ("SDIFInStreaming");
	static FactoryRegistrator<ProcessingFactory, SDIFInStreaming> reg = metadata;
}

SDIFInStreaming::SDIFInStreaming():
	mFundamentalOutput("Fundamental",this),
	mResidualSpectrumOutput("Residual Spectrum",this),
	mSpectralPeakArrayOutput("Sinusoidal Peaks",this),
	mSDIFReader(SDIFInConfig())
{
	Configure(SDIFInConfig());
}

SDIFInStreaming::SDIFInStreaming(const SDIFInConfig& c):
	mFundamentalOutput("Fundamental",this),
	mResidualSpectrumOutput("Residual Spectrum",this),
	mSpectralPeakArrayOutput("Sinusoidal Peaks",this),
	mSDIFReader(c)
{
	Configure(c);
}

SDIFInStreaming::~SDIFInStreaming()
{
}

bool SDIFInStreaming::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	mSDIFReader.Configure(mConfig);

	return true;
}

const ProcessingConfig& SDIFInStreaming::GetConfig() const
{
	return mConfig;
}

bool SDIFInStreaming::Do(void)
{
	bool result = mSDIFReader.ReadFrame(	mFundamentalOutput.GetData(),
											mSpectralPeakArrayOutput.GetData(),
											mResidualSpectrumOutput.GetData() );

	mFundamentalOutput.Produce();
	mResidualSpectrumOutput.Produce();
	mSpectralPeakArrayOutput.Produce();

	return result;
}

} // namespace CLAM

