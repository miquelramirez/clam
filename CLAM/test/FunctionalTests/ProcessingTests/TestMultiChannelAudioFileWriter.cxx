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
			mPathToTestData = GetTestDataDirectory();

		}

		void tearDown()
		{
		}

	private:
		
		void testDo_PCM_WritesRightAKnownSignal()
		{
			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "twosines-stereo.wav" );
			CLAM::MultiChannelAudioFileWriter procWriter(cfgWriter);

			CPPUNIT_ASSERT_EQUAL( true, procWriter.Configure( cfgWriter ) );

			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 256 );
			samples[1].SetSize( 256 );

			procWriter.Start();

			int  framesSynth = 0;
			CLAM::TData srate = 44100.;
			CLAM::TData f0 = 100;
			CLAM::TData f1 = 400;

			while ( framesSynth < 100 )
			{
				for ( int i = 0; i < 256; i++ )
				{
					samples[0].GetBuffer()[i] = 0.0;
					samples[1].GetBuffer()[i] = 0.0;
				}
				procWriter.Do(samples);
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
					samples[0].GetBuffer()[i] = 0.5 * cos( phi0 );
					samples[1].GetBuffer()[i] = 0.5 * cos( phi1 );
					phi0 += dphi0;
					phi1 += dphi1;
				}

				procWriter.Do(samples);
				framesSynth++;
			}


			procWriter.Stop();

		}

		void testDo_DoubleWriting_Is_Not_Allowed()
		{
			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "twosines-stereo.wav" );

			CLAM::MultiChannelAudioFileWriter procWriter;
			CLAM::MultiChannelAudioFileWriter procWriter2;
			
			procWriter.Configure( cfgWriter );

			CPPUNIT_ASSERT_EQUAL( false, procWriter2.Configure( cfgWriter ) );

		}

		void testDo_PCM_WritesTheSameThatWasRead()
		{
			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData+"test-stereo-decoding.wav" );
			CLAM::MultiChannelAudioFileReader procReader(cfgReader);

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "test-stereo-decoding-copy.wav" );
			cfgWriter.SetNChannels( procReader.GetHeader().GetChannels() );
			cfgWriter.SetSampleRate( procReader.GetHeader().GetSampleRate() );
			// Not yet supported
//			cfgWriter.SetFormat( procReader.GetHeader().GetFormat() );
//			cfgWriter.SetEncoding( procReader.GetHeader().GetEncoding() );
//			cfgWriter.SetEndianess( procReader.GetHeader().GetEndianess() );

			CLAM::MultiChannelAudioFileWriter procWriter;
			CPPUNIT_ASSERT_EQUAL( true,
					      procWriter.Configure( cfgWriter ) );

			std::vector<CLAM::Audio> samples(2);
			samples[0].SetSize( 256 );
			samples[1].SetSize( 256 );


			procReader.Start();
			procWriter.Start();

			int  framesRead = 0;

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
			cfgReader.SetSourceFile( "test-stereo-decoding-copy.wav" );
			CPPUNIT_ASSERT_EQUAL( true, procReader2.Configure( cfgReader ) );

			std::vector<CLAM::Audio> samples2(2);
			samples2[0].SetSize( 256 );
			samples2[1].SetSize( 256 );

			
			procReader.Start();
			procReader2.Start();

			int framesChecked = 0;

			while( procReader.Do(samples) && procReader2.Do(samples2) )
			{
				framesChecked++;
				double simLeft = evaluateSimilarity( samples[0].GetBuffer(), 
								     samples2[0].GetBuffer() );

				CPPUNIT_ASSERT
					(  simLeft >= 0.9999 );

				double simRight = evaluateSimilarity( samples[1].GetBuffer(),
								      samples2[1].GetBuffer() );
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
			
			CLAM::MultiChannelAudioFileReaderConfig cfgReader;
			cfgReader.SetSourceFile( mPathToTestData+"ElvisStereo.wav" );
			CLAM::MultiChannelAudioFileReader procReader;
			CPPUNIT_ASSERT_EQUAL( true,
					      procReader.Configure( cfgReader ) );		

			CLAM::MultiChannelAudioFileWriterConfig cfgWriter;
			cfgWriter.SetTargetFile( "ElvisStereo-copy.ogg" );
			cfgWriter.SetSampleRate( procReader.GetHeader().GetSampleRate());
			cfgWriter.SetNChannels( procReader.GetHeader().GetChannels());
			CLAM::MultiChannelAudioFileWriter procWriter;
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
			cfgReader.SetSourceFile( "ElvisStereo-copy.ogg" );
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

