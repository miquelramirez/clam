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

#include "RappidIO.hxx"
#include "AudioIO.hxx"
#include "AudioManager.hxx"
#include "AudioDevice.hxx"
#include "AudioDeviceList.hxx"
#include "Processing.hxx"

Enum::tEnumValue ERappidIOType::sEnumValues[] = {
	{ERappidIOType::eRealTime,"RealTime"},
	{ERappidIOType::eFile,"File"},
	{ERappidIOType::eRealTimeAndFile,"RealTimeAndFile"},
	{ERappidIOType::eWaveform,"Waveform"},
	{0,NULL}
};

Enum::tValue ERappidIOType::sDefault = ERappidIOType::eRealTime;

void RappidIOConfig::DefaultInit()
{
	AddAll();
	UpdateData();
	SetIOType(ERappidIOType::eRealTime);
	SetAudioDevice("default:default");
	SetAudioDeviceChannelID(0);
}

void RappidIOConfig::Dump()
{
	std::cout << "  " << GetName() << " Configuration:" << std::endl;
	switch (GetIOType()) {
	case ERappidIOType::eRealTime:
		std::cout << "    Using RealTime IO." << std::endl;
		std::cout << "    Requested audio device:   ";
		std::cout << GetAudioDevice();
		std::cout << std::endl;
		std::cout << "    Requested device channel: ";
		std::cout << GetAudioDeviceChannelID();
		std::cout << std::endl;
		break;
	case ERappidIOType::eFile:
		std::cout << "    Using File IO." << std::endl;
		std::cout << "    Filename:                 ";
		std::cout << GetFileName();
		std::cout << std::endl;
		break;
	case ERappidIOType::eRealTimeAndFile:
		std::cout << "    Using RealTime and File IO." << std::endl;
		std::cout << "    Filename:                 ";
		std::cout << GetFileName();
		std::cout << std::endl;
		std::cout << "    Requested audio device:   ";
		std::cout << GetAudioDevice();
		std::cout << std::endl;
		std::cout << "    Requested device channel: ";
		std::cout << GetAudioDeviceChannelID();
		std::cout << std::endl;
		break;
	case ERappidIOType::eWaveform:
		std::cout << "    Using Waveform Input." << std::endl;
		break;
	}

}

RappidIOBase::RappidIOBase()
	: mSampleRate(0.0),
	  mFrameSize(0)
{}

EAudioFileType RappidIOBase::FileTypeFromName(const std::string &name)
{
	std::string extension = name.substr(name.find_last_of(".")+1);
	if      (extension=="raw" ||
	         extension == "RAW")
		return EAudioFileType::eRaw;
	else if (extension=="wav" ||
	         extension=="WAV")
		return EAudioFileType::eWave;
	else if (extension=="aiff" ||
	         extension=="AIFF")
		return EAudioFileType::eAIFF;
	return EAudioFileType::eUnknown;
}

CLAM::AudioManager *RappidIOBase::AudioManagerSingleton()
{
	CLAM_ASSERT(mType == ERappidIOType::eRealTime ||
	            mType == ERappidIOType::eRealTimeAndFile, 
	            "RappidIOBase::AudioManagerSingleton(): "
	            "Called from a non-real-time RappidIO");

	if (pAudioManager) {

		CLAM_ASSERT(mSampleRate == mRTRequestedSampleRate,
		            "RappidIOBase::AudioManagerSingleton(): "
		            "Requested a different sample rate from the one in use.");

		return pAudioManager;

	}

	mRTRequestedSampleRate = mSampleRate;

	try {
		if (mSampleRate)
			pAudioManager = new AudioManager(int(mSampleRate),512);
		else {
			pAudioManager = new AudioManager(44100,512);
			mSampleRate = mRTSampleRate = pAudioManager->SampleRate();
		}
	}
	catch (std::exception &e) {
		return 0;
	}
	return pAudioManager;
}

void RappidIOBase::BaseConfigure(const ProcessingConfig &c)
{
	CopyAsConcreteConfig(mConfig, c);

	mSampleRate = mConfig.GetGlobalConfig().GetSampleRate();

	mFrameSize = mConfig.GetGlobalConfig().GetFrameSize();

	mType = mConfig.GetIOType();
}


void RappidIOBase::StartAudioManager()
{
	if (mType != ERappidIOType::eRealTime)
		return;

	if (mConfig.GetGlobalConfig().GetADAT()) {
		AudioDevice *d = pAudioManager->FindDevice(mConfig.GetAudioDevice());
		d->SetNChannels(8);
	}

	if (!AudioManagerStarted) {
		pAudioManager->Start();
		AudioManagerStarted = true;	
	}
}


void RappidIOBase::DumpCommon()
{
	std::cout << "Rappid Input/Output general configuration:";
	std::cout << std::endl;
	if (pAudioManager) {
		std::cout << "  Available devices:" << std::endl;
		AudioManager::list_iterator lit;
		for (lit =  pAudioManager->AudioManager::lists_begin();
			 lit != pAudioManager->AudioManager::lists_end();
			 lit++) 
		{
			const std::vector<std::string> &devices = (*lit)->AvailableDevices();
			std::vector<std::string>::const_iterator dit;
			for (dit = devices.begin(); dit!=devices.end(); dit++)
				std::cout << "    " << (*lit)->ArchName() << ":" << *dit << std::endl;
		}
		std::cout << "  Registered audio devices:" << std::endl;
		AudioManager::device_iterator udit;
		for (udit = pAudioManager->used_devices_begin(); 
			 udit != pAudioManager->used_devices_end(); 
			 udit++)
			std::cout << "    " << (*udit)->mName << std::endl;
	}
	else {
		std::cout << "  No available devices." << std::endl;
	}
}

CLAM::AudioManager *RappidIOBase::pAudioManager = 0;

bool RappidIOBase::AudioManagerStarted = false;

TData RappidIOBase::mRTSampleRate = 0.0;
TData RappidIOBase::mRTRequestedSampleRate = 0.0;
