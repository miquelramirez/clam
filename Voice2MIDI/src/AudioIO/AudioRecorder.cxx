/*
 * AudioRecorder.cxx
 * Implementation for AudioRecorder class.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include "AudioRecorder.hxx"
#include <CLAM/AudioIO.hxx>
#include <CLAM/AudioManager.hxx>
#include <CLAM/AudioIn.hxx>

using namespace CLAM;

namespace V2M
{

AudioRecorder::AudioRecorder(Slotv1<DataArray>& slot)
{
    active = false;
    mSendData.Connect(slot);
    pthread_create(&mThread,NULL,sRecordingThreadSafe,(void*)this);
}

AudioRecorder::~AudioRecorder()
{
    active = false;
    pthread_join(mThread,NULL);
}

void* AudioRecorder::sRecordingThreadSafe(void* ptr)
{
    ((AudioRecorder*)ptr)->RecordingThreadSafe();
    return NULL;
}

/* capturing the audio signal */
void AudioRecorder::RecordingThreadSafe()
{
    TData sampleRate = 11025; // sample rate -> 11025 Hz
    TSize frameSize = 2048;   // frame size
    audio.SetSampleRate(sampleRate);  // sample rate

    AudioManager manager((int)sampleRate,(int)frameSize);  
            
    AudioIn channel;   // audio channel in
    
    AudioIOConfig  audioInCfg;     // to configure the channel 

    audioInCfg.SetChannelID(0);    // channel id 0 
    channel.Configure(audioInCfg); // configure channel
    
    manager.Start(); // init manager
    channel.Start(); // init channel
   
    /* Audio aux */
    Audio audioFrame;
    audioFrame.SetSize(frameSize);

    active = true;

    TIndex leftIndex = 0;
    while(active)
    {
	channel.Do(audioFrame);  // record the frame
	audio.SetSize(leftIndex+frameSize);  // update the audio size 
	audio.SetAudioChunk(leftIndex,audioFrame); // add data
	leftIndex += frameSize; // update pos
	mSendData.Emit(audioFrame.GetBuffer()); 
    }

    channel.Stop();  // stop the process
}

void AudioRecorder::Stop()
{
    active = false;
}

const Audio& AudioRecorder::GetAudio()
{
    return audio;
}

} // namespace V2M

// END

