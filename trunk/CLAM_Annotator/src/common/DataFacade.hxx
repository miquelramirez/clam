#ifndef DATA_FACADE_HXX
#define DATA_FACADE_HXX

#include <qobject.h>
#include "Song.hxx"

#include <qthread.h>

class DataFacade;

class Player : public QThread 
{
	public:
		Player(const std::string &, DataFacade &);
		void run();
	private:
		std::string mFilename;
		DataFacade & mData;
};

class DataFacade : public QObject
{
	Q_OBJECT
public:
	DataFacade();
	static DataFacade & getInstance()
	{
		static DataFacade data;
		return data;
	}
	typedef std::vector<Song::Id  > SongVector;
	typedef std::vector<std::string > DescriptorsVector;
	typedef std::vector<std::string > GenresVector;
	
	void emitSongEnded();
	void genres(GenresVector & );
	void play(const Song::Id & );
	void stopPlaying();
	const Song & getSong(const Song::Id & ) const;
	void createSong(Song * song);
	SongVector * getAllSongs() const;
	std::string withoutSinonims(std::string);
	const DescriptorsVector & getDescriptors() const;
	int size() const;
	double normalizeDynamicComplexity( double dynamicComplexity ) const;
	double scaleDynamicComplexity( double normalizedDynamicComplexity ) const;
	double normalizeDanceability( double ) const;
	double scaleDanceability( double ) const;
	~DataFacade();

signals:
	void songEnded();

private:
	void init();
	bool isEmpty(std::string);
	typedef std::map<Song::Id, Song*> tDataFacade;		tDataFacade mSongs;
	DescriptorsVector mDescriptors;
	Player * mPlayer;
};

#endif

