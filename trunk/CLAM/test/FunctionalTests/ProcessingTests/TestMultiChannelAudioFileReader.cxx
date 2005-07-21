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
			mPathToTestData = GetTestDataDirectory("");
		}

		void tearDown()
		{
		}

	private: // tests cases

		void testConfigure_ReturnsTrueWithJustFilename()
		{
			CLAM::AudioFile file;
			file.OpenExisting( mPathToTestData + std::string( "Elvis.wav" ) );
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
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWhenFileDoesNotExist()
		{
			CLAM::AudioFile file;
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile-false.wav" ) );
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}

		void  testConfigure_DefaultChannelSelection_ChannelsOK()
		{
			CLAM::AudioFile file;
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );
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
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );
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
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );
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
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );

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
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );
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
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );
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
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			std::vector<CLAM::Audio> samples(2);
			for ( int k = 0; k < 2; k++ )
				samples[k].SetSize(256);
			

			proc.Start();
			proc.Do(samples);
			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			fileSimilarity( file, samples[0], samples[1],
					similarityLeft, similarityRight );
						
			CPPUNIT_ASSERT(  similarityLeft  >= 0.9999 );
			CPPUNIT_ASSERT(  similarityRight >= 0.9999 );

		}
		
		void testDo_PCM_JustTwoFrames()
		{
			CLAM::AudioFile file;
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			

			std::vector<CLAM::Audio> samples(2);
			for ( int k = 0; k < 2; k++ )
				samples[k].SetSize(256);
			
			

			CLAM::DataArray prevLeftSamples;
			prevLeftSamples.Resize( 256 );
			prevLeftSamples.SetSize( 256 );

			CLAM::DataArray prevRightSamples;
			prevRightSamples.Resize( 256 );
			prevRightSamples.SetSize( 256 );

			proc.Start();

			proc.Do(samples);
			
			// Save this frame to arrays
			std::copy( samples[0].GetBuffer().GetPtr(),
				   samples[0].GetBuffer().GetPtr() 
				   + samples[0].GetBuffer().Size(),
				   prevLeftSamples.GetPtr() );
			
			std::copy( samples[1].GetBuffer().GetPtr(),
				   samples[1].GetBuffer().GetPtr()
				   + samples[1].GetBuffer().Size(),				   
				   prevRightSamples.GetPtr() );

			// call the Do again
			proc.Do(samples);

			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			similarityLeft = evaluateSimilarity( samples[0].GetBuffer(),
							     prevLeftSamples );
			similarityRight = evaluateSimilarity( samples[1].GetBuffer(),
							      prevRightSamples );
		       

			CPPUNIT_ASSERT( fabs( -0.21055 // MRJ: value established by inspection
					      - similarityLeft ) < 1e-4 );
			CPPUNIT_ASSERT( fabs( -0.150199 // MRJ: value established by inspection
					      - similarityRight ) < 1e-4 );

		}

		void testDo_JustOneFrame_SampleRateIsOK()
		{
			CLAM::AudioFile file;
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
		
			std::vector<CLAM::Audio> samples(2);
			
			samples[0].SetSize( 256 );
			samples[1].SetSize( 256 );

			proc.Start();
			proc.Do(samples);
			proc.Stop();

			
			CPPUNIT_ASSERT_EQUAL( file.GetHeader().GetSampleRate(),
					      samples[0].GetSampleRate() );
			CPPUNIT_ASSERT_EQUAL( file.GetHeader().GetSampleRate(),
					      samples[1].GetSampleRate() );
		}

		void testDo_JustTwoFrames_BeginTimesAreOK()
		{
			CLAM::AudioFile file;
			file.OpenExisting( mPathToTestData + std::string( "StereoTestFile.wav" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
		
			std::vector<CLAM::Audio> samples(2);
			
			samples[0].SetSize( 256 );

			CLAM::TTime firstLeftBeginTime = 0.0;

			samples[1].SetSize( 256 );

			CLAM::TTime firstRightBeginTime = 0.0;

			proc.Start();

			proc.Do(samples);
			
			// Save Begin Time's

			firstLeftBeginTime = samples[0].GetBeginTime();
			firstRightBeginTime = samples[1].GetBeginTime();

			// call the Do again
			proc.Do(samples);

			proc.Stop();

			CLAM::TTime truthLeft = CLAM::TTime( samples[0].GetSize() ) / file.GetHeader().GetSampleRate();
			CLAM::TTime yieldLeft = samples[0].GetBeginTime() - firstLeftBeginTime; 

			CPPUNIT_ASSERT( fabs( truthLeft - yieldLeft ) < 1e-4 );

			CLAM::TTime truthRight = CLAM::TTime( samples[1].GetSize() ) / file.GetHeader().GetSampleRate(); 
			CLAM::TTime yieldRight = samples[1].GetBeginTime() - firstRightBeginTime;
			
			CPPUNIT_ASSERT( fabs( truthRight - yieldRight ) < 1e-4 );

		}

		void testDo_OggVorbis_JustOneFrame ()
		{
			CLAM::AudioFile file;
			file.OpenExisting( mPathToTestData + std::string( "ElvisStereo.ogg" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 256 );
			samples[1].SetSize( 256 );

			proc.Start();
			proc.Do(samples);
			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			fileSimilarity( file, samples[0], samples[1],
					similarityLeft, similarityRight );
						
			CPPUNIT_ASSERT(  similarityLeft  >= 0.9999 );
			CPPUNIT_ASSERT(  similarityRight >= 0.9999 );

		}
		
		void testDo_OggVorbis_JustTwoFrames()
		{
			CLAM::AudioFile file;
			file.OpenExisting( mPathToTestData + std::string( "ElvisStereo.ogg" ) );
			
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 256 );

			CLAM::DataArray prevLeftSamples;
			prevLeftSamples.Resize( 256 );
			prevLeftSamples.SetSize( 256 );

			samples[1].SetSize( 256 );

			CLAM::DataArray prevRightSamples;
			prevRightSamples.Resize( 256 );
			prevRightSamples.SetSize( 256 );

			proc.Start();

			proc.Do(samples);
			
			// Save this frame to arrays
			std::copy( samples[0].GetBuffer().GetPtr(),
				   samples[0].GetBuffer().GetPtr() 
				   + samples[0].GetBuffer().Size(),
				   prevLeftSamples.GetPtr() );
			
			std::copy( samples[1].GetBuffer().GetPtr(),
				   samples[1].GetBuffer().GetPtr()
				   + samples[1].GetBuffer().Size(),				   
				   prevRightSamples.GetPtr() );

			// call the Do again
			proc.Do(samples);

			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			similarityLeft = evaluateSimilarity( samples[0].GetBuffer(),
							     prevLeftSamples );
			similarityRight = evaluateSimilarity( samples[1].GetBuffer(),
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

			inputFile.OpenExisting( mPathToTestData + std::string( "test-stereo-decoding-128_44.mp3" ) );
			CLAM_ASSERT( inputFile.IsReadable(), "can't find: test-stereo-decoding-128_44.mp3" );

			CLAM::AudioFile outputFile;
			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.CreateNew( "test-mp3-128-44.wav", outputFileHeader );			

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );


			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 4096 );
			samples[1].SetSize( 4096 );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do(samples) )
			{
				framesRead++;

				procWriter.Do(samples);
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.OpenExisting( "test-mp3-128-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			std::vector<CLAM::Audio> samples2(2);
			samples2[0].SetSize( 4096 );
			samples2[1].SetSize( 4096 );

			
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


			while( procReader.Do(samples) && procReader2.Do(samples2) )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( samples[0].GetBuffer(), 
								     samples2[0].GetBuffer() );

				double simRight = evaluateSimilarity( samples[1].GetBuffer(),
								      samples2[1].GetBuffer() );


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
			inputFile.OpenExisting( mPathToTestData + std::string( "test-stereo-decoding-192_44.mp3" ) );

			CLAM::AudioFile outputFile;
			CLAM::AudioFileHeader outputFileHeader;
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.CreateNew( "test-mp3-192-44.wav", outputFileHeader );			

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			std::vector<CLAM::Audio> samples(2);
			
			samples[0].SetSize( 4096 );
			samples[1].SetSize( 4096 );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do(samples) )
			{
				framesRead++;

				procWriter.Do(samples);
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.OpenExisting( "test-mp3-192-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			std::vector<CLAM::Audio> samples2(2);
			samples2[0].SetSize( 4096 );
			samples2[1].SetSize( 4096 );

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


			while( procReader.Do(samples) && procReader2.Do(samples2) )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( samples[0].GetBuffer(), 
								     samples2[0].GetBuffer() );

				double simRight = evaluateSimilarity( samples[1].GetBuffer(),
								      samples2[1].GetBuffer() );


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
			inputFile.OpenExisting( mPathToTestData + std::string( "test-stereo-decoding-64_44.mp3" ) );

			CLAM::AudioFile outputFile;
			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.CreateNew( "test-mp3-64-44.wav", outputFileHeader );			

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 4096 );
			samples[1].SetSize( 4096 );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do(samples) )
			{
				framesRead++;

				procWriter.Do(samples);
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.OpenExisting( "test-mp3-64-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			std::vector<CLAM::Audio> samples2(2);
			samples2[0].SetSize( 4096 );
			samples2[1].SetSize( 4096 );

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


			while( procReader.Do(samples) && procReader2.Do(samples2) )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( samples[0].GetBuffer(), 
								     samples2[0].GetBuffer() );

				double simRight = evaluateSimilarity( samples[1].GetBuffer(),
								      samples2[1].GetBuffer() );


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
			inputFile.OpenExisting( mPathToTestData + std::string( "test-stereo-decoding-320_44.mp3" ) );

			CLAM::AudioFile outputFile;
			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
			outputFile.CreateNew( "test-mp3-320-44.wav", outputFileHeader );			

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 4096 );
			samples[1].SetSize( 4096 );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do(samples) )
			{
				framesRead++;

				procWriter.Do(samples);
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.OpenExisting( "test-mp3-320-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			std::vector<CLAM::Audio> samples2(2);
			samples2[0].SetSize( 4096 );
			samples2[1].SetSize( 4096 );
			
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


			while( procReader.Do(samples) && procReader2.Do(samples2) )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( samples[0].GetBuffer(), 
								     samples2[0].GetBuffer() );

				double simRight = evaluateSimilarity( samples[1].GetBuffer(),
								      samples2[1].GetBuffer() );


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
			inputFile.OpenExisting( mPathToTestData + std::string( "test-stereo-decoding-LowVBR_44.mp3" ) );

			CLAM::AudioFile outputFile;
			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.CreateNew( "test-mp3-LowVBR-44.wav", outputFileHeader );			

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 4096 );
			samples[1].SetSize( 4096 );

			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do(samples) )
			{
				framesRead++;

				procWriter.Do(samples);
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.OpenExisting( "test-mp3-LowVBR-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			std::vector<CLAM::Audio> samples2(2);
			samples2[0].SetSize( 4096 );
			samples2[1].SetSize( 4096 );

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


			while( procReader.Do(samples) && procReader2.Do(samples2) )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( samples[0].GetBuffer(), 
								     samples2[0].GetBuffer() );

				double simRight = evaluateSimilarity( samples[1].GetBuffer(),
								      samples2[1].GetBuffer() );


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
			inputFile.OpenExisting( mPathToTestData + std::string( "test-stereo-decoding-HighVBR_44.mp3" ) );

			CLAM::AudioFile outputFile;
			outputFile.OpenExisting( "test-mp3-HighVBR-44.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.CreateNew( "test-mp3-HighVBR-44.wav", outputFileHeader );			

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 4096 );
			samples[1].SetSize( 4096 );

			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do(samples) )
			{
				framesRead++;

				procWriter.Do(samples);
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.OpenExisting( "test-mp3-HighVBR-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			std::vector<CLAM::Audio> samples2(2);
			samples2[0].SetSize( 4096 );
			samples2[1].SetSize( 4096 );

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


			while( procReader.Do(samples) && procReader2.Do(samples2) )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( samples[0].GetBuffer(), 
								     samples2[0].GetBuffer() );

				double simRight = evaluateSimilarity( samples[1].GetBuffer(),
								      samples2[1].GetBuffer() );


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
			inputFile.OpenExisting( mPathToTestData + std::string( "test-stereo-decoding-AvgVBR_44.mp3" ) );

			CLAM::AudioFile outputFile;
			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "WAV" );
	
			outputFile.CreateNew( "test-mp3-AvgVBR-44.wav", outputFileHeader );			

			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( inputFile );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( outputFile );
			
			CLAM::MultiChannelAudioFileReader procReader;
			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 4096 );
			samples[1].SetSize( 4096 );


			procReader.Start();
			procWriter.Start();

			CLAM::TSize framesRead = 0;

			while( procReader.Do(samples) )
			{
				framesRead++;

				procWriter.Do(samples);
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.OpenExisting( "test-mp3-AvgVBR-44.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			std::vector<CLAM::Audio> samples2(2);
			samples2[0].SetSize( 4096 );
			samples2[1].SetSize( 4096 );

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


			while( procReader.Do(samples) && procReader2.Do(samples2) )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( samples[0].GetBuffer(), 
								     samples2[0].GetBuffer() );

				double simRight = evaluateSimilarity( samples[1].GetBuffer(),
								      samples2[1].GetBuffer() );


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
