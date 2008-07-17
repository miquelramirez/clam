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

#include "SpectralNotch.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "SpectralNotch",
	//	"category", "Spectral Transformations",
	//	"description", "SpectralNotch",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SpectralNotch> reg = metadata;
}

bool SpectralNotch::Do(const Spectrum& in, Spectrum& out)
{
	out = in; //we need to copy input to output no matter what
	
	DataArray& outMag = out.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	int centerFreq = Round(mFreq.GetLastValue()*spectralResolution);
	int band = Round(mBandwidthCtl.GetLastValue()*spectralResolution);
	TData gain = log2lin(mGainCtl.GetLastValue());
	int n;
	int harmonic = 1;
	int leftLimit = centerFreq-band;
	if(leftLimit<0) leftLimit = 0;
	int rightLimit = centerFreq+band+1;
	if(rightLimit>spectrumSize) rightLimit=spectrumSize;
	for (n=leftLimit; n<rightLimit;n++)
	{ 
		outMag[n] *= gain ;
	}
	//implement small rolloff to avoid Gibbs effect
	outMag[leftLimit-1] *= (gain*0.5); 
	outMag[rightLimit] *= (gain*0.5); 
	return true;
}


}

