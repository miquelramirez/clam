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

#include "SpectralReverse.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace detail
{
	static FactoryRegistrator<ProcessingFactory, SpectralReverse> regSpectralReverse("SpectralReverse");
}

bool SpectralReverse::Do(const Spectrum& in, Spectrum& out)
{
	if (!mConfig.GetPreserveOuts())
	{
		out = in; //TODO big cludge for streaming
	}
	DataArray& inPhase = in.GetPhaseBuffer();
	DataArray& inMag = in.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	
	int mirrorPoint = Round(mAmount.GetLastValue()* spectralResolution);
		
	mMag.Resize(spectrumSize);
	mMag.SetSize(spectrumSize);
	mPhase.Resize(spectrumSize);
	mPhase.SetSize(spectrumSize);
	int i;
	for(i = 0; i<mirrorPoint; i++)
	{
		mMag[i] = inMag[mirrorPoint-i];
		mPhase[i] = inPhase[mirrorPoint-i];
	}
	for(i = mirrorPoint; i<spectrumSize; i++)
	{
		mMag[i] = inMag[spectrumSize-i+mirrorPoint];
		mPhase[i] = inPhase[spectrumSize-i+mirrorPoint];
	}
	out.SetMagBuffer(mMag);
	out.SetPhaseBuffer(mPhase);
	return true;
}


}

