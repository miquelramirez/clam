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

#include "SpectralRingModulation.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "SpectralRingModulation",
	//	"category", "Spectral Transformations",
	//	"description", "SpectralRingModulation",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SpectralRingModulation> reg = metadata;
}

bool SpectralRingModulation::Do(const Spectrum& in, Spectrum& out)
{
	if ( !mConfig.GetPreserveOuts() )
	{
		out = in; //TODO big cludge for streaming
	}
	
	int spectrumSize = in.GetSize();
	
	SendFloatToInControl(mOscillator,"Pitch",mFreqCtl.GetLastValue());
	SendFloatToInControl(mOscillator,"Amplitude",mWidthCtl.GetLastValue());
	//kludge: this is not taking into account the possible overlap between windows!!
	SendFloatToInControl(mOscillator,"SamplesBetweenCalls",(spectrumSize-1)/2);
	
	DataArray& oMag = out.GetMagBuffer();
	
	TData modFactor;
	mOscillator.Do(modFactor);
	
	for(int i = 0; i<spectrumSize; i++)
	{
		oMag[i] = oMag[i] * modFactor;
	}
	return true;
}



}

