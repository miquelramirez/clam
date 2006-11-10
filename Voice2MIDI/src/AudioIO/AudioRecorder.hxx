/*
 * AudioRecorder.hxx
 * Interface for AudioRecorder class
 * Used to capture an audio signal with the use of a microphone.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _AUDIORECORDER_
#define _AUDIORECORDER_

#include <pthread.h>
#include <CLAM/Audio.hxx>
#include <CLAM/Signalv1.hxx>
#include <CLAM/Slotv1.hxx>

using CLAM::Audio;
using CLAM::DataArray;
using SigSlot::Signalv1;
using SigSlot::Slotv1;

namespace V2M
{

class AudioRecorder
{
public:
    AudioRecorder(Slotv1<DataArray>& slot);
    ~AudioRecorder();

    void Stop();
    const Audio& GetAudio();

private:
    pthread_t mThread;
    Audio audio;
    Signalv1<DataArray> mSendData;
    
    bool active;

    void RecordingThreadSafe();
    static void* sRecordingThreadSafe(void* thisObject);

};

} // namespace V2M

#endif


