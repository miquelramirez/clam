#ifndef TXT_SONG_PARSER_HXX
#define TXT_SONG_PARSER_HXX

#include <string>
#include <qstring.h>
#include <map>
class DataFacade;
class Song;
class TXTPathSongParser;

class TXTSongParser
{
	public:
		TXTSongParser( const std::string &, DataFacade & );
	private:
		void skip();
		void nameOfSong( Song & );
		void genre( Song &);
		void nameOfArtist( Song & );
		void parseString(QString &); 
		void dynamicComplexity( Song & );
		void danceability( Song & );
		void tonalDescriptorKey( Song & );
		void bpm( Song & );
		void tonalDescriptorMode( Song & );
		void tonalDescriptorStrength( Song & );
		void classicProbability( QString & );
		void danceProbability( QString & );
		void hipHopProbability( QString & );
		void jazzProbability( QString & );
		void popProbability( QString &);
		void rhythmAndBluesProbability( QString & );
		void rockProbability( QString & );
		void speechProbability( QString &);
		void id( QString &);
		void parseSongs( const std::string &, DataFacade & );
		void parsePath( TXTPathSongParser &, Song & song, const std::string & idValue );
		void parseStructure( const std::string & dirName, Song & song, const std::string & idValue );
	private:
		QString mLine;
		typedef std::map< std::string, std::string > IdPath;
		IdPath mIdPath;
};

#endif
