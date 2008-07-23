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

#include "SpectralDelay.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden
{
	static const char * metadata[] = {
		"key", "SpectralDelay",
		"category", "Spectral Transformations",
		"description", "SpectralDelay",
		0
	};
	static FactoryRegistrator<ProcessingFactory, SpectralDelay> reg = metadata;
}



bool SpectralDelay::Do(const Spectrum& in, Spectrum& out)
{
	if ( !mConfig.GetPreserveOuts() )
	{
		out = in; //TODO big cludge for streaming
	}
	
	//first convert from ms to token size
	//mFrameSize should  be set somehow in the configuration! It is the difference between one frame center and the next
	mFrameSize = 1024;
	
	
	TData lfDelay = mLowDelayCtl.GetLastValue() * in.GetSpectralRange() * 0.002 / mFrameSize;
	TData mfDelay = mMidDelayCtl.GetLastValue() * in.GetSpectralRange() * 0.002 / mFrameSize;
	TData hfDelay = mHighDelayCtl.GetLastValue() * in.GetSpectralRange() * 0.002 / mFrameSize;
	
	//these controls could just be connected at the beginning
	SendFloatToInControl(mLFDelay,"Delay Control",lfDelay);
	SendFloatToInControl(mMFDelay,"Delay Control",mfDelay);
	SendFloatToInControl(mHFDelay,"Delay Control",hfDelay);
	
	//now we are ready to get the appropriate spectrums
	mLFDelay.Do(in, mLFSpectrum);
	mMFDelay.Do(in, mMFSpectrum);
	mHFDelay.Do(in, mHFSpectrum);
	
	DataArray& hfMag = mHFSpectrum.GetMagBuffer();
	DataArray& hfPhase = mHFSpectrum.GetPhaseBuffer();
	DataArray& mfMag = mMFSpectrum.GetMagBuffer();
	DataArray& mfPhase = mMFSpectrum.GetPhaseBuffer();
	DataArray& lfMag = mLFSpectrum.GetMagBuffer();
	DataArray& lfPhase = mLFSpectrum.GetPhaseBuffer();
	
	DataArray& oMag = out.GetMagBuffer();
	DataArray& oPhase = out.GetPhaseBuffer();
	
	int spectrumSize = in.GetSize();
	
	TData spectralResolution = spectrumSize/in.GetSpectralRange();
	
	int lowCutoff = Round(mLowCutoffFreqCtl.GetLastValue()* spectralResolution);
	int highCutoff = Round(mHighCutoffFreqCtl.GetLastValue()* spectralResolution);

	for(int i = 0; i<spectrumSize; i++)
	{
		if(i>highCutoff) 
		{
			oMag[i] = hfMag[i];
			oPhase[i] = hfPhase[i];
		}
		else if(i>lowCutoff)
		{
			oMag[i] = mfMag[i];
			oPhase[i] = mfPhase[i];
		}
		else
		{
			oMag[i] = lfMag[i];
			oPhase[i] = lfPhase[i];
		}
	}
	return true;
}


}

