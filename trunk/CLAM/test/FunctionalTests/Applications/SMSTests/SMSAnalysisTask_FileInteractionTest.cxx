#include <string>
#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "SMS_ParameterChange.hxx"
#include "SMS_AnalysisTask.hxx"
#include "SMS_Errors.hxx"

namespace CLAMTest
{
	class SMSAnalysisTask_FileInteractionTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( SMSAnalysisTask_FileInteractionTest );

	class SMSAnalysisTask_FileInteractionTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( SMSAnalysisTask_FileInteractionTest );
		CPPUNIT_TEST( testSetSoundSource_FileExists );
		CPPUNIT_TEST( testSetSoundSource_FileDoesNotExist );
		CPPUNIT_TEST( testSetSoundSource_WellFormedWAVELoadsOK );
		CPPUNIT_TEST( testSetSoundSource_WellFormedWAVE_mono8bit22kHz_LoadsOK );
		CPPUNIT_TEST( testSetSoundSource_WellFormedWAVE_mono16bit22kHz_LoadsOK );
		CPPUNIT_TEST( testSetSoundSource_WellFormedWAVE_mono24bit22kHz_LoadsOK );
		CPPUNIT_TEST( testSetSoundSource_BadFormedWAVELoadFails );
		CPPUNIT_TEST( testSetSoundSource_WellFormedAIFLoadsOK );
		CPPUNIT_TEST( testSetSoundSource_WellFormedAIFFLoadsOK );
		CPPUNIT_TEST( testSetSoundSource_BadFormedAIFFLoadFails );
		CPPUNIT_TEST( testGetSoundSampleRate_AfterLoading22kHzFile );
		CPPUNIT_TEST( testGetSoundSampleRate_AfterLoading96kHzFile );
		CPPUNIT_TEST( testGetSoundLength_4_5_secs );

		CPPUNIT_TEST_SUITE_END();

	protected:
		std::string mPath;

	public:
		/// Common initialization, executed before each test method
		void setUp() 
		{ 
			mPath = "../../../../../CLAM-TestData/SMSTests/";
		}
		
		/// Common clean up, executed after each test method
		void tearDown() 
		{ 
		}
		
	private:

		void testSetSoundSource_FileExists()
		{
			// Setup

			// Exercise
			SMS::AnalysisTask newTask;

			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source" );
			changeCmd.SetNewFieldValue( mPath + "aeiou_lowpass.wav" );

			try
			{
				newTask.Process( changeCmd );
			}
			catch( SMS::ParameterConstraintViolated& error )
			{
				error.WhatHappened();
				CPPUNIT_FAIL("No constraint should have been violated!");
			}
			catch( ... )
			{
				CPPUNIT_FAIL( "No constraint should have been violated!" );
			}

		
			// Tear down
		}

		void testSetSoundSource_FileDoesNotExist()
		{
			SMS::AnalysisTask newTask;
		
			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source"  );
			changeCmd.SetNewFieldValue( mPath + "unexistingfile.wav" );

			try
			{
				newTask.Process( changeCmd );
			}
			catch( SMS::UnavailableFile& e )
			{
				e.WhatHappened();
			}
			catch( ... )
			{
				// Verification
				CPPUNIT_FAIL("Unexpected exception was thrown!");
			}
		}

		void testSetSoundSource_WellFormedWAVELoadsOK() 
		{
			SMS::AnalysisTask newTask;

			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source" );
			changeCmd.SetNewFieldValue( mPath + "aeiou_lowpass.wav" );

			try
			{
				newTask.Process( changeCmd );
			}
			catch( ... )
			{
				CPPUNIT_FAIL("Unexpected exception thrown!" );
			}

		}

		void testSetSoundSource_WellFormedWAVE_mono8bit22kHz_LoadsOK()
		{
			SMS::AnalysisTask newTask;

			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source" );
			changeCmd.SetNewFieldValue( mPath + "test_mono_8bit_22kHz.wav" );

			try
			{
				newTask.Process( changeCmd );
			}
			catch( ... )
			{
				// Verification	
				CPPUNIT_FAIL( "Unexpected exception thrown!" );
			}

		}

		void testSetSoundSource_WellFormedWAVE_mono16bit22kHz_LoadsOK()
		{
			SMS::AnalysisTask newTask;
			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source" );
			changeCmd.SetNewFieldValue( mPath + "test_mono_16bit_22kHz.wav" );

			try
			{
				newTask.Process( changeCmd );
			}
			catch( ... )
			{
				// Verification	
				CPPUNIT_FAIL( "Unexpected exception thrown!" );
			}

		}

		void testSetSoundSource_WellFormedWAVE_mono24bit22kHz_LoadsOK()
		{
			SMS::AnalysisTask newTask;

			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source" );
			changeCmd.SetNewFieldValue( mPath + "test_mono_24bit_22kHz.wav" );
		
			try
			{
				newTask.Process( changeCmd );
			}
			catch( ... )
			{
				// Verification
				CPPUNIT_FAIL( "Unexpected exception thrown!" );
			}

		}


		void testSetSoundSource_BadFormedWAVELoadFails()
		{
			SMS::AnalysisTask newTask;

			SMS::ParameterChange changedCmd;
			changedCmd.SetFieldKey( "Sound Source" );
			changedCmd.SetNewFieldValue(  mPath + "false.wav" );

			try
			{
				newTask.Process( changedCmd );
			}
			catch( SMS::UnsupportedFileFormat& error )
			{
				error.WhatHappened();
				return;
			}
			catch( ... )
			{
				// Verification
				CPPUNIT_FAIL( "Unexpected exception thrown!" );
			}

			CPPUNIT_FAIL( "No error was thrown!" );

		}

		void testSetSoundSource_WellFormedAIFLoadsOK()
		{
			SMS::AnalysisTask newTask;


			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source" );
			changeCmd.SetNewFieldValue( mPath + "aeiou_lowpass.aif" );

			try
			{
				newTask.Process( changeCmd );
			}
			catch( ... )
			{
				CPPUNIT_FAIL( "Unexpected exception thrown!" );
			}

		}

		void testSetSoundSource_WellFormedAIFFLoadsOK()
		{
			SMS::AnalysisTask newTask;

			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source" );
			changeCmd.SetNewFieldValue( mPath + "aeiou_lowpass.aiff" );

			try
			{
				newTask.Process( changeCmd );
			}
			catch( ... )
			{
				CPPUNIT_FAIL( "Unexpected exception thrown!" );
			}

		}

		void testSetSoundSource_BadFormedAIFFLoadFails()
		{
			SMS::AnalysisTask newTask;

			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source" );
			changeCmd.SetNewFieldValue( mPath + "false.aiff" );

			try
			{
				newTask.Process( changeCmd );
			}
			catch( SMS::UnsupportedFileFormat& error )
			{
				error.WhatHappened();
				return;
			}
			catch( ... )
			{
				CPPUNIT_FAIL( "Unexpected exception thrown!" );
			}

			CPPUNIT_FAIL( "No error was thrown!" );

		}

		void testGetSoundSampleRate_AfterLoading22kHzFile()
		{
			SMS::AnalysisTask newTask;

			SMS::ParameterChange changeCmd;

			changeCmd.SetFieldKey( "Sound Source" );
			changeCmd.SetNewFieldValue( mPath + "test_mono_24bit_22kHz.wav" );
		
			try
			{
				newTask.Process( changeCmd );	
			}
			catch( ... )
			{
				CPPUNIT_FAIL("Unexpected error was thrown!");
			}

			// Verification
			CPPUNIT_ASSERT_EQUAL( 22050.f, newTask.GetSampleRate() );						
		}

		void testGetSoundSampleRate_AfterLoading96kHzFile()
		{
			SMS::AnalysisTask newTask;

			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source");
			changeCmd.SetNewFieldValue( mPath + "test_mono_24bits_96kHz.wav" );

			try
			{
				newTask.Process( changeCmd );
			}
			catch( ... )
			{
				CPPUNIT_FAIL( "Unexpected error thrown!" );
			}

			// Verification
			CPPUNIT_ASSERT_EQUAL( 96000.f, newTask.GetSampleRate() );						
		}

		void testGetSoundLength_4_5_secs()
		{
			SMS::AnalysisTask newTask;

			SMS::ParameterChange changeCmd;
			changeCmd.SetFieldKey( "Sound Source" );
			changeCmd.SetNewFieldValue( mPath + "aeiou_lowpass.wav" );

			try
			{
				newTask.Process( changeCmd );
			}
			catch( ... )
			{
				CPPUNIT_FAIL( "Unexpected error thrown!" );
			}
			// Verification
			CPPUNIT_ASSERT_EQUAL( CLAM::TTime(4500), newTask.GetSoundLength() );						

		}

	};
}
