/*
 * AudioPlayer.cxx
 * Implementation for AudioPlayer class.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
 */
//#include <CLAM/AudioPlayer.hxx>
#include "AudioPlayer.hxx"
#include <CLAM/AudioIO.hxx>
#include <CLAM/AudioManager.hxx>
#include <CLAM/AudioOut.hxx>
#include <CLAM/Slotv0.hxx>

using namespace CLAM;

namespace V2M
{

AudioPlayer::AudioPlayer(const Audio& in,
			 Slotv0& slot0,
			 Slotv1<DataArray>& slot1)
    : mAudio(in)
{
    active = false;
    mRequestStop.Connect(slot0);
    mSendData.Connect(slot1);
    pthread_create(&mThread,NULL,sPlayingThreadSafe,this);
} 

AudioPlayer::~AudioPlayer()
{
    active = false;
    pthread_join(mThread,NULL);
}

void* AudioPlayer::sPlayingThreadSafe(void* ptr)
{
    ((AudioPlayer*)ptr)->PlayingThreadSafe();
    return NULL;
}

/* Play audio */
void AudioPlayer::PlayingThreadSafe()
{
    TSize nSamples = mAudio.GetSize();         // number of samples
    TData sampleRate = mAudio.GetSampleRate(); // sample rate
    TSize frameSize = 2048;                    // frame size
    
    AudioManager manager((int)sampleRate,(int)frameSize);
    manager.Start();                             // init audio manager

    AudioOut channel;   // out channel
    
    AudioIOConfig  audioOutCfg;     // to config the channel

    audioOutCfg.SetChannelID(0);    // channel id 0
    channel.Configure(audioOutCfg); // configure channel

    channel.Start();                // init channel
    
    /* send audio chunks */
    Audio samples;                // for the samples
    samples.SetSize(frameSize);   // set the frame size

    /*
     * index for the fragmentation loop
     */
    TIndex leftIndex = 0;         
    TIndex rightIndex = frameSize;

    active = true; 

    /* play loop */
    while (leftIndex < nSamples)
    {
	if(!active) break;
	/*
	 * Extract the audio chunks from original signal
	 */
	mAudio.GetAudioChunk(leftIndex,rightIndex,samples);

	/* write on the audio device */
	channel.Do(samples);
        
	leftIndex += frameSize;
	rightIndex += frameSize;

	mSendData.Emit(samples.GetBuffer());
    }

    channel.Stop();  
    if(active)	mRequestStop.Emit(); // send that playing is terminated
}

void AudioPlayer::Stop()
{
    active = false;
}

} // namespace V2M

// END


