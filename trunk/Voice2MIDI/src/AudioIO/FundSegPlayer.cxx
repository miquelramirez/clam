/*
 * FundSegPlayer.cxx
 * Implementation for FundSegPlayer class. 
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include "FundSegPlayer.hxx"
#include <CLAM/AudioIO.hxx>
#include <CLAM/AudioManager.hxx>
#include <CLAM/AudioOut.hxx>
#include <CLAM/SimpleOscillator.hxx>

using namespace CLAM;

namespace V2M
{

FundSegPlayer::FundSegPlayer(const Melody& melody,
                                   TData sampleRate,
		    		   TSize nSamples,
			       Slotv0& slot0,
			       Slotv1<DataArray>& slot1) 
			       : _melody(melody),
			         _sampleRate(sampleRate),
			         _nSamples(nSamples)
{
	active = false;
	mRequestStop.Connect(slot0);
	mSendData.Connect(slot1);
	pthread_create(&mThread,NULL,sPlayFundSegThreadSafe,(void*)this);
}

FundSegPlayer::~FundSegPlayer()
{
	active = false;
	pthread_join(mThread,NULL);
}

void* FundSegPlayer::sPlayFundSegThreadSafe(void* ptr)
{
 	((FundSegPlayer*)ptr)->PlayFundSegThreadSafe();

	return NULL;
}

void FundSegPlayer::PlayFundSegThreadSafe()
{
    TSize frameSize = 2048;                         // frame size
	
    AudioManager manager((int)_sampleRate,(int)frameSize);  
    manager.Start();                             // init audio manager

    AudioOut channel;   // audio channel out
    
    AudioIOConfig  audioOutCfg;     // to configure the channel

    audioOutCfg.SetChannelID(0);    // channel id 0 
    channel.Configure(audioOutCfg); // configure channel

    channel.Start();   // init channel
    
	SimpleOscillatorConfig oscCfg;
    oscCfg.SetSamplingRate(_sampleRate);
	oscCfg.SetAmplitude(TData(0.6));
	SimpleOscillator osc(oscCfg);
	
	InControl& freqControl = osc.GetInControls().Get("Pitch");

	Audio samples;
	samples.SetSize(frameSize);
    
	int k = 0;

	osc.Start();
    active = true;

    for(TIndex i= 0;i < _nSamples;i += frameSize)
    {
		if(!active) break;
		if(TData(i/_sampleRate) >= _melody.GetNoteArray()[k].GetTime().GetEnd()) k++;
		if(TData(i/_sampleRate) >= _melody.GetNoteArray()[k].GetTime().GetBegin() &&
		   TData(i/_sampleRate) < _melody.GetNoteArray()[k].GetTime().GetEnd())
			freqControl.DoControl(_melody.GetNoteArray()[k].GetFundFreq());
		else
            freqControl.DoControl(0);
		osc.Do(samples);
        channel.Do(samples);
		mSendData.Emit(samples.GetBuffer());
        
	}
    osc.Stop();
    channel.Stop(); 
	if(active)	mRequestStop.Emit();
}

void FundSegPlayer::Stop()
{
	active = false;
}

} // namespace V2M

// END

