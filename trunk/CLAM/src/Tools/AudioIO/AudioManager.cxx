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

using namespace CLAM;

AudioManager* AudioManager::pSingleton = 0;

AudioManager::AudioManager(int sampleRate,int latency) throw(Err)
	: mInternalBuffersNumber( 8 )
{	
	mSampleRate = sampleRate;
	mLatency = latency;

	if (pSingleton)
		throw Err("can only have one audiomanager at a time");
	pSingleton = this;

	if (sAlsaAudioDeviceList)
	{
		Singleton().mDeviceLists.push_back(sAlsaAudioDeviceList);
	}
	if (sDirectXAudioDeviceList)
	{
		Singleton().mDeviceLists.push_back(sDirectXAudioDeviceList);
	}
	if (sPAAudioDeviceList)
		{
			Singleton().mDeviceLists.push_back(sPAAudioDeviceList);
		}
	if ( sRtAAudioDeviceList )
		{
			Singleton().mDeviceLists.push_back( sRtAAudioDeviceList );
		}
}

AudioManager::~AudioManager()
{
	unsigned int i;
	for (i=0;i<mDevices.size(); i++)
	{
		mDevices[i]->Stop();
		delete mDevices[i];
	}
	pSingleton = 0;
}

AudioDevice* AudioManager::FindDevice(const std::string& name)
{
	unsigned int i;

	for (i=0;i<mDevices.size();i++)
	{
		if (mDevices[i]->mName == name)
		{
			return mDevices[i];
		}
	}

	return 0;
}

void AudioManager::Start(void) throw(Err)
{
	unsigned int i;
	for (i=0;i<mDevices.size();i++)
	{
		mDevices[i]->Start();
	}
}

AudioDevice* AudioManager::FindOrCreateDevice(const std::string& name)
{
	std::string arch =   name.substr(0,name.find(":",0));
	std::string device = name.substr(name.find(":",0)+1,name.size());

	if (arch == "" || device == "") 
	{
		std::string msg = "AudioManager::FindOrCreateDevice(...): Invalid device name: ";
		msg       += name;
		throw Err(msg.c_str());
	}
	
	if (arch == "default")
	{
		arch = DEFAULT_AUDIO_ARCH;
	}

	AudioDeviceList* list = FindList(arch);

	if (list==0)
	{
		std::string errstr;
		errstr = "AudioManager::FindOrCreateDevice(): "
			"Don't have a list of \""+arch+"\" devices";
		throw Err((char*) errstr.c_str());
	}

	if (list->AvailableDevices().size()==0)
	{
		std::string errstr;
		errstr = "AudioManager::FindOrCreateDevice(): "
			"Don't have any \""+arch+"\" devices available";
		throw Err((char*) errstr.c_str());
	}

	if (device == "default")
	{
		device = list->DefaultDevice();
	}

	if (find(list->AvailableDevices().begin(),
			 list->AvailableDevices().end(),
			 device) == 
		list->AvailableDevices().end())
	{
		std::string errstr;
		errstr = "AudioManager::FindOrCreateDevice(): "
			"No device \""+device+"\" available in architecture \""+arch+"\".\n";
		throw Err((char*) errstr.c_str());
	}

	std::string real_name = arch+":"+device;

	AudioDevice* audiodevice = FindDevice(real_name);

	if (audiodevice==0) {
		
		audiodevice = list->Create(real_name,device);

		if (audiodevice==0)
		{
			std::string errstr;
			errstr = "AudioManager::FindOrCreateDevice(): Don't know how to make device "+real_name;
			throw Err((char*) errstr.c_str());
		}
		else
		{
			mDevices.push_back(audiodevice);
		}
	}
	return audiodevice;
}


bool AudioManager::Register(AudioIn& in)
{
	AudioDevice* device = FindOrCreateDevice(in.mConfig.GetDevice());
	return device->Register(in);
}

bool AudioManager::Register(AudioOut& out)
{
	AudioDevice* device = FindOrCreateDevice(out.mConfig.GetDevice());
	return device->Register(out);
}
