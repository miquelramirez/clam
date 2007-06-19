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

namespace detail 
{
	static FactoryRegistrator<ProcessingFactory, AudioWindowing> regAudioWindowing("AudioWindowing");
}

AudioWindowing::AudioWindowing()
	: mInput("Input",this ),
	  mOutput("Output",this )
{
	AttachChildren();
	Configure(AudioWindowingConfig());
}

AudioWindowing::AudioWindowing(AudioWindowingConfig& cfg)
	: mInput("Input",this),
   	  mOutput("Output",this)
{
	AttachChildren();
	Configure(cfg);
}

AudioWindowing::~AudioWindowing()
{
}

bool AudioWindowing::ConcreteConfigure(const ProcessingConfig& cfg)
{
	CopyAsConcreteConfig(mConfig,cfg);
 
	mConfig.Sync();
	ConfigureChildren();
	ConfigureData();
	return true;
}


bool AudioWindowing::ConfigureChildren()
{
	int windowSize = mConfig.GetprWindowSize();
	int zeroPadding = mConfig.GetprZeroPadding();
	EWindowType windowType = mConfig.GetprWindowType();
	int hopSize = mConfig.GetprHopSize();
	int sampleRate = mConfig.GetprSamplingRate();
	int fftSize = nextPowerOfTwo(int((windowSize-1)*CLAM_pow(TData(2),TData(zeroPadding))));
	if(windowSize<2*hopSize+1)
		hopSize=(windowSize-1)/2;

	CLAM_ASSERT(windowSize&1, "Configuring an even window in AudioWindowing");
	CLAM_ASSERT(fftSize==mConfig.GetprFFTSize(),
		"Configured and computed fftSize missmatch");

	WindowGeneratorConfig windowGeneratorConfig;
	windowGeneratorConfig.SetSize(windowSize);
	windowGeneratorConfig.SetType(windowType);
	if (! mWindowGenerator.Configure(windowGeneratorConfig) )
	{
		// TODO: retrieve the error message
		return false;
	}

	CircularShiftConfig circularShiftConfig;
	circularShiftConfig.SetAmount(-((windowSize-1)/TData(2))); 
	if (! mCircularShift.Configure(circularShiftConfig) )
	{
		// TODO: retrieve the error message
		return false;
	}

	return true;
}

void AudioWindowing::ConfigureData()
{
	TData samplingRate=mConfig.GetSamplingRate();

	mInput.SetSampleRate( samplingRate );
	mInput.SetSize(mConfig.GetWindowSize());
	mInput.SetHop(mConfig.GetHopSize());

	mWindow.SetSize(mConfig.GetWindowSize());
	
	/*Window is generated and data is kept in internal member mWindow*/
	mWindowGenerator.Do(mWindow);
	
	/*Leaving out last sample of odd-sized window*/
	mWindow.SetSize(mWindow.GetSize()-1);

	/* Adding zero padding to windowing function */
	mWindow.SetSize(mConfig.GetprFFTSize());
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
//	out.SetSize(mConfig.GetprFFTSize());
//	out.SetSampleRate(mConfig.GetSamplingRate());

	in.GetAudioChunk(0,in.GetSize()-1 ,out,true );

	CLAM_ASSERT(out.GetSize()==mConfig.GetprFFTSize(),
		"Configuration output size differs");
	CLAM_ASSERT(out.GetSampleRate()==mConfig.GetSamplingRate(),
		"Configuration output samplerate differs");

	// TODO: it is wrong
	out.SetSize(mConfig.GetWindowSize()-1);

	/* Zero padding is added to audioframe */
	out.SetSize(mConfig.GetprFFTSize());
	
	/* Windowing function is now applied */
	mAudioProduct.Do(out, mWindow, out);
	
	/* Finally, we do the circular shift */
	mCircularShift.Do(out,out);

	return true;
}



} // namespace CLAM

