/*
 * AudioPlayer.hxx
 * Interface for AudioPlayer class
 * Used to play original audio.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
 */
#ifndef _AUDIOPLAYER_
#define _AUDIOPLAYER_

#include <pthread.h>
#include <CLAM/Audio.hxx>
#include <CLAM/Signalv0.hxx>
#include <CLAM/Slotv0.hxx>
#include <CLAM/Signalv1.hxx>
#include <CLAM/Slotv1.hxx>

using CLAM::Audio;
using CLAM::DataArray;
using SigSlot::Signalv0;
using SigSlot::Slotv0;
using SigSlot::Signalv1;
using SigSlot::Slotv1;

namespace V2M
{

class AudioPlayer
{
public:
    AudioPlayer(const Audio& in,
		Slotv0& slot0,
		Slotv1<DataArray>& slot1);

    ~AudioPlayer();

    void Stop();

private:
    pthread_t mThread;
    Audio mAudio;
    Signalv0 mRequestStop;
    Signalv1<DataArray> mSendData;

    bool active;

    void PlayingThreadSafe();
    static void* sPlayingThreadSafe(void* thisObject);

};

} // namespace V2M

#endif


