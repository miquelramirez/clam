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
#include "Audio.hxx"
#include "Signalv0.hxx"
#include "Slotv0.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"

using CLAM::Audio;
using CLAM::DataArray;
using SigSlot::Signalv0;
using SigSlot::Slotv0;
using SigSlot::Signalv1;
using SigSlot::Slotv1;

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

#endif
