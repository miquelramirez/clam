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

#include "RappidInput.hxx"
#include "AudioIO.hxx"
#include "AudioIn.hxx"
#include "AudioFileIn.hxx"
#include "WaveGenerator.hxx"

bool RappidInput::ConfigureRealTime()
{
	delete pInputFile;
	delete pWaveformIn;
	pInputFile=0;
	pWaveformIn=0;

	if (!AudioManagerSingleton()) {
		mStatus += "RappidInput::ConfigureRealTime(): Failed to obtain audio manager.";
		return false;
	}

	AudioIOConfig iocfg;
	iocfg.SetName("InputAudioDevice");
	iocfg.SetDevice(mConfig.GetAudioDevice());
	iocfg.SetChannelID(mConfig.GetAudioDeviceChannelID());
	return CreateAndConfigure(&pRTInput,iocfg);
}

bool RappidInput::ConfigureFile()
{
	delete pRTInput;
	delete pWaveformIn;
	pRTInput=0;
	pWaveformIn=0;

	AudioFileConfig fcfg;
	fcfg.SetName("InputFile");
	fcfg.SetKeepFrameSizes(true);
	fcfg.SetFilename(mConfig.GetFileName());
	fcfg.SetFiletype(FileTypeFromName(mConfig.GetFileName()));
	return CreateAndConfigure(&pInputFile,fcfg);
}

bool RappidInput::ConfigureWaveform()
{
	delete pRTInput;
	delete pInputFile;
	pRTInput=0;
	pInputFile=0;

	WaveGeneratorConfig gcfg;
	gcfg.SetName("WaveformGenerator");
	gcfg.SetFrequency(2000.0);
	gcfg.SetAmplitude(0.1);
	return CreateAndConfigure(&pWaveformIn,gcfg);
}

bool RappidInput::ConfigureChildren()
{
	bool res = true;

	switch (Type()) {
	case ERappidIOType::eRealTime:
		res = ConfigureRealTime();
		break;
	case ERappidIOType::eFile:
		res = ConfigureFile();
		break;
	case ERappidIOType::eWaveform:
		res = ConfigureWaveform();
		break;
	}
	return res;
}

bool RappidInput::ConcreteConfigure(const ProcessingConfig& c)
{
	bool res;

	BaseConfigure(c);

	CLAM_ASSERT(Type() != ERappidIOType::eRealTimeAndFile,
	            "RappidInput::Configure(): RealTimeAndFile type not implemented");

	try {
		res = ConfigureChildren();
	}
	catch (std::exception &e) {
		mStatus += "Failed to configure children:\n";
		mStatus += e.what();
		return false;
	}

	Output.SetParams(FrameSize());

	return res;
}

bool RappidInput::ConcreteStart()
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

	if (Type() == ERappidIOType::eRealTime &&
		mConfig.GetGlobalConfig().GetVerbose()) {
		AudioDevice::TInfo info;
		pRTInput->GetDeviceInfo(info);
		std::cout << GetName() << " started." << std::endl
		          << "  Device: " << info.mName << std::endl
		          << "     Device sample rate: " << info.mSampleRate << std::endl
		          << "     Input Channels in device: "
		          << info.mNReadChannels << std::endl
		          << "  " << GetName() << " channel: " 
		          << mConfig.GetAudioDeviceChannelID() << std::endl;
	}

	return true;
}

RappidInput::RappidInput()
	: pRTInput(0),
	  pInputFile(0),
	  pWaveformIn(0),
	  Output("AudioOutput",this,1)
{}

RappidInput::RappidInput(const RappidIOConfig &c)
	: pRTInput(0),
	  pInputFile(0),
	  pWaveformIn(0),
	  Output("AudioOutput",this,1)
{
	Configure(c);
}


RappidInput::~RappidInput()
{
	delete pInputFile;
	delete pRTInput;
	delete pWaveformIn;
}


bool RappidInput::Do(void)
{
	bool res = Do(Output.GetData());
	Output.LeaveData();
	return res;
}


bool RappidInput::Do(Audio& in)
{
	CLAM_ASSERT(in.GetSize() == FrameSize(),
	            "RappidInput::Do(): Input size mismatch");
	CLAM_ASSERT(in.GetSampleRate() == SampleRate(),
	            "RappidInput::Do(): SampleRate mismatch");

	bool res=false;

	switch (Type()) {
	case ERappidIOType::eRealTime:
		res = pRTInput->Do(in);
		break;
	case ERappidIOType::eFile:
		res = pInputFile->Do(in);
		break;
	case ERappidIOType::eWaveform:
		res = pWaveformIn->Do(in);
		break;
	}

	return res;
}


