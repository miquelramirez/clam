#include "TXTPathSongParser.hxx"
#include "Song.hxx"

#include <qfile.h>
#include <qdir.h>
#include <qstring.h>

#include <iostream>

TXTPathSongParser::TXTPathSongParser( const std::string & dirName ) 
{
	QString fileName = QDir::convertSeparators( QString( dirName.c_str() )+"/"+"path.txt" );
	QFile file ( fileName );
	if ( !file.open( IO_ReadOnly) )
	{
		std::cerr<<dirName<<"/path.txt not found!"<<std::endl;
		return;
	}
	QTextStream stream( &file );
	mLine = stream.readLine();
	while ( !stream.atEnd() )
	{
		if ( !mLine.isEmpty() )
		{
			QString songId;
			QString path;
			songIdInPathesFile( songId );
			pathInPathesFile( path );
			mIdPath[std::string( songId.ascii() ) ] = std::string( path.ascii() );
		}
		mLine = stream.readLine();
	}
}

void TXTPathSongParser::parsePath( Song & song, const std::string & idValue)
{
	if ( mIdPath.find(idValue) == mIdPath.end() )
	{
		std::cerr<<"path for "<<idValue<<" song not found"<<std::endl;
		return;
	}
	song.AddAttribute( "path", mIdPath[idValue] );
}

void TXTPathSongParser::songIdInPathesFile( QString & value )
{
	std::string stringToFill = std::string( mLine.ascii() );	
	value = QString( stringToFill.substr(0, stringToFill.find(char(9) ) ).c_str() );
	mLine.remove( 0, mLine.find( char(9) )+1 );
}

void TXTPathSongParser::pathInPathesFile( QString & value )
{
	std::string stringToFill = std::string( mLine.ascii() );	
	value = QString( stringToFill.c_str() );
//	value = value.replace(0, value.findRev("/"), "");
	value = "SongsTest/"+value;
}


