#include <string>
#include <algorithm>
#include <cppunit/extensions/HelperMacros.h>
#include "AnnotatorDataFacade.hxx"
#include "cppUnitHelper.hxx"
#include "Assert.hxx"
#include "TXTSongParser.hxx"

#include "DataFacade.hxx"

#include <iostream>

class AnnotatorDataFacadeDataInFileTXTTest;

CPPUNIT_TEST_SUITE_REGISTRATION( AnnotatorDataFacadeDataInFileTXTTest );

class AnnotatorDataFacadeDataInFileTXTTest : public CppUnit::TestFixture	{
	
	CPPUNIT_TEST_SUITE( AnnotatorDataFacadeDataInFileTXTTest );
	CPPUNIT_TEST( testGetFolders );
	CPPUNIT_TEST( testGetFilesInFolders );
	CPPUNIT_TEST( testGetProjects );
	CPPUNIT_TEST( testGetFilesInProject );
	CPPUNIT_TEST( testGetDescriptorsFromAFile );
	CPPUNIT_TEST( testGetGenres );
	CPPUNIT_TEST_SUITE_END();
private:
	AnnotatorDataFacade * mData;
	DataFacade * mDataFacade;
public:
	void setUp()
	{
		mDataFacade = new DataFacade();
		TXTSongParser songParser( "DataTest/", *mDataFacade );
		mData = new AnnotatorDataFacade( *mDataFacade );
	}
	
	void tearDown() 
	{
		delete mData;
		delete mDataFacade;
	}

	void testGetFolders()
	{
		AnnotatorDataFacade::StringList list;
		mData->getFolders( list );
		CPPUNIT_ASSERT( list.size() == 2 );
		CPPUNIT_ASSERT( list[0] == "Jazz");
		CPPUNIT_ASSERT( list[1] == "?" );
	}

	void testGetFilesInFolders()
	{
		AnnotatorDataFacade::StringList list;
		mData->getFilesInFolder("Pop", list);
		CPPUNIT_ASSERT( list.size() == 0);
		mData->getFilesInFolder("Jazz", list);
		CPPUNIT_ASSERT_EQUAL( list.size(), unsigned(2) );
		CPPUNIT_ASSERT( list[0] == "testpath.mp3");
		mData->getFilesInFolder("Pop", list);
		CPPUNIT_ASSERT( list.size() == 2);
	}

	void testGetProjects()
	{
		AnnotatorDataFacade::StringList listOfProjects;
		mData->getProjects( listOfProjects );
		CPPUNIT_ASSERT( listOfProjects.size() == 1 );
		CPPUNIT_ASSERT( listOfProjects[0] == "TestProject" );
	}

	void testGetFilesInProject()
	{
		AnnotatorDataFacade::StringList listOfFiles;
		mData->getFilesInProject( "project that does not exist", listOfFiles);
		CPPUNIT_ASSERT( listOfFiles.size() == 0 );
		mData->getFilesInProject( "TestProject", listOfFiles );
		CPPUNIT_ASSERT( listOfFiles.size() == 2 );
		CPPUNIT_ASSERT_EQUAL( std::string("testpath.mp3"), listOfFiles[0] );
		CPPUNIT_ASSERT_EQUAL( std::string("testpath2.mp3"), listOfFiles[1] );
	}

	void testGetDescriptorsFromAFile()
	{
		Descriptors attributes;
		mData->getDescriptorsFromFile( "testpath.mp3", attributes);
		CPPUNIT_ASSERT_EQUAL( unsigned(10), attributes.size() );
		CPPUNIT_ASSERT_EQUAL( std::string("BASEMENT JAXX"), std::string(attributes["artists"]) );
		CPPUNIT_ASSERT_EQUAL( std::string("PETRILUDE"), std::string(attributes["name"]) );
		CPPUNIT_ASSERT_EQUAL( std::string("Jazz"), std::string(attributes["genre"]) );
		const char * char_attributes = attributes["danceability"].c_str();
		CPPUNIT_ASSERT_EQUAL( unsigned(0), static_cast<unsigned>(atof(char_attributes ) ) );
	
	}
	
	void testGetGenres()
	{
		AnnotatorDataFacade::StringList genres;
		mData->getGenres( genres );
		CPPUNIT_ASSERT( genres.size() == 1);
		CPPUNIT_ASSERT( genres[0] == std::string("Jazz") );
	}
};

