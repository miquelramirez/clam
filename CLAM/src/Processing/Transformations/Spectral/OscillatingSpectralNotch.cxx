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

#include "OscillatingSpectralNotch.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "OscillatingSpectralNotch",
		"category", "Spectral Transformations",
		"description", "OscillatingSpectralNotch",
		0
	};
	static FactoryRegistrator<ProcessingFactory, OscillatingSpectralNotch> reg = metadata;
}

bool OscillatingSpectralNotch::Do(const Spectrum& in, Spectrum& out)
{
	out = in; //we need to copy input to output no matter what
	
	DataArray& outMag = out.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	
	mOscillator.GetInControl("Pitch").DoControl(mOscFreqCtl.GetLastValue());
	//note that oscillator depth is given in %
	mOscillator.GetInControl("Amplitude").DoControl(mOscWidthCtl.GetLastValue()*0.01);
	//kludge: this is not taking into account the possible overlap between windows!!
	mOscillator.GetInControl("SamplesBetweenCalls").DoControl((spectrumSize-1)/2);
	
	TData currentOscAmp;
	mOscillator.Do(currentOscAmp);
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	int centerFreq= Round(mFreqCtl.GetLastValue()*spectralResolution*(1+currentOscAmp));
	int band = Round(mBandwidthCtl.GetLastValue()*spectralResolution);
	TData gain = log2lin(mGainCtl.GetLastValue());
	int n;
	int leftLimit = centerFreq-band;
	if(leftLimit<0) leftLimit = 0;
	int rightLimit = centerFreq+band+1;
	if(rightLimit>spectrumSize) rightLimit=spectrumSize;
	for (n=leftLimit; n<rightLimit;n++)
	{ 
		outMag[n] *= gain ;
	}
	return true;
}


}

