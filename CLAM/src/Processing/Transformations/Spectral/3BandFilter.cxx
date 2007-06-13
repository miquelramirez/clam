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

#include "3BandFilter.hxx"
#include "Factory.hxx"

namespace CLAM
{


bool ThreeBandFilter::Do(const Spectrum& in, Spectrum& out)
{
	if ( !mConfig.GetPreserveOuts() )
	{
		out = in; //TODO big cludge for streaming
	}
	
	DataArray& oMag = out.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	
	int lowCutoff = Round(mLowCutoffFreqCtl.GetLastValue()* spectralResolution);
	int highCutoff = Round(mHighCutoffFreqCtl.GetLastValue()* spectralResolution);

	//note: control is supposed to be sent as dB's
	TData lowGain = 	log2lin(mLowGainCtl.GetLastValue());
	TData midGain = 	log2lin(mMidGainCtl.GetLastValue());
	TData highGain = 	log2lin(mHighGainCtl.GetLastValue());
	
	TData currentGain = lowGain;
	for(int i = 0; i<spectrumSize; i++)
	{
		if(i>highCutoff) 
			currentGain = highGain;
		else if(i==highCutoff)
			currentGain = (highGain+midGain)*0.5; //implementing small rolloff to avoid Gibbs effect
		else if(i>lowCutoff) 
			currentGain = midGain;
		else if(i==lowCutoff)
			currentGain = (lowGain+midGain)*0.5; //implementing small rolloff to avoid Gibbs effect	
		oMag[i] *= currentGain;
	}
	return true;
}

typedef Factory<Processing> ProcessingFactory;

static ProcessingFactory::Registrator<ThreeBandFilter> regtThreeBandFilter( "ThreeBandFilter" );

}

