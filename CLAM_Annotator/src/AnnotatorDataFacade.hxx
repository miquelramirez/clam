#ifndef ANNOTATOR_DATA_FACADE_HXX
#define ANNOTATOR_DATA_FACADE_HXX

#include "Song.hxx"

#include <string>
#include <vector>

class DataFacade;

class AnnotatorDataFacade 
{
	public:
		typedef std::vector< std::string > StringList;
		
		std::string genreWithoutSinonims( const std::string & ) const;
		void setDescriptor( const std::string & nameOfFile, const std::string & nameOfDescriptor, const std::string & value);
		void addDescriptorToSong( const std::string & nameDescriptor, const std::string & value, const std::string & song);
		void getProjects( StringList &) const;
		void getFolders( StringList &) const;
		void getFilesInFolder( const std::string & , StringList &);
		void loadSongInProject( const std::string & project, const std::string & nameOfFile);
		void getFilesInProject( const std::string &, StringList &) ;
		void getDescriptorsFromFile( const std::string &, Descriptors &) ;
		void getSegmentInformationFromFile( Song::Segments &, const std::string & );
		void addSegmentInformationToFile( const Song::Segments &, const std::string &);
		void addFolder( const std::string &);
		void addFileIntoFolder( const std::string &, const std::string &);
		void addProject( const std::string &);
		double normalizeDanceability( double ) const;
		double scaleDanceability ( double ) const;
		double normalizeDynamicComplexity( double ) const;
		double scaleDynamicComplexity( double ) const;
		void getGenres(StringList &);
		virtual ~AnnotatorDataFacade(){}
	public:
		AnnotatorDataFacade( DataFacade &);
	protected:
		void initData();
		void initFolders( DataFacade &);
		void initSongsInFolders( DataFacade &);
		void initDescriptors( DataFacade &);
		void initProjects( DataFacade &);
		void initGenres( DataFacade & );
	
	private:
		StringList mGenres;
		StringList mProjects;
		StringList mFolders;
		typedef std::map< std::string, StringList > tFoldersAndFiles;
		tFoldersAndFiles mFoldersAndFiles;
		typedef tFoldersAndFiles tProjectsAndFiles;
		typedef std::map< std::string, Descriptors> tFilesAndAttributes;
		tFilesAndAttributes mFilesAndAttributes;
		tProjectsAndFiles mProjectsAndFiles;
		StringList mFiles;
		typedef std::map< std::string, Song::Segments> FileSegments;
		FileSegments mFileSegments;
		DataFacade & mDataFacade;
};

#endif
