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

#include "3BandCompressor.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, ThreeBandCompressor> regThreeBandCompressor("ThreeBandCompressor");
}

bool ThreeBandCompressor::Do(const Spectrum& in, Spectrum& out)
{
	out = in; //even if out needs to be preserved, we need to copy content
	
	DataArray& oMag = out.GetMagBuffer();
	DataArray& iMag = in.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	
	int lowCutoff = Round(mLowCutoffFreqCtl.GetLastValue()* spectralResolution);
	int highCutoff = 	Round(mHighCutoffFreqCtl.GetLastValue()* spectralResolution);

	//note: control is supposed to be sent as dB's
	TData lowThreshold = 	log2lin(mLowThresholdCtl.GetLastValue());
	TData midThreshold = 	log2lin(mMidThresholdCtl.GetLastValue());
	TData highThreshold = 	log2lin(mHighThresholdCtl.GetLastValue());
	
	int i;
	TData currentThreshold = lowThreshold;
	for( i = 0; i<lowCutoff; i++)
	{
		if(iMag[i]>lowThreshold)
			oMag[i] = lowThreshold;
	}
	for( i = lowCutoff; i<highCutoff; i++)
	{
		if(iMag[i]>midThreshold)
			oMag[i] = midThreshold;
	}
	for( i = highCutoff; i<spectrumSize; i++)
	{
		if(iMag[i]>highThreshold)
			oMag[i] = highThreshold;
	}
	return true;
}


}

