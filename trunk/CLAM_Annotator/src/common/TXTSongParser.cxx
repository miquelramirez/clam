#include "TXTSongParser.hxx"
#include <qfile.h>
#include <qdir.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include "DataFacade.hxx"
#include "Song.hxx"
#include <iostream>
#include <string>

#include "TXTPathSongParser.hxx"
#include "TXTSongStructureParser.hxx"

void TXTSongParser::tonalDescriptorKey( Song & song )
{
	QString value;
	parseString(value);
	song.AddAttribute("Tonal Descriptor: Key Note", std::string( value.ascii() ) );
}

void TXTSongParser::tonalDescriptorMode( Song & song )
{
	QString value;
	parseString(value);
	song.AddAttribute("Tonal Descriptor: Mode", std::string( value.ascii() ) );
}

void TXTSongParser::tonalDescriptorStrength( Song & song )
{
	QString value;
	parseString(value);
	song.AddAttribute("Tonal Descriptor: Strength", std::string( value.ascii() ) );
}

void TXTSongParser::parseString( QString & name ) 
{
	std::string temp = std::string( mLine.ascii() );
	
	name = temp.substr(0, temp.find(";") );
	skip();
}

void TXTSongParser::danceability( Song & song )
{
	QString value;
	parseString(value);
	song.AddAttribute("Danceability", std::string( value.ascii() ) );
}

void TXTSongParser::dynamicComplexity( Song & song ) 
{
	QString value;
	parseString(value);
	song.AddAttribute("Dynamic Complexity", std::string( value.ascii() ) );
}

void TXTSongParser::nameOfArtist( Song & song )
{
	QString value;
	parseString(value);
	song.AddAttribute("Artist", std::string( value.ascii() ) );
}

void TXTSongParser::nameOfSong( Song & song ) 
{
	QString value;
	parseString( value);
	song.AddAttribute("Title", std::string( value.ascii() ) );
}

void TXTSongParser::skip() 
{
	mLine = mLine.remove(0, mLine.find(";")+1 );
}

void TXTSongParser::classicProbability( QString & value)
{
	parseString( value );
}
 
void TXTSongParser::danceProbability( QString & value )
{
	parseString( value );
}

void TXTSongParser::hipHopProbability( QString & value )
{
	parseString( value );
}

void TXTSongParser::jazzProbability( QString & value )
{
	parseString( value );
}

void TXTSongParser::popProbability( QString & value )
{
	parseString( value );
}

void TXTSongParser::rhythmAndBluesProbability( QString & value )
{
	parseString( value );
}

void TXTSongParser::rockProbability( QString & value )
{
	parseString( value );
}

void TXTSongParser::speechProbability( QString & value )
{
	parseString( value );
}

void TXTSongParser::id( QString & value )
{
	parseString( value );
}

void TXTSongParser::bpm( Song & song )
{
	QString value;
	parseString(value);
	song.AddAttribute("BPM",std::string( value.ascii() ) );
}

void TXTSongParser::genre( Song & song)
{
	QString genreWithHighestProbability="Classic";
	double probability=0.0;
	QString value;
	classicProbability( value );
	probability = value.toDouble();
	danceProbability( value );
	if ( value.toDouble() > probability )
	{
		genreWithHighestProbability = "Dance";
		probability = value.toDouble();
	}
	hipHopProbability( value );
	if ( value.toDouble() > probability )
	{
		genreWithHighestProbability = "Hip-Hop";
		probability = value.toDouble();
	}
	jazzProbability( value );
	if ( value.toDouble() > probability )
	{
		genreWithHighestProbability = "Jazz";
		probability = value.toDouble();
	}
	popProbability( value );
	if ( value.toDouble() > probability )
	{
		genreWithHighestProbability = "Pop";
		probability = value.toDouble();
	}

	rhythmAndBluesProbability( value );
	if ( value.toDouble() > probability )
	{
		genreWithHighestProbability = "Rhythm and Blues";
		probability = value.toDouble();
	}

	rockProbability( value );
	if ( value.toDouble() > probability )
	{
		genreWithHighestProbability = "Rock";
		probability = value.toDouble();
	}

	speechProbability( value );
	if ( value.toDouble() > probability )
	{
		genreWithHighestProbability = "Speech";
		probability = value.toDouble();
	}
	song.AddAttribute( "genre", std::string(genreWithHighestProbability.ascii()) );
}

void TXTSongParser::parseSongs( const std::string & dirName, DataFacade & data )
{
	QString fileName = QDir::convertSeparators( QString( dirName.c_str() )+"/"+"songs.txt" );
	QFile file ( fileName );
	if ( file.open( IO_ReadOnly ) )
	{
		TXTPathSongParser txtPathSongParser( dirName );
		
		QTextStream stream( &file );
		mLine = stream.readLine();
		while ( !stream.atEnd() )
		{
			mLine = stream.readLine(); 
			if ( mLine.contains(";") )
			{
				Song * song;
				song = new Song();
				QString value;
				id( value );
				std::string idValue = std::string( value.ascii() );
				nameOfSong( *song );
				nameOfArtist( *song );
				skip();
				dynamicComplexity( *song );
				danceability( *song );
				bpm( *song );
				skip();
				skip();
				tonalDescriptorKey( *song );
				tonalDescriptorMode( *song );
				tonalDescriptorStrength( *song );
				genre( *song );
				parsePath( txtPathSongParser, *song, idValue );
				parseStructure( dirName, *song, song->GetAttributeValue("Path") );
				data.createSong( song );
			}
		}
		file.close();
	}

}

void TXTSongParser::parsePath( TXTPathSongParser & parser, Song & song, const std::string & idValue)
{
	parser.parsePath( song, idValue );
}

void TXTSongParser::parseStructure( const std::string & dirName, Song & song, const std::string & idValue )
{
	TXTSongStructureParser( dirName, song, idValue );
}

TXTSongParser::TXTSongParser( const std::string & dirName, DataFacade & data )
{
	parseSongs( dirName , data);
}

