#ifndef __MIDITempo__
#define __MIDITempo__

namespace MIDI
{

	class Song;
	class Track;

	class Tempo
	/* class to convert midi ticks to time in milliseconds, 
	** according to the tempo information in a certain track 
	** (typically the first) of a midi song
	*/
	{
	private:
		class TempoImpl* mImpl; // hide implementation
	public:
		Tempo(Song* song = 0,Track* track = 0);

		~Tempo();

		void Init(Song* song = 0,Track* track = 0);

		Milliseconds TicksToTime(Ticks t);
	};

}

#endif
