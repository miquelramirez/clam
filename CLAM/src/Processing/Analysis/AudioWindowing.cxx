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
	static const char* metadata[] = {
		"key", "AudioWindowing",
		"category", "Analysis",
		"description", "AudioWindowing",
		0
	};
	static FactoryRegistrator<ProcessingFactory, AudioWindowing> reg = metadata;
}

AudioWindowing::~AudioWindowing()
{
}

bool AudioWindowing::ConcreteConfigure(const ProcessingConfig& cfg)
{
	CopyAsConcreteConfig(mConfig,cfg);
 
	if (! ConfigureChildren()) return false;
	ConfigureData();
	return true;
}

bool AudioWindowing::ConfigureChildren()
{
	int windowSize = mConfig.GetWindowSize();
	EWindowType windowType = mConfig.GetWindowType();
	if (not (windowSize&1))
	{
		AddConfigErrorMessage("FFT Restriction:");
		AddConfigErrorMessage("Window size should be odd.");
		return false;
	}
	// TODO: Review those restriction
	/*
	if (windowSize<2*hopSize+1)
	{
		AddConfigErrorMessage("FFT Restriction:");
		AddConfigErrorMessage("This condition is not met: WindowSize < 2*HopSize+1");
		return false;
	}
	*/
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
	mInput.SetSize(mConfig.GetWindowSize()-1);
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
	in.GetAudioChunk(0,in.GetSize() ,out,true );
	out.SetSampleRate(mConfig.GetSamplingRate());

	// Zero padding
	out.SetSize(mConfig.GetFFTSize());
	
	// Windowing
	if (mConfig.GetWindowType()!=EWindowType::eNone )
		mAudioProduct.Do(out, mWindow, out);
	
	// Half Window Shift
	if (mConfig.GetDoHalfWindowShift())
		mCircularShift.Do(out,out);


	return true;
}



} // namespace CLAM

