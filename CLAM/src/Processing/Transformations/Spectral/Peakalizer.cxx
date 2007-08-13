/*
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

#include "Peakalizer.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Peakalizer",
		"category", "Spectral Transformations",
		"description", "Peakalizer",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, Peakalizer> regPeakalizer("Peakalizer");
	static FactoryRegistrator<ProcessingFactory, Peakalizer> reg = metadata;
}

bool Peakalizer::Do(const Spectrum& in, Spectrum& out)
{
	if (!mConfig.GetPreserveOuts())
	{
		out = in; //TODO big cludge for streaming
	}
	DataArray& inMag = in.GetMagBuffer();
	DataArray& outMag = out.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	mMag.Resize(spectrumSize);
	mMag.SetSize(spectrumSize);
	
	//control is supposed to be in dB's
	TData threshold = log2lin(mThresholdCtl.GetLastValue());
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	int bandwidth = Round(mBandWidthCtl.GetLastValue()* spectralResolution*0.5);
	
	int i,n;
	//first we set everything to 0 (it could be done more efficiently)
	for(i = 0; i<spectrumSize; i++)
	{
		mMag[i] = 0.;
	}
	for(i = 0; i<spectrumSize; i++)
	{
		if(inMag[i] > threshold)
		{
			for(n = i-bandwidth; n < i+bandwidth; n++)
			{
				mMag[n] = inMag[n];
			}
		}
	}
	out.SetMagBuffer(mMag);
	return true;
}



}

