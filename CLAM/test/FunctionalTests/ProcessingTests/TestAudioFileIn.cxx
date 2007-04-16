#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" 
#include "ErrSoundFileIO.hxx"
#include "AudioFileIn.hxx"
#include "Audio.hxx"

namespace CLAMTest
{
	class AudioFileInFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( AudioFileInFunctionalTest );

	class AudioFileInFunctionalTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( AudioFileInFunctionalTest );
		CPPUNIT_TEST( testExceptionThrowingWorksAsExpected );
		CPPUNIT_TEST( testUnavailableSoundFile_DestructorIsSafe );
		CPPUNIT_TEST( testConfigureAndStart_FileExists );
		CPPUNIT_TEST( testConfigureAndStart_FileDoesNotExist );
		CPPUNIT_TEST( testConfigureAndStart_BadFormedWAVEFails );
		CPPUNIT_TEST( testConfigureAndStart_BadFormedAIFFFails );
		CPPUNIT_TEST( testDo_WAVE_mono_16bit_22kHz_LoadsOK );
		CPPUNIT_TEST( testDo_WAVE_mono_8bit_22kHz_LoadsOK );
		CPPUNIT_TEST( testDo_WAVE_mono_24bit_22kHz_LoadsOK );
		CPPUNIT_TEST( testDo_AIFF_mono_16bit_22kHz_LoadsOK );
		CPPUNIT_TEST( testDo_ReadSampleRateAsExpected_22kHz_WAVE );
		CPPUNIT_TEST( testDo_ReadSampleRateAsExpected_96kHz_WAVE );
		CPPUNIT_TEST( testDo_ReadAudioObjectLength_is_OK );

		CPPUNIT_TEST_SUITE_END();

	public:

		void setUp( )
		{
			mPathToTestData = GetTestDataDirectory("SMSTests/");
		}

		void tearDown( )
		{
		}

	protected:
		std::string mPathToTestData;
		
		void doProofOfConcept() throw( CLAM::UnavailableSoundFile )
		{
			CLAM::UnavailableSoundFile error( "Proof of concept");

			throw error;
		}

	private:
		
		void testExceptionThrowingWorksAsExpected( )
		{
			try
			{
				doProofOfConcept();
			}
			catch( CLAM::UnavailableSoundFile&  )
			{
				// As expected...
			}
			catch( ... )
			{
				// Unexpected...
				CPPUNIT_FAIL( "Exception type lost" );
			}
		}

		void testUnavailableSoundFile_DestructorIsSafe()
		{

			int a = 0;
			{
				CLAM::UnavailableSoundFile error( "Unwind me!" );

				error.what();
			}
			
			a = a+3;
			int b = a;
			b++;
		}

		void testConfigureAndStart_FileExists()
		{
			CLAM::AudioFileConfig processingConfig;
			processingConfig.SetFiletype( CLAM::EAudioFileType::eWave );
			processingConfig.SetFilename( mPathToTestData + "aeiou_lowpass.wav" );

			CLAM::AudioFileIn processing;
			
			bool result = processing.Configure( processingConfig );
			
			processing.Start();

			CPPUNIT_ASSERT_EQUAL( true, result );
		}

		void testConfigureAndStart_FileDoesNotExist()
		{
			CLAM::AudioFileConfig processingConfig;
			processingConfig.SetFiletype( CLAM::EAudioFileType::eWave );
			processingConfig.SetFilename( "idonotexist.wav" );

			CLAM::AudioFileIn processing;
			bool result = processing.Configure( processingConfig );
			CPPUNIT_ASSERT_EQUAL_MESSAGE( "Configuration should have failed", false, result);
		}

		void testConfigureAndStart_BadFormedWAVEFails() 
		{
			CLAM::AudioFileConfig processingConfig;
			processingConfig.SetFiletype( CLAM::EAudioFileType::eWave );
			processingConfig.SetFilename( mPathToTestData + "false.wav" );

			CLAM::AudioFileIn processing;
			
			try
			{
				processing.Configure( processingConfig );
			}
			catch( CLAM::UnsupportedSoundFileFormat& error )
			{
				error.what();
				
				return;
			}
		}

		void testConfigureAndStart_BadFormedAIFFFails()
		{
			CLAM::AudioFileConfig processingConfig;
			processingConfig.SetFiletype( CLAM::EAudioFileType::eAIFF );
			processingConfig.SetFilename( mPathToTestData + "false.aiff" );

			CLAM::AudioFileIn processing;
			
			try
			{
				processing.Configure( processingConfig );
			}
			catch( CLAM::UnsupportedSoundFileFormat& error )
			{
				error.what();
				
				return;
			}
		}
	
		void testDo_WAVE_mono_8bit_22kHz_LoadsOK()
		{
			CLAM::AudioFileConfig processingConfig;
			processingConfig.SetFiletype( CLAM::EAudioFileType::eWave );
			processingConfig.SetFilename( mPathToTestData + "test_mono_8bit_22kHz.wav" );

			CLAM::AudioFileIn processing;

			bool result = processing.Configure( processingConfig );
			
			processing.Start();

			CLAM::Audio loadedSamples;

			loadedSamples.SetSampleRate( processing.SampleRate() );
			loadedSamples.SetSize( processing.Size() );

			result = processing.Do( loadedSamples );

			CPPUNIT_ASSERT_EQUAL( true, result );
		}

		void testDo_WAVE_mono_16bit_22kHz_LoadsOK()
		{
			CLAM::AudioFileConfig processingConfig;
			processingConfig.SetFiletype( CLAM::EAudioFileType::eWave );
			processingConfig.SetFilename( mPathToTestData + "test_mono_16bit_22kHz.wav" );

			CLAM::AudioFileIn processing;

			bool result = processing.Configure( processingConfig );
			
			processing.Start();

			CLAM::Audio loadedSamples;

			loadedSamples.SetSampleRate( processing.SampleRate() );
			loadedSamples.SetSize( processing.Size() );

			result = processing.Do( loadedSamples );

			CPPUNIT_ASSERT_EQUAL( true, result );
		}

		void testDo_WAVE_mono_24bit_22kHz_LoadsOK()
		{
			CLAM::AudioFileConfig processingConfig;
			processingConfig.SetFiletype( CLAM::EAudioFileType::eWave );
			processingConfig.SetFilename( mPathToTestData + "test_mono_24bit_22kHz.wav" );

			CLAM::AudioFileIn processing;

			bool result = processing.Configure( processingConfig );

			processing.Start();

			CLAM::Audio loadedSamples;

			loadedSamples.SetSampleRate( processing.SampleRate() );
			loadedSamples.SetSize( processing.Size() );

			result = processing.Do( loadedSamples );

			CPPUNIT_ASSERT_EQUAL( true, result );

		}

		void testDo_AIFF_mono_16bit_22kHz_LoadsOK()
		{
			CLAM::AudioFileConfig processingConfig;

			processingConfig.SetFiletype( CLAM::EAudioFileType::eAIFF );
			processingConfig.SetFilename( mPathToTestData + "aeiou_lowpass.aiff" );
			
			CLAM::AudioFileIn processing;

			processing.Configure( processingConfig );

			processing.Start();

			CLAM::Audio loadedSamples;
			loadedSamples.SetSampleRate( processing.SampleRate() );
			loadedSamples.SetSize( processing.Size() );	

			CPPUNIT_ASSERT_EQUAL( true, processing.Do( loadedSamples ) );
		}

		void testDo_ReadSampleRateAsExpected_22kHz_WAVE()
		{
			CLAM::AudioFileConfig processingConfig;

			processingConfig.SetFiletype( CLAM::EAudioFileType::eWave );
			processingConfig.SetFilename( mPathToTestData + "test_mono_16bit_22kHz.wav" );
			
			CLAM::AudioFileIn processing;

			CPPUNIT_ASSERT_EQUAL( true, processing.Configure( processingConfig ) );
			
			processing.Configure( processingConfig );

			processing.Start();

			CPPUNIT_ASSERT_EQUAL( 22050.0, double(processing.SampleRate()) );			
		}

		void testDo_ReadSampleRateAsExpected_96kHz_WAVE()
		{
			CLAM::AudioFileConfig processingConfig;

			processingConfig.SetFiletype( CLAM::EAudioFileType::eWave );
			processingConfig.SetFilename( mPathToTestData + "test_mono_24bits_96kHz.wav" );
			
			CLAM::AudioFileIn processing;

			processing.Configure( processingConfig );

			processing.Start();

			CPPUNIT_ASSERT_EQUAL( 96000.0, double(processing.SampleRate()) );			
		}

		void testDo_ReadAudioObjectLength_is_OK()
		{
			CLAM::AudioFileConfig processingConfig;

			processingConfig.SetFiletype( CLAM::EAudioFileType::eWave );
			processingConfig.SetFilename( mPathToTestData + "aeiou_lowpass.wav" );
			
			CLAM::AudioFileIn processing;

			CPPUNIT_ASSERT_EQUAL( true, processing.Configure( processingConfig ) );

			processing.Configure( processingConfig );
			
			processing.Start();

			CLAM::Audio loadedSamples;
			loadedSamples.SetSampleRate( processing.SampleRate() );
			loadedSamples.SetSize( processing.Size() );

			CPPUNIT_ASSERT_EQUAL( CLAM::TTime(4500), loadedSamples.GetEndTime() - loadedSamples.GetBeginTime() );

		}
		
	};
}

