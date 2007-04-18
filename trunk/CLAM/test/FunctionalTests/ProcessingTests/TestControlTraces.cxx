#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include <CLAM/ControlTrace.hxx>
#include "similarityHelper.hxx"
#include <algorithm>
#include <cstdio>

namespace CLAMTest
{

class ControlTraceReaderFunctionalTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ControlTraceReaderFunctionalTest );

class ControlTraceReaderFunctionalTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ControlTraceReaderFunctionalTest );

	// Configuration values checking tests
	CPPUNIT_TEST( testConfigure_ReturnsTrueWithJustFilename );
	CPPUNIT_TEST( testConfigure_ReturnsFalseWithoutTraceFileInConfig );
	CPPUNIT_TEST( testConfigure_ReturnsTrueWhenFileExists );
	CPPUNIT_TEST( testConfigure_ReturnsFalseWhenFileDoesNotExist );
	CPPUNIT_TEST( testConfigure_ReturnsTrueWhenTraceEventsExist );
	CPPUNIT_TEST( testConfigure_ReturnsFalseWhenTraceFileIsEmpty );

	CPPUNIT_TEST_SUITE_END();

protected: // Attributes

	std::string mPathToTestData;
	std::string mInputFileName;
	std::string mEmptyFileName;

protected: // Auxiliary methods

public: // TestFixture interface

	void setUp()
	{
		mPathToTestData = GetTestDataDirectory() + "ControlTraceReader";
		mInputFileName = mPathToTestData + "Input.clamtrace";
		mEmptyFileName = mPathToTestData + "Empty.clamtrace";
	}

	void tearDown()
	{
	}

private: // tests cases

	void testConfigure_ReturnsTrueWithJustFilename()
	{
		CLAM::ControlTraceReaderConfig cfg;

		CLAM::ControlTraceInFilename file(mInputFileName);
		cfg.SetTraceFile( file );

		CLAM::ControlTraceReader proc;

		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( true, configResult );
	}

	void testConfigure_ReturnsFalseWithoutTraceFileInConfig()
	{
		CLAM::ControlTraceReaderConfig cfg;
		cfg.RemoveTraceFile();
		cfg.UpdateData();
		
		CLAM::ControlTraceReader proc;
		
		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( false, configResult );
	}
	
	void testConfigure_ReturnsTrueWhenFileExists()
	{
		CLAM::ControlTraceReaderConfig cfg;

		CLAM::ControlTraceInFilename file(mInputFileName);
		cfg.SetTraceFile( file );

		CLAM::ControlTraceReader proc;
		
		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( true, configResult );
	}

	void testConfigure_ReturnsFalseWhenFileDoesNotExist()
	{
		CLAM::ControlTraceReaderConfig cfg;

		CLAM::ControlTraceInFilename file(mPathToTestData + 
				std::string( "missing-file.clamtrace" ) );
		cfg.SetTraceFile( file );

		CLAM::ControlTraceReader proc;

		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( false, configResult );
	}
	
	void testConfigure_ReturnsTrueWhenTraceEventsExist()
	{
		CLAM::ControlTraceReaderConfig cfg;

		CLAM::ControlTraceInFilename file(mInputFileName);
		cfg.SetTraceFile( file );

		CLAM::ControlTraceReader proc;

		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( true, configResult );
	}

	void testConfigure_ReturnsFalseWhenTraceFileIsEmpty()
	{
		CLAM::ControlTraceReaderConfig cfg;

		CLAM::ControlTraceInFilename file(mEmptyFileName);
		cfg.SetTraceFile( file );

		CLAM::ControlTraceReader proc;

		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( false, configResult );
		
	}

};

class ControlTraceWriterFunctionalTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ControlTraceWriterFunctionalTest );

class ControlTraceWriterFunctionalTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ControlTraceWriterFunctionalTest );

	// Configuration values checking tests
	CPPUNIT_TEST( testConfigure_ReturnsTrueWithJustFilename );
	CPPUNIT_TEST( testConfigure_ReturnsFalseWithoutTraceFileInConfig );
	CPPUNIT_TEST( testConfigure_ReturnsTrueWhenFileExists );
	CPPUNIT_TEST( testConfigure_ReturnsFalseWithoutInputsInConfig );
	CPPUNIT_TEST( testConfigure_ReturnsFalseWhenConfiguredWithNoInputs );

	CPPUNIT_TEST_SUITE_END();

protected: // Attributes

	std::string mPathToTestData;
	std::string mOutputFileName;

protected: // Auxiliary methods

public: // TestFixture interface

	void setUp()
	{
		mPathToTestData = GetTestDataDirectory() + "ControlTraceWriter";
		mOutputFileName = mPathToTestData + "Output.clamtrace";
	}

	void tearDown()
	{
	}

private: // tests cases

	void testConfigure_ReturnsTrueWithJustFilename()
	{
		CLAM::ControlTraceWriterConfig cfg;

		CLAM::ControlTraceOutFilename file(mOutputFileName);
		cfg.SetTraceFile( file );

		CLAM::ControlTraceWriter proc;

		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( true, configResult );
	}

	void testConfigure_ReturnsFalseWithoutTraceFileInConfig()
	{
		CLAM::ControlTraceWriterConfig cfg;
		cfg.RemoveTraceFile();
		cfg.UpdateData();
		
		CLAM::ControlTraceWriter proc;
		
		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( false, configResult );
	}
	
	void testConfigure_ReturnsTrueWhenFileExists()
	{
		CLAM::ControlTraceWriterConfig cfg;

		CLAM::ControlTraceOutFilename file(mOutputFileName);
		cfg.SetTraceFile( file );

		CLAM::ControlTraceWriter proc;
		
		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( true, configResult );
	}

	void testConfigure_ReturnsFalseWithoutInputsInConfig()
	{
		CLAM::ControlTraceWriterConfig cfg;
		cfg.RemoveNumberOfInputs();
		cfg.UpdateData();

		CLAM::ControlTraceOutFilename file(mOutputFileName);
		cfg.SetTraceFile( file );

		CLAM::ControlTraceWriter proc;

		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( false, configResult );
		
	}

	void testConfigure_ReturnsFalseWhenConfiguredWithNoInputs()
	{
		CLAM::ControlTraceWriterConfig cfg;

		CLAM::ControlTraceOutFilename file(mOutputFileName);
		cfg.SetTraceFile( file );
		cfg.SetNumberOfInputs( 0 );

		CLAM::ControlTraceWriter proc;

		bool configResult = proc.Configure( cfg );

		CPPUNIT_ASSERT_EQUAL( false, configResult );
		
	}
};

}

