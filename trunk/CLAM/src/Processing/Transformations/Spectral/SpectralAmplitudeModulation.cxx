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

#include "SpectralAmplitudeModulation.hxx"
#include "Factory.hxx"

namespace CLAM
{


bool SpectralAmplitudeModulation::Do(const Spectrum& in, Spectrum& out)
{
	if ( !mConfig.GetPreserveOuts() )
	{
		out = in; //TODO big cludge for streaming
	}
	
	int spectrumSize = in.GetSize();
	
	mOscillator.GetInControl("Pitch").DoControl(mFreqCtl.GetLastValue());
	mOscillator.GetInControl("Amplitude").DoControl(mWidthCtl.GetLastValue());
	//kludge: this is not taking into account the possible overlap between windows!!
	mOscillator.GetInControl("SamplesBetweenCalls").DoControl((spectrumSize-1)/2);
	
	DataArray& oMag = out.GetMagBuffer();
	
	TData modFactor;
	mOscillator.Do(modFactor);
	
	for(int i = 0; i<spectrumSize; i++)
	{
		oMag[i] = oMag[i] * (1 + modFactor);
	}
	return true;
}

typedef Factory<Processing> ProcessingFactory;

static ProcessingFactory::Registrator<SpectralAmplitudeModulation> regtSpectralAmplitudeModulation( "SpectralAM" );

}

