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

#include "Segment.hxx"
#include "Frame.hxx"
#include "SpectrumConfig.hxx"
#include "AudioWindowing.hxx"
#include "ProcessingFactory.hxx"

namespace CLAM
{

namespace Hidden 
{
	static FactoryRegistrator<ProcessingFactory, AudioWindowing> regAudioWindowing("AudioWindowing");
}

AudioWindowing::~AudioWindowing()
{
}

bool AudioWindowing::ConcreteConfigure(const ProcessingConfig& cfg)
{
	CopyAsConcreteConfig(mConfig,cfg);
 
	ConfigureChildren();
	ConfigureData();
	return true;
}

bool AudioWindowing::ConfigureChildren()
{
	int windowSize = mConfig.GetWindowSize();
	EWindowType windowType = mConfig.GetWindowType();
	int hopSize = mConfig.GetHopSize();
	int sampleRate = mConfig.GetSamplingRate();
	if (not windowSize&1)
	{
		AddConfigErrorMessage("FFT Restriction:");
		AddConfigErrorMessage("Window size should be odd.");
		return false;
	}
	if(windowSize<2*hopSize+1)
	{
		AddConfigErrorMessage("FFT Restriction:");
		AddConfigErrorMessage("This condition is not met: WindowSize < 2*HopSize+1");
		return false;
	}

	if (not isPowerOfTwo(mConfig.GetFFTSize()))
	{
		AddConfigErrorMessage("FFT Restriction:");
		AddConfigErrorMessage("FFT Size should be a power of two.");
		return false;
	}
	WindowGeneratorConfig windowGeneratorConfig;
	windowGeneratorConfig.SetSize(windowSize);
	windowGeneratorConfig.SetType(windowType);
	if (! mWindowGenerator.Configure(windowGeneratorConfig) )
	{
		AddConfigErrorMessage("Window Generator configuration failed.");
		AddConfigErrorMessage(mWindowGenerator.GetConfigErrorMessage());
		return false;
	}

	CircularShiftConfig circularShiftConfig;
	circularShiftConfig.SetAmount(-((windowSize-1)/TData(2))); 
	if (! mCircularShift.Configure(circularShiftConfig) )
	{
		AddConfigErrorMessage("Circular Shift configuration failed.");
		AddConfigErrorMessage(mCircularShift.GetConfigErrorMessage());
		return false;
	}

	return true;
}

void AudioWindowing::ConfigureData()
{
	mInput.SetSize(mConfig.GetWindowSize());
	mInput.SetHop(mConfig.GetHopSize());

	mWindow.SetSize(mConfig.GetWindowSize());
	
	/*Window is generated and data is kept in internal member mWindow*/
	mWindowGenerator.Do(mWindow);
	
	/*Leaving out last sample of odd-sized window*/
	mWindow.SetSize(mWindow.GetSize()-1);

	/* Adding zero padding to windowing function */
	mWindow.SetSize(mConfig.GetFFTSize());
}

void AudioWindowing::AttachChildren()
{
	mWindowGenerator.SetParent(this);
	mAudioProduct.SetParent(this);
	mCircularShift.SetParent(this);
}

bool AudioWindowing::Do(void)
{
	bool result =  Do(mInput.GetAudio(),mOutput.GetData());

	mInput.Consume();
	mOutput.Produce();

	return result;
}

bool AudioWindowing::Do(const Audio& in,Audio& out)
{
	in.GetAudioChunk(0,in.GetSize()-1 ,out,true );

	CLAM_ASSERT(out.GetSize()==mConfig.GetFFTSize(),
		"Configuration output size differs");
	CLAM_ASSERT(out.GetSampleRate()==mConfig.GetSamplingRate(),
		"Configuration output samplerate differs");

	// TODO: it is wrong
	out.SetSize(mConfig.GetWindowSize()-1);

	/* Zero padding is added to audioframe */
	out.SetSize(mConfig.GetFFTSize());
	
	/* Windowing function is now applied */
	mAudioProduct.Do(out, mWindow, out);
	
	/* Finally, we do the circular shift */
	mCircularShift.Do(out,out);

	out.SetSampleRate(mConfig.GetSamplingRate());

	return true;
}



} // namespace CLAM

