#ifndef TXT_PATH_SONG_PARSER_HXX
#define TXT_PATH_SONG_PARSER_HXX

#include <qstring.h>

#include <string>
#include <map>

class Song;

class TXTPathSongParser
{
	public:
		TXTPathSongParser ( const std::string & dirName );
		void parsePath( Song & , const std::string &) ;
	private:
		void songIdInPathesFile( QString & value);
		void pathInPathesFile( QString & value);
	private:
		typedef std::map< std::string, std::string > IdPath;
		IdPath mIdPath;
		QString mLine;
};

#endif

