#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "RhythmDescriptionHelpers.hxx"
#include "XMLStorage.hxx"
#include "OnsetDetector.hxx"
#include "Normalization.hxx"
#include "AudioFile.hxx"
#include "MonoAudioFileReader.hxx"
#include "AubioOnsetDetector.hxx"

namespace CLAMTest
{
	class TestOnsetDetector;

	CPPUNIT_TEST_SUITE_REGISTRATION( TestOnsetDetector );

	class TestOnsetDetector : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( TestOnsetDetector );

		CPPUNIT_TEST( test_02_alles_Onsets );
		CPPUNIT_TEST( test_12_cubic_Onsets );
		CPPUNIT_TEST( test_ALoCubano_Onsets );
		CPPUNIT_TEST( test_AmourEnPoudre_Onsets );
		CPPUNIT_TEST( test_Amsterdam_Onsets );
		CPPUNIT_TEST( test_blue_monday_Onsets );
		CPPUNIT_TEST( test_clicseq_Onsets );
		CPPUNIT_TEST( test_drums_electronic_guit_Onsets );
		CPPUNIT_TEST( test_elecpiano_drums_Onsets );
		CPPUNIT_TEST( test_test1_Onsets );
		CPPUNIT_TEST( test_test2_Onsets );
		CPPUNIT_TEST( test_test_Onsets );

		CPPUNIT_TEST_SUITE_END();

	protected:
		std::string mPathToTests;

		CLAM::TData mAudioSampleRate;

	public:

		void setUp()
		{
			mPathToTests = "../../../../../CLAM-TestData/RhythmDescription";
		}

		void tearDown()
		{
		}

		void LoadAudio( CLAM::Audio& audio, const std::string& audioFileName )
		{
			CLAM::AudioFile file;
			file.OpenExisting( audioFileName );
			
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
			mAudioSampleRate = audio.GetSampleRate();
		}

		void ExtractOnsetsSequence( std::string filename,
					    CLAM::Array< CLAM::TimeIndex >& transients )
		{
			CLAM::Audio readAudio;
			
			LoadAudio( readAudio, filename );
			
			CLAM::TData sampleRate = readAudio.GetSampleRate();
			float duration=readAudio.GetSize()/sampleRate;

			/*
			CLAM::RhythmDescription::AubioOnsetDetectorConfig odCfg;

			CLAM::RhythmDescription::AubioOnsetDetector onsetDetector;

			odCfg.SetMethod( CLAM::RhythmDescription::EODAlgorithms::ePhase );
			onsetDetector.Configure( odCfg );

			onsetDetector.Start();

			onsetDetector.Do( readAudio, transients );

			onsetDetector.Stop();
			*/

			
			CLAM::Segment seg;
			seg.AddAudio();
			seg.UpdateData();
			seg.SetHoldsData(true);
			seg.SetEndTime(duration);

			seg.GetAudio().SetSize( readAudio.GetSize() );
			seg.GetAudio().SetSampleRate( readAudio.GetSampleRate() );

			CLAM::NormalizationConfig ncfg;
			ncfg.SetType( 3 ); // scaling factor computed from "dominant energy"

			CLAM::Normalization audioNorm;

			audioNorm.Configure( ncfg );

			audioNorm.Start();

			audioNorm.Do( readAudio, seg.GetAudio() );

			audioNorm.Stop();
			
			CLAM::OnsetDetectorConfig onsetconfig;
			CLAM::OnsetDetector onset(onsetconfig);

			onsetconfig.SetComputeOffsets(false);
			onset.Configure(onsetconfig);
			onset.Start();
			onset.Do(seg, transients);
			
			transients.Resize(transients.Size()+1);
			transients.SetSize(transients.Size()+1);
			
			for(int i=transients.Size()-1;i>0;i--)
			{
				transients[i].SetPosition(transients[i-1].GetPosition()*sampleRate);
				transients[i].SetWeight(transients[i-1].GetWeight());
			}
			
			transients[0].SetPosition(0.0);
			transients[0].SetWeight(0.0);

		}
					    
					    

	private:

				
		void test_02_alles_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/02_alles_Onsets.xml" );
			
			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/02_alles_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;

			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "02_alles_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "02_alles_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check 02_alles_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check 02_alles_Onsets_diff.xml",
					       diff.GetMaxWeightDeviation() <= 0.5 );


		}

		void test_12_cubic_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/12_cubic_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE(	"Error loading back to back test data: problem with RhythmDescription/Onsets/12_cubic_Onsets.xml",
						 inst.IsValid() == true );

			CLAM::Array< CLAM::TimeIndex > extractedEvents;

			
			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "12_cubic_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "12_cubic_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check 12_cubic_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check 12_cubic_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );



		}

		void test_blue_monday_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/blue_monday_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/blue_monday_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;


			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "blue_monday_Onsets.xml" );
			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "blue_monday_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check blue_monday_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check blue_monday_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );


		}

		void test_ALoCubano_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/ALoCubano_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/ALoCubano_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;


			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );


			CLAM::XMLStorage::Dump( res, "DetectedTransients", "ALoCubano_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "ALoCubano_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check ALoCubano_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check ALoCubano_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );


		}

		void test_AmourEnPoudre_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/AmourEnPoudre_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/AmourEnPoudre_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;


			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "AmourEnPoudre_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "AmourEnPoudre_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check AmourEnPoudre_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check AmourEnPoudre_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

		}

		void test_Amsterdam_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/Amsterdam_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/Amsterdam_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;


			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "Amsterdam_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "Amsterdam_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check Amsterdam_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check Amsterdam_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );


		}

		void test_clicseq_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/clicseq_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/clicseq_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;


			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "clicseq_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "clicseq_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check clicseq_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check clicseq_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );


		}

		void test_drums_electronic_guit_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/drums-electronic-guit_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/drums-electronic-guit_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;


			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "drums-electronic-guit_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "drums-electronic-guit_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check drums-electronic-guit_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check drums-electronic-guit_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

		}

		void test_elecpiano_drums_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/elecpiano-drums_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/elecpiano-drums_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;


			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "elecpiano-drums_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "elecpiano-drums_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check elecpiano-drums_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check elecpiano-drums_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );


			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );


		}

		void test_test1_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/test1_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/test1_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;


			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "test1_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "test1_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check test1_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check test1_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );





		}

		void test_test2_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/test2_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/test2_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;


			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "test2_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "test2_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check test2_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check test2_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );




		}

		void test_test_Onsets()
		{
			RhythmDescriptionTests::TransientEventList inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Onsets/test_Onsets.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Onsets/test_Onsets.xml",
						inst.IsValid() == true);

			CLAM::Array< CLAM::TimeIndex > extractedEvents;


			ExtractOnsetsSequence( mPathToTests + "/" + inst.GetRelativePath(), extractedEvents);

			RhythmDescriptionTests::TransientEventList res;
			
			res.AddAll();
			res.UpdateData();

			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "DetectedTransients", "test_Onsets.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents(), extractedEvents );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "test_Onsets_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().Size(), extractedEvents.Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check test_Onsets_diff.xml!",
					       diff.GetMaxTimePositionDeviation()/mAudioSampleRate <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check test_Onsets_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );





		}


	};
}

