#ifndef __MIDITYPES__
#define __MIDITYPES__

#include <string>
#include "DynamicType.hxx"
#include "DataTypes.hxx"
#include "List.hxx"
#include "Array.hxx"
#include "MIDIMelody.hxx"
#include "ProcessingData.hxx"

namespace CLAM
{
	class MIDIEvent : public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDIEvent, 4, ProcessingData);
		/* Event name */
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/* Time in seconds */
		DYN_ATTRIBUTE (1, public, TTime, Time);   
		/* flag to indicate that this is the last event */
		DYN_ATTRIBUTE (2, public, bool, Last);
		/* Message */
		DYN_ATTRIBUTE (3, public, Array<TMIDIByte>, Message);
		
		unsigned Ticks() const;
		unsigned TicksFromDuration() const;
		int Length() const;

		bool operator==(const MIDIEvent& ev) {return Ticks()==ev.Ticks();}
		bool operator<(const MIDIEvent& ev) {return Ticks()<ev.Ticks();}
		bool operator>(const MIDIEvent& ev) {return Ticks()>ev.Ticks();}
		bool operator==(const MIDIEvent& ev) const {return Ticks()==ev.Ticks();}
		bool operator<(const MIDIEvent& ev) const {return Ticks()<ev.Ticks();}
		bool operator>(const MIDIEvent& ev) const {return Ticks()>ev.Ticks();}
		
	private:
		void DefaultInit();
	};

	class MIDITrack : public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDITrack, 4, ProcessingData);
		/* Track name */
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/* MIDI channels for notes in the melody */
		DYN_ATTRIBUTE (1, public, List<int>, Channels);
		/* Event info list */
		DYN_ATTRIBUTE (2, public, List<MIDIEvent>, EventInfo);   
		/* Track melody */
		DYN_ATTRIBUTE (3, public, MIDIMelody, TrackMelody);

	private:
		void DefaultInit();
	};

	class MIDISong : public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDISong, 3, ProcessingData);
		/* Song name */
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/* Song tracks */
		DYN_ATTRIBUTE (1, public, Array<MIDITrack>, Tracks);
		/* Number of tracks */
		DYN_ATTRIBUTE (2, public, int, NumberOfTracks);   
		
	private:
		void DefaultInit();
	};
}

#endif

