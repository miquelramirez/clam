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
			char* pathToTestData = getenv("CLAM_TEST_DATA");

			if ( !pathToTestData )
				mPathToTestData ="../../../../../CLAM-TestData/"; 
			else
				mPathToTestData = pathToTestData;

		}

		void tearDown()
		{
		}

	private: // tests cases

		void testConfigure_ReturnsTrueWithJustFilename()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

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
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MonoAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWhenFileDoesNotExist()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "QWERTY.wav" ) );
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}
		
		void testConfigure_ReturnsTrueWhenSelectedChannelExists()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWhenSelectedChannelIsOutOfRange()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 32768 );

			CLAM::MonoAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
			
		}

		void testDo_PCM_JustOneFrameFromMonoFile()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples;
			readSamples.SetSampleRate( file.GetHeader().GetSampleRate() );
			readSamples.SetSize( 256 );

			proc.GetOutPorts().GetByNumber(0).Attach( readSamples );

			proc.Start();
			proc.Do();
			proc.Stop();

			double similarity = fileSimilarity( file, 0, readSamples );
						
			CPPUNIT_ASSERT(  similarity >= 0.9999 );
			
		}

		void testDo_PCM_JustTwoFramesFromMonoFile()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples_1;
			readSamples_1.SetSampleRate( file.GetHeader().GetSampleRate() );
			readSamples_1.SetSize( 256 );

			CLAM::DataArray previous;
			previous.Resize( readSamples_1.GetSize() );
			previous.SetSize( readSamples_1.GetSize() );


			proc.Start();

			proc.GetOutPorts().GetByNumber(0).Attach( readSamples_1 );

			proc.Do();
			
			std::copy( readSamples_1.GetBuffer().GetPtr(),
				   readSamples_1.GetBuffer().GetPtr()+readSamples_1.GetSize(),
				   previous.GetPtr() );

			proc.Do();

			proc.Stop();

			CPPUNIT_ASSERT(  fabs( -0.803968 // MRJ: Value established by inspection
					       - evaluateSimilarity( previous,
								     readSamples_1.GetBuffer()) 
						 ) < 1e-4 );

		}

		void testDo_PCM_JustOneFrameFromStereoFile()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples;
			readSamples.SetSampleRate( file.GetHeader().GetSampleRate() );
			readSamples.SetSize( 256 );
			
			proc.GetOutPorts().GetByNumber(0).Attach( readSamples );

			proc.Start();
			proc.Do();
			proc.Stop();

			double similarity = fileSimilarity( file, 1, readSamples );
						
			CPPUNIT_ASSERT(  similarity >= 0.9999 );
			
		}

		void testDo_PCM_JustTwoFramesFromStereoFile()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples_1;
			readSamples_1.SetSampleRate( file.GetHeader().GetSampleRate() );
			readSamples_1.SetSize( 256 );

			CLAM::DataArray previous;
			previous.Resize( readSamples_1.GetSize() );
			previous.SetSize( readSamples_1.GetSize() );


			proc.Start();

			proc.GetOutPorts().GetByNumber(0).Attach( readSamples_1 );

			proc.Do();

			std::copy( readSamples_1.GetBuffer().GetPtr(),
				   readSamples_1.GetBuffer().GetPtr()+readSamples_1.GetSize(),
				   previous.GetPtr() );

			proc.Do();

			proc.Stop();

			CPPUNIT_ASSERT( evaluateSimilarity( readSamples_1.GetBuffer(),
							    previous )
					< 1.0 );
			
		}

		void testDo_JustTwoFramesBeginTimesAreRight()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples_1;
			readSamples_1.SetSampleRate( file.GetHeader().GetSampleRate() );
			readSamples_1.SetSize( 256 );

			CLAM::TTime previousBeginTime = 0.0;


			proc.Start();

			proc.GetOutPorts().GetByNumber(0).Attach( readSamples_1 );

			proc.Do();

			previousBeginTime = readSamples_1.GetBeginTime();

			proc.Do();

			proc.Stop();
			
			CLAM::TTime truth = CLAM::TTime( readSamples_1.GetSize() ) / file.GetHeader().GetSampleRate();
			CLAM::TTime valueObtained = readSamples_1.GetBeginTime() - previousBeginTime;

			CPPUNIT_ASSERT( fabs( truth - valueObtained ) < 1e-4 );

		}

		void testDo_JustOneFrame_SampleRateIsOK()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples;
			readSamples.SetSize( 256 );


			proc.GetOutPorts().GetByNumber(0).Attach( readSamples );

			proc.Start();
			proc.Do();
			proc.Stop();

			CPPUNIT_ASSERT_EQUAL( file.GetHeader().GetSampleRate(),
					      readSamples.GetSampleRate() );

		}

		void testDo_OggVorbis_JustOneFrameFromStereoFile()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "ElvisStereo.ogg" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples;
			readSamples.SetSampleRate( file.GetHeader().GetSampleRate() );
			readSamples.SetSize( 256 );
			
			proc.GetOutPorts().GetByNumber(0).Attach( readSamples );

			proc.Start();
			proc.Do();
			proc.Stop();

			double similarity = fileSimilarity( file, 1, readSamples );
						
			CPPUNIT_ASSERT(  similarity >= 0.9999 );
			


		}

		void testDo_OggVorbis_JustTwoFramesFromStereoFile()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "ElvisStereo.ogg" ) );
			
			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			cfg.SetSelectedChannel( 1 );

			CLAM::MonoAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamples_1;
			readSamples_1.SetSampleRate( file.GetHeader().GetSampleRate() );
			readSamples_1.SetSize( 4096 );

			CLAM::DataArray previous;
			previous.Resize( readSamples_1.GetSize() );
			previous.SetSize( readSamples_1.GetSize() );


			proc.Start();

			proc.GetOutPorts().GetByNumber(0).Attach( readSamples_1 );

			for ( int i = 0; i < 10; i++ )
			{
				proc.Do();
				
				std::copy( readSamples_1.GetBuffer().GetPtr(),
					   readSamples_1.GetBuffer().GetPtr()+readSamples_1.GetSize(),
					   previous.GetPtr() );

				proc.Do();
			}

			proc.Stop();

			double sim = evaluateSimilarity( readSamples_1.GetBuffer(),
							 previous );


			CPPUNIT_ASSERT( sim <= 1.0 );
			
		}
		
		void test_WindowsMedia_WAVE_File()
		{
			CLAM::AudioFile file;
			
			file.SetLocation( mPathToTestData + std::string("1-wav-8000.wav" ) );
			
			CPPUNIT_ASSERT_EQUAL( true,
					       file.IsReadable() );
		}

		void test_MpegAudioFiles_AreDecoded_OK()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "trumpet.mp3" ) );


			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "trumpet-copy.wav" );

			CLAM::AudioFileHeader outputFileHeader;

			outputFileHeader.AddAll();
			outputFileHeader.UpdateData();

			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    1,
						    "WAV" );


			outputFile.SetHeader( outputFileHeader );

			CLAM::MonoAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );
			cfgReader.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MonoAudioFileReader procReader;
			CLAM::MonoAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamples;
			readSamples.SetSize( 256 );

			procReader.GetOutPorts().GetByNumber(0).Attach( readSamples );
			procWriter.GetInPorts().GetByNumber(0).Attach( readSamples );

			procReader.Start();
			procWriter.Start();

			int  framesRead = 0;

			while( procReader.Do() )
			{
				framesRead++;
				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MonoAudioFileReader procReader2;
			inputFile.SetLocation( "trumpet-copy.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamples2;
			readSamples2.SetSize( 256 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamples2 );
		
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			while( procReader.Do() && procReader2.Do() )
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
