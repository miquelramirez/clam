namespace MIDI
{

	class Event;
	class Song;
	
	class SongPlayer
	/* class to obtain events from a midi song, ordered in time */
	{
	private:
		class SongPlayerImpl* mImpl; // hide implementation
	public:
		SongPlayer(Song* song = NULL);
		void Init(Song* song);
		~SongPlayer();

		/* get the next event from the song */
		bool GetEvent(Event& event,int& trackId);

		/* todo: add method :GoBack */
		
		/* todo: add method :GoTo(ticks) */
		
	};

}
