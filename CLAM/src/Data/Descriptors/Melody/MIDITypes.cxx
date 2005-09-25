#include "MIDITypes.hxx"

namespace CLAM
{
	////////////// MIDIEvent /////////////////////

	void MIDIEvent::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetTime(0.0);
		SetLast(false);
	}

	unsigned MIDIEvent::Ticks() const
	{
		return (unsigned)((GetTime() * 1000.0 * 96.0) / 480.0);
	}

	int MIDIEvent::Length() const
	{
		return GetMessage().Size();
	}


	////////////// MIDITrack ///////////////////

	void MIDITrack::DefaultInit()
	{
		AddAll();
		UpdateData();
		GetTrackMelody().AddNumberOfNotes();
		GetTrackMelody().UpdateData();
		GetTrackMelody().SetNumberOfNotes(0);
	}


	/////////////// MIDISong ////////////////////////////

	void MIDISong::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetNumberOfTracks(0);
	}
}

// END

