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
namespace detail
{
	static FactoryRegistrator<ProcessingFactory, ThreeBandAM> regThreeBandAM("ThreeBandAM");
}


bool ThreeBandAM::Do(const Spectrum& in, Spectrum& out)
{
	if ( !mConfig.GetPreserveOuts() )
	{
		out = in; //TODO big cludge for streaming
	}
	
	int spectrumSize = in.GetSize();
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	
	mLFOscillator.GetInControl("Pitch").DoControl(mLowPitchCtl.GetLastValue());
	mMFOscillator.GetInControl("Pitch").DoControl(mMidPitchCtl.GetLastValue());
	mHFOscillator.GetInControl("Pitch").DoControl(mHighPitchCtl.GetLastValue());
	
	mLFOscillator.GetInControl("Amplitude").DoControl(mModAmplitudeCtl.GetLastValue());
	mMFOscillator.GetInControl("Amplitude").DoControl(mModAmplitudeCtl.GetLastValue());
	mHFOscillator.GetInControl("Amplitude").DoControl(mModAmplitudeCtl.GetLastValue());
	
	//kludge: this is not taking into account the possible overlap between windows!!
	mLFOscillator.GetInControl("SamplesBetweenCalls").DoControl((spectrumSize-1)/2);
	mMFOscillator.GetInControl("SamplesBetweenCalls").DoControl((spectrumSize-1)/2);
	mHFOscillator.GetInControl("SamplesBetweenCalls").DoControl((spectrumSize-1)/2);
	
	
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

