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

#include "SpectralExciter.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "SpectralExciter",
	//	"category", "Spectral Transformations",
	//	"description", "SpectralExciter",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, SpectralExciter> regSpectralExciter("SpectralExciter");
	static FactoryRegistrator<ProcessingFactory, SpectralExciter> reg = metadata;
}


bool SpectralExciter::Do(const Spectrum& in, Spectrum& out)
{
	out = in; //we are better off copying it no matter whether out has to be preserved or not.
		
	
	int spectrumSize = in.GetSize();
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	
	int highCutoff = Round(mHighCutoffFreqCtl.GetLastValue()* spectralResolution);

	TData threshold = log2lin(mThresholdCtl.GetLastValue());
	TData gain = log2lin(mHarmonicsGainCtl.GetLastValue());
	
	DataArray& outMag = out.GetMagBuffer();
	
	int i,n;
	
	//note: we might want to do something to the phase
	for(i = spectrumSize; i>highCutoff; i--)
	{
		if(outMag[i]>threshold) 
		{
			for (n=i*2; n<spectrumSize; n+=i) {
				outMag[n] += outMag[i]*gain;
				if (outMag[n]>1.) {
					outMag[n]=1.;
				}
			}
		}
	}
	return true;
}


}

