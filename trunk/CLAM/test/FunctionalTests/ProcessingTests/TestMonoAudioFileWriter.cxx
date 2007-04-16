#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileWriterConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "Audio.hxx"
#include "similarityHelper.hxx"
#include <deque>
#include "CLAM_Math.hxx"

namespace CLAMTest
{
	class MonoAudioFileWriterFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( MonoAudioFileWriterFunctionalTest );
	
	class MonoAudioFileWriterFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MonoAudioFileWriterFunctionalTest );
		
		CPPUNIT_TEST( testConfigure_twoOnTheSameFileFail );
		CPPUNIT_TEST( testDo_PCM_WritesTheSameItWasRead );
		CPPUNIT_TEST( testDo_OggVorbis_WritesTheSameItWasRead );

		CPPUNIT_TEST_SUITE_END();

	protected:
		
		std::string mPathToTestData;

	public:
		void setUp()
		{
			mPathToTestData = GetTestDataDirectory();
		}

		void tearDown()
		{
		}

	private:
		
		void testConfigure_ReturnsFalse_WithJustFilename()
		{
			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.SetTargetFile( "NewFile.wav" );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );
		}

		void testConfigure_twoOnTheSameFileFail()
		{
			CLAM::MonoAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "twosines-stereo.wav" );			

			CLAM::MonoAudioFileWriter procWriter;
			CLAM::MonoAudioFileWriter procWriter2;
			procWriter.Configure( cfgWriter );

			CPPUNIT_ASSERT_EQUAL( false, procWriter2.Configure( cfgWriter ) );
		}

		void testDo_PCM_WritesTheSameItWasRead()
		{
			CLAM::MonoAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData + std::string( "Elvis.wav" ) );
			cfgReader.SetSelectedChannel( 0 );

			CLAM::MonoAudioFileReader procReader;
			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		

			CLAM::MonoAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "CopyOfElvis.wav.wav" );
			cfgWriter.SetSampleRate( procReader.GetAudioFile().GetHeader().GetSampleRate() );
			// TODO: Those parameters have not been moved from the header to configuration
//			cfgWriter.SetFormat( procReader.GetAudioFile().GetHeader().GetFormat() );
//			cfgWriter.SetEncoding( procReader.GetAudioFile().GetHeader().GetEncoding() );
//			cfgWriter.SetEndianess( procReader.GetAudioFile().GetHeader().GetEndianess() );
			CLAM::MonoAudioFileWriter procWriter;
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamples;
			readSamples.SetSize( 256 );

			procReader.Start();
			procWriter.Start();

			int  frameCounter = 0;

			while( procReader.Do(readSamples) )
			{
				frameCounter++;
				procWriter.Do(readSamples);
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MonoAudioFileReader procReader2;
			cfgReader.SetSourceFile( "CopyOfElvis.wav.wav" );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamples2;
			readSamples2.SetSize( 256 );

			procReader.Start();
			procReader2.Start();

			frameCounter = 0;

			while( procReader.Do(readSamples) && procReader2.Do(readSamples2) )
			{
				double sim = evaluateSimilarity( readSamples.GetBuffer(), readSamples2.GetBuffer() );

				CPPUNIT_ASSERT
					(  sim >= 0.9999 );

			}

			procReader.Stop();
			procReader2.Stop();		

		}

		void testDo_OggVorbis_WritesTheSameItWasRead()
		{
			CLAM::MonoAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData + std::string( "Elvis.wav" ) );
			cfgReader.SetSelectedChannel( 0 );
			CLAM::MonoAudioFileReader procReader;
			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );

			CLAM::MonoAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "CopyOfElvis.ogg" );
			cfgWriter.SetSampleRate( procReader.GetAudioFile().GetHeader().GetSampleRate() );
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
			cfgReader.SetSourceFile( "CopyOfElvis.ogg" );

			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamples2;

			readSamples2.SetSize( 256 );

			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			double maxSim = -1e20;
			int    maxSimFrame = 0;
			double minSim = 1e20;
			int    minSimFrame = 0;
			double averageSim = 0.0;

			while( procReader.Do(readSamples) && procReader2.Do(readSamples2) )
			{
				double sim = evaluateSimilarity( readSamples.GetBuffer(), readSamples2.GetBuffer() );

				framesChecked++;

				if ( sim > maxSim )
				{
					maxSim = sim;
					maxSimFrame = framesChecked;
				}
				if ( sim < minSim )
				{
					minSim = sim;
					minSimFrame = framesChecked;
				}

				averageSim += sim;
				
				// MRJ: Note that due to decoding/encoding approximation errors
				// correlation is not as higher as in the PCM case. Note we have
				// put it to 0.5 which is the 'parameter' value for deciding
				// trade-off between quality and bitrate.
				CPPUNIT_ASSERT(  sim >= 0.5);

			}
			
			averageSim *= (1.0/double(framesChecked));
			
			/*
			std::cout << std::endl;
			std::cout << "Maximum similarity: " << maxSim << " at " << maxSimFrame;
			std::cout << std::endl;
			std::cout << "Minimum similarity: " << minSim << " at " << minSimFrame;
			std::cout << std::endl;
			std::cout << "Average similarity: " << averageSim  << std::endl;
			*/
			CPPUNIT_ASSERT( fabs( maxSim - 1.0 ) < 1e-2 );
			CPPUNIT_ASSERT( fabs( minSim - 0.644362 ) < 1e-2 );
			CPPUNIT_ASSERT( fabs( averageSim - 0.99589 ) < 1e-2 );

			procReader.Stop();
			procReader2.Stop();		


			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );


		}


	};
}

