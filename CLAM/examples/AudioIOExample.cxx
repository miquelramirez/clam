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

#include "AudioIO.hxx"
#include "AudioIn.hxx"
#include "AudioOut.hxx"
#include "AudioManager.hxx"

#include "ProcessingComposite.hxx"
#include "OSDefines.hxx"
#include <iostream>


using namespace CLAM;

class TremoloConfig: public ProcessingConfig {
public:

		DYNAMIC_TYPE_USING_INTERFACE(TremoloConfig,4,ProcessingConfig);

		DYN_ATTRIBUTE(0,public, TData , Frequency);
		DYN_ATTRIBUTE(1,public, TData , MaxAtenuation);
		DYN_ATTRIBUTE(2,public, TData , StartPhase);
		DYN_ATTRIBUTE(3,public, TData , SamplingRate);
private:
	void DefaultInit();
};

void TremoloConfig::DefaultInit()
{
	AddFrequency();
	AddMaxAtenuation();
	AddStartPhase();
	AddSamplingRate();
	UpdateData();
	SetFrequency( TData(5.0) );
	SetMaxAtenuation( TData(0.2) );
	SetStartPhase( TData(0.0) );
	SetSamplingRate(44100);
}

class Tremolo: public Processing {
	TremoloConfig mConfig;
	TData mPhase;
	TData mPhaseDelta;
	TData mMidAmplitude;
	TData mTremoloAmplitude;
	const char* GetClassName() const {return "Tremolo";}
	bool ConcreteStart();
	bool ConcreteConfigure(const ProcessingConfig &c);
public:
	Tremolo(const TremoloConfig& cfg=TremoloConfig());
	const ProcessingConfig &GetConfig() const {return mConfig;}
	bool Do() {return true;}
	bool Do(const Audio &in, Audio &out);
};

Tremolo::Tremolo(const TremoloConfig &cfg)
{
	Configure(cfg);
}

bool Tremolo::ConcreteStart()
{
	mPhase = mConfig.GetStartPhase();
	return true;
}

bool Tremolo::ConcreteConfigure(const ProcessingConfig &c)
{
	CopyAsConcreteConfig(mConfig,c);
	mMidAmplitude = (1.0+mConfig.GetMaxAtenuation())/2.0;
	mTremoloAmplitude = (1.0-mMidAmplitude)*0.99;
	mPhaseDelta = 0.0;
	if (mConfig.GetSamplingRate())
		mPhaseDelta = 2*PI*mConfig.GetFrequency()/mConfig.GetSamplingRate();
	else
		return false;
	return true;
}


bool Tremolo::Do(const Audio &in_audio, Audio &out_audio)
{
	TData amplitude;
	const Array<TData> &in  =  in_audio.GetBuffer();
	Array<TData>       &out = out_audio.GetBuffer();

	for (int i=0; i<out.Size(); i++) {
		amplitude = mMidAmplitude + mTremoloAmplitude*sin(mPhase);
		mPhase += mPhaseDelta;
		out[i]=in[i]*amplitude;
	}

	if ( mPhase > 2*PI )
		mPhase -= 2*TData(PI);

	return true;
}

class AudioIOExampleConfig : public ProcessingConfig {
public:

		DYNAMIC_TYPE_USING_INTERFACE (AudioIOExampleConfig, 6, ProcessingConfig);

		DYN_ATTRIBUTE (0, public, TData, FirstTremoloFreq );
		DYN_ATTRIBUTE (1, public, TData, FirstTremoloStartingPhase );
		DYN_ATTRIBUTE (2, public, TData, FirstTremoloMaxAtenuation );
		DYN_ATTRIBUTE (3, public, TData, SecondTremoloFreq );
		DYN_ATTRIBUTE (4, public, TData, SecondTremoloStartingPhase );
		DYN_ATTRIBUTE (5, public, TData, SecondTremoloMaxAtenuation );

private:
	void DefaultInit();
};


void AudioIOExampleConfig::DefaultInit()
{
	AddAll();
	UpdateData();

	SetFirstTremoloFreq( 50.0 );
	SetFirstTremoloStartingPhase( 0 );
	SetFirstTremoloMaxAtenuation( 0.25 );
	SetSecondTremoloFreq( 25.0 );
	SetSecondTremoloStartingPhase( TData(PI/2) );
	SetSecondTremoloMaxAtenuation( 0.125 );
}

class AudioIOExample : public ProcessingComposite {
	AudioIOExampleConfig mConfig;
	int mSize;

	AudioIn     mInput;
	AudioIn     mInput2;
	AudioOut    mOutput;
	AudioOut    mOutput2;
	Tremolo     mTremoloApplier;
	Tremolo     mTremoloApplier2;

	Audio mInputData;
	Audio mInputData2;
	Audio mOutputData;
	Audio mOutputData2;

	void AttachChildren();
	bool ConfigureChildren();
	bool ConfigureData();
	void ConfigureAudio(Audio&);

	const char* GetClassName() const {return "AudioIOExample";}

	bool ConcreteStart() throw(ErrProcessingObj);
	bool ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast);
public:
	AudioIOExample(const AudioIOExampleConfig &cfg);
	const ProcessingConfig& GetConfig() const {return mConfig;}
	bool Do();
};

bool AudioIOExample::ConcreteStart() throw(ErrProcessingObj)
{
	iterator obj;
	for (obj=composite_begin(); obj!=composite_end(); obj++)
		(*obj)->Start();

	try {
		AudioManager::Current().Start();
	}
	catch (Err) {
		throw(ErrProcessingObj("Could not start AudioManager",this));
	}
	
	return true;
}




void AudioIOExample::AttachChildren()
{
	mInput.SetParent(this);
	mInput2.SetParent(this);
	mTremoloApplier.SetParent(this);
	mTremoloApplier2.SetParent(this);
	mOutput.SetParent(this);
	mOutput2.SetParent(this);
}


bool AudioIOExample::ConfigureChildren()
{
	AudioIOConfig cfg;

	cfg.SetChannelID(0);
	mInput.Configure(cfg);

 	cfg.SetChannelID(1);
 	mInput2.Configure(cfg);

	cfg.SetChannelID(0);
	mOutput.Configure(cfg);

 	cfg.SetChannelID(1);
 	mOutput2.Configure(cfg);

	TremoloConfig tcfg;
	tcfg.SetFrequency(mConfig.GetFirstTremoloFreq());
	tcfg.SetStartPhase( mConfig.GetFirstTremoloStartingPhase());
	tcfg.SetMaxAtenuation(mConfig.GetFirstTremoloMaxAtenuation());
	tcfg.SetSamplingRate(AudioManager::Current().SampleRate());

	mTremoloApplier.Configure(tcfg);

	tcfg.SetFrequency(mConfig.GetSecondTremoloFreq());
	tcfg.SetMaxAtenuation( mConfig.GetSecondTremoloMaxAtenuation());
	tcfg.SetStartPhase(mConfig.GetSecondTremoloStartingPhase());

	mTremoloApplier2.Configure(tcfg);

	return true;
}

void AudioIOExample::ConfigureAudio(Audio& a)
{
	a.SetSize(mSize);
	a.SetSampleRate(AudioManager::Current().SampleRate());
}


bool AudioIOExample::ConfigureData()
{
	ConfigureAudio(mInputData);
	ConfigureAudio(mInputData2);
	ConfigureAudio(mOutputData);
	ConfigureAudio(mOutputData2);
	return true;
}

bool AudioIOExample::ConcreteConfigure(const ProcessingConfig& c) throw(std::bad_cast)
{
	try {
		mConfig = dynamic_cast<const AudioIOExampleConfig&>(c);
		ConfigureChildren();
	}
	catch (Err &e) {
//		mStatus+=e.what(); // TODO: Port to the new way
		return false;
	}
	ConfigureData();
	return true;
}

AudioIOExample::AudioIOExample(const AudioIOExampleConfig &cfg)
	: mSize(512)
{
	AttachChildren();
	Configure(cfg);
}

bool AudioIOExample::Do()
{
	while (1) 
	{
		mInput.Do(mInputData);
		mInput2.Do(mInputData2);
		mTremoloApplier.Do(mInputData,mOutputData);
		mTremoloApplier2.Do(mInputData2,mOutputData2);
		mOutput.Do(mOutputData);
		mOutput2.Do(mOutputData2);
	}

	return true;
}


int main()
{
	try {
		AudioManager manager(44100, 512);
		manager.Start();

		// AudioIOExample configuration. This could be a good example on a
		// fairly complex configuration usage.
		AudioIOExampleConfig cfg;
		cfg.SetFirstTremoloFreq( TData(1.0) );
		cfg.SetFirstTremoloStartingPhase( TData(PI) );
		cfg.SetFirstTremoloMaxAtenuation( TData(0.6) );
		cfg.SetSecondTremoloFreq( TData(3.0) );
		cfg.SetSecondTremoloStartingPhase( TData(0) );
		cfg.SetSecondTremoloMaxAtenuation( TData(0.45) );

		AudioIOExample app(cfg);

		app.Start();

		std::cout << "Close the console ( or kill the job ) for terminating the application" << std::endl;
		std::cout << "If you move around the console window you may experience artifacts" << std::endl;
		std::cout << "Note for Windoze users: if the console window is minimized sound will stop" << std::endl;

		app.Do();

		return 0;

	}
	catch (Err &e) {
		e.Print();
		return 1;
	}
}

