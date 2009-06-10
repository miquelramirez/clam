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
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"Elvis.wav" );

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
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

			CLAM::MultiChannelAudioFileReader proc;
			
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( true, configResult );
		}

		void testConfigure_ReturnsFalseWhenFileDoesNotExist()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile-false.wav" );

			CLAM::MultiChannelAudioFileReader proc;

			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false, configResult );
		}

		void  testConfigure_DefaultChannelSelection_ChannelsOK()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );

			CLAM::MultiChannelAudioFileReaderConfig cfgToBeChecked =
				dynamic_cast< const CLAM::MultiChannelAudioFileReaderConfig& > ( proc.GetConfig() );
			
			const std::vector< CLAM::TIndex >& channels = proc.GetSelectedChannels();

			CPPUNIT_ASSERT_EQUAL ( channels.size(), size_t(proc.GetHeader().GetChannels()) );

			bool allChannelsPresent = true;
			for ( unsigned i = 0; i < channels.size(); i++ )
				allChannelsPresent &= (channels[i] == i);

			CPPUNIT_ASSERT_EQUAL( true, allChannelsPresent );
			
		}

		void testConfigure_DefaultChannelSelection_PortNumberIsOK()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( 2u, proc.GetNOutPorts());
			
		}
		
		void testConfigure_ManualChannelSelection_ChannelsOK()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.AddSelectedChannels();
			cfg.UpdateData();
			cfg.GetSelectedChannels().Resize( 2 );
			cfg.GetSelectedChannels().SetSize( 2 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 0;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );

			CLAM::MultiChannelAudioFileReaderConfig cfgToBeChecked =
				dynamic_cast< const CLAM::MultiChannelAudioFileReaderConfig& > ( proc.GetConfig() );
			
			const std::vector< CLAM::TIndex >& channels = proc.GetSelectedChannels();

			CPPUNIT_ASSERT_EQUAL ( channels.size(), size_t(cfg.GetSelectedChannels().Size()) );

			bool allChannelsPresent = true;
			for (unsigned i = 0; i < channels.size(); i++ )
				allChannelsPresent &= (channels[i] == cfg.GetSelectedChannels()[i] );
			CPPUNIT_ASSERT_EQUAL( true,
					      allChannelsPresent );
		}

		void testConfigure_ManualChannelSelection_PortNumberIsOK()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;

			cfg.AddSelectedChannels();
			cfg.UpdateData();

			cfg.GetSelectedChannels().Resize( 2 );
			cfg.GetSelectedChannels().SetSize( 2 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 0;

			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( 2u, proc.GetNOutPorts());
		}

		void testConfigure_ReturnsFalse_ManualChannelSelection_TooMany()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.AddSelectedChannels();
			cfg.UpdateData();
			cfg.GetSelectedChannels().Resize( 3 );
			cfg.GetSelectedChannels().SetSize( 3 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 0;
			cfg.GetSelectedChannels()[2] = 2;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

			CLAM::MultiChannelAudioFileReader proc;

			bool cfgResult = proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( false,
					      cfgResult );			
		}

		void testConfigure_ReturnsFalse_ManualChannelSelection_InvalidChannel()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.AddSelectedChannels();
			cfg.UpdateData();
			cfg.GetSelectedChannels().Resize( 2 );
			cfg.GetSelectedChannels().SetSize( 2 );
			cfg.GetSelectedChannels()[0] = 1;
			cfg.GetSelectedChannels()[1] = 65;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

			CLAM::MultiChannelAudioFileReader proc;

			bool cfgResult = proc.Configure( cfg );


			CPPUNIT_ASSERT_EQUAL( false,
					      cfgResult );			
			
		}

		void testDo_PCM_JustOneFrame ()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
			
			std::vector<CLAM::Audio> samples(2);
			for ( int k = 0; k < 2; k++ )
				samples[k].SetSize(256);
			

			proc.Start();
			proc.Do(samples);
			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			fileSimilarity( cfg.GetSourceFile(), samples[0], samples[1],
					similarityLeft, similarityRight );
						
			CPPUNIT_ASSERT(  similarityLeft  >= 0.9999 );
			CPPUNIT_ASSERT(  similarityRight >= 0.9999 );

		}
		
		void testDo_PCM_JustTwoFrames()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

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
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
		
			std::vector<CLAM::Audio> samples(2);
			
			samples[0].SetSize( 256 );
			samples[1].SetSize( 256 );

			proc.Start();
			proc.Do(samples);
			proc.Stop();

			
			CPPUNIT_ASSERT_EQUAL( proc.GetHeader().GetSampleRate(),
					      samples[0].GetSampleRate() );
			CPPUNIT_ASSERT_EQUAL( proc.GetHeader().GetSampleRate(),
					      samples[1].GetSampleRate() );
		}

		void testDo_JustTwoFrames_BeginTimesAreOK()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"StereoTestFile.wav" );

			CLAM::MultiChannelAudioFileReader proc;

			proc.Configure( cfg );
		
			std::vector<CLAM::Audio> samples(2);
			
			samples[0].SetSize( 256 );
			samples[1].SetSize( 256 );

			proc.Start();

			proc.Do(samples);
			
			// Save Begin Time's

			CLAM::TTime firstLeftBeginTime = samples[0].GetBeginTime();
			CLAM::TTime firstRightBeginTime = samples[1].GetBeginTime();

			// call the Do again
			proc.Do(samples);

			proc.Stop();

			CLAM::TTime truthLeft = CLAM::TTime( samples[0].GetSize() ) / proc.GetHeader().GetSampleRate() * 1000;
			CLAM::TTime yieldLeft = samples[0].GetBeginTime() - firstLeftBeginTime; 

			CPPUNIT_ASSERT( fabs( truthLeft - yieldLeft ) < 1e-4 );

			CLAM::TTime truthRight = CLAM::TTime( samples[1].GetSize() ) / proc.GetHeader().GetSampleRate() * 1000; 
			CLAM::TTime yieldRight = samples[1].GetBeginTime() - firstRightBeginTime;
			
			CPPUNIT_ASSERT( fabs( truthRight - yieldRight ) < 1e-4 );

		}

		void testDo_OggVorbis_JustOneFrame ()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"ElvisStereo.ogg" );
			CLAM::MultiChannelAudioFileReader proc( cfg );
			
			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 256 );
			samples[1].SetSize( 256 );

			proc.Start();
			proc.Do(samples);
			proc.Stop();

			double similarityLeft = 0.0, similarityRight = 0.0;

			fileSimilarity( cfg.GetSourceFile(), samples[0], samples[1],
					similarityLeft, similarityRight );
						
			CPPUNIT_ASSERT(  similarityLeft  >= 0.9999 );
			CPPUNIT_ASSERT(  similarityRight >= 0.9999 );

		}
		
		void testDo_OggVorbis_JustTwoFrames()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfg;
			cfg.SetSourceFile( mPathToTestData+"ElvisStereo.ogg" );
			CLAM::MultiChannelAudioFileReader proc( cfg );
			
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
			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData+"test-stereo-decoding-128_44.mp3" );
			CLAM::MultiChannelAudioFileReader procReader(cfgReader);

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "test-mp3-128-44.wav" );
			cfgWriter.SetSampleRate( procReader.GetHeader().GetSampleRate() );
			cfgWriter.SetNChannels( procReader.GetHeader().GetChannels() );
			CLAM::MultiChannelAudioFileWriter procWriter(cfgWriter);

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
			cfgReader.SetSourceFile( "test-mp3-128-44.wav" );
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
			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData+"test-stereo-decoding-192_44.mp3" );
			CLAM::MultiChannelAudioFileReader procReader(cfgReader);

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "test-mp3-192-44.wav" );
			cfgWriter.SetSampleRate( procReader.GetHeader().GetSampleRate() );
			cfgWriter.SetNChannels( procReader.GetHeader().GetChannels() );
			CLAM::MultiChannelAudioFileWriter procWriter(cfgWriter);

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
			cfgReader.SetSourceFile( "test-mp3-192-44.wav" );
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
			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData+"test-stereo-decoding-64_44.mp3" );
			CLAM::MultiChannelAudioFileReader procReader(cfgReader);
			if (not procReader.IsConfigured())
				std::cout << "Reader: " << procReader.GetConfigErrorMessage() << std::endl;

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "test-mp3-64-44.wav" );
			cfgWriter.SetSampleRate( procReader.GetHeader().GetSampleRate() );
			cfgWriter.SetNChannels( procReader.GetHeader().GetChannels() );
			CLAM::MultiChannelAudioFileWriter procWriter(cfgWriter);
			if (not procWriter.IsConfigured())
				std::cout << "Writer: " << procWriter.GetConfigErrorMessage() << std::endl;

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
			cfgReader.SetSourceFile( "test-mp3-64-44.wav" );
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
			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData+"test-stereo-decoding-320_44.mp3" );
			CLAM::MultiChannelAudioFileReader procReader(cfgReader);

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "test-mp3-320-44.wav" );
			cfgWriter.SetSampleRate( procReader.GetHeader().GetSampleRate() );
			cfgWriter.SetNChannels( procReader.GetHeader().GetChannels() );
			CLAM::MultiChannelAudioFileWriter procWriter(cfgWriter);

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
			cfgReader.SetSourceFile( "test-mp3-320-44.wav" );
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
			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData+"test-stereo-decoding-LowVBR_44.mp3" );
			CLAM::MultiChannelAudioFileReader procReader(cfgReader);

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "test-mp3-LowVBR-44.wav" );
			cfgWriter.SetSampleRate( procReader.GetHeader().GetSampleRate() );
			cfgWriter.SetNChannels( procReader.GetHeader().GetChannels() );
			CLAM::MultiChannelAudioFileWriter procWriter(cfgWriter);

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
			cfgReader.SetSourceFile( "test-mp3-LowVBR-44.wav" );
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
			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData+"test-stereo-decoding-HighVBR_44.mp3" );
			CLAM::MultiChannelAudioFileReader procReader(cfgReader);

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "test-mp3-HighVBR-44.wav" );
			cfgWriter.SetSampleRate( procReader.GetHeader().GetSampleRate() );
			cfgWriter.SetNChannels( procReader.GetHeader().GetChannels() );
			CLAM::MultiChannelAudioFileWriter procWriter(cfgWriter);

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
			cfgReader.SetSourceFile( "test-mp3-HighVBR-44.wav" );
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
			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData+"test-stereo-decoding-AvgVBR_44.mp3" );
			CLAM::MultiChannelAudioFileReader procReader(cfgReader);

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "test-mp3-AvgVBR-44.wav" );
			cfgWriter.SetSampleRate( procReader.GetHeader().GetSampleRate() );
			cfgWriter.SetNChannels( procReader.GetHeader().GetChannels() );
			CLAM::MultiChannelAudioFileWriter procWriter(cfgWriter);

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
			cfgReader.SetSourceFile( "test-mp3-AvgVBR-44.wav" );
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

