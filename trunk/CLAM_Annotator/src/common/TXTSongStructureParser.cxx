#include "TXTSongStructureParser.hxx"
#include "Song.hxx"

#include <iostream>

#include <qdir.h>

TXTSongStructureParser::TXTSongStructureParser( const std::string & dirName, Song & song, const std::string fileName )
{
	QDir dir( QString(dirName.c_str() ), fileName+"*.lab");
	QStringList list = dir.entryList();
	for ( QStringList::Iterator it = list.begin(); it!=list.end(); it++)
	{
		QString fileName = QDir::convertSeparators( QString( dirName.c_str() )+"/"+*it );
		QFile file ( fileName );
		if ( !file.open( IO_ReadOnly) )
			return;
		QTextStream stream( &file );
		mLine = stream.readLine();
		while ( !stream.atEnd() )
		{
			if ( !mLine.isEmpty() )
			{
				mLine = mLine.simplifyWhiteSpace();
				double origin;
				double end;
				QString info;
				originInString( origin );
				endInString( end );
				infoInString( info );
				if ( info != std::string("#") )
					song.AddSegmentInformation( origin, end, info);	
			}
			mLine = stream.readLine();
		}
	}
}

void TXTSongStructureParser::endInString( double & end )
{
	std::string stringToFill = std::string( mLine.ascii() );
	QString	value = QString( stringToFill.substr(0, stringToFill.find(" ") ).c_str() );
	end = value.toDouble();
	mLine.remove( 0, mLine.find( " " )+1 );
}

void TXTSongStructureParser::originInString( double & origin )
{
	std::string stringToFill = std::string( mLine.ascii() );
	QString	value = QString( stringToFill.substr(0, stringToFill.find(" ") ).c_str() );
	origin = value.toDouble();
	mLine.remove( 0, mLine.find( " " )+1 );
}

void TXTSongStructureParser::infoInString( QString & value )
{
	std::string stringToFill = std::string( mLine.ascii() );
	value = QString( stringToFill.substr(0, stringToFill.find(" ") ).c_str() );
	mLine.remove( 0, mLine.find( " " )+1 );
}
