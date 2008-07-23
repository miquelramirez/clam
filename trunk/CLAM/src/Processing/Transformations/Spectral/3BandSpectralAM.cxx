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

#include "3BandSpectralAM.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{
namespace Hidden
{
	static const char * metadata[] = {
		"key", "ThreeBandAM",
		"category", "Spectral Transformations",
		"description", "ThreeBandAM",
		0
	};
	static FactoryRegistrator<ProcessingFactory, ThreeBandAM> reg = metadata;
}


bool ThreeBandAM::Do(const Spectrum& in, Spectrum& out)
{
	if ( !mConfig.GetPreserveOuts() )
	{
		out = in; //TODO big cludge for streaming
	}
	
	int spectrumSize = in.GetSize();
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	
	SendFloatToInControl(mLFOscillator,"Pitch",mLowPitchCtl.GetLastValue());
	SendFloatToInControl(mMFOscillator,"Pitch",mMidPitchCtl.GetLastValue());
	SendFloatToInControl(mHFOscillator,"Pitch",mHighPitchCtl.GetLastValue());
	
	SendFloatToInControl(mLFOscillator,"Amplitude",mModAmplitudeCtl.GetLastValue());
	SendFloatToInControl(mMFOscillator,"Amplitude",mModAmplitudeCtl.GetLastValue());
	SendFloatToInControl(mHFOscillator,"Amplitude",mModAmplitudeCtl.GetLastValue());

	//kludge: this is not taking into account the possible overlap between windows!!
	SendFloatToInControl(mLFOscillator,"SamplesBetweenCalls",(spectrumSize-1)/2);
	SendFloatToInControl(mMFOscillator,"SamplesBetweenCalls",(spectrumSize-1)/2);
	SendFloatToInControl(mHFOscillator,"SamplesBetweenCalls",(spectrumSize-1)/2);
	
	
	DataArray& oMag = out.GetMagBuffer();
	
	int lowCutoff = Round(mLowCutoffFreqCtl.GetLastValue()* spectralResolution);
	int highCutoff = 	Round(mHighCutoffFreqCtl.GetLastValue()* spectralResolution);

	TData modFactorLF,modFactorMF, modFactorHF;
	mLFOscillator.Do(modFactorLF);
	mMFOscillator.Do(modFactorMF);
	mHFOscillator.Do(modFactorHF);
	
	TData currentGain = 1+modFactorLF;
	for(int i = 0; i<spectrumSize; i++)
	{
		if(i>highCutoff) 
			currentGain = (1+modFactorHF);
		else if(i>lowCutoff) 
			currentGain = (1+modFactorMF);
		oMag[i] *= currentGain;
	}
	return true;
}


}

