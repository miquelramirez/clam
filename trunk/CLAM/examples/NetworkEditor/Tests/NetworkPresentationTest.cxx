
#include <cppunit/extensions/HelperMacros.h>
#include "BaseLoggable.hxx" // also includes <sstream>
#include "NetworkController.hxx"
#include "Qt_NetworkPresentation.hxx"
#include "Network.hxx"
#include "PushFlowControl.hxx"

#include "Oscillator.hxx"
#include "AudioMultiplier.hxx"

#include <iostream> // TODO: remove

namespace CLAMTest 
{

class NetworkPresentationTest;
CPPUNIT_TEST_SUITE_REGISTRATION( NetworkPresentationTest );


class NetworkPresentationTest : public CppUnit::TestFixture, public NetworkGUI::Qt_NetworkPresentation
{
	CPPUNIT_TEST_SUITE( NetworkPresentationTest );

	CPPUNIT_TEST( testNetworkPresentationIsCreatedCorrectly_afterAttached );
	CPPUNIT_TEST( testAddProcessingRegisterProcessingInNetwork );
	CPPUNIT_TEST( testRemoveProcessingDeletesProcessing );
	CPPUNIT_TEST( testClearDeletesProcessings );
	CPPUNIT_TEST( testRemoveProcessingDeletesPresentationAndController );
	CPPUNIT_TEST( testClearDeletesPresentationsAndControllers );
//	CPPUNIT_TEST( testRemoveProcessingDeletesItsConnections );
	
	CPPUNIT_TEST_SUITE_END();

	CLAM::Network mNetwork;
	CLAMVM::NetworkController mController;
public:
	NetworkPresentationTest()
	{
	}
	void setUp()
	{
		mNetwork.AddFlowControl( new CLAM::PushFlowControl );
	}
	void tearDown()
	 {
		 Clear();
	 }

	
	void testNetworkPresentationIsCreatedCorrectly_afterAttached()
	{
		mNetwork.AddProcessing( "osc", new CLAM::Oscillator );
		mNetwork.AddProcessing( "osc2", new CLAM::Oscillator );
		mNetwork.AddProcessing( "multiplier", new CLAM::AudioMultiplier );

		mController.BindTo( mNetwork );
		AttachTo( mController );

		CPPUNIT_ASSERT_EQUAL( 3, (int)mProcessingPresentations.size() );
	}
	
	void testAddProcessingRegisterProcessingInNetwork()
	{
		mNetwork.AddProcessing( "osc", new CLAM::Oscillator );

		mController.BindTo( mNetwork );
		AttachTo( mController );

		CPPUNIT_ASSERT_EQUAL( 1, (int)mProcessingPresentations.size() );

		SignalAddProcessing.Emit( "osc2", new CLAM::Oscillator );

		CLAM::Network::ProcessingsMap::const_iterator it;
		int size = 0;
		for( it=mNetwork.BeginProcessings(); it!=mNetwork.EndProcessings(); it++ )
			size++;
		
		CPPUNIT_ASSERT_EQUAL( 2, (int)mProcessingPresentations.size() );
		CPPUNIT_ASSERT_EQUAL( 2, size) ;
		CPPUNIT_ASSERT_EQUAL( true, mNetwork.HasProcessing( "osc" ) );
		CPPUNIT_ASSERT_EQUAL( true, mNetwork.HasProcessing( "osc2" ) );
	}
	
	void testRemoveProcessingDeletesProcessing()
	{
		mNetwork.AddProcessing( "osc", new CLAM::Oscillator );
		mNetwork.AddProcessing( "multiplier", new CLAM::AudioMultiplier );

		mController.BindTo( mNetwork );
		AttachTo( mController );
		
		SignalRemoveProcessing.Emit( "osc" );
		
		CLAM::Network::ProcessingsMap::const_iterator it;
		int size = 0;
		for( it=mNetwork.BeginProcessings(); it!=mNetwork.EndProcessings(); it++ )
			size++;
		CPPUNIT_ASSERT_EQUAL( 1, size) ;

	}
	
	void testClearDeletesProcessings()
	{
		mNetwork.AddProcessing( "osc", new CLAM::Oscillator );
		mNetwork.AddProcessing( "multiplier", new CLAM::AudioMultiplier );
	
		mController.BindTo( mNetwork );
		AttachTo( mController );
		
		SignalAddProcessing.Emit( "osc2", new CLAM::Oscillator );
		SignalAddProcessing.Emit("multiplier2", new CLAM::AudioMultiplier );


		CPPUNIT_ASSERT_EQUAL( 4, (int)mProcessingPresentations.size() ) ;
		
		SignalClear.Emit();
		
		CLAM::Network::ProcessingsMap::const_iterator it;
		int size = 0;
		for( it=mNetwork.BeginProcessings(); it!=mNetwork.EndProcessings(); it++ )
			size++;
		
		CPPUNIT_ASSERT_EQUAL( 0, size) ;

	}
	
	void testRemoveProcessingDeletesPresentationAndController()
	{
		mNetwork.AddProcessing( "osc", new CLAM::Oscillator );
		mNetwork.AddProcessing( "multiplier", new CLAM::AudioMultiplier );

		mController.BindTo( mNetwork );
		AttachTo( mController );
		
		CPPUNIT_ASSERT_EQUAL( 2 , (int)mProcessingPresentations.size() );

		SignalRemoveProcessing.Emit( "osc" );
		
		CLAMVM::NetworkController::ProcessingControllersMapIterator it;
		int size = 0;
		for( it=mController.BeginProcessingControllers(); it!=mController.EndProcessingControllers(); it++ )
			size++;
		CPPUNIT_ASSERT_EQUAL( 1, (int)mProcessingPresentations.size() ) ;
		CPPUNIT_ASSERT_EQUAL( 1, size );
	}
	
	void testClearDeletesPresentationsAndControllers()
	{	
		mNetwork.AddProcessing( "osc", new CLAM::Oscillator );
		mNetwork.AddProcessing( "multiplier", new CLAM::AudioMultiplier );
	
		mController.BindTo( mNetwork );
		AttachTo( mController );
		
		SignalAddProcessing.Emit( "osc2", new CLAM::Oscillator );
		SignalAddProcessing.Emit("multiplier2", new CLAM::AudioMultiplier );

		CPPUNIT_ASSERT_EQUAL( 4 , (int)mProcessingPresentations.size() );

		SignalClear.Emit();
		
		CLAMVM::NetworkController::ProcessingControllersMapIterator it;
		int size = 0;
		for( it=mController.BeginProcessingControllers(); it!=mController.EndProcessingControllers(); it++ )
			size++;

		CPPUNIT_ASSERT_EQUAL( 0, size) ;
		CPPUNIT_ASSERT_EQUAL( 0 , (int)mProcessingPresentations.size() );

	}
	
};

} // namespace CLAMTest
