#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileReaderConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "Audio.hxx"
#include "CLAM_Math.hxx"
#include "AudioFileOpsTestsHelper.hxx"
#include "similarityHelper.hxx"
#include <algorithm>
#include <cstdio>

namespace CLAMTest
{
	class MonoAudioFileReaderFunctionalTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( MonoAudioFileReaderFunctionalTest );

	class MonoAudioFileReaderFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MonoAudioFileReaderFunctionalTest );

		// Configuration values checking tests
		CPPUNIT_TEST( testConfigure_ReturnsTrueWithJustFilename );
		CPPUNIT_TEST( testConfigure_ReturnsFalseWithoutAudioFileInConfig );
		CPPUNIT_TEST( testConfigure_ReturnsTrueWhenFileExists );
		CPPUNIT_TEST( testConfigure_ReturnsFalseWhenFileDoesNotExist );
		CPPUNIT_TEST( testConfigure_ReturnsTrueWhenSelectedChannelExists );
		CPPUNIT_TEST( testConfigure_ReturnsFalseWhenSelectedChannelIsOutOfRange );

		// File reading checking
		CPPUNIT_TEST( testDo_PCM_JustOneFrameFromMonoFile );
		CPPUNIT_TEST( testDo_PCM_JustTwoFramesFromMonoFile );
		CPPUNIT_TEST( testDo_PCM_JustOneFrameFromStereoFile );
		CPPUNIT_TEST( testDo_PCM_JustTwoFramesFromStereoFile );

		CPPUNIT_TEST( testDo_OggVorbis_JustOneFrameFromStereoFile );
		CPPUNIT_TEST( testDo_OggVorbis_JustTwoFramesFromStereoFile );

		CPPUNIT_TEST( test_MpegAudioFiles_AreDecoded_OK );

		CPPUNIT_TEST( testDo_JustTwoFramesBeginTimesAreRight );
		CPPUNIT_TEST( testDo_JustOneFrame_SampleRateIsOK );

		CPPUNIT_TEST( test_WindowsMedia_WAVE_File );

		CPPUNIT_TEST_SUITE_END();

	protected: // Attributes

		std::string mPathToTestData;

	protected: // Auxiliary methods

	public: // TestFixture interface

		void setUp()
		{
			mPathToTestData = GetTestDataDirectory();

		}

		void tearDown()
		{
		}

	private: // tests cases

		void testConfigure_ReturnsTrueWithJustFilename()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"Elvis.wav" );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWithoutAudioFileInConfig()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.RemoveSourceFile();
			cfg.UpdateData();
			
			CLAM::MonoAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}
		
		void testConfigure_ReturnsTrueWhenFileExists()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"Elvis.wav" );

			CLAM::MonoAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWhenFileDoesNotExist()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"QWERTY.wav" );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}
		
		void testConfigure_ReturnsTrueWhenSelectedChannelExists()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"Elvis.wav" );
			cfg.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWhenSelectedChannelIsOutOfRange()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"Elvis.wav" );
			cfg.SetSelectedChannel( 32768 );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
			
		}

		void testDo_PCM_JustOneFrameFromMonoFile()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"Elvis.wav" );
			cfg.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples;
			readSamples.SetSampleRate( proc.GetAudioFile().GetHeader().GetSampleRate() );
			readSamples.SetSize( 256 );

			proc.Start();
			proc.Do(readSamples);
			proc.Stop();

			double similarity = fileSimilarity( proc.GetAudioFile(), 0, readSamples );
						
			CPPUNIT_ASSERT(  similarity >= 0.9999 );
			
		}

		void testDo_PCM_JustTwoFramesFromMonoFile()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"Elvis.wav" );
			cfg.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples_1;
			readSamples_1.SetSampleRate( proc.GetAudioFile().GetHeader().GetSampleRate() );
			readSamples_1.SetSize( 256 );

			CLAM::DataArray previous;
			previous.Resize( readSamples_1.GetSize() );
			previous.SetSize( readSamples_1.GetSize() );


			proc.Start();

			proc.Do(readSamples_1);
			
			std::copy( readSamples_1.GetBuffer().GetPtr(),
				   readSamples_1.GetBuffer().GetPtr()+readSamples_1.GetSize(),
				   previous.GetPtr() );

			proc.Do(readSamples_1);

			proc.Stop();

			CPPUNIT_ASSERT(  fabs( -0.803968 // MRJ: Value established by inspection
					       - evaluateSimilarity( previous,
								     readSamples_1.GetBuffer()) 
						 ) < 1e-4 );

		}

		void testDo_PCM_JustOneFrameFromStereoFile()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples;
			readSamples.SetSampleRate( proc.GetAudioFile().GetHeader().GetSampleRate() );
			readSamples.SetSize( 256 );
			

			proc.Start();
			proc.Do(readSamples);
			proc.Stop();

			double similarity = fileSimilarity( proc.GetAudioFile(), 1, readSamples );
						
			CPPUNIT_ASSERT(  similarity >= 0.9999 );
			
		}

		void testDo_PCM_JustTwoFramesFromStereoFile()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples_1;
			readSamples_1.SetSampleRate( proc.GetAudioFile().GetHeader().GetSampleRate() );
			readSamples_1.SetSize( 256 );

			CLAM::DataArray previous;
			previous.Resize( readSamples_1.GetSize() );
			previous.SetSize( readSamples_1.GetSize() );


			proc.Start();

			proc.Do(readSamples_1);

			std::copy( readSamples_1.GetBuffer().GetPtr(),
				   readSamples_1.GetBuffer().GetPtr()+readSamples_1.GetSize(),
				   previous.GetPtr() );

			proc.Do(readSamples_1);

			proc.Stop();

			CPPUNIT_ASSERT( evaluateSimilarity( readSamples_1.GetBuffer(),
							    previous )
					< 1.0 );
			
		}

		void testDo_JustTwoFramesBeginTimesAreRight()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples;
			readSamples.SetSampleRate( proc.GetAudioFile().GetHeader().GetSampleRate() );
			readSamples.SetSize( 256 );

			proc.Start();
			proc.Do(readSamples);
			CLAM::TTime previousBeginTime = readSamples.GetBeginTime();
			proc.Do(readSamples);
			proc.Stop();
			
			CLAM::TTime truth = CLAM::TTime( readSamples.GetSize() ) / proc.GetAudioFile().GetHeader().GetSampleRate() *1000;
			CLAM::TTime valueObtained = readSamples.GetBeginTime() - previousBeginTime;

			CPPUNIT_ASSERT( fabs( truth - valueObtained ) < 1e-4 );
		}

		void testDo_JustOneFrame_SampleRateIsOK()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples;
			readSamples.SetSize( 256 );


			proc.Start();
			proc.Do(readSamples);
			proc.Stop();

			CPPUNIT_ASSERT_EQUAL( proc.GetAudioFile().GetHeader().GetSampleRate(),
					      readSamples.GetSampleRate() );

		}

		void testDo_OggVorbis_JustOneFrameFromStereoFile()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"ElvisStereo.ogg" );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples;
			readSamples.SetSampleRate( proc.GetAudioFile().GetHeader().GetSampleRate() );
			readSamples.SetSize( 256 );
			
			proc.Start();
			proc.Do(readSamples);
			proc.Stop();

			double similarity = fileSimilarity( proc.GetAudioFile(), 1, readSamples );
						
			CPPUNIT_ASSERT(  similarity >= 0.9999 );
		}

		void testDo_OggVorbis_JustTwoFramesFromStereoFile()
		{
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"ElvisStereo.ogg" );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples_1;
			readSamples_1.SetSampleRate( proc.GetAudioFile().GetHeader().GetSampleRate() );
			readSamples_1.SetSize( 4096 );

			CLAM::DataArray previous;
			previous.Resize( readSamples_1.GetSize() );
			previous.SetSize( readSamples_1.GetSize() );


			proc.Start();

			for ( int i = 0; i < 10; i++ )
			{
				proc.Do();
				
				std::copy( readSamples_1.GetBuffer().GetPtr(),
					   readSamples_1.GetBuffer().GetPtr()+readSamples_1.GetSize(),
					   previous.GetPtr() );

				proc.Do(readSamples_1);
			}

			proc.Stop();

			double sim = evaluateSimilarity( readSamples_1.GetBuffer(),
							 previous );


			CPPUNIT_ASSERT( sim <= 1.0 );
			
		}
		
		void test_WindowsMedia_WAVE_File()
		{
			CLAM::AudioFileSource file;
			
			file.OpenExisting( mPathToTestData + std::string("1-wav-8000.wav" ) );
			
			CPPUNIT_ASSERT_EQUAL( true,
					       file.IsReadable() );
		}

		void test_MpegAudioFiles_AreDecoded_OK()
		{
			CLAM::MonoAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData+"trumpet.mp3" );
			cfgReader.SetSelectedChannel( 0 );
			CLAM::MonoAudioFileReader procReader;
			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		

			CLAM::AudioFileTarget outputFile;
			CLAM::AudioFileHeader outputFileHeader;

			outputFileHeader.AddAll();
			outputFileHeader.UpdateData();

			outputFileHeader.SetValues( procReader.GetAudioFile().GetHeader().GetSampleRate(),
						    1,
						    "WAV" );
			outputFile.CreateNew( "trumpet-copy.wav", outputFileHeader );

			CLAM::MonoAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MonoAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamples;
			readSamples.SetSize( 256 );

			procReader.Start();
			procWriter.Start();

			int  framesRead = 0;

			while( procReader.Do(readSamples) )
			{
				framesRead++;
				procWriter.Do(readSamples);
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MonoAudioFileReader procReader2;
			cfgReader.SetSourceFile( "trumpet-copy.wav" );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamples2;
			readSamples2.SetSize( 256 );

			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			while( procReader.Do(readSamples) && procReader2.Do(readSamples2) )
			{
				double sim = evaluateSimilarity( readSamples.GetBuffer(), readSamples2.GetBuffer() );
				framesChecked++;

				CPPUNIT_ASSERT
					(  sim >= 0.5 );

			}

			procReader.Stop();
			procReader2.Stop();		
		       		
			CPPUNIT_ASSERT_EQUAL(framesRead, framesChecked );
		}

	};

}
