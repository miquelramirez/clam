#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileWriterConfig.hxx"
#include "MonoAudioFileReader.hxx"
#include "MonoAudioFileWriter.hxx"
#include "Audio.hxx"
#include "similarityHelper.hxx"
#include <deque>

namespace CLAMTest
{
	class MonoAudioFileWriterFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( MonoAudioFileWriterFunctionalTest );
	
	class MonoAudioFileWriterFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MonoAudioFileWriterFunctionalTest );
		
		CPPUNIT_TEST( testDo_DoubleWriting_Is_Not_Allowed );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_WithJustFilename );
		CPPUNIT_TEST( testDo_PCM_WritesTheSameItWasRead );
		CPPUNIT_TEST( testDo_OggVorbis_WritesTheSameItWasRead );

		CPPUNIT_TEST_SUITE_END();

	protected:
		
		std::string mPathToTestData;

	public:
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

	private:
		

		void testConfigure_ReturnsFalse_WithJustFilename()
		{
			CLAM::AudioFile file;
			file.SetLocation( std::string( "NewFile.wav" ) );

			CLAM::MonoAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MonoAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );
		}

		void testDo_DoubleWriting_Is_Not_Allowed()
		{
			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "twosines-stereo.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
						
			outputFileHeader.SetValues( 44100, 1, "WAV" );

			outputFile.SetHeader( outputFileHeader );

			CLAM::MonoAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			

			CLAM::MonoAudioFileWriter procWriter;
			
			CLAM::MonoAudioFileWriter procWriter2;

			
			procWriter.Configure( cfgWriter );

			CPPUNIT_ASSERT_EQUAL( false, procWriter2.Configure( cfgWriter ) );

		}



		void testDo_PCM_WritesTheSameItWasRead()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );


			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "CopyOfElvis.wav.wav" );

			CLAM::AudioFileHeader outputFileHeader;

			outputFileHeader.AddAll();
			outputFileHeader.UpdateData();

			outputFileHeader.SetChannels( inputFile.GetHeader().GetChannels() );
			outputFileHeader.SetSampleRate( inputFile.GetHeader().GetSampleRate() );
			outputFileHeader.SetFormat( inputFile.GetHeader().GetFormat() );
			outputFileHeader.SetEncoding( inputFile.GetHeader().GetEncoding() );
			outputFileHeader.SetEndianess( inputFile.GetHeader().GetEndianess() );

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

			int  frameCounter = 0;

			while( procReader.Do() )
			{
				frameCounter++;
				procWriter.Do();
			}

			procReader.Stop();
			procWriter.Stop();

			// Once written to disk, now we recover it, and 
			// check it is the same frame by frame
			
			CLAM::MonoAudioFileReader procReader2;
			inputFile.SetLocation( "CopyOfElvis.wav.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamples2;
			readSamples2.SetSize( 256 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamples2 );
		
			procReader.Start();
			procReader2.Start();

			frameCounter = 0;

			while( procReader.Do() && procReader2.Do() )
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


			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "Elvis.wav" ) );


			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "CopyOfElvis.ogg" );

			CLAM::AudioFileHeader outputFileHeader;

			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "VorbisMk1" );

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
			inputFile.SetLocation( "CopyOfElvis.ogg" );
			cfgReader.SetSourceFile( inputFile );


			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamples2;

			readSamples2.SetSize( 256 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamples2 );
		
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			double maxSim = -1e20;
			int    maxSimFrame = 0;
			double minSim = 1e20;
			int    minSimFrame = 0;
			double averageSim = 0.0;

			while( procReader.Do() && procReader2.Do() )
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
