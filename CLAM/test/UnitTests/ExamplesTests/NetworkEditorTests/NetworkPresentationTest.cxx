
#include <cppunit/extensions/HelperMacros.h>
#include "BaseLoggable.hxx" // also includes <sstream>
#include "NetworkController.hxx"
#include "Qt_NetworkPresentation.hxx"
#include "ProcessingPresentation.hxx"
#include "ConnectionPresentation.hxx"
#include "Network.hxx"
#include "PushFlowControl.hxx"

#include "Oscillator.hxx"
#include "AudioMultiplier.hxx"
#include "AutoPanner.hxx"

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
	CPPUNIT_TEST( testProcessingNameChangedCancelsModificationIfNotValidName );
	CPPUNIT_TEST( testProcessingNameChangedModifiesControlConnectionPresentationsName );
	CPPUNIT_TEST( testProcessingNameChangedModifiesPortConnectionPresentationsName );
	
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
		
		RemoveProcessing( &GetProcessingPresentation("osc") );
		
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
		
		Clear(); 
		
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
		mController.Publish();
		AttachTo( mController );
		
		CPPUNIT_ASSERT_EQUAL( 2 , (int)mProcessingPresentations.size() );

		RemoveProcessing( &GetProcessingPresentation("osc") );
		UpdatePresentations();
		
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

		Clear();
		
		CLAMVM::NetworkController::ProcessingControllersMapIterator it;
		int size = 0;
		for( it=mController.BeginProcessingControllers(); it!=mController.EndProcessingControllers(); it++ )
			size++;

		CPPUNIT_ASSERT_EQUAL( 0, size) ;
		CPPUNIT_ASSERT_EQUAL( 0 , (int)mProcessingPresentations.size() );

	}

	void testProcessingNameChangedCancelsModificationIfNotValidName()
	{
		mNetwork.AddProcessing( "original", new CLAM::Oscillator );
		mNetwork.AddProcessing( "repeatedName", new CLAM::Oscillator );

		mController.BindTo( mNetwork );
		AttachTo( mController );

		NetworkGUI::ProcessingPresentation * processingPresentation = *(mProcessingPresentations.begin());
		processingPresentation->ChangeProcessingPresentationName("repeatedName");
		processingPresentation->SignalProcessingNameChanged.Emit( "repeatedName" );
		std::string oldName("original");
		std::string newName(processingPresentation->GetName());	
		CPPUNIT_ASSERT_EQUAL( oldName, newName );
		
	}
	
	void testProcessingNameChangedModifiesPortConnectionPresentationsName()
	{
		mNetwork.AddProcessing( "oscillator", new CLAM::Oscillator );
		mNetwork.AddProcessing( "multiplier", new CLAM::AudioMultiplier );

		mController.BindTo( mNetwork );
		AttachTo( mController );

		SignalCreatePortConnection.Emit(  "oscillator.Audio Output", "multiplier.First Audio Input" );

		ProcessingPresentationIterator it = mProcessingPresentations.begin();
		for( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++ )
		{
			if( (*it)->GetName() == "oscillator")
			{
				break;
			}
		}

		(*it)->ChangeProcessingPresentationName( "mynewname" );
		(*it)->SignalProcessingNameChanged.Emit( "mynewname" );
		
		ConnectionPresentationIterator itc;
		itc = mConnectionPresentations.begin();
		std::string newName( "mynewname.Audio Output" );
		CPPUNIT_ASSERT_EQUAL( newName, (*itc)->GetOutName() );
	}
	
	void testProcessingNameChangedModifiesControlConnectionPresentationsName()
	{
		mNetwork.AddProcessing( "oscillator", new CLAM::Oscillator );
		mNetwork.AddProcessing( "panner", new CLAM::AutoPanner );

		mController.BindTo( mNetwork );
		AttachTo( mController );

		SignalCreateControlConnection.Emit( "panner.Left Control", "oscillator.Amplitude" );

		ProcessingPresentationIterator it = mProcessingPresentations.begin();
		for( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++ )
		{
			if( (*it)->GetName() == "oscillator")
			{
				break;
			}
		}

		(*it)->ChangeProcessingPresentationName( "mynewname" );
		(*it)->SignalProcessingNameChanged.Emit( "mynewname" );
		
		ConnectionPresentationIterator itc;
		itc = mConnectionPresentations.begin();
		std::string newName ("mynewname.Amplitude");
		CPPUNIT_ASSERT_EQUAL( newName, (*itc)->GetInName() );
	}
};

} // namespace CLAMTest
