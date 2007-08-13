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

#include "SpectralAverage.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "SpectralAverage",
	//	"category", "Spectral Transformations",
	//	"description", "SpectralAverage",
		0
	};
	//static FactoryRegistrator<ProcessingFactory, SpectralAverage> regSpectralAverage("SpectralAverage");
	static FactoryRegistrator<ProcessingFactory, SpectralAverage> reg = metadata;
}

bool SpectralAverage::Do(const Spectrum& in, Spectrum& out)
{
	if (!mConfig.GetPreserveOuts())
	{
		out = in; //TODO big cludge for streaming
	}
	DataArray& inMag = in.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	
	int nPointsToAverage = int(mAmount.GetLastValue());
	int nPointsToAverageHalf = int(mAmount.GetLastValue()* 0.5);
	TData oneOverNPointsToAverage = 1./nPointsToAverage;
		
	mMag.Resize(spectrumSize);
	mMag.SetSize(spectrumSize);
	int i, z;
	TData average;
	for(i = nPointsToAverageHalf; i<spectrumSize-nPointsToAverageHalf; i++)
	{
		for(z=i-nPointsToAverageHalf; z<i+nPointsToAverageHalf; z++)
		{
			average+=inMag[z];
		}
		mMag[i] = average*oneOverNPointsToAverage;
		average = 0;
	}
	out.SetMagBuffer(mMag);
	return true;
}


}

