#include "DataFacade.hxx"
#include <iostream>
#include <sstream>
#include "ClipBoard.hxx"

#include "Assert.hxx"
#include "AudioIO.hxx"      // imports CLAM::AudioManager and CLAM::AudioIOConfig declarations
#include "AudioManager.hxx"
#include "AudioOut.hxx"     // imports CLAM::AudioOut declaration
#include "AudioFile.hxx"    // imports CLAM::AudioFile declaration
#include "MultiChannelAudioFileReader.hxx" // imports CLAM::MultiChannelAudioFileReader
#include "Audio.hxx"        // imports the CLAM::Audio ProcessingData class interface
#include "Err.hxx"          // imports CLAM::Err exception class declaration

#include <algorithm>
static bool playSong = true;

Player::Player(const std::string & filename, DataFacade & data) : QThread(), mFilename(filename), mData(data)
{
}

void Player::run()
{
	try
	{

		CLAM::AudioFile file;
		file.OpenExisting( mFilename );
		
		if ( !file.IsReadable() )
		{
			std::cerr << "Error: file " << file.GetLocation() << " cannot be opened ";
			std::cerr << "or is encoded in an unrecognized format" << std::endl;
			exit();
		}

		std::vector<CLAM::Audio> incomingAudioChannels;
		incomingAudioChannels.resize( file.GetHeader().GetChannels() );
			
		CLAM::MultiChannelAudioFileReaderConfig cfg;
		
		cfg.SetSourceFile( file );
		
		CLAM::MultiChannelAudioFileReader reader;
		
		reader.Configure( cfg );
		
		const CLAM::TData playbackSampleRate = file.GetHeader().GetSampleRate();
		const CLAM::TSize frameSize = 1024; 

		CLAM::AudioManager theAudioManager( playbackSampleRate, frameSize );
		for ( unsigned i = 0; i < incomingAudioChannels.size(); i++ )
		{
			incomingAudioChannels[i].SetSize( frameSize );
		}



		CLAM::Array<CLAM::AudioOut > audioOutputs;
		audioOutputs.Resize( incomingAudioChannels.size() );
		audioOutputs.SetSize( incomingAudioChannels.size() );

		CLAM::AudioIOConfig  audioOutCfg;

		for ( int i = 0; i < audioOutputs.Size(); i++ )
		{		
			audioOutCfg.SetChannelID( i );
			audioOutputs[i].Configure( audioOutCfg );
			audioOutputs[i].Start();
		}

		reader.Start();
		
		bool ended = !playSong;
		
		while( reader.Do( incomingAudioChannels ) &&  !ended)
		{
			for ( int i = 0; i < audioOutputs.Size(); i++ )
				audioOutputs[i].Do( incomingAudioChannels[i] );
			ended = !playSong;
		}

		for ( int i = 0; i < audioOutputs.Size(); i++ )
			audioOutputs[i].Stop();

		reader.Stop();
		
		if (!ended)
			mData.emitSongEnded();

	}
	catch ( CLAM::Err& e )
	{
		e.Print();
		exit();
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		exit();		
	}
}

int DataFacade::size() const
{
	return mSongs.size();
}

const DataFacade::DescriptorsVector & DataFacade::getDescriptors() const
{
	return mDescriptors;
}

bool DataFacade::isEmpty(std::string temp)
{
	return temp == "";
}

std::string DataFacade::withoutSinonims(std::string withSinonims)
{
	unsigned int pos = withSinonims.find(",",0);
	if (pos == std::string::npos)
	{
		return withSinonims;
	}
	else
	{
		withSinonims.erase(pos);
		return withSinonims;
	}
}

void DataFacade::genres(DataFacade::GenresVector & genresV)
{
	for (tDataFacade::iterator it = mSongs.begin() ; it != mSongs.end() ; it ++)
	{
		Descriptors attr = (*it).second->GetAllAttributes();
		if ( !isEmpty(attr["genre"]))
		{
			std::string temp = withoutSinonims(attr["genre"]);
			if (std::find(genresV.begin(), genresV.end(), temp) == genresV.end() )
				genresV.push_back(temp);	
		}
	}
}
void DataFacade::play(const Song::Id & id)
{
	stopPlaying();
	playSong = true ;
	mPlayer = new Player(mSongs[id]->GetAttributeValue("LocalId"), *this);
	mPlayer->start();
}

void DataFacade::emitSongEnded()
{
	emit songEnded();
}

void DataFacade::stopPlaying()
{
	playSong = false;
	if (mPlayer != 0 && mPlayer->running())
	{
		mPlayer->wait();
		delete mPlayer;
		mPlayer = 0;
	}
}

DataFacade::SongVector * DataFacade::getAllSongs() const
{
	SongVector * songs = new SongVector;
	for (tDataFacade::const_iterator it=mSongs.begin();it!=mSongs.end();it++)
	{
		songs->push_back(it->first);
	}
	return songs;
}
DataFacade::DataFacade()
{
	init();
}

void DataFacade::init()
{
	mPlayer = 0;
}

void DataFacade::createSong(Song * song)
{
	mSongs.insert(std::make_pair<Song::Id,Song*>(song->GetId(),song));
	Descriptors::const_iterator it=song->GetAllAttributes().begin();
	for ( ;it!=song->GetAllAttributes().end();it++)
	{
		if (std::find(mDescriptors.begin(), mDescriptors.end(), it->first) == mDescriptors.end())
			mDescriptors.push_back(it->first);
	}
}
DataFacade::~DataFacade()
{
	stopPlaying();
	for (tDataFacade::iterator it = mSongs.begin();it!=mSongs.end();it++)
	{
		delete it->second;
	}
}
const Song& DataFacade::getSong(const Song::Id & id) const
{
	CLAM_ASSERT(mSongs.find(id)!=mSongs.end() ,("No song with such id: '"+id+"'").c_str());
	return *(mSongs.find(id)->second);
}
double DataFacade::normalizeDynamicComplexity( double dynamicComplexity ) const
{
	if (dynamicComplexity > 25.0 )
		return 1.0;
	return (dynamicComplexity / 25.0 );
}

double DataFacade::scaleDynamicComplexity( double normalizedComplexity ) const
{
	return normalizedComplexity * 25.0;
}

double DataFacade::normalizeDanceability( double danceability) const
{
	if ( danceability < 0.4 )
		return 0;
	if ( danceability > 1.5)
		return 0;
	danceability = danceability - 0.4;
	danceability = danceability / ( 1.5 - 0.4 );
	return 1.0-danceability;
}

double DataFacade::scaleDanceability( double normalizedDancebility ) const
{
	double result = 1-normalizedDancebility ;
	result = result * ( 1.5 - 0.4 );
	result = result + 0.4;
	return result;
}


