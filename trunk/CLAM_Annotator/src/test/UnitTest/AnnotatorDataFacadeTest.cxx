#include <string>
#include <algorithm>
#include <cppunit/extensions/HelperMacros.h>
#include "AnnotatorDataFacade.hxx"
#include "DataFacade.hxx"
#include "cppUnitHelper.hxx"
#include "Assert.hxx"

#include <iostream>

class AnnotatorDataFacadeTest;

CPPUNIT_TEST_SUITE_REGISTRATION(AnnotatorDataFacadeTest );

class AnnotatorDataFacadeTest : public CppUnit::TestFixture	{
	CPPUNIT_TEST_SUITE(AnnotatorDataFacadeTest );
	CPPUNIT_TEST( testGetFolders );
	CPPUNIT_TEST( testGetFilesInFolders );
	CPPUNIT_TEST( testGetProjects );
	CPPUNIT_TEST( testGetFilesInProject );
	CPPUNIT_TEST( testGetDescriptorsFromAFile );
	CPPUNIT_TEST( testDanceability );
	CPPUNIT_TEST( testGenres );
	CPPUNIT_TEST( testgenreWithoutSinonims );
	CPPUNIT_TEST( testSetDescriptor );
	CPPUNIT_TEST( testNormalizeDynamicComplexity );
	CPPUNIT_TEST( testgetSegmentInformationFromFile );
	CPPUNIT_TEST_SUITE_END();
private:
	AnnotatorDataFacade * mData;
public:
	void setUp()
	{
		DataFacade data;
		mData = new AnnotatorDataFacade( data );
		mData->addFolder("Rock");
		mData->addFolder("Pop");
		mData->addFolder("Jazz");
		mData->addFolder("Folk");
		mData->addFolder("Funk");
		mData->addFolder("Reggae");
		mData->addFolder("House");
		mData->addFolder("Soul");
		mData->addFolder("Techno");
		mData->addFolder("Trance");
		mData->addFolder("Trance");
		mData->addFileIntoFolder("hola.mp3", "Rock");
		mData->addProject("project 1");
		mData->loadSongInProject( "project 1", "hola.mp3");
		mData->loadSongInProject( "project 1", "hola.mp3");
		mData->loadSongInProject( "unexistent project ", "hola.mp3"); 
		mData->addDescriptorToSong( "Artist", "hola artist", "hola.mp3" );
		mData->addDescriptorToSong( "Title", "hola title", "hola.mp3" );
		mData->addDescriptorToSong( "Genre", "Rock", "hola.mp3" );
		mData->addDescriptorToSong( "Danceability", "0.4", "hola.mp3" );
		mData->addDescriptorToSong( "Danceability", "0.9", "hola.mp3" );
	}
	
	void tearDown() 
	{

	}

	void testGetFolders()
	{
		AnnotatorDataFacade::StringList list;
		mData->getFolders( list );
		CPPUNIT_ASSERT_EQUAL( unsigned(11), list.size() );
		CPPUNIT_ASSERT_EQUAL( std::string("?"), list[0] );
		CPPUNIT_ASSERT_EQUAL( std::string("Rock"), list[1] );
		CPPUNIT_ASSERT_EQUAL( std::string("Pop"), list[2] );
		CPPUNIT_ASSERT_EQUAL( std::string("Jazz"), list[3] );
		CPPUNIT_ASSERT_EQUAL( std::string("Folk"), list[4] );
		CPPUNIT_ASSERT_EQUAL( std::string("Funk"), list[5] );
		CPPUNIT_ASSERT_EQUAL( std::string("Reggae"), list[6] );
		CPPUNIT_ASSERT_EQUAL( std::string("House"), list[7] );
		CPPUNIT_ASSERT_EQUAL( std::string("Soul"), list[8] );
		CPPUNIT_ASSERT_EQUAL( std::string("Techno"), list[9] );
		CPPUNIT_ASSERT_EQUAL( std::string("Trance"), list[10] );
	}

	void testGetFilesInFolders()
	{
		AnnotatorDataFacade::StringList list;
		mData->getFilesInFolder("Pop", list);
		CPPUNIT_ASSERT( list.size() == 0);
		mData->getFilesInFolder("Rock", list);
		CPPUNIT_ASSERT( list.size() == 1);
		mData->getFilesInFolder("Pop", list);
		CPPUNIT_ASSERT( list.size() == 1);
	}

	void testGetProjects()
	{
		AnnotatorDataFacade::StringList listOfProjects;
		mData->getProjects( listOfProjects );
		CPPUNIT_ASSERT_EQUAL( unsigned(2), listOfProjects.size() );
		CPPUNIT_ASSERT_EQUAL( std::string("TestProject"), listOfProjects[0] );
		CPPUNIT_ASSERT_EQUAL( std::string("project 1"), listOfProjects[1] );
	}

	void testGetFilesInProject()
	{
		AnnotatorDataFacade::StringList listOfFiles;
		mData->getFilesInProject( "project that does not exist", listOfFiles);
		CPPUNIT_ASSERT( listOfFiles.size() == 0 );
		mData->getFilesInProject( "project 1", listOfFiles );
		CPPUNIT_ASSERT( listOfFiles.size() == 1 );
	}

	void testGetDescriptorsFromAFile()
	{
		Descriptors attributes;
		mData->getDescriptorsFromFile( "hola.mp3", attributes);
		CPPUNIT_ASSERT( attributes.size() == 4);
		CPPUNIT_ASSERT( attributes["Artist"] == "hola artist" );
		CPPUNIT_ASSERT( attributes["Title"] == "hola title" );
		CPPUNIT_ASSERT( attributes["Genre"] == "Rock" );
		CPPUNIT_ASSERT( attributes["Danceability"] == "0.9");
		
	}

	void testDanceability()
	{
		CPPUNIT_ASSERT( mData->normalizeDanceability(0.0) == 0 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, mData->normalizeDanceability(0.4), 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, mData->normalizeDanceability(1.5), 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, mData->normalizeDanceability(1.6), 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.4909, mData->normalizeDanceability(0.96), 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.4, mData->scaleDanceability( mData->normalizeDanceability( 0.4 ) ), 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, mData->scaleDanceability( mData->normalizeDanceability( 1.5 ) ),  0.0001 );
	}

	void testGenres()
	{
		AnnotatorDataFacade::StringList genres;
		mData->getGenres( genres );
		CPPUNIT_ASSERT( genres.size() == 0 );
	}

	void testgenreWithoutSinonims()
	{
		CPPUNIT_ASSERT( "Pop" == mData->genreWithoutSinonims("Pop") );
		CPPUNIT_ASSERT( "Pop" == mData->genreWithoutSinonims("Pop, pop music") );
		CPPUNIT_ASSERT( "Pop music" == mData->genreWithoutSinonims("Pop music, Pop") );
	}

	void testSetDescriptor()
	{
		Descriptors attributes;
		mData->setDescriptor( "hola.mp3", "Artist", "artist changed");
		mData->setDescriptor( "hola.mp3", "descriptorThatDoesNotExist", "foo" );
		mData->getDescriptorsFromFile( "hola.mp3", attributes);
		CPPUNIT_ASSERT( attributes.size() == 4);
		CPPUNIT_ASSERT( attributes["Artist"] == "artist changed" );
		CPPUNIT_ASSERT( attributes["Title"] == "hola title" );
		CPPUNIT_ASSERT( attributes["Genre"] == "Rock" );
		CPPUNIT_ASSERT( attributes["Danceability"] == "0.9");
	}

	void testNormalizeDynamicComplexity()
	{
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, mData->normalizeDynamicComplexity( 0 ), 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( mData->normalizeDynamicComplexity( 25 ), 1.0, 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( mData->normalizeDynamicComplexity( 12.5 ), 0.5, 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( mData->normalizeDynamicComplexity( 50 ),  1.0, 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( mData->normalizeDynamicComplexity( 100 ), 1, 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, mData->scaleDynamicComplexity( mData->normalizeDynamicComplexity( 0 ) ),  0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 12.5, mData->scaleDynamicComplexity( mData->normalizeDynamicComplexity( 12.5 ) ), 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 12.5, mData->scaleDynamicComplexity( mData->normalizeDynamicComplexity( 12.5 ) ), 0.0001 );
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 25.0, mData->scaleDynamicComplexity( mData->normalizeDynamicComplexity( 25.0 ) ), 0.0001 );
	}

	void testgetSegmentInformationFromFile()
	{
		Song::Segments segments0;
		segments0.push_back( SegmentInformation(0,0,"a") );
		segments0.push_back( SegmentInformation(2,3, "e") );
		mData->addSegmentInformationToFile( segments0, "hola.mp3" );
		Song::Segments segments;
		segments.push_back( SegmentInformation(0, 10, "info" ) );
		mData->addSegmentInformationToFile( segments, "hola.mp3");
		Song::Segments segments2;
		mData->getSegmentInformationFromFile( segments2, "hola.mp3" );
		CPPUNIT_ASSERT( segments.size() == segments2.size() );
		CPPUNIT_ASSERT( segments.size() == 1 );
		CPPUNIT_ASSERT( segments[0].origin() == segments2[0].origin() );
		CPPUNIT_ASSERT( segments[0].end() == segments2[0].end() );
		std::string info1;
		std::string info2;
		segments[0].information( info1 );
		segments2[0].information( info2 );
		CPPUNIT_ASSERT( info1 == info2 );
	}
};

