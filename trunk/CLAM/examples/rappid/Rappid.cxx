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

#include"Rappid.hxx"
#include<unistd.h>


Rappid::Rappid(const RappidConfig &cfg)
	: mTime(0.0),
	  mTimeLimit(0.0),
	  mFrameTime(0.0),
	  mRunOnce(false),
	  mStopRequest(false)
{
	Configure(cfg);

	AdoptChildren();

	ControlSetup();

	pthread_mutex_init(&mProcessingMutex,0);

	pthread_mutex_init(&mRunningConditionMutex,0);

	pthread_cond_init(&mRunningCondition,0);

	pthread_attr_t attrs;
	pthread_attr_init(&attrs);
	pthread_create(&mProcessingThread,&attrs,&Rappid::ProcessingThread,this);
	setuid(getuid());
}

void Rappid::Reconfigure()
{
	RappidConfig cfg = mConfig;
	Configure(cfg);
}

bool Rappid::ConcreteConfigure(const ProcessingConfig& c)
{
	CopyAsConcreteConfig(mConfig, c);

	mConfig.Synchronise();

	if (mConfig.GetDebugSignal() != RappidConfig::eNoSignal)
		mDebug=true;
	else
		mDebug=false;

	bool res = true;

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
		AttachChildren();
	}
	catch (std::exception &e)
	{
		mStatus += "Failed to configure children:\n";
		mStatus += e.what();
		return false;
	}
	if (res && mConfig.GetGlobalConfig().GetVerbose())
		RappidIOBase::DumpCommon();

	mTimeLimit = mConfig.GetTimeLimit();

	mFrameTime = 1000.0 * mConfig.GetGlobalConfig().GetFrameSize() / mSampleRate;

	if (Control.GetExecState() == Processing::Ready)
		InitializeControlValues();

	return res;
}

bool Rappid::StartRunning()
{
	pthread_mutex_lock(&mRunningConditionMutex);
	pthread_cond_signal(&mRunningCondition);
	pthread_mutex_unlock(&mRunningConditionMutex);
	return true;
}

bool Rappid::StopRunning()
{
	mStopRequest = true;
	return true;
}

void* Rappid::ProcessingThread(void* arg)
{
	Rappid &rappid = *(Rappid*)arg;

	const RappidConfig &cfg = rappid.GetConfiguration();

	if (cfg.GetRealTimeSched())
	{
		if (!rappid.mWatchdog.Start(2,cfg.GetGlobalConfig().GetVerbose()))
		{
			return 0;
		}

		sched_param scheduler_param;

		scheduler_param.sched_priority = cfg.GetRealTimePriority();

		if (scheduler_param.sched_priority > 98)
			scheduler_param.sched_priority = 98;

		if (pthread_setschedparam(pthread_self(),SCHED_FIFO,&scheduler_param) != 0)
		{
			perror("sched_setscheduler():");
			return 0;
		}
	}

	setuid(getuid());

	while (1) {

		pthread_mutex_lock(&rappid.mRunningConditionMutex);
		if (!rappid.mRunOnce)
			pthread_cond_wait(&rappid.mRunningCondition,&rappid.mRunningConditionMutex);
		pthread_mutex_unlock(&rappid.mRunningConditionMutex);

		try {

			if (rappid.GetExecState() == Processing::Unconfigured)
			{
				rappid.Reconfigure();
			}

			rappid.Start();

			rappid.mStopRequest = false;

			while(rappid.Do())
				if (rappid.mStopRequest) break;

			rappid.Stop();

		}
		catch(Err &er) {
			er.Print();
		}

		if (rappid.mRunOnce)
			return 0;

	}
	return 0;
}

void Rappid::WaitWhileProcessing()
{
	pthread_mutex_lock(&mRunningConditionMutex);
	mRunOnce=true;
	pthread_cond_signal(&mRunningCondition);
	pthread_mutex_unlock(&mRunningConditionMutex);
	pthread_join(mProcessingThread,0);
}

void Rappid::InitializeControlValues()
{
	Control.OutValues[eEnableEM1] =
		mMixerL.Enable[0].GetLastValue();
	Control.OutValues[eEM1InterpolationPeriod] =
		mEM1.InterpolationPeriod();
	Control.OutValues[eEM1IntegrationLength] =
		mEM1.IntegrationLength();
	Control.OutValues[eEM1NormalLevel] =
		mEM1.NormalLevel();
	Control.OutValues[eEM1SilenceLevel] =
		mEM1.SilenceLevel();
	Control.OutValues[eEnableEM2] =
		mMixerR.Enable[0].GetLastValue();
	Control.OutValues[eEM2InterpolationPeriod] =
		mEM2.InterpolationPeriod();
	Control.OutValues[eEM2IntegrationLength] =
		mEM2.IntegrationLength();
	Control.OutValues[eEM2NormalLevel] =
		mEM2.NormalLevel();
	Control.OutValues[eEM2SilenceLevel] =
		mEM2.SilenceLevel();
}

void Rappid::ControlSetup(void)
{
	Control.OutControls[eEnableEM1].
	     AddLink(&mMixerL.Enable[0]);

	Control.OutControls[eEM1InterpolationPeriod].
	         AddLink(&mEM1.cInterpolationPeriod);

	Control.OutControls[eEM1IntegrationLength].
	         AddLink(&mEM1.cIntegrationLength);

	Control.OutControls[eEM1NormalLevel].
	         AddLink(&mEM1.cNormalLevel);

	Control.OutControls[eEM1SilenceLevel].
	         AddLink(&mEM1.cSilenceLevel);

	Control.OutControls[eEnableEM2].
	     AddLink(&mMixerR.Enable[0]);

	Control.OutControls[eEM2InterpolationPeriod].
	         AddLink(&mEM2.cInterpolationPeriod);

	Control.OutControls[eEM2IntegrationLength].
	         AddLink(&mEM2.cIntegrationLength);

	Control.OutControls[eEM2NormalLevel].
	         AddLink(&mEM2.cNormalLevel);

	Control.OutControls[eEM2SilenceLevel].
	         AddLink(&mEM2.cSilenceLevel);
}

void Rappid::ConfigureAudio(Audio &a)
{
	a.SetSize        (mConfig.GetGlobalConfig().GetFrameSize());
	a.SetSampleRate  (mSampleRate);
}

bool Rappid::ConfigureData()
{
	ConfigureAudio(mHarpAudio);
	ConfigureAudio(mViolaAudio);
	ConfigureAudio(mSynthAudioL);
	ConfigureAudio(mSynthAudioR);
	ConfigureAudio(mEM1OutputAudio);
	ConfigureAudio(mEM2OutputAudio);
	return true;
}

void Rappid::InitializeData()
{
}

void Rappid::AdoptChildren()
{
	Control.SetParent(this);
	mHarp.SetParent(this);
	mViola.SetParent(this);
	mEM1.SetParent(this);
	mEM2.SetParent(this);
	mMixerL.SetParent(this);
	mMixerR.SetParent(this);
	mSynthL.SetParent(this);
	mSynthR.SetParent(this);
}

void Rappid::AttachChildren()
{
	// Inputs
	mViola.Output.Attach(mViolaAudio);
	mHarp.Output.Attach(mHarpAudio);

	// Modules
	mEM1.InputAudio.Attach(mViolaAudio);
	mEM1.ControlAudio.Attach(mHarpAudio);
	mEM1.OutputAudio.Attach(mEM1OutputAudio);

	mEM2.InputAudio.Attach(mHarpAudio);
	mEM2.ControlAudio.Attach(mViolaAudio);
	mEM2.OutputAudio.Attach(mEM2OutputAudio);

	// Mixer
	mMixerL.Input[0].Attach(mEM1OutputAudio);
	mMixerR.Input[0].Attach(mEM2OutputAudio);
	mMixerL.Output.Attach(mSynthAudioL);
	mMixerR.Output.Attach(mSynthAudioR);

	// Outputs
	mSynthL.Input.Attach(mSynthAudioL);
	mSynthR.Input.Attach(mSynthAudioR);

	// Debug
	if (mDebug) {
		switch (mConfig.GetDebugSignal()) {
		case RappidConfig::eHarpSignal:
			mDebugOutput.Input.Attach(mHarpAudio);
			break;
		case RappidConfig::eViolaSignal:
			mDebugOutput.Input.Attach(mViolaAudio);
			break;
		case RappidConfig::eEM1Signal:
			mDebugOutput.Input.Attach(mEM1OutputAudio);
			break;
		case RappidConfig::eEM2Signal:
			mDebugOutput.Input.Attach(mEM2OutputAudio);
			break;
		case RappidConfig::eSynthLSignal:
			mDebugOutput.Input.Attach(mSynthAudioL);
			break;
		case RappidConfig::eSynthRSignal:
			mDebugOutput.Input.Attach(mSynthAudioR);
			break;
		}
	}
}

bool Rappid::ConfigureChildren()
{
	ControllerConfig cfg;
	cfg.SetName("RappidController");
	cfg.SetNumControls(eNUMControls);
	if (!Control.Configure(cfg)) {
		mStatus += "Control: ";
		mStatus += Control.GetStatus();
		return false;
	}

	if (!mViola.Configure(mConfig.GetViolaIOConfig())) {
		mStatus += "Viola Input: ";
		mStatus += mViola.GetStatus();
		return false;
	}

	if (!mHarp.Configure(mConfig.GetHarpIOConfig())) {
		mStatus += "Harp Input: ";
		mStatus += mHarp.GetStatus();
		return false;
	}

	if (!mSynthL.Configure(mConfig.GetSynthLeftIOConfig())) {
		mStatus += "Synth Left Output: ";
		mStatus += mSynthL.GetStatus();
		return false;
	}

	if (!mSynthR.Configure(mConfig.GetSynthRightIOConfig())) {
		mStatus += "Synth Right Output: ";
		mStatus += mSynthR.GetStatus();
		return false;
	}

	if (!mEM1.Configure(mConfig.GetEnvMorph1Cfg())) {
		mStatus += "EnvelopeMorpher: ";
		mStatus += mEM1.GetStatus();
		return false;
	}

	if (!mEM2.Configure(mConfig.GetEnvMorph2Cfg())) {
		mStatus += "EnvelopeMorpher: ";
		mStatus += mEM2.GetStatus();
		return false;
	}

	if (RappidIOBase::RealTimeSampleRate())
		mSampleRate = RappidIOBase::RealTimeSampleRate();
	else
		mSampleRate = mViola.SampleRate();

	DynamicMixerConfig mcfg;
	mcfg.SetSampleRate(mSampleRate);
	mcfg.SetFrameSize(mConfig.GetGlobalConfig().GetFrameSize());
	mcfg.SetName("MixerLeft");
	if (!mMixerL.Configure(mcfg)) {
		mStatus += "DynamicMixerLeft: ";
		mStatus += mMixerL.GetStatus();
		return false;
	}
	mcfg.SetName("MixerRight");
	if (!mMixerR.Configure(mcfg)) {
		mStatus += "DynamicMixerRight: ";
		mStatus += mMixerR.GetStatus();
		return false;
	}

	if (mDebug) {
		AudioFileConfig fcfg;
		fcfg.SetName("DebugFile");
		fcfg.SetFilename("debug.wav");
		fcfg.SetFrameSize(mConfig.GetGlobalConfig().GetFrameSize());
		fcfg.SetSampleRate(mConfig.GetGlobalConfig().GetSampleRate());
		fcfg.SetFiletype(RappidIOBase::FileTypeFromName("debug.wav"));
		if (!mDebugOutput.Configure(fcfg))
		{
			mStatus += "DebugFile: ";
			mStatus += mDebugOutput.GetStatus();
			return false;
		}
		mDebugOutput.SetParent(this);
	}

	return true;
}

bool Rappid::ConcreteStart()
{
	ProcessingComposite::ConcreteStart();

	InitializeData();

	return true;
}


bool Rappid::Do()
{
	bool ok=true;

	ok &= Control.Do();
	ok &= mHarp.Do();
	ok &= mViola.Do();
	ok &= mEM1.Do();
	ok &= mEM2.Do();
	ok &= mMixerL.Do();
	ok &= mMixerR.Do();
	ok &= mSynthL.Do();
	ok &= mSynthR.Do();
	if (mDebug)
		mDebugOutput.Do();
	mTime += mFrameTime;
	if (mTimeLimit && mTime > mTimeLimit)
		return false;

	return ok;
}
