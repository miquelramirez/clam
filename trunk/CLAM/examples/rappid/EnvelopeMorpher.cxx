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

#include "EnvelopeMorpher.hxx"
#include "RappidIO.hxx"

#include <fstream>
#include <iostream>

namespace EnvelopeMorpherDebug
{
	void dump_bpf(const BPF bpf,const char* name)
	{
		std::fstream file;
		file.open("envelope.txt",std::ios::out);
		int i;
		for (i=0; i<bpf.Size(); i++)
			file << bpf.GetXValue(i) << " " << bpf.GetValueFromIndex(i) << std::endl;
		file.close();
	}
}


void EnvelopeMorpherConfig::DefaultInit(void)
{
	AddAll();
	UpdateData();
	SetInterpolationType(EInterpolation::eSpline);

	GetInterpolationPeriod().SetInitValue(5.0);
	GetInterpolationPeriod().SetMinValue(2.0);
	GetInterpolationPeriod().SetMaxValue(10.0);

	GetIntegrationLength().SetInitValue(50.0);
	GetIntegrationLength().SetMinValue(10.0);
	GetIntegrationLength().SetMaxValue(2000.0);

	GetNormalLevel().SetInitValue(0.25);
	GetNormalLevel().SetMinValue(0.01);
	GetNormalLevel().SetMaxValue(10.0);

	GetSilenceLevel().SetInitValue(0.0);
	GetSilenceLevel().SetMinValue(0.0);
	GetSilenceLevel().SetMaxValue(0.2);
}

void EnvelopeMorpherConfig::Dump(void)
{
	std::cout << "      Name:                ";
	std::cout << GetName();
	std::cout << std::endl;
	std::cout << "      Sample rate:         ";
	std::cout << GetGlobalConfig().GetSampleRate();
	std::cout << std::endl;
	std::cout << "      Frame size:          ";
	std::cout << GetGlobalConfig().GetFrameSize();
	std::cout << std::endl;
	std::cout << "      Interpolation period ";
	std::cout << GetInterpolationPeriod().GetInitValue();
	std::cout << std::endl;
	std::cout << "      Integration length   ";
	std::cout << GetIntegrationLength().GetInitValue();
	std::cout << std::endl;
	std::cout << "      Interpolation type   ";
	std::cout << GetInterpolationType();
	std::cout << std::endl;
	std::cout << "      NormalLevel          ";
	std::cout << GetNormalLevel().GetInitValue();
	std::cout << std::endl;
	std::cout << "      SilenceLevel         ";
	std::cout << GetSilenceLevel().GetInitValue();
	std::cout << std::endl;
}

#define CONTROL(name) c##name(#name,this,&EnvelopeMorpher::name##Change)

EnvelopeMorpher::EnvelopeMorpher() 
	: InputAudio("Input",this,1),
	  ControlAudio("Control",this,1),
	  OutputAudio("Output",this,1),
	  CONTROL(InterpolationPeriod),
	  CONTROL(IntegrationLength),
	  CONTROL(NormalLevel),
	  CONTROL(SilenceLevel),
	  pEnvelopeGen(0),
	  pEnvelopeFile(0),
	  mDumpEnvelope(false)
{
	Configure(EnvelopeMorpherConfig());
	AdoptChildren();
}

EnvelopeMorpher::EnvelopeMorpher(const EnvelopeMorpherConfig &cfg)
	: InputAudio("Input",this,1),
	  ControlAudio("Control",this,1),
	  OutputAudio("Output",this,1),
	  CONTROL(InterpolationPeriod),
	  CONTROL(IntegrationLength),
	  CONTROL(NormalLevel),
	  CONTROL(SilenceLevel),
	  pEnvelopeGen(0),
	  pEnvelopeFile(0),
	  mDumpEnvelope(false)
{
	Configure(cfg);
	AdoptChildren();
}

#undef CONTROL

EnvelopeMorpher::~EnvelopeMorpher()
{
	delete pEnvelopeGen;
	delete pEnvelopeFile;
}

void EnvelopeMorpher::ConfigureAudio(Audio &a)
{
	a.SetSize        (mConfig.GetGlobalConfig().GetFrameSize());
	a.SetSampleRate  (mSampleRate);
}

bool EnvelopeMorpher::ConfigureData()
{
	ConfigureAudio(mEnvelopeAudio);
	return true;
}

void EnvelopeMorpher::InitializeData()
{
	int i;
	for (i=0; i<mAudioEnvelope.GetBPFSize(); i++)
		mAudioEnvelope.GetAmplitudeBPF().SetValue(i,0.0);
}

void EnvelopeMorpher::AdoptChildren()
{
	mModulator.SetParent(this);
	mAnalysis.SetParent (this);
}

void EnvelopeMorpher::AttachChildren()
{
	if (pEnvelopeGen)  pEnvelopeGen->Attach(mAudioEnvelope, mEnvelopeAudio);
	if (pEnvelopeFile) pEnvelopeFile->Attach(mEnvelopeAudio);

	mModulator.InputEnvelope.Attach(mAudioEnvelope);
	mModulator.InputAudio.Attach(InputAudio);
	mModulator.Output.Attach(OutputAudio);

	mAnalysis.Input.Attach(ControlAudio);
	mAnalysis.Output.Attach(mAudioEnvelope);
}

bool EnvelopeMorpher::ConfigureChildren()
{
	EnvelopeGeneratorConfig egcfg;
	EnvModulatorConfig      mcfg;
	EnvExtractorConfig      acfg;

	if (mConfig.GetEnvelopeFile() == "")
	{
		mDumpEnvelope=false;
		delete pEnvelopeGen;
		delete pEnvelopeFile;
		pEnvelopeGen=0;
		pEnvelopeFile=0;
	}
	else
	{
		mDumpEnvelope=true;

		AudioFileConfig fcfg;
		fcfg.SetName("EnvelopeFile");
		fcfg.SetFilename(mConfig.GetEnvelopeFile());
		fcfg.SetFrameSize(mConfig.GetGlobalConfig().GetFrameSize());
		fcfg.SetSampleRate(mConfig.GetGlobalConfig().GetSampleRate());
		fcfg.SetFiletype(RappidIOBase::FileTypeFromName(mConfig.GetEnvelopeFile()));
		if (!pEnvelopeFile)
			pEnvelopeFile = new AudioFileOut;
		if (!pEnvelopeFile)  {
			mStatus += "Memory full";
			return false;
		}
		if (!pEnvelopeFile->Configure(fcfg))
		{
			mStatus += "EnvelopeFile: ";
			mStatus += pEnvelopeFile->GetStatus();
			return false;
		}
		pEnvelopeFile->SetParent(this);

		egcfg.SetName("EnvelopeGenerator");
		egcfg.SetDuration(1.0);
		egcfg.SetSampleRate(mSampleRate);
		egcfg.SetFrameEnvelopes(true);
		egcfg.SetFrameSize(mConfig.GetGlobalConfig().GetFrameSize());
		if (!pEnvelopeGen)
			pEnvelopeGen = new EnvelopeGenerator;
		if (!pEnvelopeGen) {
			mStatus += "Memory full";
			return false;
		}
		if (!pEnvelopeGen->Configure(egcfg))
		{
			mStatus += "EnvelopeGenerator: ";
			mStatus += pEnvelopeGen->GetStatus();
			return false;
		}
		pEnvelopeGen->SetParent(this);
	}

	mcfg.SetName("EnvelopeApplicator");
	mcfg.SetSampleRate(mSampleRate);
	mcfg.SetFrameSize(mConfig.GetGlobalConfig().GetFrameSize());
	if (!mModulator.Configure(mcfg)) {
		mStatus += "EnvelopeApplicator: ";
		mStatus += mModulator.GetStatus();
		return false;
	}
	acfg.SetName("EnvelopeExtractor");
	acfg.SetSampleRate(mSampleRate);
	acfg.SetFrameSize(mConfig.GetGlobalConfig().GetFrameSize());
	acfg.SetInterpolationPeriod(mConfig.GetInterpolationPeriod());
	acfg.SetIntegrationLength(mConfig.GetIntegrationLength());
	acfg.SetNormalLevel(mConfig.GetNormalLevel());
	acfg.SetSilenceLevel(mConfig.GetSilenceLevel());
	acfg.SetInterpolationType(mConfig.GetInterpolationType());
	if (!mAnalysis.Configure(acfg)) {
		mStatus += "EnvelopeExtractor: ";
		mStatus += mAnalysis.GetStatus();
		return false;
	}

	return true;
}

bool EnvelopeMorpher::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	bool res = true;

	mSampleRate = mConfig.GetGlobalConfig().GetSampleRate();

	try {
		if (!ConfigureChildren())
		{
			std::string aux(mStatus);
			mStatus = "\nFailed to configure children:\n";
			mStatus += aux;
			res=false;
		}
		if (!ConfigureData())
		{
			std::string aux(mStatus);
			mStatus = "Failed to configure data:\n";
			mStatus += aux;
			res=false;
		}
	}
	catch (std::exception &e)
	{
		mStatus += "Failed to configure children:\n";
		mStatus += e.what();
		return false;
	}
	if (mConfig.GetGlobalConfig().GetVerbose()) {
		std::cout << "New interpolation points/frame: "
				  << mAnalysis.NPoints() << std::endl;
	}

	int frame_size = mConfig.GetGlobalConfig().GetFrameSize();

	ControlAudio.SetParams(frame_size);
	 OutputAudio.SetParams(frame_size);
	  InputAudio.SetParams(frame_size);

	return res;
}

void EnvelopeMorpher::Attach(Audio& in, Audio &ctl, Audio &out)
{
	InputAudio.Attach(in);
	ControlAudio.Attach(ctl);
	OutputAudio.Attach(out);
	
	AttachChildren();
}

bool EnvelopeMorpher::ConcreteStart()
{
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

	InitializeData();

	return true;
}

bool EnvelopeMorpher::Do(void)
{
	bool res = Do(InputAudio.GetData(),
				  ControlAudio.GetData(),
				  OutputAudio.GetData());
	InputAudio.LeaveData();
	ControlAudio.LeaveData();
	OutputAudio.LeaveData();
	return res;
}

int EnvelopeMorpher::InterpolationPeriodChange(TControlData val)
{
	mAnalysis.cInterpolationPeriod.DoControl(val);
	return 0;
}

int EnvelopeMorpher::IntegrationLengthChange(TControlData val)
{
	mAnalysis.cIntegrationLength.DoControl(val);
	return 0;
}

int EnvelopeMorpher::NormalLevelChange(TControlData val)
{
	mAnalysis.cNormalLevel.DoControl(val);
	return 0;
}

int EnvelopeMorpher::SilenceLevelChange(TControlData val)
{
	mAnalysis.cSilenceLevel.DoControl(val);
	return 0;
}

bool EnvelopeMorpher::Do(Audio& in, Audio &ctl, Audio &out)
{
	mAnalysis.Do(ctl,mAudioEnvelope);

	if (mDumpEnvelope)
	{
		pEnvelopeGen->Do(mAudioEnvelope, mEnvelopeAudio);
		pEnvelopeFile->Do(mEnvelopeAudio);
	}

	mModulator.Do(mAudioEnvelope,in,out);

	return true;
}
