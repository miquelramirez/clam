#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "MultiChannelAudioFileReader.hxx"
#include "MultiChannelAudioFileWriter.hxx"
#include "similarityHelper.hxx"
#include "CLAM_Math.hxx"
#include "OSDefines.hxx"

namespace CLAMTest
{
	class MultiChannelAudioFileWriterFunctionalTest;

	CPPUNIT_TEST_SUITE_REGISTRATION( MultiChannelAudioFileWriterFunctionalTest );

	class MultiChannelAudioFileWriterFunctionalTest
		: public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MultiChannelAudioFileWriterFunctionalTest );
		CPPUNIT_TEST( testConfigure_ReturnsFalse_WithJustFilename );
		CPPUNIT_TEST( testDo_PCM_WritesRightAKnownSignal );
		CPPUNIT_TEST( testDo_DoubleWriting_Is_Not_Allowed );
		CPPUNIT_TEST( testDo_PCM_WritesTheSameThatWasRead );
		CPPUNIT_TEST( testDo_OggVorbis_WritesTheSameThatWasRead );
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


			CLAM::MultiChannelAudioFileWriterConfig cfg;
			cfg.AddTargetFile();
			cfg.UpdateData();
			cfg.SetTargetFile( file );

			CLAM::MultiChannelAudioFileWriter proc;
			bool configResult = proc.Configure( cfg );

			CPPUNIT_ASSERT_EQUAL( false,
					      configResult );

		}

		void testDo_PCM_WritesRightAKnownSignal()
		{
			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "twosines-stereo.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
						
			outputFileHeader.SetValues( 44100, 2, "WAV" );

			outputFile.SetHeader( outputFileHeader );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			

			CLAM::MultiChannelAudioFileWriter procWriter;

			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			CLAM::Audio readSamplesLeft;
			readSamplesLeft.SetSize( 256 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );


			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.Start();

			int  framesSynth = 0;
			CLAM::TData srate = 44100.;
			CLAM::TData f0 = 100;
			CLAM::TData f1 = 400;

			while ( framesSynth < 100 )
			{
				for ( int i = 0; i < 256; i++ )
				{
					readSamplesLeft.GetBuffer()[i] = 0.0;
					readSamplesRight.GetBuffer()[i] = 0.0;
				}
				procWriter.Do();
				framesSynth++;
				
			}
			
			// cosinus starts at zero ( -pi/2 intial phase )
			CLAM::TData phi0 = -M_PI / 2.0;
			CLAM::TData phi1 = -M_PI / 2.0;
			CLAM::TData dphi0 = (2.0*M_PI*f0) / srate;
			CLAM::TData dphi1 = (2.0*M_PI*f1) / srate;

			while( framesSynth < 200 )
			{

				for ( int i = 0; i < 256; i++ )
				{
					readSamplesLeft.GetBuffer()[i] = 0.5 * cos( phi0 );
					readSamplesRight.GetBuffer()[i] = 0.5 * cos( phi1 );
					phi0 += dphi0;
					phi1 += dphi1;
				}

				procWriter.Do();
				framesSynth++;
			}


			procWriter.Stop();

		}

		void testDo_DoubleWriting_Is_Not_Allowed()
		{
			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "twosines-stereo.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
						
			outputFileHeader.SetValues( 44100, 2, "WAV" );

			outputFile.SetHeader( outputFileHeader );

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.AddTargetFile();
			cfgWriter.UpdateData();
			cfgWriter.SetTargetFile( outputFile );
			

			CLAM::MultiChannelAudioFileWriter procWriter;
			
			CLAM::MultiChannelAudioFileWriter procWriter2;

			
			procWriter.Configure( cfgWriter );

			CPPUNIT_ASSERT_EQUAL( false, procWriter2.Configure( cfgWriter ) );

		}

		void testDo_PCM_WritesTheSameThatWasRead()
		{
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "test-stereo-decoding.wav" ) );


			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "test-stereo-decoding-copy.wav" );			

			CLAM::AudioFileHeader outputFileHeader;
						
			outputFileHeader.AddAll();
			outputFileHeader.UpdateData();

			outputFileHeader.SetChannels( inputFile.GetHeader().GetChannels() );
			outputFileHeader.SetSampleRate( inputFile.GetHeader().GetSampleRate() );
			outputFileHeader.SetFormat( inputFile.GetHeader().GetFormat() );
			outputFileHeader.SetEncoding( inputFile.GetHeader().GetEncoding() );
			outputFileHeader.SetEndianess( inputFile.GetHeader().GetEndianess() );

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
			readSamplesLeft.SetSize( 256 );
			CLAM::Audio readSamplesRight;
			readSamplesRight.SetSize( 256 );


			procReader.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft );
			procReader.GetOutPorts().GetByNumber(1).Attach( readSamplesRight );

			procWriter.GetInPorts().GetByNumber(0).Attach( readSamplesLeft );
			procWriter.GetInPorts().GetByNumber(1).Attach( readSamplesRight );


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
			
			CLAM::MultiChannelAudioFileReader procReader2;
			inputFile.SetLocation( "test-stereo-decoding-copy.wav" );
			cfgReader.SetSourceFile( inputFile );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			CLAM::Audio readSamplesLeft2;
			readSamplesLeft2.SetSize( 256 );
			CLAM::Audio readSamplesRight2;
			readSamplesRight2.SetSize( 256 );

			procReader2.GetOutPorts().GetByNumber(0).Attach( readSamplesLeft2 );
			procReader2.GetOutPorts().GetByNumber(1).Attach( readSamplesRight2 );
			
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			while( procReader.Do() && procReader2.Do() )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( readSamplesLeft.GetBuffer(), 
								     readSamplesLeft2.GetBuffer() );

				CPPUNIT_ASSERT
					(  simLeft >= 0.9999 );

				double simRight = evaluateSimilarity( readSamplesRight.GetBuffer(),
								      readSamplesRight2.GetBuffer() );
				CPPUNIT_ASSERT
					( simRight >= 0.9999 );

			}

			procReader.Stop();
			procReader2.Stop();		

			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );
			
		}

		void testDo_OggVorbis_WritesTheSameThatWasRead()
		{
			
			CLAM::AudioFile inputFile;
			inputFile.SetLocation( mPathToTestData + std::string( "ElvisStereo.wav" ) );

			CLAM::AudioFile outputFile;
			outputFile.SetLocation( "ElvisStereo-copy.ogg" );			

			CLAM::AudioFileHeader outputFileHeader;
					
			outputFileHeader.SetValues( inputFile.GetHeader().GetSampleRate(),
						    inputFile.GetHeader().GetChannels(),
						    "VorbisMk1" );
	
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
			inputFile.SetLocation( "ElvisStereo-copy.ogg" );
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
			std::cout << "test_Do_OggVorbis" << std::endl;
			std::cout << "Maximum Left similarity: " << maxSimLeft << " at " << maxSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Minimum Left similarity: " << minSimLeft << " at " << minSimLeftFrame;
			std::cout << std::endl;
			std::cout << "Average Left similarity: " << averageSimLeft  << std::endl;

			std::cout << "Maximum Right similarity: " << maxSimRight << " at " << maxSimRightFrame;
			std::cout << std::endl;
			std::cout << "Minimum Right similarity: " << minSimRight << " at " << minSimRightFrame;
			std::cout << std::endl;
			std::cout << "Average Right similarity: " << averageSimRight  << std::endl;
			*/

			CPPUNIT_ASSERT( fabs( maxSimLeft -  0.999595) < 1e-2 );
			CPPUNIT_ASSERT( fabs( minSimLeft -  0.980736) < 1e-2 );
			CPPUNIT_ASSERT( fabs( averageSimLeft - 0.99788 ) < 1e-2 );

			CPPUNIT_ASSERT( fabs( maxSimRight - 0.999595) < 1e-2 );
			CPPUNIT_ASSERT( fabs( minSimRight - 0.980736) < 1e-2 );
			CPPUNIT_ASSERT( fabs( averageSimRight - 0.997888) < 1e-2 );

			CPPUNIT_ASSERT_EQUAL( framesRead,
					      framesChecked );
			
		}


	};
}
