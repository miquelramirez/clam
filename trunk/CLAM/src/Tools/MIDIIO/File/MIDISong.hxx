#ifndef __MIDISong__
#define __MIDISong__

namespace MIDI
{

	class Track;

	class Song
	/* a midi song is a collection of tracks */
	{
	private:
		class SongImpl* mImpl; /* hide implementation */
	public:
		Song(void);
		~Song(void);
		
		int Tracks(void) const;
		Track* GetTrack(int i) const;
		void AddTrack(Track* t);
		unsigned short GetTicksPerQ(void) const;
		void SetTicksPerQ(unsigned int v);
	};

}

#endif
