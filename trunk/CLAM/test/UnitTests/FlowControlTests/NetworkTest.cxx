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
#include "Processing.hxx"
#include "Network.hxx"
#include "BasicFlowControl.hxx"
#include <string>
#include "BaseLoggable.hxx" // also includes <sstream>
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"
#include "InControl.hxx"
#include "OutControl.hxx"
#include "DummyProcessingData.hxx"

#include "Oscillator.hxx"
#include "AudioMultiplier.hxx"
#include "AudioFileIn.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"

namespace CLAMTest {

class NetworkTest;
CPPUNIT_TEST_SUITE_REGISTRATION( NetworkTest );


class NetworkTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( NetworkTest );
		
	CPPUNIT_TEST( testGetProcessing_WhenNoProcessings );
	CPPUNIT_TEST( testGetProcessing_WhenProcessingAdded );
	CPPUNIT_TEST( testGetProcessing_WithTwoProcessings );
	CPPUNIT_TEST(  testAddProcessing_UsingFactory );
	CPPUNIT_TEST( testHasProcessing_WhenHasIt );
	CPPUNIT_TEST( testHasProcessing_WhenHasntIt );
	CPPUNIT_TEST( testDestructor_DeletesChildrenProcessings );

	// InPort 
	CPPUNIT_TEST( testGetInPortByCompleteName_WhenPortExist );
	CPPUNIT_TEST( testGetInPortByCompleteName_WhenPortDoesntExist );
	CPPUNIT_TEST( testGetInPortByCompleteName_WhenProcessingDoesntExist );
	CPPUNIT_TEST( testGetInPortByCompleteName_WhithMalformedName_WithNoDot );
	CPPUNIT_TEST( testGetInPortByCompleteName_WithThreeIdentifiers );

	// OutPort
	CPPUNIT_TEST( testGetOutPortByCompleteName_WhenPortExist );
	CPPUNIT_TEST( testGetOutPortByCompleteName_WhenPortDoesntExist );
	CPPUNIT_TEST( testGetOutPortByCompleteName_WhenProcessingDoesntExist );
	CPPUNIT_TEST( testGetOutPortByCompleteName_WhithMalformedName_WithNoDot );
	CPPUNIT_TEST( testGetOutPortByCompleteName_WithThreeIdentifiers );

	// InControl
	CPPUNIT_TEST( testGetInControlByCompleteName_WhenControlExist );
	CPPUNIT_TEST( testGetInControlByCompleteName_WhenControlDoesntExist );
	CPPUNIT_TEST( testGetInControlByCompleteName_WhenProcessingDoesntExist );
	CPPUNIT_TEST( testGetInControlByCompleteName_WhithMalformedName_WithNoDot );
	CPPUNIT_TEST( testGetInControlByCompleteName_WithThreeIdentifiers );

	// OutControl
	CPPUNIT_TEST( testGetOutControlByCompleteName_WhenControlExist );
	CPPUNIT_TEST( testGetOutControlByCompleteName_WhenControlDoesntExist );
	CPPUNIT_TEST( testGetOutControlByCompleteName_WhenProcessingDoesntExist );
	CPPUNIT_TEST( testGetOutControlByCompleteName_WhithMalformedName_WithNoDot );
	CPPUNIT_TEST( testGetOutControlByCompleteName_WithThreeIdentifiers );

	CPPUNIT_TEST( testConnectPorts_WhenConnectionIsValid );
	CPPUNIT_TEST( testConnectPorts_WhenConnectionIsNotValid );
	CPPUNIT_TEST( testRemovePortsConnection_WhenPortsAreNotConnected );
	CPPUNIT_TEST( testRemovePortsConnection_WhenPortsAreConnected );

	CPPUNIT_TEST( testRemoveProcessing_WhenHasIt );
	CPPUNIT_TEST( testRemoveProcessing_WhenHasntIt );

	CPPUNIT_TEST( testConnectControls_WhenConnectionIsValid );
	CPPUNIT_TEST( testConnectControls_WhenConnectionIsNotValid );
	CPPUNIT_TEST( testRemoveControlsConnection_WhenControlsAreNotConnected );
	CPPUNIT_TEST( testRemoveControlsConnection_WhenControlsAreConnected );

	// network state tests
	CPPUNIT_TEST( testStartNetworkStartsProcessings_WhenAreReady );
	CPPUNIT_TEST( testStartNetworkDoesntStartProcessings_WhenAreNotReady );	
	CPPUNIT_TEST( testStopNetworkStopsProcessings_WhenAreRunning );
	CPPUNIT_TEST( testStopNetworkDoesntStopProcessings_WhenAreNotRunning );

	CPPUNIT_TEST( testUseOfString_substr );
	CPPUNIT_TEST_SUITE_END();


	void testGetProcessing_WhenNoProcessings()
	{
		CLAM::Network net;

		try{
			net.GetProcessing(std::string("not existing processing"));
			CPPUNIT_FAIL("Assert expected, but no exception was thrown");
		}
		catch( CLAM::ErrAssertionFailed& )
		{}
	}

	class DummyProcessing : public CLAM::Processing
	{
		const char* GetClassName() const
		{
			return "DummyProcessing";
		}
		const CLAM::ProcessingConfig & GetConfig() const
		{
			CLAM_ASSERT( false, "This is a dummy processing and doesn't have config" );
		}
		bool ConcreteConfigure( const CLAM::ProcessingConfig & cfg)
		{
			return false;
		}
		bool Do()
		{
			return false;
		}

	};

	void testGetProcessing_WhenProcessingAdded()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
			
		CLAM::Processing* proc = new DummyProcessing;
		std::string name( "dummy-processing" );
		net.AddProcessing( name, proc );

		CPPUNIT_ASSERT_EQUAL ( proc, &net.GetProcessing( name ) );
	}

	void testAddProcessing_UsingFactory()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
			
		std::string name( "oscillator" );
		std::string key( "Oscillator" );
		net.AddProcessing( name, key );

		CPPUNIT_ASSERT_EQUAL ( true , net.HasProcessing( name ) );
	}


	void testGetProcessing_WithTwoProcessings()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
	
		CLAM::Processing* proc = new DummyProcessing;
		std::string name1( "the first" );
		net.AddProcessing( name1, proc );

		CLAM::Processing* proc2 = new DummyProcessing;
		net.AddProcessing( std::string("the second"),proc2 );

		CPPUNIT_ASSERT_EQUAL( proc, &net.GetProcessing( name1 ) );
	}

	void testHasProcessing_WhenHasIt()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
		
		std::string name("the name");
		net.AddProcessing( name,  new DummyProcessing );

		CPPUNIT_ASSERT_EQUAL( true, net.HasProcessing( name ) );
	}

	void testHasProcessing_WhenHasntIt()
	{
		CLAM::Network net;
		CPPUNIT_ASSERT_EQUAL( false, net.HasProcessing(std::string("non-existing") ) );
	}

	class LoggableDummyProcessing : public DummyProcessing
	{
		BaseLoggable& mLog;
	public:
		LoggableDummyProcessing( BaseLoggable& log ) :
			mLog(log)
		{}
		~LoggableDummyProcessing() {
			mLog.ToLog() << this << " deleted\n";
		}
	};

	void testDestructor_DeletesChildrenProcessings()
	{
		BaseLoggable log;
		LoggableDummyProcessing* proc1 = new LoggableDummyProcessing(log);
		LoggableDummyProcessing* proc2 = new LoggableDummyProcessing(log);
		
		CLAM::Network* net = new CLAM::Network;
		const int nodeSize=1;
		net->AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		net->AddProcessing( std::string("first"), proc1 );
		net->AddProcessing( std::string("second"), proc2 );
		delete net;

		std::ostringstream expected;
		expected << proc1 << " deleted\n" << proc2 << " deleted\n";

		CPPUNIT_ASSERT_EQUAL(expected.str(), log.GetLog() );
	}

	// helper model-class-adapter
	class NetworkProtectedInterfacePublisher : public CLAM::Network
	{
	public:
		CLAM::InPort & GetInPortByCompleteName( const std::string& name )
		{	
			return Network::GetInPortByCompleteName( name );
		}
		CLAM::OutPort & GetOutPortByCompleteName( const std::string& name )
		{	
			return Network::GetOutPortByCompleteName( name );
		}	
		CLAM::InControl & GetInControlByCompleteName( const std::string& name )
		{	
			return Network::GetInControlByCompleteName( name );
		}	
		CLAM::OutControl & GetOutControlByCompleteName( const std::string& name )
		{	
			return Network::GetOutControlByCompleteName( name );
		}


	};



	///////////////////////////////////////////
	//////////   GetInPort testing  ///////////
	///////////////////////////////////////////

	void testGetInPortByCompleteName_WhenPortExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		DummyProcessing* theProc = new DummyProcessing;
		net.AddProcessing( "theOnlyProcessing", theProc );
		const int dummyLength = 1;
		CLAM::InPort* expectedInPort = 
			new CLAM::InPortTmpl<DummyProcessingData>( std::string("theOnlyInPort"), theProc, dummyLength );
	
		// exercice and test
		CPPUNIT_ASSERT_EQUAL( 
			expectedInPort, 
			&net.GetInPortByCompleteName( std::string("theOnlyProcessing.theOnlyInPort") ) 
		);

		// tear down
		delete expectedInPort;
	}

	void testGetInPortByCompleteName_WhenPortDoesntExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		net.AddProcessing( "theOnlyProcessing", new DummyProcessing );

		// exercice and test
		try {
			net.GetInPortByCompleteName( std::string("theOnlyProcessing.NonExistingPort") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "name not found in InPorts collection: NonExistingPort" ), 
				std::string( expected.what() ) );

		}
	}

	void testGetInPortByCompleteName_WhenProcessingDoesntExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		
		// exercice and test
		try {
			net.GetInPortByCompleteName( std::string("NonExistingProcessing.NonExistingPort") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "No Processing with the given name" ), 
				std::string( expected.what() ) );

		}
	}

	void testGetInPortByCompleteName_WhithMalformedName_WithNoDot()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		
		// exercice and test
		try {
			net.GetInPortByCompleteName( std::string("TheNameShould_ContainADot") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "Malformed port/control name. It should be ProcessingName.[Port/Control]Name" ), 
				std::string( expected.what() ) );

		}
	}
	
	void testGetInPortByCompleteName_WithThreeIdentifiers()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		
		DummyProcessing* theProc = new DummyProcessing;
		net.AddProcessing( "theOnlyProcessing", theProc );
		const int dummyLength = 1;
		CLAM::InPort* expectedInPort = 
			new CLAM::InPortTmpl<DummyProcessingData>( std::string("theOnlyInPort"), theProc, dummyLength );
	
		// exercice and test
		CPPUNIT_ASSERT_EQUAL( 
			expectedInPort, 
			&net.GetInPortByCompleteName( std::string("nonExistingNetwork.theOnlyProcessing.theOnlyInPort") ) 
		);

		// tear down
		delete expectedInPort;
	}

	///////////////////////////////////////////
	//////////   GetOutPort testing  //////////
	///////////////////////////////////////////


	void testGetOutPortByCompleteName_WhenPortExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		DummyProcessing* theProc = new DummyProcessing;
		net.AddProcessing( "theOnlyProcessing", theProc );
		const int dummyLength = 1;
		CLAM::OutPort* expectedOutPort = 
			new CLAM::OutPortTmpl<DummyProcessingData>( std::string("theOnlyOutPort"), theProc, dummyLength );
	
		// exercice and test
		CPPUNIT_ASSERT_EQUAL( 
			expectedOutPort, 
			&net.GetOutPortByCompleteName( std::string("theOnlyProcessing.theOnlyOutPort") ) 
		);

		// tear down
		delete expectedOutPort;
	}

	void testGetOutPortByCompleteName_WhenPortDoesntExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		net.AddProcessing( "theOnlyProcessing", new DummyProcessing );

		// exercice and test
		try {
			net.GetOutPortByCompleteName( std::string("theOnlyProcessing.NonExistingPort") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "name not found in OutPorts collection: NonExistingPort" ), 
				std::string( expected.what() ) );

		}
	}

	void testGetOutPortByCompleteName_WhenProcessingDoesntExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		
		// exercice and test
		try {
			net.GetOutPortByCompleteName( std::string("NonExistingProcessing.NonExistingPort") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "No Processing with the given name" ), 
				std::string( expected.what() ) );

		}
	}

	void testGetOutPortByCompleteName_WhithMalformedName_WithNoDot()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		
		// exercice and test
		try {
			net.GetOutPortByCompleteName( std::string("TheNameShould_ContainADot") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "Malformed port/control name. It should be ProcessingName.[Port/Control]Name" ), 
				std::string( expected.what() ) );

		}
	}
	
	void testGetOutPortByCompleteName_WithThreeIdentifiers()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		DummyProcessing* theProc = new DummyProcessing;
		net.AddProcessing( "theOnlyProcessing", theProc );
		const int dummyLength = 1;
		CLAM::OutPort* expectedOutPort = 
			new CLAM::OutPortTmpl<DummyProcessingData>( std::string("theOnlyOutPort"), theProc, dummyLength );
	
		// exercice and test
		CPPUNIT_ASSERT_EQUAL( 
			expectedOutPort, 
			&net.GetOutPortByCompleteName( std::string("nonExistingNetwork.theOnlyProcessing.theOnlyOutPort") ) 
		);

		// tear down
		delete expectedOutPort;
	}


	///////////////////////////////////////////
	//////////   GetInControl testing  ///////////
	///////////////////////////////////////////

	void testGetInControlByCompleteName_WhenControlExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		DummyProcessing* theProc = new DummyProcessing;
		net.AddProcessing( "theOnlyProcessing", theProc );
		CLAM::InControl* expectedInControl = 
			new CLAM::InControl( std::string("theOnlyInControl"), theProc );
	
		// exercice and test
		CPPUNIT_ASSERT_EQUAL( 
			expectedInControl, 
			&net.GetInControlByCompleteName( std::string("theOnlyProcessing.theOnlyInControl") ) 
		);

		// tear down
		delete expectedInControl;
	}

	void testGetInControlByCompleteName_WhenControlDoesntExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		net.AddProcessing( "theOnlyProcessing", new DummyProcessing );

		// exercice and test
		try {
			net.GetInControlByCompleteName( std::string("theOnlyProcessing.NonExistingControl") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "name not found in InControls collection: NonExistingControl" ), 
				std::string( expected.what() ) );

		}
	}

	void testGetInControlByCompleteName_WhenProcessingDoesntExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		
		// exercice and test
		try {
			net.GetInControlByCompleteName( std::string("NonExistingProcessing.NonExistingControl") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "No Processing with the given name" ), 
				std::string( expected.what() ) );

		}
	}

	void testGetInControlByCompleteName_WhithMalformedName_WithNoDot()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		
		// exercice and test
		try {
			net.GetInControlByCompleteName( std::string("TheNameShould_ContainADot") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "Malformed port/control name. It should be ProcessingName.[Port/Control]Name" ), 
				std::string( expected.what() ) );

		}
	}
	
	void testGetInControlByCompleteName_WithThreeIdentifiers()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		DummyProcessing* theProc = new DummyProcessing;
		net.AddProcessing( "theOnlyProcessing", theProc );
		CLAM::InControl* expectedInControl = 
			new CLAM::InControl( std::string("theOnlyInControl"), theProc );
	
		// exercice and test
		CPPUNIT_ASSERT_EQUAL( 
			expectedInControl, 
			&net.GetInControlByCompleteName( std::string("nonExistingNetwork.theOnlyProcessing.theOnlyInControl") ) 
		);

		// tear down
		delete expectedInControl;
	}



	///////////////////////////////////////////
	//////////   GetOutControl testing  ///////////
	///////////////////////////////////////////

	void testGetOutControlByCompleteName_WhenControlExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		DummyProcessing* theProc = new DummyProcessing;
		net.AddProcessing( "theOnlyProcessing", theProc );
		CLAM::OutControl* expectedOutControl = 
			new CLAM::OutControl( std::string("theOnlyOutControl"), theProc );
	
		// exercice and test
		CPPUNIT_ASSERT_EQUAL( 
			expectedOutControl, 
			&net.GetOutControlByCompleteName( std::string("theOnlyProcessing.theOnlyOutControl") ) 
		);

		// tear down
		delete expectedOutControl;
	}

	void testGetOutControlByCompleteName_WhenControlDoesntExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		net.AddProcessing( "theOnlyProcessing", new DummyProcessing );

		// exercice and test
		try {
			net.GetOutControlByCompleteName( std::string("theOnlyProcessing.NonExistingControl") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "name not found in OutControls collection: NonExistingControl" ), 
				std::string( expected.what() ) );

		}
	}

	void testGetOutControlByCompleteName_WhenProcessingDoesntExist()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		
		// exercice and test
		try {
			net.GetOutControlByCompleteName( std::string("NonExistingProcessing.NonExistingControl") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "No Processing with the given name" ), 
				std::string( expected.what() ) );

		}
	}

	void testGetOutControlByCompleteName_WhithMalformedName_WithNoDot()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		
		// exercice and test
		try {
			net.GetOutControlByCompleteName( std::string("TheNameShould_ContainADot") );
			CPPUNIT_FAIL("Expected assert, but didn't happened");
		}
		catch( CLAM::ErrAssertionFailed& expected) {
			CPPUNIT_ASSERT_EQUAL( 
				std::string( "Malformed port/control name. It should be ProcessingName.[Port/Control]Name" ), 
				std::string( expected.what() ) );

		}
	}
	
	void testGetOutControlByCompleteName_WithThreeIdentifiers()
	{
		//set up
		NetworkProtectedInterfacePublisher net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		DummyProcessing* theProc = new DummyProcessing;
		net.AddProcessing( "theOnlyProcessing", theProc );
		CLAM::OutControl* expectedOutControl = 
			new CLAM::OutControl( std::string("theOnlyOutControl"), theProc );
	
		// exercice and test
		CPPUNIT_ASSERT_EQUAL( 
			expectedOutControl, 
			&net.GetOutControlByCompleteName( std::string("nonExistingNetwork.theOnlyProcessing.theOnlyOutControl") ) 
		);

		// tear down
		delete expectedOutControl;
	}






	void testUseOfString_substr()
	{
		std::string today("today it rains");
		CPPUNIT_ASSERT_EQUAL( std::string("it"), today.substr(6,2) );
	}

	void testConnectPorts_WhenConnectionIsValid()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		DummyProcessing* firstProc = new DummyProcessing;
		DummyProcessing* secondProc = new DummyProcessing;

		net.AddProcessing( "first", firstProc );
		net.AddProcessing( "second", secondProc );

		const int dummyLength = 1;
		CLAM::OutPort* outPortOfFirstProc = 
			new CLAM::OutPortTmpl<CLAM::Audio>
			( std::string("outPortOfFirstProc"), firstProc, dummyLength );

		CLAM::InPort* inPortOfSecondProc = 
			new CLAM::InPortTmpl<CLAM::Audio>
			( std::string("inPortOfSecondProc"), secondProc, dummyLength );
		
		net.ConnectPorts("first.outPortOfFirstProc","second.inPortOfSecondProc");
		CPPUNIT_ASSERT_EQUAL( true, outPortOfFirstProc->IsConnectedTo(*inPortOfSecondProc) );
	}

	void testConnectPorts_WhenConnectionIsNotValid()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		DummyProcessing* firstProc = new DummyProcessing;
		DummyProcessing* secondProc = new DummyProcessing;

		net.AddProcessing( "first", firstProc );
		net.AddProcessing( "second", secondProc );

		const int dummyLength = 1;
		CLAM::OutPort* outPortOfFirstProc = 
			new CLAM::OutPortTmpl<DummyProcessingData>
			( std::string("outPortOfFirstProc"), firstProc, dummyLength );

		CLAM::InPort* inPortOfSecondProc = 
			new CLAM::InPortTmpl<CLAM::Audio>
			( std::string("inPortOfSecondProc"), secondProc, dummyLength );

		
		CPPUNIT_ASSERT_EQUAL( false, net.ConnectPorts(
					      "first.outPortOfFirstProc","second.inPortOfSecondProc") );
	}

	void testRemovePortsConnection_WhenPortsAreNotConnected()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		DummyProcessing* firstProc = new DummyProcessing;
		DummyProcessing* secondProc = new DummyProcessing;

		net.AddProcessing( "first", firstProc );
		net.AddProcessing( "second", secondProc );

		const int dummyLength = 1;
		CLAM::OutPort* outPortOfFirstProc = 
			new CLAM::OutPortTmpl<CLAM::Audio>
			( std::string("outPortOfFirstProc"), firstProc, dummyLength );

		CLAM::InPort* inPortOfSecondProc = 
			new CLAM::InPortTmpl<CLAM::Audio>
			( std::string("inPortOfSecondProc"), secondProc, dummyLength );

		
		CPPUNIT_ASSERT_EQUAL( false, net.DisconnectPorts(
					      "first.outPortOfFirstProc","second.inPortOfSecondProc") );
	}

	void testRemovePortsConnection_WhenPortsAreConnected()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		DummyProcessing* firstProc = new DummyProcessing;
		DummyProcessing* secondProc = new DummyProcessing;

		net.AddProcessing( "first", firstProc );
		net.AddProcessing( "second", secondProc );

		const int dummyLength = 1;
		CLAM::OutPort* outPortOfFirstProc = 
			new CLAM::OutPortTmpl<CLAM::Audio>
			( std::string("outPortOfFirstProc"), firstProc, dummyLength );

		CLAM::InPort* inPortOfSecondProc = 
			new CLAM::InPortTmpl<CLAM::Audio>
			( std::string("inPortOfSecondProc"), secondProc, dummyLength );
		
		net.ConnectPorts("first.outPortOfFirstProc","second.inPortOfSecondProc");
		net.DisconnectPorts( "first.outPortOfFirstProc","second.inPortOfSecondProc");
		CPPUNIT_ASSERT_EQUAL( false, outPortOfFirstProc->IsConnectedTo(*inPortOfSecondProc) );

	}

	void testRemoveProcessing_WhenHasIt()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		DummyProcessing* proc = new DummyProcessing;

		net.AddProcessing( "the processing", proc );
		net.RemoveProcessing( "the processing" );
		CPPUNIT_ASSERT_EQUAL( false, net.HasProcessing( "the processing" ));
	}

	void testRemoveProcessing_WhenHasntIt()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		DummyProcessing* proc = new DummyProcessing;

		net.AddProcessing( "the processing", proc );

		try{ 
			net.RemoveProcessing( "false processing" );
			CPPUNIT_FAIL("Assert expected, but no exception was thrown");
		}
		catch( CLAM::ErrAssertionFailed& )
		{}
	}

	void testConnectControls_WhenConnectionIsValid()
	{

		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		DummyProcessing* firstProc = new DummyProcessing;
		DummyProcessing* secondProc = new DummyProcessing;

		net.AddProcessing( "first", firstProc );
		net.AddProcessing( "second", secondProc );

		const int dummyLength = 1;
		CLAM::OutControl* outControlOfFirstProc =  
			new CLAM::OutControl( std::string("outControlOfFirstProc"), firstProc );

		CLAM::InControl* inControlOfSecondProc = 
			new CLAM::InControl( std::string("inControlOfSecondProc"), secondProc );
		
		net.ConnectControls("first.outControlOfFirstProc","second.inControlOfSecondProc");
		CPPUNIT_ASSERT_EQUAL( true, outControlOfFirstProc->IsConnectedTo(*inControlOfSecondProc) );
	}

	void testConnectControls_WhenConnectionIsNotValid()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		DummyProcessing* firstProc = new DummyProcessing;
		DummyProcessing* secondProc = new DummyProcessing;

		net.AddProcessing( "first", firstProc );
		net.AddProcessing( "second", secondProc );

		const int dummyLength = 1;
		CLAM::OutControl* outControlOfFirstProc =  
			new CLAM::OutControl( std::string("outControlOfFirstProc"), firstProc );

		CLAM::InControl* inControlOfSecondProc = 
			new CLAM::InControl( std::string("inControlOfSecondProc"), secondProc );
		
		CPPUNIT_ASSERT_EQUAL( false, outControlOfFirstProc->IsConnectedTo(*inControlOfSecondProc) );
	}

	void testRemoveControlsConnection_WhenControlsAreNotConnected()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		DummyProcessing* firstProc = new DummyProcessing;
		DummyProcessing* secondProc = new DummyProcessing;

		net.AddProcessing( "first", firstProc );
		net.AddProcessing( "second", secondProc );

		const int dummyLength = 1;
		CLAM::OutControl* outControlOfFirstProc =  
			new CLAM::OutControl( std::string("outControlOfFirstProc"), firstProc );

		CLAM::InControl* inControlOfSecondProc = 
			new CLAM::InControl( std::string("inControlOfSecondProc"), secondProc );
		
		CPPUNIT_ASSERT_EQUAL( false, net.DisconnectControls( "first.outControlOfFirstProc","second.inControlOfSecondProc"  ));

	}

	void testRemoveControlsConnection_WhenControlsAreConnected()
	{		
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );
	
		DummyProcessing* firstProc = new DummyProcessing;
		DummyProcessing* secondProc = new DummyProcessing;

		net.AddProcessing( "first", firstProc );
		net.AddProcessing( "second", secondProc );

		const int dummyLength = 1;
		CLAM::OutControl* outControlOfFirstProc =  
			new CLAM::OutControl( std::string("outControlOfFirstProc"), firstProc );

		CLAM::InControl* inControlOfSecondProc = 
			new CLAM::InControl( std::string("inControlOfSecondProc"), secondProc );
		
		net.ConnectControls("first.outControlOfFirstProc","second.inControlOfSecondProc");
		CPPUNIT_ASSERT_EQUAL( true, net.DisconnectControls( "first.outControlOfFirstProc","second.inControlOfSecondProc" ));

	}

	void testStartNetworkStartsProcessings_WhenAreReady()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		CLAM::AudioMultiplier * multiplier = new CLAM::AudioMultiplier;
		net.AddProcessing( "oscillator", oscillator) ;
		net.AddProcessing( "multiplier", multiplier );
		net.Start();

		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Running, oscillator->GetExecState() );
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Running, multiplier->GetExecState() );
		

	}

	void testStartNetworkDoesntStartProcessings_WhenAreNotReady()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		CLAM::AudioFileIn * filein = new CLAM::AudioFileIn;
		net.AddProcessing( "oscillator", oscillator) ;
		net.AddProcessing( "filein", filein );
		net.Start();

		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Running, oscillator->GetExecState() );
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Unconfigured, filein->GetExecState() );		
	}

	void testStopNetworkStopsProcessings_WhenAreRunning()
	{
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		CLAM::AudioMultiplier * multiplier = new CLAM::AudioMultiplier;
		net.AddProcessing( "oscillator", oscillator) ;
		net.AddProcessing( "multiplier", multiplier );
		net.Start();
		net.Stop();
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Ready, oscillator->GetExecState() );
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Ready, multiplier->GetExecState() );

	}

	void testStopNetworkDoesntStopProcessings_WhenAreNotRunning()
	{	
		CLAM::Network net;
		const int nodeSize=1;
		net.AddFlowControl( new CLAM::BasicFlowControl(nodeSize) );

		CLAM::SimpleOscillator * oscillator = new CLAM::SimpleOscillator;
		CLAM::AudioFileIn * filein = new CLAM::AudioFileIn;
		net.AddProcessing( "oscillator", oscillator) ;
		net.AddProcessing( "filein", filein );
		net.Start();
		net.Stop();

		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Ready, oscillator->GetExecState() );
		CPPUNIT_ASSERT_EQUAL( CLAM::Processing::Unconfigured, filein->GetExecState() );		
	}
};
   
} // namespace 
