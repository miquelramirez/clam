#ifndef TXT_SONG_STRUCTURE_PARSER_HXX
#define TXT_SONG_STRUCTURE_PARSER_HXX

#include <string>

#include <qstring.h>

class Song;

class TXTSongStructureParser
{
	public:
		TXTSongStructureParser( const std::string & dirName, Song & song, const std::string idValue );
	private:
		void originInString( double & );
		void endInString( double & );
		void infoInString( QString & );
	private:
		QString mLine;
};

#endif
