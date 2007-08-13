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

#include "HumRemover.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "HumRemover",
	//	"category", "Spectral Transformations",
	//	"description", "HumRemover",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, HumRemover> regHumRemover("HumRemover");
	static FactoryRegistrator<ProcessingFactory, HumRemover> reg = metadata;
}

bool HumRemover::Do(const Spectrum& in, Spectrum& out)
{
	out = in; //we need to copy input to output no matter what
	
	DataArray& outMag = out.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	int humFundamental= Round(mAmount.GetLastValue()*spectralResolution);
	int band = Round(mBandwidthCtl.GetLastValue()*spectralResolution);
	int nHarmonics = Round(mHarmonicsCtl.GetLastValue());
	int i, n;
	int harmonic = 1;
	for(i = 0; i<spectrumSize; i++)
	{
		if(i == humFundamental*harmonic)
		{
			int leftLimit = i-band;
			if(leftLimit<0) leftLimit = 0;
			int rightLimit = i+band+1;
			if(rightLimit>spectrumSize) rightLimit=spectrumSize;
			for (n=leftLimit; n<rightLimit;n++)
			{ 
				outMag[n] = 0;
			}
			harmonic++;
		}
		if (harmonic == nHarmonics) break;
	}
	return true;
}



}

