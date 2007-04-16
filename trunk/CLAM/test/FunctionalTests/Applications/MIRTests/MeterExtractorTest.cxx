#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "RhythmDescriptionHelpers.hxx"
#include "TickExtractor.hxx"
#include "TickExtractorConfig.hxx"
#include "Meter.hxx"
#include "RD_MeterEstimator.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileReader.hxx"
#include "XMLStorage.hxx"

namespace CLAMTest
{
	class MeterEstimatorTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( MeterEstimatorTest );

	class MeterEstimatorTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( MeterEstimatorTest );

		CPPUNIT_TEST( test_02_alles_Ticks );
		
		CPPUNIT_TEST( test_12_cubic_Ticks );
		CPPUNIT_TEST( test_ALoCubano_Ticks );
		CPPUNIT_TEST( test_AmourEnPoudre_Ticks );
		CPPUNIT_TEST( test_Amsterdam_Ticks );
		CPPUNIT_TEST( test_blue_monday_Ticks );
		CPPUNIT_TEST( test_clicseq_Ticks );
		CPPUNIT_TEST( test_drums_electronic_guit_Ticks );
		CPPUNIT_TEST( test_elecpiano_drums_Ticks );
		CPPUNIT_TEST( test_test1_Ticks );
		CPPUNIT_TEST( test_test2_Ticks );
		CPPUNIT_TEST( test_test_Ticks );


		CPPUNIT_TEST_SUITE_END();

	protected:

		std::string                  mPathToTests;
		std::string                  mOnsetDetectionAlgorithm;
		CLAM::RhythmDescription::MeterEstimatorConfig  mMeterEstCfg;

		void LoadAudio( const std::string& audioFileName, CLAM::Audio& audio )
		{
			CLAM::AudioFile file;
			file.SetLocation( audioFileName );
			
			if ( !file.IsReadable() )
			{
				throw CLAM::Err( "Could not open file!" );
			}

			CLAM::MonoAudioFileReaderConfig cfg;
			cfg.SetSourceFile( file );
			CLAM::MonoAudioFileReader reader;
			reader.Configure( cfg );
			
			CLAM::TSize fileSize = file.GetHeader().GetSamples();
			
			audio.SetSize(fileSize);
			audio.SetSampleRate(file.GetHeader().GetSampleRate());
			
			//Read Audio File
			reader.Start();		
			reader.Do(audio);
			reader.Stop();
		}



	public:

		void setUp()
		{
			mPathToTests = "../../../../../CLAM-TestData/RhythmDescription";
			mOnsetDetectionAlgorithm = "MTG";
			mMeterEstCfg.SetTempoLimInf( 50 );
			mMeterEstCfg.SetTempoLimSup( 200 );
			mMeterEstCfg.SetAutomaticIntegTime( true );
			mMeterEstCfg.SetACFIntegrationTime( 40 );
		}

		void tearDown()
		{
		}

	private:
		
		void test_02_alles_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/02_alles_Meter.xml" );
			
			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/02_alles.wav", cfg, extractedEvents, extractedBeats );

			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/02_alles.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing 02_alles.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();

			CLAM::XMLStorage::Dump( result, "Meter", "02_alles_Meter.xml");
			
			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );

		}

		void test_12_cubic_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/12_cubic_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod(mOnsetDetectionAlgorithm) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/12_cubic.wav", cfg, extractedEvents, extractedBeats );

			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/12_cubic.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing 12_cubic.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();

			CLAM::XMLStorage::Dump( result, "Meter", "12_cubic_Meter.xml");
			
			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );

		}

		void test_blue_monday_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/blue_monday_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod(mOnsetDetectionAlgorithm) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/blue_monday.wav", cfg, extractedEvents, extractedBeats );
			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/blue_monday.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing blue_monday.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();

			CLAM::XMLStorage::Dump( result, "Meter", "blue_monday_Meter.xml");
			
			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );


		}

		void test_ALoCubano_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/ALoCubano_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/ALoCubano.wav", cfg, extractedEvents, extractedBeats );

			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/ALoCubano.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing ALoCubano.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();
			
			CLAM::XMLStorage::Dump( result, "Meter", "ALoCubano_Meter.xml");

			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );


		}

		void test_AmourEnPoudre_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/AmourEnPoudre_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/AmourEnPoudre.wav", cfg, extractedEvents, extractedBeats );


			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/AmourEnPoudre.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing AmourEnPoudre.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();
			
			CLAM::XMLStorage::Dump( result, "Meter", "AmourEnPoudre_Meter.xml");

			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );

		}

		void test_Amsterdam_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/Amsterdam_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/Amsterdam.wav", cfg, extractedEvents, extractedBeats );


			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/Amsterdam.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing Amsterdam.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();
			
			CLAM::XMLStorage::Dump( result, "Meter", "Amsterdam_Meter.xml");

			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );

		}

		void test_clicseq_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/clicseq_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;
			
			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/clicseq.wav", cfg, extractedEvents, extractedBeats );


			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/clicseq.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing clicseq.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();

			CLAM::XMLStorage::Dump( result, "Meter", "clicseq_Meter.xml");
			
			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );

		}

		void test_drums_electronic_guit_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/drums-electronic-guit_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/drums-electronic-guit.wav", cfg, extractedEvents, extractedBeats );

			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/drums-electronic-guit.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing drums-electronic-guit.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();

			CLAM::XMLStorage::Dump( result, "Meter", "drums-electronic-guit_Meter.xml");
			
			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );


		}

		void test_elecpiano_drums_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/elecpiano-drums_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/elecpiano-drums.wav", cfg, extractedEvents, extractedBeats );

			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/elecpiano-drums.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing elecpiano-drums.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();
			
			CLAM::XMLStorage::Dump( result, "Meter", "elecpiano-drums_Meter.xml");

			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );


		}

		void test_test1_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/test1_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/test1.wav", cfg, extractedEvents, extractedBeats );


			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/test1.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing test1.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();
			
			CLAM::XMLStorage::Dump( result, "Meter", "test1_Meter.xml");

			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );

		}

		void test_test2_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/test2_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/test2.wav", cfg, extractedEvents, extractedBeats );


			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/test2.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing test2.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();
			
			CLAM::XMLStorage::Dump( result, "Meter", "test2_Meter.xml");

			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );

		}

		void test_test_Ticks()
		{
			CLAM::RhythmDescription::Meter inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Meter/test_Meter.xml" );

			CLAM::Pulse extractedEvents;
			CLAM::Pulse extractedBeats;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/Data/test.wav", cfg, extractedEvents, extractedBeats );

			CLAM::Audio signal;
			LoadAudio( mPathToTests + "/Data/test.wav", signal );

			CLAM::RhythmDescription::Meter        result;
			CLAM::RhythmDescription::MeterEstimator meterEstimator;
			meterEstimator.Configure( mMeterEstCfg );
			meterEstimator.Start();
			meterEstimator.Log() << "Processing test.wav" << std::endl;
			meterEstimator.Do( signal, extractedBeats, result );
			meterEstimator.Stop();

			CLAM::XMLStorage::Dump( result, "Meter", "test_Meter.xml");
			
			CPPUNIT_ASSERT_EQUAL( inst.GetNumerator(), result.GetNumerator() );
			CPPUNIT_ASSERT_EQUAL( inst.GetDenominator(), result.GetDenominator() );

		}


	};
}

