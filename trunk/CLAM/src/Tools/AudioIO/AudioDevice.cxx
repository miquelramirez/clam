/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "AudioManager.hxx"
#include "AudioDevice.hxx"
#include "AudioIn.hxx"
#include "AudioOut.hxx"

#include <algorithm>
using std::find;

using namespace CLAM;

bool AudioDevice::Register(AudioIn& in)
{
	unsigned int i;
	for (i=0; i<mInputs.size(); i++)
		if (dynamic_cast<const AudioIOConfig&>(mInputs[i]->GetConfig()).GetChannelID() ==
			dynamic_cast<const AudioIOConfig&>(in.GetConfig()).GetChannelID()) 
		{
			in.mpDevice = 0;
			return false;
		}
	mInputs.push_back(&in);
	in.mpDevice = this;
	return true;
}

bool AudioDevice::Register(AudioOut& out)
{
	unsigned int i;
	for (i=0; i<mOutputs.size(); i++)
		if (dynamic_cast<const AudioIOConfig&>(mOutputs[i]->GetConfig()).GetChannelID() ==
			dynamic_cast<const AudioIOConfig&>(out.GetConfig()).GetChannelID())
		{
			out.mpDevice = 0;
			return false;
		}
	mOutputs.push_back(&out);
	out.mpDevice = this;
	return true;
}

void AudioDevice::Unregister(AudioIn& in)
{
	if (in.mpDevice != this)
	{
		throw(Err("AudioDevice::Unregister(): I am not this AudioIn object's device."));
	}
	std::vector<AudioIn*>::iterator it = std::find(mInputs.begin(),mInputs.end(),&in);
	if (it == mInputs.end())
	{
		throw(Err("AudioDevice::Unregister(): AudioIn object not registered in this device."));
	}
	mInputs.erase(it);
	in.mpDevice = 0;
}

void AudioDevice::Unregister(AudioOut& out)
{
	if (out.mpDevice != this)
	{
		throw(Err("AudioDevice::Unregister(): I am not this AudioOut object's device."));
	}
	std::vector<AudioOut*>::iterator it = std::find(mOutputs.begin(),mOutputs.end(),&out);
	if (it == mOutputs.end())
	{
		throw(Err("AudioDevice::Unregister(): AudioOut object not registered in this device."));
	}
	mOutputs.erase(it);
	out.mpDevice = 0;
}

void AudioDevice::GetInfo(AudioDevice::TInfo &info)
{
	info.mSampleRate = SampleRate();
	info.mName = mName;
	info.mNChannels = mNChannels;
	info.mNReadChannels = mNReadChannels;
	info.mNWriteChannels = mNWriteChannels;
}

int AudioDevice::SampleRate(void)
{
	/* determine the sample rate */
	unsigned int i;
	int sr = 0;
	for (i=0;i<mInputs.size();i++)
	{
		if (sr == 0) sr = mInputs[i]->mConfig.GetSampleRate();
		else if (mInputs[i]->mConfig.GetSampleRate() && 
			mInputs[i]->mConfig.GetSampleRate() != sr)
		{
			throw Err("AudioDevice::Register():"
				      "all audio ports need the same samplerate");
		}
	}
	for (i=0;i<mOutputs.size();i++)
	{
		if (sr == 0) sr = mOutputs[i]->mConfig.GetSampleRate();
		else if (mOutputs[i]->mConfig.GetSampleRate() && 
			mOutputs[i]->mConfig.GetSampleRate() != sr )
		{
			throw Err("AudioDevice::Register():"
				      "all audio ports need the same samplerate");
		}
	}
	
	if (sr == 0)
		sr = AudioManager::Singleton().SampleRate();
	
	for (i=0;i<mInputs.size();i++)
	{
		mInputs[i]->mConfig.SetSampleRate(sr);
	}
	for (i=0;i<mOutputs.size();i++)
	{
		mOutputs[i]->mConfig.SetSampleRate(sr);
	}
	
	return sr;
}

int AudioDevice::Latency(void)
{
	return AudioManager::Singleton().Latency();
}

unsigned AudioDevice::NumberOfInternalBuffers()
{
	return AudioManager::Singleton().GetInternalBuffersNumber();
}

void AudioDevice::SetNChannels(int channels)
{
	mForceNChannels = true;
	mNChannels = channels;
}
