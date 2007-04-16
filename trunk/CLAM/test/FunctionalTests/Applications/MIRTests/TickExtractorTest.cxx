#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "RhythmDescriptionHelpers.hxx"
#include "TickExtractor.hxx"
#include "TickExtractorConfig.hxx"
#include "XMLStorage.hxx"

namespace CLAMTest
{
	class TickExtractorTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( TickExtractorTest );

	class TickExtractorTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( TickExtractorTest );

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

		std::string mPathToTests;
		std::string mOnsetDetectionAlgorithm;
		CLAM::Pulse extractedBeats;

	public:

		void setUp()
		{
			mPathToTests = "../../../../../CLAM-TestData/RhythmDescription";
			mOnsetDetectionAlgorithm = "MTG";
		}

		void tearDown()
		{
		}

	private:
		
		void test_02_alles_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/02_alles_Ticks.xml" );
			
			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/02_alles_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );

			
			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "02_alles_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "test_Ticks_diff.xml" );


			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check test_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check test_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );
			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );

		}

		void test_12_cubic_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/12_cubic_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE(	"Error loading back to back test data: problem with RhythmDescription/Tick/12_cubic_Ticks.xml",
						 inst.IsValid() == true );

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod(mOnsetDetectionAlgorithm) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );

			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "12_cubic_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "12_cubic_Ticks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check 12_cubic_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check 12_cubic_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );

		}

		void test_blue_monday_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/blue_monday_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/blue_monday_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod(mOnsetDetectionAlgorithm) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );


			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "blue_monday_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "blue_monday_Ticks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check blue_monday_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check blue_monday_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );


		}

		void test_ALoCubano_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/ALoCubano_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/ALoCubano_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );



			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "ALoCubano_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "ALoCubano_Ticks_diff.xml" );

			RhythmDescriptionTests::TickSequence ideal;
			CLAM::XMLStorage::Restore( ideal, mPathToTests + "/Tick/ALoCubano_IdealTickSequence.xml" );
			RhythmDescriptionTests::EventListDifference diffWithIdeal;

			diffWithIdeal.SetFileTested( inst.GetRelativePath() );
			diffWithIdeal.CompareWithIdeal( ideal, extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diffWithIdeal, "EventListDifference", "ALoCubano_IdealTicks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check ALoCubano_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check ALoCubano_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );


		}

		void test_AmourEnPoudre_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/AmourEnPoudre_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/AmourEnPoudre_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );



			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "AmourEnPoudre_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "AmourEnPoudre_Ticks_diff.xml" );

			RhythmDescriptionTests::TickSequence ideal;
			CLAM::XMLStorage::Restore( ideal, mPathToTests + "/Tick/AmourEnPoudre_IdealTickSequence.xml" );
			RhythmDescriptionTests::EventListDifference diffWithIdeal;

			diffWithIdeal.SetFileTested( inst.GetRelativePath() );
			diffWithIdeal.CompareWithIdeal( ideal, extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diffWithIdeal, "EventListDifference", "AmourEnPoudre_IdealTicks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check AmourEnPoudre_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check AmourEnPoudre_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );


		}

		void test_Amsterdam_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/Amsterdam_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/Amsterdam_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );



			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "Amsterdam_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "Amsterdam_Ticks_diff.xml" );

			RhythmDescriptionTests::TickSequence ideal;
			CLAM::XMLStorage::Restore( ideal, mPathToTests + "/Tick/Amsterdam_IdealTickSequence.xml" );
			RhythmDescriptionTests::EventListDifference diffWithIdeal;

			diffWithIdeal.SetFileTested( inst.GetRelativePath() );
			diffWithIdeal.CompareWithIdeal( ideal, extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diffWithIdeal, "EventListDifference", "Amsterdam_IdealTicks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check Amsterdam_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check Amsterdam_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );


		}

		void test_clicseq_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/clicseq_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/clicseq_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );



			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "clicseq_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "clicseq_Ticks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check clicseq_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check clicseq_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );


		}

		void test_drums_electronic_guit_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/drums-electronic-guit_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/drums-electronic-guit_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );



			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "drums-electronic-guit_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "drums-electronic-guit_Ticks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check drums-electronic-guit_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check drums-electronic-guit_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );


		}

		void test_elecpiano_drums_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/elecpiano-drums_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/elecpiano-drums_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );



			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "elecpiano-drums_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "elecpiano-drums_Ticks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check elecpiano-drums_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check elecpiano-drums_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );


		}

		void test_test1_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/test1_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/test1_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );



			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "test1_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "test1_Ticks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check test1_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check test1_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );


		}

		void test_test2_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/test2_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/test2_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );



			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "test2_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "test2_Ticks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check test2_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check test2_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );


		}

		void test_test_Ticks()
		{
			RhythmDescriptionTests::RhythmEventsB2B inst;

			CLAM::XMLStorage::Restore( inst, mPathToTests + "/Tick/test_Ticks.xml" );

			CPPUNIT_ASSERT_MESSAGE( "Error loading back to back test data: problem with RhythmDescription/Tick/test_Ticks.xml",
						inst.IsValid() == true);

			CLAM::Pulse extractedEvents;

			CLAM::TickExtractorConfig cfg;
			cfg.SetOnsetDetection( CLAM::OnsetDetectMethod( mOnsetDetectionAlgorithm ) );
			CLAM::ExtractTicksSequence( mPathToTests + "/" + inst.GetRelativePath(), cfg, extractedEvents, extractedBeats );



			RhythmDescriptionTests::RhythmEventsB2B res;

			res.AddAll();
			res.UpdateData();
			
			res.SetRelativePath( inst.GetRelativePath() );
			res.SetEvents( extractedEvents );

			CLAM::XMLStorage::Dump( res, "RhythmEventsB2B", "test_Ticks.xml" );

			RhythmDescriptionTests::EventListDifference diff;

			diff.SetFileTested( inst.GetRelativePath() );
			diff.Compare( inst.GetEvents().GetIndexes(), extractedEvents.GetIndexes() );

			CLAM::XMLStorage::Dump( diff, "EventListDifference", "test_Ticks_diff.xml" );

			CPPUNIT_ASSERT_EQUAL( inst.GetEvents().GetIndexes().Size(), extractedEvents.GetIndexes().Size() );
			CPPUNIT_ASSERT_MESSAGE("Maximum time position difference above 30 ms! Check test_Ticks_diff.xml!",
					       diff.GetMaxTimePositionDeviation() <= 0.03 );
			CPPUNIT_ASSERT_MESSAGE("Maximum weight difference above 0.5! Check test_Ticks_diff.xml!",
					       diff.GetMaxWeightDeviation() <= 0.5 );

			CPPUNIT_ASSERT_EQUAL( int(inst.GetEvents().GetRate()), int(extractedEvents.GetRate()) );


		}


	};
}

