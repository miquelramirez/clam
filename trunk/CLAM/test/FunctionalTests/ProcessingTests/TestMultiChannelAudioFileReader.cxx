#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "AudioFile.hxx"
#include "MultiChannelAudioFileReaderConfig.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "MultiChannelAudioFileWriter.hxx"
#include "Audio.hxx"
#include "CLAM_Math.hxx"
#include "AudioFileOpsTestsHelper.hxx"
#include "similarityHelper.hxx"
#include <algorithm>
#include <iterator>
#include <cstdlib>

namespace CLAMTest
{
	class MultiChannelAudioFileReaderFunctionalTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( MultiChannelAudioFileReaderFunctionalTest );

	class MultiChannelAudioFileReaderFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MultiChannelAudioFileReaderFunctionalTest );

		// Configuration values checking tests
		CPPUNIT_TEST( testConfigure_ReturnsTrueWithJustFilename );
		CPPUNIT_TEST( testConfigure_ReturnsFalseWithoutAudioFileInConfig );
		CPPUNIT_TEST( testConfigure_ReturnsTrueWhenFileExists );
		CPPUNIT_TEST( testConfigure_ReturnsFalseWhenFileDoesNotExist );
		CPPUNIT_TEST( testConfigure_DefaultChannelSelection_ChannelsOK );
		CPPUNIT_TEST( testConfigure_DefaultChannelSelection_PortNumberIsOK );
		CPPUNIT_TEST( testConfigure_ManualChannelSelection_ChannelsOK );
		CPPUNIT_TEST( testConfigure_ManualChannelSelection_PortNumberIsOK );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_ManualChannelSelection_TooMany );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_ManualChannelSelection_InvalidChannel );

		// Do test cases
		CPPUNIT_TEST( testDo_PCM_JustOneFrame );
		CPPUNIT_TEST( testDo_PCM_JustTwoFrames );

		CPPUNIT_TEST( testDo_OggVorbis_JustOneFrame );
		CPPUNIT_TEST( testDo_OggVorbis_JustTwoFrames );		

		CPPUNIT_TEST( test_MpegAudioFiles_128kbps_44kHz_AreDecoded_OK );
		CPPUNIT_TEST( test_MpegAudioFiles_192kbps_44kHz_AreDecoded_OK );

		CPPUNIT_TEST( test_MpegAudioFiles_64kbps_44kHz_AreDecoded_OK );
		CPPUNIT_TEST( test_MpegAudioFiles_320kbps_44kHz_AreDecoded_OK );
		CPPUNIT_TEST( test_MpegAudioFiles_LowVBR_44kHz_AreDecoded_OK );
		CPPUNIT_TEST( test_MpegAudioFiles_AvgVBR_44kHz_AreDecoded_OK );
		CPPUNIT_TEST( test_MpegAudioFiles_HighVBR_44kHz_AreDecoded_OK );


		CPPUNIT_TEST( testDo_JustOneFrame_SampleRateIsOK );
		CPPUNIT_TEST( testDo_JustTwoFrames_BeginTimesAreOK );

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
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWithoutAudioFileInConfig()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.RemoveSourceFile();
			cfg.UpdateData();
			
			CLAM::MultiChannelAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}

		void testConfigure_ReturnsTrueWhenFileExists()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWhenFileDoesNotExist()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile-false.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}

		void  testConfigure_DefaultChannelSelection_ChannelsOK()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );

			CLAM::MultiChannelAudioFileReaderConfig cfgToBeChecked =
				dynamic_cast< const CLAM::MultiChannelAudioFileReaderConfig& > ( proc.GetConfig() );
			
			CLAM::Array< CLAM::TIndex >& channels =
				cfgToBeChecked.GetSelectedChannels();

			bool allChannelsPresent = true;


			if ( channels.Size() == file.GetHeader().GetChannels() )
			{
				for ( int i = 0; i < channels.Size(); i++ )
					allChannelsPresent &= (channels[i] == i);
			}
			else
				allChannelsPresent = false;

			CPPUNIT_ASSERT_EQUAL( true,
					      allChannelsPresent );
			
		}

		void testConfigure_DefaultChannelSelection_PortNumberIsOK()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( 2,
					      std::distance( proc.GetOutPorts().Begin(),
							     proc.GetOutPorts().End() ) );
			
		}
		
		void testConfigure_ManualChannelSelection_ChannelsOK()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.AddSelectedChannels();
			cfg.UpdateData();
			cfg.GetSelectedChannels().Resize( 2 );
			cfg.GetSelectedChannels().SetSize( 2 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 0;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );

			CLAM::MultiChannelAudioFileReaderConfig cfgToBeChecked =
				dynamic_cast< const CLAM::MultiChannelAudioFileReaderConfig& > ( proc.GetConfig() );
			
			CLAM::Array< CLAM::TIndex >& channels =
				cfgToBeChecked.GetSelectedChannels();

			bool allChannelsPresent = true;


			if ( channels.Size() == cfg.GetSelectedChannels().Size() )
			{
				for ( int i = 0; i < channels.Size(); i++ )
					allChannelsPresent &= (channels[i] == cfg.GetSelectedChannels()[i] );
			}
			else
				allChannelsPresent = false;

			CPPUNIT_ASSERT_EQUAL( true,
					      allChannelsPresent );
			
		}

		void testConfigure_ManualChannelSelection_PortNumberIsOK()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );

			CLAM::MultiChannelAudioFileReaderConfig cfg;

			cfg.AddSelectedChannels();
			cfg.UpdateData();

			cfg.GetSelectedChannels().Resize( 2 );
			cfg.GetSelectedChannels().SetSize( 2 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 0;

			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( 2,
					      std::distance( proc.GetOutPorts().Begin(),
							     proc.GetOutPorts().End() ) );
		}

		void testConfigure_ReturnsFalse_ManualChannelSelection_TooMany()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.AddSelectedChannels();
			cfg.UpdateData();
			cfg.GetSelectedChannels().Resize( 3 );
			cfg.GetSelectedChannels().SetSize( 3 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 0;
			cfg.GetSelectedChannels()[2] = 2;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			bool cfgResult = proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( false,
					      cfgResult );			
		}

		void testConfigure_ReturnsFalse_ManualChannelSelection_InvalidChannel()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.AddSelectedChannels();
			cfg.UpdateData();
			cfg.GetSelectedChannels().Resize( 2 );
			cfg.GetSelectedChannels().SetSize( 2 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 65;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			bool cfgResult = proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( false,
					      cfgResult );			
			
		}

		void testDo_PCM_JustOneFrame ()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			proc.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			proc.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			proc.Start();
			proc.Do();
			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			fileSimilarity( file, readSamplesLeft, readSamplesRight,
					similarityLeft, similarityRight );
						
			CPPUNIT_ASSERT(  similarityLeft  >= 0.9999 );
			CPPUNIT_ASSERT(  similarityRight >= 0.9999 );

		}
		
		void testDo_PCM_JustTwoFrames()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );

			CLAM::DataArray prevLeftSamples;
			prevLeftSamples.Resize( 256 );
			prevLeftSamples.SetSize( 256 );

			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			CLAM::DataArray prevRightSamples;
			prevRightSamples.Resize( 256 );
			prevRightSamples.SetSize( 256 );

			proc.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			proc.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );


			proc.Start();

			proc.Do();
			
			// Save this frame to arrays
			std::copy( readSamplesLeft.GetBuffer().GetPtr(),
				   readSamplesLeft.GetBuffer().GetPtr() 
				   + readSamplesLeft.GetBuffer().Size(),
				   prevLeftSamples.GetPtr() );
			
			std::copy( readSamplesRight.GetBuffer().GetPtr(),
				   readSamplesRight.GetBuffer().GetPtr()
				   + readSamplesRight.GetBuffer().Size(),				   
				   prevRightSamples.GetPtr() );

			// call the Do again
			proc.Do();

			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			similarityLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(),
							     prevLeftSamples );
			similarityRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
							      prevRightSamples );
		       

			CPPUNIT_ASSERT( fabs( -0.21055 // MRJ: value established by inspection
					      - similarityLeft ) < 1e-4 );
			CPPUNIT_ASSERT( fabs( -0.150199 // MRJ: value established by inspection
					      - similarityRight ) < 1e-4 );

		}

		void testDo_JustOneFrame_SampleRateIsOK()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			proc.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			proc.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );	

			proc.Start();
			proc.Do();
			proc.Stop();

			
			CPPUNIT_ASSERT_EQUAL( file.GetHeader().GetSampleRate(),
					      readSamplesLeft.GetSampleRate() );
			CPPUNIT_ASSERT_EQUAL( file.GetHeader().GetSampleRate(),
					      readSamplesRight.GetSampleRate() );
		}

		void testDo_JustTwoFrames_BeginTimesAreOK()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );

			CLAM::TTime firstLeftBeginTime = 0.0;

			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			CLAM::TTime firstRightBeginTime = 0.0;

			proc.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			proc.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );
			
			proc.Start();

			proc.Do();
			
			// Save Begin Time's

			firstLeftBeginTime = readSamplesLeft.GetBeginTime();
			firstRightBeginTime = readSamplesRight.GetBeginTime();

			// call the Do again
			proc.Do();

			proc.Stop();

			CLAM::TTime truthLeft = CLAM::TTime( readSamplesLeft.GetSize() ) / file.GetHeader().GetSampleRate();
			CLAM::TTime yieldLeft = readSamplesLeft.GetBeginTime() - firstLeftBeginTime; 

			CPPUNIT_ASSERT( fabs( truthLeft - yieldLeft ) < 1e-4 );

			CLAM::TTime truthRight = CLAM::TTime( readSamplesRight.GetSize() ) / file.GetHeader().GetSampleRate(); 
			CLAM::TTime yieldRight = readSamplesRight.GetBeginTime() - firstRightBeginTime;
			
			CPPUNIT_ASSERT( fabs( truthRight - truthLeft ) < 1e-4 );

		}

		void testDo_OggVorbis_JustOneFrame ()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "ElvisStereo.ogg" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			proc.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			proc.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			proc.Start();
			proc.Do();
			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			fileSimilarity( file, readSamplesLeft, readSamplesRight,
					similarityLeft, similarityRight );
						
			CPPUNIT_ASSERT(  similarityLeft  >= 0.9999 );
			CPPUNIT_ASSERT(  similarityRight >= 0.9999 );

		}
		
		void testDo_OggVorbis_JustTwoFrames()
		{
			CLAM::AudioFile file;
			file.SetLocation( mPathToTestData + std::string( "ElvisStereo.ogg" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );

			CLAM::DataArray prevLeftSamples;
			prevLeftSamples.Resize( 256 );
			prevLeftSamples.SetSize( 256 );

			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );

			CLAM::DataArray prevRightSamples;
			prevRightSamples.Resize( 256 );
			prevRightSamples.SetSize( 256 );

			proc.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			proc.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );


			proc.Start();

			proc.Do();
			
			// Save this frame to arrays
			std::copy( readSamplesLeft.GetBuffer().GetPtr(),
				   readSamplesLeft.GetBuffer().GetPtr() 
				   + readSamplesLeft.GetBuffer().Size(),
				   prevLeftSamples.GetPtr() );
			
			std::copy( readSamplesRight.GetBuffer().GetPtr(),
				   readSamplesRight.GetBuffer().GetPtr()
				   + readSamplesRight.GetBuffer().Size(),				   
				   prevRightSamples.GetPtr() );

			// call the Do again
			proc.Do();

			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			similarityLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(),
							     prevLeftSamples );
			similarityRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
							      prevRightSamples );

			// MRJ: In Windows this is giving out very strange values
			/*
			std::cerr << "\n" << similarityLeft << "\n";
			std::cerr << "\n" << similarityRight << "\n";
			*/

			CPPUNIT_ASSERT( fabs( -0.840286 // MRJ: value established by inspection
					      - similarityLeft ) < 1e-4 );
			CPPUNIT_ASSERT( fabs( -0.840286 // MRJ: value established by inspection
					      - similarityRight ) < 1e-4 );


		}

		void test_MpegAudioFiles_128kbps_44kHz_AreDecoded_OK()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "test-stereo-decoding-128_44.mp3" ) );

			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "test-mp3-128-44.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.SetHeader( outputFileHeader );

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );


			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 4096 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 4096 );


			procReader.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			procReader.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do() )
			{
				framesRead++;

				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.SetLocation( "test-mp3-128-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 4096 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 4096 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft2 );
			procReader2.GetOutPorts().GetByNumber(1).Attach( readSamplesRight2 );
			
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			double maxSimLeft = -1e20;
			int    maxSimLeftFrame = 0;
			double minSimLeft = 1e20;
			int    minSimLeftFrame = 0;
			double averageSimLeft = 0.0;

			double maxSimRight = -1e20;
			int    maxSimRightFrame = 0;
			double minSimRight = 1e20;
			int    minSimRightFrame = 0;
			double averageSimRight = 0.0;


			while( procReader.Do() && procReader2.Do() )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(), 
								     readSamplesLeft2.GetBuffer() );

				double simRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
								      readSamplesRight2.GetBuffer() );


				if ( simLeft > maxSimLeft )
				{
					maxSimLeft = simLeft;
					maxSimLeftFrame = framesChecked;
				}
				if ( simLeft < minSimLeft )
				{
					minSimLeft = simLeft;
					minSimLeftFrame = framesChecked;
				}

				averageSimLeft += simLeft;


				if ( simRight > maxSimRight )
				{
					maxSimRight = simRight;
					maxSimRightFrame = framesChecked;
				}
				if ( simRight < minSimRight )
				{
					minSimRight = simRight;
					minSimRightFrame = framesChecked;
				}

				averageSimRight += simRight;
				


				CPPUNIT_ASSERT
					(  simLeft >= 0.99 );

				CPPUNIT_ASSERT
					( simRight >= 0.99 );

			}

			procReader.Stop();
			procReader2.Stop();		

			averageSimLeft *= (1.0/double(framesChecked));
			averageSimRight *= (1.0/double(framesChecked));

			/*

			std::cout << std::endl;
			std::cout << "Mp3 - 128kbps" << std::endl;
			std::cout << "Maximum Left similarity: " << maxSimLeft << " at " << maxSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Minimum Left similarity: " << minSimLeft << " at " << minSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Average Left similarity: " << averageSimLeft  << std::endl;

			std::cout << std::endl;
			std::cout << "Maximum Right similarity: " << maxSimRight << " at " << maxSimRightFrame;
			std::cout << std::endl;
			std::cout << "Minimum Right similarity: " << minSimRight << " at " << minSimRightFrame;
			std::cout << std::endl;
			std::cout << "Average Right similarity: " << averageSimRight  << std::endl;
			*/
			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );
			

		}

		void test_MpegAudioFiles_192kbps_44kHz_AreDecoded_OK()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "test-stereo-decoding-192_44.mp3" ) );

			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "test-mp3-192-44.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.SetHeader( outputFileHeader );

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 4096 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 4096 );


			procReader.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			procReader.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do() )
			{
				framesRead++;

				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.SetLocation( "test-mp3-192-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 4096 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 4096 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft2 );
			procReader2.GetOutPorts().GetByNumber(1).Attach( readSamplesRight2 );
			
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			double maxSimLeft = -1e20;
			int    maxSimLeftFrame = 0;
			double minSimLeft = 1e20;
			int    minSimLeftFrame = 0;
			double averageSimLeft = 0.0;

			double maxSimRight = -1e20;
			int    maxSimRightFrame = 0;
			double minSimRight = 1e20;
			int    minSimRightFrame = 0;
			double averageSimRight = 0.0;


			while( procReader.Do() && procReader2.Do() )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(), 
								     readSamplesLeft2.GetBuffer() );

				double simRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
								      readSamplesRight2.GetBuffer() );


				if ( simLeft > maxSimLeft )
				{
					maxSimLeft = simLeft;
					maxSimLeftFrame = framesChecked;
				}
				if ( simLeft < minSimLeft )
				{
					minSimLeft = simLeft;
					minSimLeftFrame = framesChecked;
				}

				averageSimLeft += simLeft;


				if ( simRight > maxSimRight )
				{
					maxSimRight = simRight;
					maxSimRightFrame = framesChecked;
				}
				if ( simRight < minSimRight )
				{
					minSimRight = simRight;
					minSimRightFrame = framesChecked;
				}

				averageSimRight += simRight;
				


				CPPUNIT_ASSERT
					(  simLeft >= 0.9 );

				CPPUNIT_ASSERT
					( simRight >= 0.9 );

			}

			procReader.Stop();
			procReader2.Stop();		

			averageSimLeft *= (1.0/double(framesChecked));
			averageSimRight *= (1.0/double(framesChecked));

			
			/*
			std::cout << std::endl;
			std::cout << "Mp3 - 196kbps" << std::endl;
			std::cout << "Maximum Left similarity: " << maxSimLeft << " at " << maxSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Minimum Left similarity: " << minSimLeft << " at " << minSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Average Left similarity: " << averageSimLeft  << std::endl;

			std::cout << std::endl;
			std::cout << "Maximum Right similarity: " << maxSimRight << " at " << maxSimRightFrame;
			std::cout << std::endl;
			std::cout << "Minimum Right similarity: " << minSimRight << " at " << minSimRightFrame;
			std::cout << std::endl;
			std::cout << "Average Right similarity: " << averageSimRight  << std::endl;
			*/
			
			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );
			

		}

		void test_MpegAudioFiles_64kbps_44kHz_AreDecoded_OK()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "test-stereo-decoding-64_44.mp3" ) );

			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "test-mp3-64-44.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.SetHeader( outputFileHeader );

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 4096 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 4096 );


			procReader.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			procReader.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do() )
			{
				framesRead++;

				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.SetLocation( "test-mp3-64-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 4096 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 4096 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft2 );
			procReader2.GetOutPorts().GetByNumber(1).Attach( readSamplesRight2 );
			
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			double maxSimLeft = -1e20;
			int    maxSimLeftFrame = 0;
			double minSimLeft = 1e20;
			int    minSimLeftFrame = 0;
			double averageSimLeft = 0.0;

			double maxSimRight = -1e20;
			int    maxSimRightFrame = 0;
			double minSimRight = 1e20;
			int    minSimRightFrame = 0;
			double averageSimRight = 0.0;


			while( procReader.Do() && procReader2.Do() )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(), 
								     readSamplesLeft2.GetBuffer() );

				double simRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
								      readSamplesRight2.GetBuffer() );


				if ( simLeft > maxSimLeft )
				{
					maxSimLeft = simLeft;
					maxSimLeftFrame = framesChecked;
				}
				if ( simLeft < minSimLeft )
				{
					minSimLeft = simLeft;
					minSimLeftFrame = framesChecked;
				}

				averageSimLeft += simLeft;


				if ( simRight > maxSimRight )
				{
					maxSimRight = simRight;
					maxSimRightFrame = framesChecked;
				}
				if ( simRight < minSimRight )
				{
					minSimRight = simRight;
					minSimRightFrame = framesChecked;
				}

				averageSimRight += simRight;
				


				CPPUNIT_ASSERT
					(  simLeft >= 0.9 );

				CPPUNIT_ASSERT
					( simRight >= 0.9 );

			}

			procReader.Stop();
			procReader2.Stop();		

			averageSimLeft *= (1.0/double(framesChecked));
			averageSimRight *= (1.0/double(framesChecked));
			
			/*
			std::cout << std::endl;
			std::cout << "Mp3 - 64kbps" << std::endl;
			std::cout << "Maximum Left similarity: " << maxSimLeft << " at " << maxSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Minimum Left similarity: " << minSimLeft << " at " << minSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Average Left similarity: " << averageSimLeft  << std::endl;

			std::cout << std::endl;
			std::cout << "Maximum Right similarity: " << maxSimRight << " at " << maxSimRightFrame;
			std::cout << std::endl;
			std::cout << "Minimum Right similarity: " << minSimRight << " at " << minSimRightFrame;
			std::cout << std::endl;
			std::cout << "Average Right similarity: " << averageSimRight  << std::endl;
			*/
			
			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );
			

		}

		void test_MpegAudioFiles_320kbps_44kHz_AreDecoded_OK()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "test-stereo-decoding-320_44.mp3" ) );

			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "test-mp3-320-44.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.SetHeader( outputFileHeader );

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 4096 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 4096 );


			procReader.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			procReader.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do() )
			{
				framesRead++;

				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.SetLocation( "test-mp3-320-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 4096 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 4096 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft2 );
			procReader2.GetOutPorts().GetByNumber(1).Attach( readSamplesRight2 );
			
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			double maxSimLeft = -1e20;
			int    maxSimLeftFrame = 0;
			double minSimLeft = 1e20;
			int    minSimLeftFrame = 0;
			double averageSimLeft = 0.0;

			double maxSimRight = -1e20;
			int    maxSimRightFrame = 0;
			double minSimRight = 1e20;
			int    minSimRightFrame = 0;
			double averageSimRight = 0.0;


			while( procReader.Do() && procReader2.Do() )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(), 
								     readSamplesLeft2.GetBuffer() );

				double simRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
								      readSamplesRight2.GetBuffer() );


				if ( simLeft > maxSimLeft )
				{
					maxSimLeft = simLeft;
					maxSimLeftFrame = framesChecked;
				}
				if ( simLeft < minSimLeft )
				{
					minSimLeft = simLeft;
					minSimLeftFrame = framesChecked;
				}

				averageSimLeft += simLeft;


				if ( simRight > maxSimRight )
				{
					maxSimRight = simRight;
					maxSimRightFrame = framesChecked;
				}
				if ( simRight < minSimRight )
				{
					minSimRight = simRight;
					minSimRightFrame = framesChecked;
				}

				averageSimRight += simRight;
				


				CPPUNIT_ASSERT
					(  simLeft >= 0.9 );

				CPPUNIT_ASSERT
					( simRight >= 0.9 );

			}

			procReader.Stop();
			procReader2.Stop();		

			averageSimLeft *= (1.0/double(framesChecked));
			averageSimRight *= (1.0/double(framesChecked));
			
			/*
			std::cout << std::endl;
			std::cout << "Mp3 - 320kbps" << std::endl;
			std::cout << "Maximum Left similarity: " << maxSimLeft << " at " << maxSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Minimum Left similarity: " << minSimLeft << " at " << minSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Average Left similarity: " << averageSimLeft  << std::endl;

			std::cout << std::endl;
			std::cout << "Maximum Right similarity: " << maxSimRight << " at " << maxSimRightFrame;
			std::cout << std::endl;
			std::cout << "Minimum Right similarity: " << minSimRight << " at " << minSimRightFrame;
			std::cout << std::endl;
			std::cout << "Average Right similarity: " << averageSimRight  << std::endl;
			*/

			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );
			

		}

		void test_MpegAudioFiles_LowVBR_44kHz_AreDecoded_OK()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "test-stereo-decoding-LowVBR_44.mp3" ) );

			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "test-mp3-LowVBR-44.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.SetHeader( outputFileHeader );

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 4096 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 4096 );


			procReader.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			procReader.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do() )
			{
				framesRead++;

				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.SetLocation( "test-mp3-LowVBR-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 4096 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 4096 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft2 );
			procReader2.GetOutPorts().GetByNumber(1).Attach( readSamplesRight2 );
			
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			double maxSimLeft = -1e20;
			int    maxSimLeftFrame = 0;
			double minSimLeft = 1e20;
			int    minSimLeftFrame = 0;
			double averageSimLeft = 0.0;

			double maxSimRight = -1e20;
			int    maxSimRightFrame = 0;
			double minSimRight = 1e20;
			int    minSimRightFrame = 0;
			double averageSimRight = 0.0;


			while( procReader.Do() && procReader2.Do() )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(), 
								     readSamplesLeft2.GetBuffer() );

				double simRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
								      readSamplesRight2.GetBuffer() );


				if ( simLeft > maxSimLeft )
				{
					maxSimLeft = simLeft;
					maxSimLeftFrame = framesChecked;
				}
				if ( simLeft < minSimLeft )
				{
					minSimLeft = simLeft;
					minSimLeftFrame = framesChecked;
				}

				averageSimLeft += simLeft;


				if ( simRight > maxSimRight )
				{
					maxSimRight = simRight;
					maxSimRightFrame = framesChecked;
				}
				if ( simRight < minSimRight )
				{
					minSimRight = simRight;
					minSimRightFrame = framesChecked;
				}

				averageSimRight += simRight;
				


				CPPUNIT_ASSERT
					(  simLeft >= 0.9 );

				CPPUNIT_ASSERT
					( simRight >= 0.9 );

			}

			procReader.Stop();
			procReader2.Stop();		

			averageSimLeft *= (1.0/double(framesChecked));
			averageSimRight *= (1.0/double(framesChecked));
			
			/*
			std::cout << std::endl;
			std::cout << "Mp3 - LowVBR" << std::endl;
			std::cout << "Maximum Left similarity: " << maxSimLeft << " at " << maxSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Minimum Left similarity: " << minSimLeft << " at " << minSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Average Left similarity: " << averageSimLeft  << std::endl;

			std::cout << std::endl;
			std::cout << "Maximum Right similarity: " << maxSimRight << " at " << maxSimRightFrame;
			std::cout << std::endl;
			std::cout << "Minimum Right similarity: " << minSimRight << " at " << minSimRightFrame;
			std::cout << std::endl;
			std::cout << "Average Right similarity: " << averageSimRight  << std::endl;
			*/

			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );
			

		}

		void test_MpegAudioFiles_HighVBR_44kHz_AreDecoded_OK()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "test-stereo-decoding-HighVBR_44.mp3" ) );

			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "test-mp3-HighVBR-44.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.SetHeader( outputFileHeader );

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 4096 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 4096 );


			procReader.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			procReader.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do() )
			{
				framesRead++;

				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.SetLocation( "test-mp3-HighVBR-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 4096 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 4096 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft2 );
			procReader2.GetOutPorts().GetByNumber(1).Attach( readSamplesRight2 );
			
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			double maxSimLeft = -1e20;
			int    maxSimLeftFrame = 0;
			double minSimLeft = 1e20;
			int    minSimLeftFrame = 0;
			double averageSimLeft = 0.0;

			double maxSimRight = -1e20;
			int    maxSimRightFrame = 0;
			double minSimRight = 1e20;
			int    minSimRightFrame = 0;
			double averageSimRight = 0.0;


			while( procReader.Do() && procReader2.Do() )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(), 
								     readSamplesLeft2.GetBuffer() );

				double simRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
								      readSamplesRight2.GetBuffer() );


				if ( simLeft > maxSimLeft )
				{
					maxSimLeft = simLeft;
					maxSimLeftFrame = framesChecked;
				}
				if ( simLeft < minSimLeft )
				{
					minSimLeft = simLeft;
					minSimLeftFrame = framesChecked;
				}

				averageSimLeft += simLeft;


				if ( simRight > maxSimRight )
				{
					maxSimRight = simRight;
					maxSimRightFrame = framesChecked;
				}
				if ( simRight < minSimRight )
				{
					minSimRight = simRight;
					minSimRightFrame = framesChecked;
				}

				averageSimRight += simRight;
				


				CPPUNIT_ASSERT
					(  simLeft >= 0.9 );

				CPPUNIT_ASSERT
					( simRight >= 0.9 );

			}

			procReader.Stop();
			procReader2.Stop();		

			averageSimLeft *= (1.0/double(framesChecked));
			averageSimRight *= (1.0/double(framesChecked));
			
			/*
			std::cout << std::endl;
			std::cout << "Mp3 - HighVBR" << std::endl;
			std::cout << "Maximum Left similarity: " << maxSimLeft << " at " << maxSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Minimum Left similarity: " << minSimLeft << " at " << minSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Average Left similarity: " << averageSimLeft  << std::endl;

			std::cout << std::endl;
			std::cout << "Maximum Right similarity: " << maxSimRight << " at " << maxSimRightFrame;
			std::cout << std::endl;
			std::cout << "Minimum Right similarity: " << minSimRight << " at " << minSimRightFrame;
			std::cout << std::endl;
			std::cout << "Average Right similarity: " << averageSimRight  << std::endl;
			*/

			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );
			

		}

		
		void test_MpegAudioFiles_AvgVBR_44kHz_AreDecoded_OK()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "test-stereo-decoding-AvgVBR_44.mp3" ) );

			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "test-mp3-AvgVBR-44.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.SetHeader( outputFileHeader );

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 4096 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 4096 );


			procReader.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			procReader.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do() )
			{
				framesRead++;

				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.SetLocation( "test-mp3-AvgVBR-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 4096 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 4096 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft2 );
			procReader2.GetOutPorts().GetByNumber(1).Attach( readSamplesRight2 );
			
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			double maxSimLeft = -1e20;
			int    maxSimLeftFrame = 0;
			double minSimLeft = 1e20;
			int    minSimLeftFrame = 0;
			double averageSimLeft = 0.0;

			double maxSimRight = -1e20;
			int    maxSimRightFrame = 0;
			double minSimRight = 1e20;
			int    minSimRightFrame = 0;
			double averageSimRight = 0.0;


			while( procReader.Do() && procReader2.Do() )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(), 
								     readSamplesLeft2.GetBuffer() );

				double simRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
								      readSamplesRight2.GetBuffer() );


				if ( simLeft > maxSimLeft )
				{
					maxSimLeft = simLeft;
					maxSimLeftFrame = framesChecked;
				}
				if ( simLeft < minSimLeft )
				{
					minSimLeft = simLeft;
					minSimLeftFrame = framesChecked;
				}

				averageSimLeft += simLeft;


				if ( simRight > maxSimRight )
				{
					maxSimRight = simRight;
					maxSimRightFrame = framesChecked;
				}
				if ( simRight < minSimRight )
				{
					minSimRight = simRight;
					minSimRightFrame = framesChecked;
				}

				averageSimRight += simRight;
				


				CPPUNIT_ASSERT
					(  simLeft >= 0.9 );

				CPPUNIT_ASSERT
					( simRight >= 0.9 );

			}

			procReader.Stop();
			procReader2.Stop();		

			averageSimLeft *= (1.0/double(framesChecked));
			averageSimRight *= (1.0/double(framesChecked));
			
			/*
			std::cout << std::endl;
			std::cout << "Mp3 - AvgVBR" << std::endl;
			std::cout << "Maximum Left similarity: " << maxSimLeft << " at " << maxSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Minimum Left similarity: " << minSimLeft << " at " << minSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Average Left similarity: " << averageSimLeft  << std::endl;

			std::cout << std::endl;
			std::cout << "Maximum Right similarity: " << maxSimRight << " at " << maxSimRightFrame;
			std::cout << std::endl;
			std::cout << "Minimum Right similarity: " << minSimRight << " at " << minSimRightFrame;
			std::cout << std::endl;
			std::cout << "Average Right similarity: " << averageSimRight  << std::endl;
			*/

			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );
			

		}

	};
}
