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

#include "Deesser.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "Deesser",
	//	"category", "Spectral Transformations",
	//	"description", "Deesser",
		0
	};
	static FactoryRegistrator<ProcessingFactory, Deesser> reg = metadata;
}

bool Deesser::Do(const Spectrum& in, Spectrum& out)
{
	out = in; //even if out needs to be preserved, we need to copy content
	
	DataArray& oMag = out.GetMagBuffer();
	DataArray& iMag = in.GetMagBuffer();
	
	int spectrumSize = in.GetSize();
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	
	int lowCutoff = Round(mLowCutoffFreqCtl.GetLastValue()* spectralResolution);
	int highCutoff = 	Round(mHighCutoffFreqCtl.GetLastValue()* spectralResolution);
	
	TData midFreq = mLowCutoffFreqCtl.GetLastValue()+(mHighCutoffFreqCtl.GetLastValue()-mLowCutoffFreqCtl.GetLastValue())*0.5;
	SendFloatToInControl(mNotch,"Amount",midFreq);
	SendFloatToInControl(mNotch,"Bandwidth",mHighCutoffFreqCtl.GetLastValue()-mLowCutoffFreqCtl.GetLastValue());

	//note: control is supposed to be sent as dB's
	TData threshold = 	log2lin(mThresholdCtl.GetLastValue());
	
	int i;
	TData maxMag = 0;
	for( i = lowCutoff; i<highCutoff; i++)
	{
		if(iMag[i]>threshold) oMag[i] = threshold;
		/*if(iMag[i]>threshold && iMag[i]>maxMag)
		//use difference from the threshold to control gain of the Notch
			maxMag = iMag[i];*/
	}
	/*if (maxMag>0) {
		mNotch.GetInControl("Gain").DoControl(DB(threshold/maxMag));
		mNotch.Do(out,out);
	}*/
	return true;
}


}

