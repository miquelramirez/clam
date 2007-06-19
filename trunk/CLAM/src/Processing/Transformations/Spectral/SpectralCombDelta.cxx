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

#include "SpectralCombDelta.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static FactoryRegistrator<ProcessingFactory, SpectralCombDelta> regSpectralCombDelta("SpectralCombDelta");
}

bool SpectralCombDelta::Do(const Spectrum& in, Spectrum& out)
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
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	int fundamental = Round(mFundamentalCtl.GetLastValue()* spectralResolution);
	TData overallFactor = log2lin(mAmount.GetLastValue());
	int i;
	int n;
	//first we set everything to 0 (it could be done more efficiently)
	for(i = 0; i<spectrumSize; i++)
	{
		mMag[i] = 0.;
	}
	n = 1;
	for(i = 0; i<spectrumSize; i++)
	{
		if(i == int (TData(fundamental) * n))
		{
			mMag[i] = inMag[i]*overallFactor;
			n++;
		}
	}
	out.SetMagBuffer(mMag);
	return true;
}

/*bool SpectralCombDelta::Do(const Spectrum& in, Spectrum& out)
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
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	TData freqFactor = mFreqSpaceFactorCtl.GetLastValue();
	int fundamental = Round(mFundamentalCtl.GetLastValue()* spectralResolution);
	TData overallFactor = log2lin(mAmount.GetLastValue());
	int i;
	TData n;
	//first we set everything to 0 (it could be done more efficiently)
	for(i = 0; i<spectrumSize; i++)
	{
		mMag[i] = 0.;
	}
	n = 1.;
	for(i = 0; i<spectrumSize; i++)
	{
		if(i == int (TData(fundamental) * n))
		{
			mMag[i] = inMag[i]*overallFactor;
			n*=freqFactor;
		}
	}
	out.SetMagBuffer(mMag);
	return true;
}
*/


}

