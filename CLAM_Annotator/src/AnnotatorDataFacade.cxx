#include "AnnotatorDataFacade.hxx"
#include "DataFacade.hxx"
#include <iostream>
#include <algorithm>

AnnotatorDataFacade::AnnotatorDataFacade( DataFacade & data ) : mDataFacade( data )
{
	initData();
}

std::string AnnotatorDataFacade::genreWithoutSinonims( const std::string & genreWithSinonims ) const
{
	DataFacade data;	
	return data.withoutSinonims( genreWithSinonims );
}

void AnnotatorDataFacade::initData()
{
	initFolders( mDataFacade );
	initSongsInFolders( mDataFacade );
	initProjects( mDataFacade );
	initDescriptors( mDataFacade );
	initGenres( mDataFacade );
}

void AnnotatorDataFacade::setDescriptor( const std::string & nameOfFile, const std::string & nameOfDescriptor, const std::string & value)
{
	if ( std::find( mFiles.begin(), mFiles.end(), nameOfFile ) == mFiles.end() )
	{
		std::cout<<"AnnotatorDataFacade descriptor "<<nameOfDescriptor<<" not found!"<<std::endl;
		return;
	}
	if ( mFilesAndAttributes.find( nameOfFile ) == mFilesAndAttributes.end() )
		return;
	if ( mFilesAndAttributes[ nameOfFile ].find( nameOfDescriptor ) == mFilesAndAttributes[ nameOfFile ].end() )
		return;
	mFilesAndAttributes[ nameOfFile ][ nameOfDescriptor ]=value;

}

double AnnotatorDataFacade::normalizeDynamicComplexity( double dynamicComplexity ) const
{
	return mDataFacade.normalizeDynamicComplexity( dynamicComplexity );	
}

double AnnotatorDataFacade::scaleDynamicComplexity( double normalizedComplexity ) const
{
	return mDataFacade.scaleDynamicComplexity( normalizedComplexity);
}

double AnnotatorDataFacade::normalizeDanceability( double danceability) const
{
	return mDataFacade.normalizeDanceability( danceability );
}

double AnnotatorDataFacade::scaleDanceability( double normalizedDancebility ) const
{
	return mDataFacade.scaleDanceability( normalizedDancebility );
}

void AnnotatorDataFacade::addProject( const std::string & name)
{
	if ( std::find( mProjects.begin(), mProjects.end(), name ) == mProjects.end() )
		mProjects.push_back( name );
}

void AnnotatorDataFacade::getProjects( AnnotatorDataFacade::StringList & listOfProjects ) const
{
	AnnotatorDataFacade::StringList::const_iterator it = mProjects.begin();
	AnnotatorDataFacade::StringList::const_iterator itEnd = mProjects.end();

	for( ; it != itEnd ; it++)
	{
		listOfProjects.push_back( *it );
	}
}

void AnnotatorDataFacade::addFileIntoFolder( const std::string & file, const std::string & folder)
{
	if ( std::find( mFoldersAndFiles[folder].begin(), mFoldersAndFiles[folder].end(), file) == mFoldersAndFiles[folder].end() )
		{
			mFoldersAndFiles[folder].push_back(file);
		}
	
}



void AnnotatorDataFacade::initFolders( DataFacade & data )
{
	DataFacade::GenresVector genres;
	data.genres(genres);
	DataFacade::GenresVector::iterator it = genres.begin();
	DataFacade::GenresVector::iterator itEnd = genres.end();
	for ( ; it != itEnd ; it++)
	{
		addFolder( *it );
	}
	addFolder("?");
}
void AnnotatorDataFacade::addDescriptorToSong( const std::string & nameDescriptor, const std::string & value, const std::string & song)
{
	if ( std::find( mFiles.begin(), mFiles.end(), song ) == mFiles.end() )
		return;
	mFilesAndAttributes[song][nameDescriptor]=value;
}

void AnnotatorDataFacade::getDescriptorsFromFile( const std::string & file, Descriptors & descriptors) 
{
	if ( std::find( mFiles.begin(), mFiles.end(), file) == mFiles.end() )
	{
	  ASSERT(false);
	  std::cout<<"no files"<<std::endl;
	  return;
	}
	Descriptors::const_iterator it = mFilesAndAttributes[file].begin();
	Descriptors::const_iterator itEnd = mFilesAndAttributes[file].end();
	for ( ; it != itEnd ; it++)
	{
		descriptors[it->first] = it->second;
	}
}

void AnnotatorDataFacade::addSegmentInformationToFile( const Song::Segments & segments, const std::string & file )
{
	if ( std::find( mFiles.begin(), mFiles.end(), file) == mFiles.end() )
		return;
	mFileSegments[ file ] = segments;
}

void AnnotatorDataFacade::getSegmentInformationFromFile( Song::Segments & segments, const std::string & file ) 
{
	if ( mFileSegments.find( file ) == mFileSegments.end() )
		return;
	Song::Segments::const_iterator it = mFileSegments[file].begin();
	Song::Segments::const_iterator itEnd = mFileSegments[file].end();
	for ( ; it != itEnd ; it++ )
	{
		segments.push_back( *it );
	}
}

void AnnotatorDataFacade::getFilesInProject( const std::string & project , StringList & listOfFiles) 
{
	if ( std::find( mProjects.begin(), mProjects.end(), project) == mProjects.end() )
		return;
	AnnotatorDataFacade::StringList::const_iterator it = mProjectsAndFiles[ project ].begin();
	AnnotatorDataFacade::StringList::const_iterator itEnd = mProjectsAndFiles[ project ].end();
	for ( ; it != itEnd ; it++)
	{
		listOfFiles.push_back( *it );
	}
}

void AnnotatorDataFacade::loadSongInProject( const std::string & project, const std::string & nameOfFile)
{
	if ( std::find( mProjects.begin(), mProjects.end(), project) == mProjects.end() )
	{
		return;
	}
	AnnotatorDataFacade::StringList::const_iterator it = mProjectsAndFiles[project].begin();
	AnnotatorDataFacade::StringList::const_iterator itEnd = mProjectsAndFiles[project].end();
	if ( std::find( it, itEnd, nameOfFile) != itEnd)
	{
		return;
	}
	mProjectsAndFiles[project].push_back( nameOfFile );
	if (std::find( mFiles.begin(), mFiles.end(), nameOfFile) == mFiles.end() )
	{
		mFiles.push_back( nameOfFile );
	}
}

void AnnotatorDataFacade::addFolder( const std::string & name)
{
	if (std::find( mFolders.begin(), mFolders.end(), name) != mFolders.end() )
	{
		return;
	}
	mFolders.push_back( name );
}

void AnnotatorDataFacade::getFolders( AnnotatorDataFacade::StringList & list) const
{
	AnnotatorDataFacade::StringList::const_iterator it = mFolders.begin() ;
	for (; it != mFolders.end() ; it++)
	{
		list.push_back(*it);
	}
}

void AnnotatorDataFacade::getFilesInFolder(const std::string & folder, AnnotatorDataFacade::StringList & listOfFiles) 
{
	AnnotatorDataFacade::StringList::const_iterator it = mFoldersAndFiles[folder].begin();
	AnnotatorDataFacade::StringList::const_iterator itEnd = mFoldersAndFiles[folder].end();
	for( ; it != itEnd ; it++)
	{
		listOfFiles.push_back(*it);
	}
}

void AnnotatorDataFacade::initGenres( DataFacade & data )
{
	DataFacade::GenresVector genres;
	data.genres( genres );
	DataFacade::GenresVector::iterator it = genres.begin();
	DataFacade::GenresVector::iterator itEnd = genres.end();
	for ( ; it != itEnd ; it++ )
	{
		mGenres.push_back(*it);
	}
}

void AnnotatorDataFacade::getGenres( AnnotatorDataFacade::StringList & genres)
{
	AnnotatorDataFacade::StringList::iterator it = mGenres.begin();
	AnnotatorDataFacade::StringList::iterator itEnd = mGenres.end();
	
	for ( ; it != itEnd; it++ )
	{
		genres.push_back(*it);
	}
}

void AnnotatorDataFacade::initDescriptors( DataFacade & data)
{
	DataFacade::SongVector * songVector = data.getAllSongs();
	DataFacade::SongVector::iterator it = songVector->begin();
	DataFacade::SongVector::iterator itEnd = songVector->end();

	for ( ; it!=itEnd ; it++)
	{
		Descriptors attributes = data.getSong( *it ).GetAllAttributes();
		std::string song = data.getSong( *it ).GetAttributeValue( "path" );
		Descriptors::iterator itAttributes = attributes.begin();
		Descriptors::iterator itAttributesEnd = attributes.end();
		for ( ; itAttributes != itAttributesEnd ; itAttributes++ )
		{
			std::string name = itAttributes->first;
			std::string value = itAttributes->second;
			addDescriptorToSong( name, value, song);
		}
	}
	delete songVector;

}

void AnnotatorDataFacade::initProjects( DataFacade & data)
{
	addProject( "TestProject" );
	DataFacade::SongVector * songVector = data.getAllSongs();
	DataFacade::SongVector::iterator it = songVector->begin();
	DataFacade::SongVector::iterator itEnd = songVector->end();
	for ( ; it != itEnd ; it++ )
	{
		std::string nameOfTheFile = data.getSong(*it).GetAttributeValue("path");
		loadSongInProject( "TestProject", nameOfTheFile);	
	}
	delete songVector;
}

void AnnotatorDataFacade::initSongsInFolders( DataFacade & data)
{	
	DataFacade::SongVector * songVector = data.getAllSongs();
	DataFacade::SongVector::iterator it = songVector->begin();
	DataFacade::SongVector::iterator itEnd = songVector->end();

	for ( ; it!=itEnd ; it++)
	{
		std::string nameOfTheFile = data.getSong(*it).GetAttributeValue("path");
		std::string genre = data.getSong( *it ).GetAttributeValue("genre");
		if ( genre == std::string("") )
		{
			addFileIntoFolder( nameOfTheFile, "?" );
		}
		addFileIntoFolder( nameOfTheFile, genreWithoutSinonims( genre) );
	}
	delete songVector;
}
