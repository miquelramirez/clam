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
		DYNAMIC_TYPE_USING_INTERFACE (MIDIEvent, 3, ProcessingData);
		/* Event name */
		DYN_ATTRIBUTE (0, public, std::string, Name);
		/* Time in seconds */
		DYN_ATTRIBUTE (1, public, TTime, Time);   
		/* Event */
		DYN_ATTRIBUTE (2, public, Array<TMIDIByte>, Event);
		
		unsigned Ticks() const;
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
		DYNAMIC_TYPE_USING_INTERFACE (MIDITrack, 3, ProcessingData);
		/* Channel number */
		DYN_ATTRIBUTE (0, public, int, Channel);
		/* Event info list */
		DYN_ATTRIBUTE (1, public, List<MIDIEvent>, EventInfo);   
		/* Track melody */
		DYN_ATTRIBUTE (2, public, MIDIMelody, TrackMelody);

	private:
		void DefaultInit();
	};

	class MIDISong : public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDISong, 2, ProcessingData);
		/* Song tracks */
		DYN_ATTRIBUTE (0, public, Array<MIDITrack>, Tracks);
		/* Number of tracks */
		DYN_ATTRIBUTE (1, public, int, NumberOfTracks);   
		
	private:
		void DefaultInit();
	};
}

#endif

