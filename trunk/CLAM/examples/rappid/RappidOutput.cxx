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

#include "RappidOutput.hxx"
#include "AudioIO.hxx"
#include "AudioOut.hxx"
#include "AudioFileOut.hxx"


bool RappidOutput::ConfigureRealTime(bool exclusive)
{
	if (exclusive) {
		delete pOutputFile;
		pOutputFile=0;
	}

	if (!AudioManagerSingleton()) {
		mStatus += "RappidInput::ConfigureRealTime(): Failed to obtain audio manager.";
		return false;
	}

	AudioIOConfig iocfg;
	iocfg.SetName("OutputAudioDevice");
	iocfg.SetDevice(mConfig.GetAudioDevice());
	iocfg.SetChannelID(mConfig.GetAudioDeviceChannelID());
	return CreateAndConfigure(&pRTOutput,iocfg);
}

bool RappidOutput::ConfigureFile(bool exclusive)
{
	if (exclusive) {
		delete pRTOutput;
		pRTOutput=0;
	}

	AudioFileConfig fcfg;
	fcfg.SetName("OutputFile");
	fcfg.SetFilename(mConfig.GetFileName());
	fcfg.SetFiletype(FileTypeFromName(mConfig.GetFileName()));
	return CreateAndConfigure(&pOutputFile,fcfg);
}

bool RappidOutput::ConfigureChildren()
{
	bool res = true;

	switch (Type()) {
	case ERappidIOType::eRealTime:
		res = ConfigureRealTime();
		break;
	case ERappidIOType::eFile:
		res = ConfigureFile();
		break;
	case ERappidIOType::eRealTimeAndFile:
		res = ConfigureRealTime(false);
		res &= ConfigureFile(false);
		break;
	}
	return res;
}

bool RappidOutput::ConcreteConfigure(const ProcessingConfig& c)
{
	bool res;

	BaseConfigure(c);

	CLAM_ASSERT(Type() != ERappidIOType::eWaveform,
				"RappidOutput::Configure(): Waveform type not implemented");

	try {
		res = ConfigureChildren();
	}
	catch (std::exception &e) {
		mStatus += "Failed to configure children:\n";
		mStatus += e.what();
		return false;
	}

	Input.SetParams(FrameSize());

	return res;
}

bool RappidOutput::ConcreteStart()
{
	StartAudioManager();

	iterator obj;
	for (obj=composite_begin(); obj!=composite_end(); obj++)
	{
		try {
			(*obj)->Start();
		}
		catch (Err &e) {
			ErrProcessingObj ne("ConcreteStart(): Child failed to start",this);
			ne.Embed(e);
			throw(ne);
		}
		if ((*obj)->GetExecState() != Running)
		{
			iterator obj2;
			for (obj2=composite_begin(); obj2!=obj; obj2++)
				(*obj2)->Stop();
			throw(ErrProcessingObj("ConcreteStart(): Child failed to start",this));
		}
	}

	if ( ( Type() == ERappidIOType::eRealTime || 
		   Type() == ERappidIOType::eRealTimeAndFile) &&
		 mConfig.GetGlobalConfig().GetVerbose()) {
		AudioDevice::TInfo info;
		pRTOutput->GetDeviceInfo(info);
		std::cout << GetName() << " started." << std::endl
				  << "  Device: " << info.mName << std::endl
				  << "     Device sample rate: " << info.mSampleRate << std::endl
				  << "     Output Channels in device: " 
				  << info.mNWriteChannels << std::endl
				  << "  " << GetName() << " channel: " 
				  << mConfig.GetAudioDeviceChannelID() << std::endl;
	}

	return true;
}

RappidOutput::RappidOutput()
	: pRTOutput(0),
	  pOutputFile(0),
	  Input("AudioOutput",this,1)
{}

RappidOutput::RappidOutput(const RappidIOConfig &c)
	: pRTOutput(0),
	  pOutputFile(0),
	  Input("AudioOutput",this,1)
{
	Configure(c);
}


RappidOutput::~RappidOutput()
{
	delete pOutputFile;
	delete pRTOutput;
}


bool RappidOutput::Do(void)
{
	bool res = Do(Input.GetData());
	Input.LeaveData();
	return res;
}


bool RappidOutput::Do(Audio& in)
{
	CLAM_ASSERT(in.GetSize() == FrameSize(),
				"RappidOutput::Do(): Output size mismatch");
	CLAM_ASSERT(in.GetSampleRate() == SampleRate(),
				"RappidOutput::Do(): SampleRate mismatch");

	bool res = false;

	switch (Type()) {
	case ERappidIOType::eRealTime:
		res = pRTOutput->Do(in);
		break;
	case ERappidIOType::eFile:
		res = pOutputFile->Do(in);
		break;
	case ERappidIOType::eRealTimeAndFile:
		res  = pRTOutput->Do(in);
		res &= pOutputFile->Do(in);
		break;
	}
			
	return res;
}


