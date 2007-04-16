#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert
#include "FundamentalAdapter.hxx"
#include "FundamentalAdapterTestHelper.hxx"
#include "Segment.hxx"

namespace CLAMTest
{
	class FundamentalAdapterTest;
	
	CPPUNIT_TEST_SUITE_REGISTRATION( FundamentalAdapterTest );
	
	class FundamentalAdapterTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( FundamentalAdapterTest );
		CPPUNIT_TEST( testDefaultConstructor_AdapteeInitialization );
		CPPUNIT_TEST( testDefaultConstructor_DataCacheVoid );
		CPPUNIT_TEST( testBindToSegment_AdapteeMatch );
		CPPUNIT_TEST( testPublish_DataCacheNotVoid );
		CPPUNIT_TEST( testPublish_ConnectedToOne );
		CPPUNIT_TEST( testPublish_ConnectedToTwo );
		CPPUNIT_TEST_SUITE_END();

		static CLAM::Segment       smTestSegment;
		static bool                smSegmentLoaded;

	public:

		FundamentalAdapterTest()
		{
			if ( !smSegmentLoaded )
			{

				GenerateSegment();
			}
		}

		

		// init
		void setUp()
		{

		}

		// tearDown
		void tearDown()
		{
		}

	private:

		void GenerateSegment()
		{
			smTestSegment.SetBeginTime( 0.0 );
			smTestSegment.SetEndTime( 3.0 );
			smTestSegment.SetSamplingRate( 44100 );

			CLAM::List< CLAM::Frame >& frameList = smTestSegment.GetFramesArray();

			CLAM::Frame fakeFrame;

			for ( int i = 0; i < 150; i++ )
			{
				if ( !fakeFrame.HasFundamental() )
				{
					fakeFrame.AddFundamental();
					fakeFrame.UpdateData();
				}

				CLAM::Fundamental& currFund = fakeFrame.GetFundamental();
				
				currFund.SetnCandidates( 1 );
				currFund.GetCandidatesFreq().Resize( 1 );
				currFund.GetCandidatesFreq().SetSize( 1 );
				currFund.GetCandidatesFreq()[0] = 500.0;
				currFund.GetCandidatesErr().Resize( 1 );
				currFund.GetCandidatesErr().SetSize( 1 );
				currFund.GetCandidatesErr()[0] = 0.71;

				frameList.AddElem( fakeFrame );


			}
		}

		void testDefaultConstructor_AdapteeInitialization()
		{
			CLAMVM::FundamentalAdapter adapter;

			CPPUNIT_ASSERT_EQUAL_MESSAGE( "Observed object reference not properly initialized", 
						      (const CLAM::Segment*)NULL, adapter.Adaptee());
		}
		
		void testDefaultConstructor_DataCacheVoid()
		{
			CLAMVM::FundamentalAdapter adapter;
			
			CPPUNIT_ASSERT_EQUAL_MESSAGE( "Adapter told to have cached data, when it was supposed not to", 
						      false, adapter.HasCachedData() );
		}

		void testBindToSegment_AdapteeMatch()
		{
			CLAMVM::FundamentalAdapter adapter;
			
			adapter.BindTo( smTestSegment );

			CPPUNIT_ASSERT_EQUAL_MESSAGE( "Adapter is not bound to the expected object",
				(const CLAM::Segment*)(&smTestSegment), adapter.Adaptee() );
		}

		void testPublish_DataCacheNotVoid()
		{
			CLAMVM::FundamentalAdapter adapter;

			CPPUNIT_ASSERT_EQUAL( true, adapter.BindTo( smTestSegment ) );

			CPPUNIT_ASSERT_EQUAL( true, adapter.Publish() );

			CPPUNIT_ASSERT_EQUAL_MESSAGE( "Adapter seems to fail extract properly information",
						      true, adapter.HasCachedData() );
		}

		void testPublish_ConnectedToOne()
		{
			CLAMVM::FundamentalAdapter adapter;
			CLAMVM::FundFreqPresStub   stub;
			
			adapter.TrajectoryExtracted.Connect( stub.NewTrajectory );
			adapter.TimeSpanChanged.Connect( stub.NewTimeSpan );

			adapter.BindTo( smTestSegment );
			adapter.Publish( );

			CPPUNIT_ASSERT_EQUAL_MESSAGE( "Stub not notified!", true, stub.WasNotified() );
		}

		void testPublish_ConnectedToTwo()
		{
			CLAMVM::FundamentalAdapter adapter;
			CLAMVM::FundFreqPresStub   stub0;
			CLAMVM::FundFreqPresStub   stub1;

			adapter.TrajectoryExtracted.Connect( stub0.NewTrajectory );
			adapter.TimeSpanChanged.Connect( stub0.NewTimeSpan );
			adapter.TrajectoryExtracted.Connect( stub1.NewTrajectory );
			adapter.TimeSpanChanged.Connect( stub1.NewTimeSpan );

			adapter.BindTo( smTestSegment );
			adapter.Publish();

			int nNotified = 0;

			if ( stub0.WasNotified() ) nNotified++;
			if ( stub1.WasNotified() ) nNotified++;

			CPPUNIT_ASSERT_EQUAL_MESSAGE( "Some stub was not notified!", 2, nNotified );
		}

	};

	bool FundamentalAdapterTest::smSegmentLoaded = false;
	CLAM::Segment FundamentalAdapterTest::smTestSegment;
}

