/*
 * MIDIMelodyPlayer.hxx
 * Interface for MIDIMelodyPlayer class.
 * Used to play MIDI from the obtained MIDI melody.
 *
 * Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#ifndef _MIDIMELODYPLAYER_
#define _MIDIMELODYPLAYER_

#include <string>
#include <pthread.h>
#include <CLAM/MIDIMelody.hxx>
#include <CLAM/Signalv0.hxx>
#include <CLAM/Slotv0.hxx>

using CLAM::MIDIMelody;
using SigSlot::Signalv0;
using SigSlot::Slotv0;

namespace V2M
{

class MIDIMelodyPlayer 
{
public:
	MIDIMelodyPlayer(const MIDIMelody& melody,
					 const std::string& device,
					 int program,
					 Slotv0& slot);

	~MIDIMelodyPlayer();

    void Stop();

private:
	pthread_t mThread;
	MIDIMelody _melody;
	std::string _device;
	int _program;

	Signalv0 mRequestStop;

	bool active;

    void PlayMIDIThreadSafe();
	static void* sPlayMIDIThreadSafe(void* thisObject);

	unsigned getmilliseconds();
};

} // namespace V2M

#endif

