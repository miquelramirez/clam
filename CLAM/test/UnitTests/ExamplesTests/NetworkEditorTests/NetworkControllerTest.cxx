/*
* Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
*                         UNIVERSITAT POMPEU FABRA
*
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
*/

#include <cppunit/extensions/HelperMacros.h>
#include "BaseLoggable.hxx" // also includes <sstream>
#include "NetworkPresentation.hxx"
#include "ProcessingPresentation.hxx"
#include "NetworkController.hxx"
#include "SimpleOscillator.hxx"
#include "Processing.hxx"
#include "AudioMultiplier.hxx"
#include "AutoPanner.hxx"
#include "PushFlowControl.hxx"
#include "Network.hxx"

#include <string>

namespace CLAMTest 
{

class NetworkControllerTest;
CPPUNIT_TEST_SUITE_REGISTRATION( NetworkControllerTest );


class NetworkControllerTest : public CppUnit::TestFixture, public NetworkGUI::NetworkPresentation
{
public:
	virtual void Show()
	{
	}
	
	virtual void Hide()
	{
	}
	void setUp()
	{
		char* pathToTestData = getenv("CLAM_TEST_DATA");
		if ( !pathToTestData )
			mPathToTestData ="../../../../../CLAM-TestData"; 
		else
			mPathToTestData = pathToTestData;
		mPathToTestData += "/networkTestsData/";
	}

protected:		
	std::string mPathToTestData;
	bool mSignalEmitted;
	void SetName(const std::string& name)
	{
	}
	
	void CreateProcessingPresentation( const std::string &, CLAMVM::ProcessingController * )
	{
		mSignalEmitted = true;
	}
	
	void CreatePortConnectionPresentation( const std::string & ,const std::string & )
	{	
		mSignalEmitted = true;
	}
	
	void CreateControlConnectionPresentation( const std::string & ,const std::string & )
	{	
		mSignalEmitted = true;
	}
	
	CPPUNIT_TEST_SUITE( NetworkControllerTest );
	
	CPPUNIT_TEST( testPublishThrowsAssert_whenControllerNotBinded );
	CPPUNIT_TEST( testBindToProcessingAttachesControllerToNetwork );

	CPPUNIT_TEST( testSlotChangeState_whenCorrectChange );
	CPPUNIT_TEST( testSlotChangeState_whenTheActualStateIsTheSame );
	
	CPPUNIT_TEST( testRemovePortConnectionNotifiesActionToNetwork );
	CPPUNIT_TEST( testRemovePortConnectionDeletesPortAdapterAndSendsSignalToPresentation );
	CPPUNIT_TEST( testCreatePortConnectionNotifiesActionToNetwork );
	CPPUNIT_TEST( testCreatePortConnectionCreatesControlAdapterAndSendsSignalToPresentation );
	CPPUNIT_TEST( testCreatePortConnection_whenNetworkIsRunning );
	
	CPPUNIT_TEST( testRemoveControlConnectionNotifiesActionToNetwork );
	CPPUNIT_TEST( testRemoveControlConnectionDeletesControlAdapterAndSendsSignalToPresentation );
	CPPUNIT_TEST( testCreateControlConnectionNotifiesActionToNetwork );
	CPPUNIT_TEST( testCreateControlConnectionCreatesControlAdapterAndSendsSignalToPresentation );
	CPPUNIT_TEST( testCreateControlConnection_whenNetworkIsRunning );
	
	CPPUNIT_TEST( testAddProcessingRegistersTheProcessingInNetwork );
	CPPUNIT_TEST( testAddProcessingCreatesProcessingControllerAndSendsSignalToPresentation );

	CPPUNIT_TEST( testConfigureProcessingNotifiesActionToNetwork );
	CPPUNIT_TEST( testRemoveProcessingDeletesProcessingController );
	CPPUNIT_TEST( testRemoveProcessingNotifiesActionToNetwork );


	CPPUNIT_TEST( testProcessingNameChangedNotifiesActionToNetwork );
	CPPUNIT_TEST( testRemoveAllConnectionNotifiesActionToNetwork );

	CPPUNIT_TEST( testLoadNetworkNotifiesActionToNetwork );
	CPPUNIT_TEST( testLoadAndStartNetwork_whenAnotherNetworkIsLoadedButStopped );
	CPPUNIT_TEST( testLoadAndStartNetwork_whenAnotherNetworkIsLoadedAndRunning );

	CPPUNIT_TEST( testSaveNetworkNotifiesActionToNetwork );
	CPPUNIT_TEST( testClearNotifiesActionToNetwork );
	CPPUNIT_TEST( testClearDeletesAllControllers );
	
	CPPUNIT_TEST_SUITE_END();

public:	
	void testPublishThrowsAssert_whenControllerNotBinded()
	{	
		CLAMVM::NetworkController proc;
		try
		{
			proc.Publish();
			CPPUNIT_FAIL(  "Assert expected, but no exception was thrown" );
		}
		catch(...)
		{
		}

	}
	
	void testBindToProcessingAttachesControllerToNetwork()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );
		network.AddProcessing( "multiplier" , new CLAM::AudioMultiplier );
		controller.BindTo( network );		
		try
		{
			controller.Publish();
		}
		catch(...)
		{
			CPPUNIT_FAIL( "exception should not be thrown, processing not binded correctly to controller" );
		}
		
		CLAMVM::NetworkController::ProcessingControllersMap::iterator it;
		it = controller.BeginProcessingControllers();		
		CPPUNIT_ASSERT_EQUAL( std::string("multiplier"), it->first );
		it++;
		CPPUNIT_ASSERT_EQUAL(  std::string("osc"), it->first );
		it++;
		CPPUNIT_ASSERT_MESSAGE( "Can't be more than two processings in the test", 
				(it==controller.EndProcessingControllers() ));
	}

	void testSlotChangeState_whenCorrectChange()
	{
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		network.AddProcessing( "osc" , oscillator );
		network.AddProcessing( "multiplier" , new CLAM::AudioMultiplier );
		controller.BindTo( network );
		controller.Publish();		
		SignalChangeState.Connect(  controller.SlotChangeState );
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Ready, oscillator->GetExecState() );
		SignalChangeState.Emit( true );
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Running, oscillator->GetExecState() );
		SignalChangeState.Emit( false );
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Ready, oscillator->GetExecState() );
	}
	
	void testSlotChangeState_whenTheActualStateIsTheSame()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		network.AddProcessing( "osc" , oscillator );
		network.AddProcessing( "multiplier" , new CLAM::AudioMultiplier );
		controller.BindTo( network );
		controller.Publish();	
		network.Start();
		SignalChangeState.Connect(  controller.SlotChangeState );
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Running, oscillator->GetExecState() );
		SignalChangeState.Emit( true );
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Running, oscillator->GetExecState() );
		network.Stop();
		SignalChangeState.Emit( false );
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Ready, oscillator->GetExecState() );

	}
	
	void testRemovePortConnectionNotifiesActionToNetwork()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );
		network.AddProcessing( "multiplier" , new CLAM::AudioMultiplier );
		controller.BindTo( network );
		controller.Publish();
		SignalCreatePortConnection.Connect( controller.SlotCreatePortConnection );
		SignalRemovePortConnection.Connect( controller.SlotRemovePortConnection );
		SignalCreatePortConnection.Emit( "osc.Audio Output", "multiplier.First Audio Input" );
		SignalRemovePortConnection.Emit( "osc.Audio Output", "multiplier.First Audio Input" );

		CLAM::Network::NamesList list = network.GetInPortsConnectedTo( "osc.Audio Output" );
		CPPUNIT_ASSERT_MESSAGE( "Oscillator out port shouldn't have any connections", (list.begin()==list.end()) );

	}
	
	void testRemovePortConnectionDeletesPortAdapterAndSendsSignalToPresentation()
	{
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );
		network.AddProcessing( "multiplier" , new CLAM::AudioMultiplier );
		controller.BindTo( network );
		controller.Publish();
		SignalCreatePortConnection.Connect( controller.SlotCreatePortConnection );
		SignalRemovePortConnection.Connect( controller.SlotRemovePortConnection );
		controller.SignalRemoveConnectionPresentation.Connect( SlotRemoveConnectionPresentation );
		SignalCreatePortConnection.Emit( "osc.Audio Output", "multiplier.First Audio Input" );
		SignalRemovePortConnection.Emit( "osc.Audio Output", "multiplier.First Audio Input" );
		CPPUNIT_ASSERT_MESSAGE( "presentation shouldn't have any connection", mConnectionPresentations.size()==0 );
	}
	
	void testCreatePortConnectionNotifiesActionToNetwork()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );
		network.AddProcessing( "multiplier" , new CLAM::AudioMultiplier );
		controller.BindTo( network );
		controller.Publish();
		SignalCreatePortConnection.Connect( controller.SlotCreatePortConnection );
		SignalCreatePortConnection.Emit( "osc.Audio Output", "multiplier.First Audio Input" );

		CLAM::Network::NamesList list = network.GetInPortsConnectedTo( "osc.Audio Output" );
		CLAM::Network::NamesList::iterator it;
		CPPUNIT_ASSERT_EQUAL( std::string("multiplier.First Audio Input"), *(list.begin()) );

	}
	
	void testCreatePortConnectionCreatesControlAdapterAndSendsSignalToPresentation()
	{		
		mSignalEmitted = false;
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );
		network.AddProcessing( "multiplier" , new CLAM::AudioMultiplier );
		controller.BindTo( network );
		controller.Publish();
		SignalCreatePortConnection.Connect( controller.SlotCreatePortConnection );
		controller.SignalCreatePortConnectionPresentation.Connect( SlotCreatePortConnectionPresentation );
		SignalCreatePortConnection.Emit( "osc.Audio Output", "multiplier.First Audio Input" );
		CPPUNIT_ASSERT_EQUAL( true, mSignalEmitted );
	}

	void testCreatePortConnection_whenNetworkIsRunning()
	{
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );
		network.AddProcessing( "multiplier" , new CLAM::AudioMultiplier );
		controller.BindTo( network );
		controller.Publish();
		SignalChangeState.Connect(  controller.SlotChangeState );
		SignalCreatePortConnection.Connect( controller.SlotCreatePortConnection );
		SignalChangeState.Emit( true );

		SignalCreatePortConnection.Emit( "osc.Audio Output", "multiplier.First Audio Input" );
		// to let the audio thread execute the events
		sleep(1);

		CLAM::Network::NamesList list = network.GetInPortsConnectedTo( "osc.Audio Output" );
		CLAM::Network::NamesList::iterator it;
		CPPUNIT_ASSERT_EQUAL( std::string("multiplier.First Audio Input"), *(list.begin()) );
	}

	void testRemoveControlConnectionNotifiesActionToNetwork()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );
		network.AddProcessing( "panner" , new CLAM::AutoPanner );
		controller.BindTo( network );
		controller.Publish();
		SignalCreateControlConnection.Connect( controller.SlotCreateControlConnection );
		SignalRemoveControlConnection.Connect( controller.SlotRemoveControlConnection );
		SignalCreateControlConnection.Emit( "panner.Left Control", "osc.Pitch" );
		SignalRemoveControlConnection.Emit( "panner.Left Control", "osc.Pitch" );

		CLAM::Network::NamesList list = network.GetInControlsConnectedTo( "panner.Left Control" );
		CPPUNIT_ASSERT_MESSAGE( "AutoPanner out control shouldn't have any connections", (list.begin()==list.end()) );
	}
	
	void testRemoveControlConnectionDeletesControlAdapterAndSendsSignalToPresentation()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );	
		network.AddProcessing( "panner" , new CLAM::AutoPanner );
		controller.BindTo( network );
		controller.Publish();
		SignalCreateControlConnection.Connect( controller.SlotCreateControlConnection );
		SignalRemoveControlConnection.Connect( controller.SlotRemoveControlConnection );
		controller.SignalRemoveConnectionPresentation.Connect( SlotRemoveConnectionPresentation );
		SignalCreateControlConnection.Emit( "panner.Left Control", "osc.Pitch" );
		SignalRemoveControlConnection.Emit( "panner.Left Control", "osc.Pitch" );
		CPPUNIT_ASSERT_MESSAGE( "presentation shouldn't have any connection", mConnectionPresentations.size()==0 );
	}
	
	void testCreateControlConnectionNotifiesActionToNetwork()
	{
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );
		network.AddProcessing( "panner" , new CLAM::AutoPanner );
		controller.BindTo( network );
		controller.Publish();
		SignalCreateControlConnection.Connect( controller.SlotCreateControlConnection );
		SignalCreateControlConnection.Emit(  "panner.Left Control", "osc.Pitch" );
		CLAM::Network::NamesList list = network.GetInControlsConnectedTo(  "panner.Left Control" );
		CLAM::Network::NamesList::iterator it;
		CPPUNIT_ASSERT_EQUAL( std::string("osc.Pitch"), *(list.begin()) );
	}
	
	void testCreateControlConnectionCreatesControlAdapterAndSendsSignalToPresentation()
	{	
		mSignalEmitted = false;
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );
		network.AddProcessing( "panner" , new CLAM::AutoPanner );
		controller.BindTo( network );
		controller.Publish();
		SignalCreateControlConnection.Connect( controller.SlotCreateControlConnection );
		controller.SignalCreateControlConnectionPresentation.Connect( SlotCreateControlConnectionPresentation );
		SignalCreateControlConnection.Emit(  "panner.Left Control", "osc.Pitch" );
		CPPUNIT_ASSERT_EQUAL( true, mSignalEmitted );

	}
	
	void testCreateControlConnection_whenNetworkIsRunning()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc" , new CLAM::SimpleOscillator );
		network.AddProcessing( "panner" , new CLAM::AutoPanner );
		controller.BindTo( network );
		controller.Publish();
		SignalChangeState.Connect(  controller.SlotChangeState );
		SignalCreateControlConnection.Connect( controller.SlotCreateControlConnection );
		SignalChangeState.Emit( true );
		SignalCreateControlConnection.Emit(  "panner.Left Control", "osc.Pitch" );

		// to let the audio thread execute the events
		sleep(1);
		CLAM::Network::NamesList list = network.GetInControlsConnectedTo(  "panner.Left Control" );
		CLAM::Network::NamesList::iterator it;
		CPPUNIT_ASSERT_EQUAL( std::string("osc.Pitch"), *(list.begin()) );
	}
	

	void testAddProcessingRegistersTheProcessingInNetwork()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		controller.BindTo( network );
		controller.Publish();
		SignalAddProcessing.Connect( controller.SlotAddProcessing );
		SignalAddProcessing.Emit( "osc", new CLAM::SimpleOscillator );
		CPPUNIT_ASSERT_EQUAL( true, network.HasProcessing( "osc" ));
	}
	
	void testAddProcessingCreatesProcessingControllerAndSendsSignalToPresentation()
	{	
		mSignalEmitted = false;
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		controller.BindTo( network );
		controller.Publish();
		SignalAddProcessing.Connect( controller.SlotAddProcessing );
		controller.SignalCreateProcessingPresentation.Connect( SlotCreateProcessingPresentation );
		
		SignalAddProcessing.Emit( "osc", new CLAM::SimpleOscillator );
		CPPUNIT_ASSERT_EQUAL( true, mSignalEmitted );
	}
	
	void testConfigureProcessingNotifiesActionToNetwork()
	{
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		CLAM::SimpleOscillatorConfig cfg;
		network.AddProcessing( "osc" , oscillator );
		controller.BindTo( network );
		controller.Publish();
		
		CLAMVM::ProcessingController * procController = controller.BeginProcessingControllers()->second;
		SigSlot::Signalv1< const CLAM::ProcessingConfig & > SignalTestConfigureProcessing;
		SignalTestConfigureProcessing.Connect( procController->SlotConfigureProcessing );
		cfg.SetFrequency(880.0);
		SignalTestConfigureProcessing.Emit( cfg );

		CLAM::SimpleOscillatorConfig & newConfig = (CLAM::SimpleOscillatorConfig &)(oscillator->GetConfig());
		CPPUNIT_ASSERT_EQUAL( 880.0f, newConfig.GetFrequency() );
	}
	
	void testRemoveProcessingDeletesProcessingController()
	{
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		controller.BindTo( network );
		controller.Publish();
		SignalAddProcessing.Connect( controller.SlotAddProcessing );
		SignalRemoveProcessing.Connect( controller.SlotRemoveProcessing );
		SignalAddProcessing.Emit( "osc", new CLAM::SimpleOscillator );
		SignalRemoveProcessing.Emit( "osc" );

		CPPUNIT_ASSERT_MESSAGE( "network controller shouldn't have any processing controllers",
				controller.BeginProcessingControllers()==controller.EndProcessingControllers() );
		
	}
	
	void testRemoveProcessingNotifiesActionToNetwork()
	{		
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc", new CLAM::SimpleOscillator );
		controller.BindTo( network );
		controller.Publish();
		SignalRemoveProcessing.Connect( controller.SlotRemoveProcessing );
		SignalRemoveProcessing.Emit( "osc" );

		CPPUNIT_ASSERT_EQUAL( false, network.HasProcessing("osc"));
	}
	
	
	void testProcessingNameChangedNotifiesActionToNetwork()
	{
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc", new CLAM::SimpleOscillator );
		controller.BindTo( network );
		controller.Publish();
		
		CLAMVM::ProcessingController * procController = controller.BeginProcessingControllers()->second;
		procController->SignalProcessingNameChanged.Emit( "generator", procController );
		CPPUNIT_ASSERT_EQUAL( false, network.HasProcessing("osc") );
		CPPUNIT_ASSERT_EQUAL( true, network.HasProcessing("generator") );
	}
	
	void testRemoveAllConnectionNotifiesActionToNetwork()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc", new CLAM::SimpleOscillator );
		network.AddProcessing( "panner" , new CLAM::AutoPanner );
		network.AddProcessing( "multiplier" , new CLAM::AudioMultiplier );		
		network.ConnectPorts( "osc.Audio Output", "multiplier.First Audio Input" );
		network.ConnectControls( "panner.Right Control", "osc.Amplitude" );
		controller.BindTo( network );
		controller.Publish();
		
		CLAMVM::ProcessingController * procController = controller.BeginProcessingControllers()->second;
		procController->SignalRemoveAllConnections.Emit( &(network.GetProcessing( "osc" )) );
		CLAM::Network::NamesList list = network.GetInPortsConnectedTo( "osc.Audio Output" );
		CPPUNIT_ASSERT_MESSAGE( "Oscillator out port shouldn't have any connections", (list.begin()==list.end()) );
		list = network.GetInControlsConnectedTo( "panner.Right Control" );
		CPPUNIT_ASSERT_MESSAGE( "AutoPanner out control shouldn't have any connections", (list.begin()==list.end()) );
	}
	
	
	void testLoadNetworkNotifiesActionToNetwork()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		SignalLoadNetworkFrom.Connect( controller.SlotLoadNetwork );
		controller.BindTo( network );
		controller.Publish();
		SignalLoadNetworkFrom.Emit( mPathToTestData+"networkwithoneproc.xml" );

		CPPUNIT_ASSERT_EQUAL( true, network.HasProcessing("oscillator"));
	}

	void testLoadAndStartNetwork_whenAnotherNetworkIsLoadedButStopped()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		SignalLoadNetworkFrom.Connect( controller.SlotLoadNetwork );
		controller.BindTo( network );
		controller.Publish();
		SignalLoadNetworkFrom.Emit( mPathToTestData+"networkwithoneproc.xml" );
		SignalLoadNetworkFrom.Emit( mPathToTestData+"networkwithanotherproc.xml" );
	
		CPPUNIT_ASSERT_EQUAL( false, network.HasProcessing("oscillator"));
		CPPUNIT_ASSERT_EQUAL( true, network.HasProcessing("panner"));
	}

	void testLoadAndStartNetwork_whenAnotherNetworkIsLoadedAndRunning()
	{
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		SignalLoadNetworkFrom.Connect( controller.SlotLoadNetwork );
		controller.BindTo( network );
		controller.Publish();
		SignalChangeState.Connect(  controller.SlotChangeState );

		SignalLoadNetworkFrom.Emit( mPathToTestData+"networkwithoneproc.xml" );
		SignalChangeState.Emit( true );	
		
		CLAM::Processing & proc = network.GetProcessing("oscillator");
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Running, proc.GetExecState() );

		SignalLoadNetworkFrom.Emit( mPathToTestData+"networkwithanotherproc.xml" );
		SignalChangeState.Emit( true );	
		
		CLAM::Processing & proc2 = network.GetProcessing("panner");
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Running, proc2.GetExecState() );

		SignalChangeState.Emit( false );
	}


	void testSaveNetworkNotifiesActionToNetwork()
	{
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc", new CLAM::SimpleOscillator );
		SignalSaveNetworkTo.Connect( controller.SlotSaveNetwork );
		controller.BindTo( network );
		controller.Publish();
		SignalSaveNetworkTo.Emit( mPathToTestData+"testsavenetwork.xml" );
		
		CLAM::Network network2;
		network2.AddFlowControl( new CLAM::PushFlowControl );
		CLAMVM::NetworkController controller2;
		SignalLoadNetworkFrom.Connect( controller2.SlotLoadNetwork );
		controller2.BindTo( network2 );
		controller2.Publish();
		SignalLoadNetworkFrom.Emit( mPathToTestData+"testsavenetwork.xml" );

		CPPUNIT_ASSERT_EQUAL( true, network2.HasProcessing("osc") );
	}
	
	void testClearNotifiesActionToNetwork()
	{
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc", new CLAM::SimpleOscillator );
		controller.BindTo( network );
		controller.Publish();

		SignalClear.Connect( controller.SlotClear );
		SignalClear.Emit();
		
		CPPUNIT_ASSERT_EQUAL( false, network.HasProcessing("osc"));
	}
	
	void testClearDeletesAllControllers()
	{	
		CLAM::Network network;
		CLAMVM::NetworkController controller;		
		network.AddFlowControl( new CLAM::PushFlowControl );
		network.AddProcessing( "osc", new CLAM::SimpleOscillator );
		controller.BindTo( network );
		controller.Publish();

		SignalClear.Connect( controller.SlotClear );
		SignalClear.Emit();

		CPPUNIT_ASSERT_MESSAGE( "network controller shouldn't have any processing controller", 
				controller.BeginProcessingControllers() == controller.EndProcessingControllers() );
	}
	
};

} // namespace CLAMTest
