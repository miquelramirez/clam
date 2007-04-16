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

#include "NoSpectralTransformation.hxx"
#include "Factory.hxx"

namespace CLAM
{


bool NoSpectralTransformation::Do(const Spectrum& in, Spectrum& out)
{
	if (!mConfig.GetPreserveOuts())
	{
		out = in; //TODO big cludge for streaming
	}
	DataArray& inMag = in.GetMagBuffer();
	DataArray& outMag = out.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	
	//control is supposed to be in dB's
	TData gain = log2lin(mAmount.GetLastValue());
	int i;	
	for(i = 0; i<spectrumSize; i++)
	{
		outMag[i] = inMag[i] * gain;
	}
	return true;
}

typedef Factory<Processing> ProcessingFactory;

static ProcessingFactory::Registrator<NoSpectralTransformation> regtNoSpectralTransformation( "NoSpectralTransformation" );

}

