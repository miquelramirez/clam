/*
 * FundPlayer.cxx
 * Implementation for FundPlayer class. 
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include "FundPlayer.hxx"
#include "AudioIO.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "SimpleOscillator.hxx"

using namespace CLAM;

FundPlayer::FundPlayer(const Segment& originalSegment,
							 Slotv0& slot0,
							 Slotv1<DataArray>& slot1) 
							 : mSegment(originalSegment)
{
	active = false;
	mRequestStop.Connect(slot0);
	mSendData.Connect(slot1);
	pthread_create(&mThread,NULL,sPlayFundThreadSafe,(void*)this);
}

FundPlayer::~FundPlayer()
{
	active = false;
	pthread_join(mThread,NULL);
}

void* FundPlayer::sPlayFundThreadSafe(void* ptr)
{
 	((FundPlayer*)ptr)->PlayFundThreadSafe();

	return NULL;
}

void FundPlayer::PlayFundThreadSafe()
{
	TSize nSamples = mSegment.GetAudio().GetSize(); // number of samples
	TData sampleRate = mSegment.GetSamplingRate();  // sample rate
	TSize frameSize = 2048;                         // frame size

	AudioManager manager((int)sampleRate,(int)frameSize);  
	manager.Start();                             // init audio manager

	AudioOut channel;   // audio channel out

	AudioIOConfig  audioOutCfg;     // to configure the channel

	audioOutCfg.SetChannelID(0);    // channel id 0 
	channel.Configure(audioOutCfg); // configure channel

	channel.Start();   // init channel

	SimpleOscillatorConfig oscCfg;
	oscCfg.SetSamplingRate(sampleRate);
	oscCfg.SetAmplitude(TData(0.6));
	SimpleOscillator osc(oscCfg);
	
	InControlRegistry controls = osc.GetInControls();
	InControl& freqControl = controls.Get("Pitch");

	Audio samples;
	samples.SetSize(frameSize);

	int nFrames = mSegment.GetnFrames();

	osc.Start();
	active = true;
	for(TIndex i= 0;i < nSamples;i += frameSize)
	{
		if(!active) break;
		freqControl.DoControl(mSegment.GetFrame(int(i*nFrames/nSamples)).GetFundamental().GetFreq(0));
		osc.Do(samples);
		channel.Do(samples);
		mSendData.Emit(samples.GetBuffer());
	}
	osc.Stop();
	channel.Stop();  
	if(active)	mRequestStop.Emit();
}

void FundPlayer::Stop()
{
	active = false;
}

// END
