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

//DEPRECATED TO REMOVE
//

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // needed for assertion_traits<bool>
#include "InPort.hxx"
#include "InPort.hxx"
#include "OutPort.hxx"
#include "OutPort.hxx"
#include "Processing.hxx"
#include "DummyProcessingData.hxx"

namespace CLAMTest {

class PortsTest;
CPPUNIT_TEST_SUITE_REGISTRATION( PortsTest );


class PortsTest : public CppUnit::TestFixture, public CLAM::Processing
{
	CPPUNIT_TEST_SUITE( PortsTest );
	// Tests for the Port base interface:

	//InPorts
	CPPUNIT_TEST( testInPort_Attach_WithGeneralTemplateInPort_UpdatesConcretePortState );
	CPPUNIT_TEST( testInPort_Attach_WithGeneralTemplateInPort_BadTypeAssertionFails );
	CPPUNIT_TEST( testInPort_IsAttached_WithGeneralTemplateInPort_AfterConstruction );
	CPPUNIT_TEST( testInPort_IsAttached_WithGeneralTemplateInPort_AfterAttach );
	CPPUNIT_TEST( testInPort_IsAttached_WithGeneralTemplateInPort_AfterUnattach );
	CPPUNIT_TEST( testInPort_UnAttach_WithGeneralTemplateInPort_WhenIsNotAttached );
	//  todo: remove these Audio specific tests if the Audio template specialization is 
	//        no longer necessary (VC6 issue)
	CPPUNIT_TEST( testInPort_Attach_WithAudioInPort_UpdatesConcretePortState );
	CPPUNIT_TEST( testInPort_Attach_WithAudioInPort_BadTypeAssertionFails );
	CPPUNIT_TEST( testInPort_IsAttached_WithAudioInPort_AfterConstruction );
	CPPUNIT_TEST( testInPort_IsAttached_WithAudioInPort_AfterAttach );
	CPPUNIT_TEST( testInPort_IsAttached_WithAudioInPort_AfterUnattach );
	CPPUNIT_TEST( testInPort_UnAttach_WithAudioInPort_WhenIsNotAttached );
	// end todo

	//OutPorts
	CPPUNIT_TEST( testOutPort_Attach_WithGeneralTemplateOutPort_UpdatesConcretePortState );
	CPPUNIT_TEST( testOutPort_Attach_WithGeneralTemplateOutPort_BadTypeAssertionFails );
	CPPUNIT_TEST( testOutPort_IsAttached_WithGeneralTemplateOutPort_AfterConstruction );
	CPPUNIT_TEST( testOutPort_IsAttached_WithGeneralTemplateOutPort_AfterAttach );
	CPPUNIT_TEST( testOutPort_IsAttached_WithGeneralTemplateOutPort_AfterUnattach );
	CPPUNIT_TEST( testOutPort_UnAttach_WithGeneralTemplateOutPort_WhenIsNotAttached );
	//  todo: remove these Audio specific tests if the Audio template specialization is 
	//        no longer necessary (VC6 issue)
	CPPUNIT_TEST( testOutPort_Attach_WithAudioOutPort_UpdatesConcretePortState );
	CPPUNIT_TEST( testOutPort_Attach_WithAudioOutPort_BadTypeAssertionFails );
	CPPUNIT_TEST( testOutPort_IsAttached_WithAudioOutPort_AfterConstruction );
	CPPUNIT_TEST( testOutPort_IsAttached_WithAudioOutPort_AfterAttach );
	CPPUNIT_TEST( testOutPort_IsAttached_WithAudioOutPort_AfterUnattach );
	CPPUNIT_TEST( testOutPort_UnAttach_WithAudioOutPort_WhenIsNotAttached );
	// end todo

	// testing of GetProcessingData methods

	CPPUNIT_TEST( testPort_GetProcessingData_WithAudioPort_AfterAttach );
	CPPUNIT_TEST( testPort_GetProcessingData_WithGeneralTemplatePort_AfterAttach );
	CPPUNIT_TEST( testPort_GetProcessingData_WithAudioPort_AfterConstruction );
	CPPUNIT_TEST( testPort_GetProcessingData_WithGeneralTemplatePort_AfterConstruction );

	// Testing of AreConnected() method
	CPPUNIT_TEST( testOutPort_IsConnectedTo_WithAudioPorts_WhenPortsAreConnected );
	CPPUNIT_TEST( testOutPort_IsConnectedTo_WithGeneralTemplatePorts_WhenPortsAreConnected );
	CPPUNIT_TEST( testOutPort_IsConnectedTo_WithAudioPorts_WhenPortsAreNotConnected );
	CPPUNIT_TEST( testOutPort_IsConnectedTo_WithGeneralTemplatePorts_WhenPortsAreNotConnected );
	CPPUNIT_TEST( testInPort_IsConnectedTo_DelegatesToOutPort );

	// Testing OutPort interface
	CPPUNIT_TEST( testOutPort_IsConnectableTo_WithAudioOutPort_WhenInPortIsTheSameType );
	CPPUNIT_TEST( testOutPort_IsConnectableTo_WithGeneralTemplate_WhenInPortIsTheSameType );
	CPPUNIT_TEST( testOutPort_IsConnectableTo_WithAudioOutPort_WhenInPortIsDifferentType );
	CPPUNIT_TEST( testOutPort_IsConnectableTo_WithGeneralTemplate_WhenInPortIsDifferentType );

	// Tests for the concrete Port classes
	CPPUNIT_TEST_SUITE_END();

	// Testing pattern: Self Shunt
	// Processing interface:
	const char* GetClassName() const { return "for testing"; }
	bool Do() { return false; }
	const CLAM::ProcessingConfig& GetConfig() const { throw 0; }
	bool ConcreteConfigure( const CLAM::ProcessingConfig& ) { return false; }
	
	///////////////// TESTING IN PORTS /////////////////
	void testInPort_Attach_WithGeneralTemplateInPort_UpdatesConcretePortState()
	{
		CLAM::InPort<DummyProcessingData> concreteInPort("in-port", this);
		DummyProcessingData attachedData;
		attachedData.SetState(1);
		CLAM::InPort &baseInPort = concreteInPort;
		baseInPort.Attach( attachedData );	
		
		CPPUNIT_ASSERT_EQUAL( concreteInPort.GetData().GetState(), attachedData.GetState() );
	}

	void testInPort_Attach_WithAudioInPort_UpdatesConcretePortState()
	{
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::Audio attachedAudio;
		CLAM::InPort &baseInPort = concreteInPort;
		baseInPort.Attach( attachedAudio );
		// compare by reference (pointer) because Audio haven't yet operator==
		CPPUNIT_ASSERT_EQUAL( &concreteInPort.GetData(), &attachedAudio );
	}

	void testInPort_Attach_WithGeneralTemplateInPort_BadTypeAssertionFails()
	{
		CLAM::InPort<DummyProcessingData> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;
		CLAM::Audio attachedAudio;
		try {
			baseInPort.Attach( attachedAudio );	
			CPPUNIT_FAIL("assertion failed expected, but nothing happened");
		} catch(CLAM::ErrAssertionFailed& )	{}
	}

	void testInPort_Attach_WithAudioInPort_BadTypeAssertionFails()
	{
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;
		DummyProcessingData attachedDummy;
		try {
			baseInPort.Attach( attachedDummy );	
			CPPUNIT_FAIL("assertion failed expected, but nothing happened");
		} catch(CLAM::ErrAssertionFailed& )	{}
	}

	void testInPort_IsAttached_WithAudioInPort_AfterConstruction()
	{
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;
		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsAttached() );
	}

	void testInPort_IsAttached_WithGeneralTemplateInPort_AfterConstruction()
	{
		CLAM::InPort<DummyProcessingData> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;
		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsAttached() );
	}

	void testInPort_IsAttached_WithGeneralTemplateInPort_AfterAttach()
	{
		CLAM::InPort<DummyProcessingData> concreteInPort("in-port", this);
		DummyProcessingData attached;
		CLAM::InPort &baseInPort = concreteInPort;
		baseInPort.Attach( attached );	

		CPPUNIT_ASSERT_EQUAL( true, concreteInPort.IsAttached() );
	}
	void testInPort_IsAttached_WithAudioInPort_AfterAttach()
	{
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::Audio attached;
		CLAM::InPort &baseInPort = concreteInPort;
		baseInPort.Attach( attached );	

		CPPUNIT_ASSERT_EQUAL( true, baseInPort.IsAttached() );
	}

	void testInPort_IsAttached_WithAudioInPort_AfterUnattach()
	{
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::Audio attached;
		CLAM::InPort &baseInPort = concreteInPort;
		baseInPort.Attach( attached );	
		baseInPort.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsAttached() );
	}
	void testInPort_IsAttached_WithGeneralTemplateInPort_AfterUnattach()
	{
		CLAM::InPort<DummyProcessingData> concreteInPort("in-port", this);
		DummyProcessingData attached;
		CLAM::InPort &baseInPort = concreteInPort;
		
		baseInPort.Attach( attached );	
		baseInPort.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsAttached() );
	}
	void testInPort_UnAttach_WithAudioInPort_WhenIsNotAttached()
	{
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;

		baseInPort.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsAttached() );
	}
	void testInPort_UnAttach_WithGeneralTemplateInPort_WhenIsNotAttached()
	{
		CLAM::InPort<DummyProcessingData> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;

		baseInPort.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsAttached() );
	}

	///////////////// TESTING OUT PORTS /////////////////
	
	void testOutPort_Attach_WithGeneralTemplateOutPort_UpdatesConcretePortState()
	{
		CLAM::OutPort<DummyProcessingData> concreteOutPort("out-port", this);
		DummyProcessingData attachedData;
		attachedData.SetState(1);
		CLAM::OutPort &baseOutPort = concreteOutPort;
		baseOutPort.Attach( attachedData );	
		
		CPPUNIT_ASSERT_EQUAL( concreteOutPort.GetData().GetState(), attachedData.GetState() );
	}

	void testOutPort_Attach_WithAudioOutPort_UpdatesConcretePortState()
	{
		CLAM::OutPort<CLAM::Audio> concreteOutPort("out-port", this);
		CLAM::Audio attachedAudio;
		CLAM::OutPort &baseOutPort = concreteOutPort;
		baseOutPort.Attach( attachedAudio );
		// compare by reference (pointer) because Audio haven't yet operator==
		CPPUNIT_ASSERT_EQUAL( &concreteOutPort.GetData(), &attachedAudio );
	}

	void testOutPort_Attach_WithGeneralTemplateOutPort_BadTypeAssertionFails()
	{
		CLAM::OutPort<DummyProcessingData> concreteOutPort("out-port", this);
		CLAM::OutPort &baseOutPort = concreteOutPort;
		CLAM::Audio attachedAudio;
		try {
			baseOutPort.Attach( attachedAudio );	
			CPPUNIT_FAIL("assertion failed expected, but nothing happened");
		} catch(CLAM::ErrAssertionFailed& )	{}
	}

	void testOutPort_Attach_WithAudioOutPort_BadTypeAssertionFails()
	{
		CLAM::OutPort &baseOutPort = concreteOutPort;
		DummyProcessingData attachedDummy;
		try {
			baseOutPort.Attach( attachedDummy );	
			CPPUNIT_FAIL("assertion failed expected, but nothing happened");
		} catch(CLAM::ErrAssertionFailed& )	{}
	}

	void testOutPort_IsAttached_WithAudioOutPort_AfterConstruction()
	{
		CLAM::OutPort<CLAM::Audio> concreteOutPort("out-port", this);
		CLAM::OutPort &baseOutPort = concreteOutPort;
		CPPUNIT_ASSERT_EQUAL( false, baseOutPort.IsAttached() );
	}

	void testOutPort_IsAttached_WithGeneralTemplateOutPort_AfterConstruction()
	{
		CLAM::OutPort<DummyProcessingData> concreteOutPort("out-port", this);
		CLAM::OutPort &baseOutPort = concreteOutPort;
		CPPUNIT_ASSERT_EQUAL( false, baseOutPort.IsAttached() );
	}

	void testOutPort_IsAttached_WithGeneralTemplateOutPort_AfterAttach()
	{
		CLAM::OutPort<DummyProcessingData> concreteOutPort("out-port", this);
		DummyProcessingData attached;
		CLAM::OutPort &baseOutPort = concreteOutPort;
		baseOutPort.Attach( attached );	

		CPPUNIT_ASSERT_EQUAL( true, concreteOutPort.IsAttached() );
	}
	void testOutPort_IsAttached_WithAudioOutPort_AfterAttach()
	{
		CLAM::OutPort<CLAM::Audio> concreteOutPort("out-port", this);
		CLAM::Audio attached;
		CLAM::OutPort &baseOutPort = concreteOutPort;
		baseOutPort.Attach( attached );	

		CPPUNIT_ASSERT_EQUAL( true, baseOutPort.IsAttached() );
	}

	void testOutPort_IsAttached_WithAudioOutPort_AfterUnattach()
	{
		CLAM::OutPort<CLAM::Audio> concreteOutPort("out-port", this);
		CLAM::Audio attached;
		CLAM::OutPort &baseOutPort = concreteOutPort;
		baseOutPort.Attach( attached );	
		baseOutPort.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, baseOutPort.IsAttached() );
	}
	void testOutPort_IsAttached_WithGeneralTemplateOutPort_AfterUnattach()
	{
		const int dummyLength = 0;
		CLAM::OutPort<DummyProcessingData> concreteOutPort("out-port", this);
		DummyProcessingData attached;
		CLAM::OutPort &baseOutPort = concreteOutPort;
		
		baseOutPort.Attach( attached );	
		baseOutPort.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, baseOutPort.IsAttached() );
	}
	void testOutPort_UnAttach_WithAudioOutPort_WhenIsNotAttached()
	{
		const int dummyLength = 0;
		CLAM::OutPort<CLAM::Audio> concreteOutPort("out-port", this);
		CLAM::OutPort &baseOutPort = concreteOutPort;

		baseOutPort.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, baseOutPort.IsAttached() );
	}
	void testOutPort_UnAttach_WithGeneralTemplateOutPort_WhenIsNotAttached()
	{
		const int dummyLength = 0;
		CLAM::OutPort<DummyProcessingData> concreteOutPort("out-port", this);
		CLAM::OutPort &baseOutPort = concreteOutPort;

		baseOutPort.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, baseOutPort.IsAttached() );
	}
	
	//////////  GetProcessingData testing ///////

	void testPort_GetProcessingData_WithAudioPort_AfterAttach()
	{
		const int dummyLength = 0;
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::Port &basePort = concreteInPort;
		CLAM::Audio attached;
		
		concreteInPort.Attach( attached );
		basePort.GetProcessingData();
		CPPUNIT_ASSERT_EQUAL( (CLAM::ProcessingData*)&attached, basePort.GetProcessingData() );
	}
	void testPort_GetProcessingData_WithGeneralTemplatePort_AfterAttach()
	{
		const int dummyLength = 0;
		CLAM::InPort<DummyProcessingData> concreteInPort("in-port", this);
		CLAM::Port &basePort = concreteInPort;
		DummyProcessingData attached;

		concreteInPort.Attach( attached );
		CPPUNIT_ASSERT_EQUAL( (CLAM::ProcessingData*)&attached, basePort.GetProcessingData() );

	}
	void testPort_GetProcessingData_WithAudioPort_AfterConstruction()
	{
		const int dummyLength = 0;
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::Port &basePort = concreteInPort;
		CLAM::ProcessingData* p = 0;
		CPPUNIT_ASSERT_EQUAL( p , basePort.GetProcessingData() );	

	}
	void testPort_GetProcessingData_WithGeneralTemplatePort_AfterConstruction()
	{
		const int dummyLength = 0;
		CLAM::InPort<DummyProcessingData> concreteInPort("in-port", this);
		CLAM::Port &basePort = concreteInPort;
		CLAM::ProcessingData* p = 0;
		CPPUNIT_ASSERT_EQUAL( p , basePort.GetProcessingData() );
	}

	//////////// Connectability testing //////////////

	void testOutPort_IsConnectedTo_WithAudioPorts_WhenPortsAreConnected()
	{
		const int dummyLength = 0;
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;
		CLAM::OutPort<CLAM::Audio> concreteOutPort("out-port", this);
		CLAM::OutPort &baseOutPort = concreteOutPort;

		CLAM::Audio attached;
		baseInPort.Attach( attached );
		baseOutPort.Attach( attached );	
		CPPUNIT_ASSERT_EQUAL( true, baseOutPort.IsConnectedTo( baseInPort ) );

	}
	void testOutPort_IsConnectedTo_WithGeneralTemplatePorts_WhenPortsAreConnected()
	{
		const int dummyLength = 0;
		CLAM::InPort<DummyProcessingData> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;
		CLAM::OutPort<DummyProcessingData> concreteOutPort("out-port", this);
		CLAM::OutPort &baseOutPort = concreteOutPort;
		
		DummyProcessingData attached;
		baseInPort.Attach( attached );
		baseOutPort.Attach( attached );	
		CPPUNIT_ASSERT_EQUAL( true, baseInPort.IsConnectedTo(baseOutPort) );
	
	}
	void testOutPort_IsConnectedTo_WithAudioPorts_WhenPortsAreNotConnected()
	{
		const int dummyLength = 0;
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;
		CLAM::OutPort<CLAM::Audio> concreteOutPort("out-port", this);
		CLAM::OutPort &baseOutPort = concreteOutPort;

		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsConnectedTo(baseOutPort) );

	}
	void testInPort_IsConnectedTo_DelegatesToOutPort()
	{
		const int dummyLength = 0;
		CLAM::InPort<CLAM::Audio> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;
		CLAM::OutPort<CLAM::Audio> concreteOutPort("out-port", this);
		CLAM::OutPort &baseOutPort = concreteOutPort;

		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsConnectedTo(baseOutPort) );
	}

	void testOutPort_IsConnectedTo_WithGeneralTemplatePorts_WhenPortsAreNotConnected()
	{
		const int dummyLength = 0;
		CLAM::InPort<DummyProcessingData> concreteInPort("in-port", this);
		CLAM::InPort &baseInPort = concreteInPort;
		CLAM::OutPort<DummyProcessingData> concreteOutPort("out-port", this);
		CLAM::OutPort &baseOutPort = concreteOutPort;

		CPPUNIT_ASSERT_EQUAL( false, baseInPort.IsConnectedTo(baseOutPort) );

	}

	void testOutPort_IsConnectableTo_WithAudioOutPort_WhenInPortIsTheSameType()
	{
		const int dummyLength = 0;
		CLAM::OutPort<CLAM::Audio> out("out-port", this );
		CLAM::InPort<CLAM::Audio> in("in-port", this );

		CLAM::OutPort & baseOutPort = out;
		CLAM::InPort & baseInPort = in;

		CPPUNIT_ASSERT_EQUAL( true, baseOutPort.IsConnectableTo(baseInPort) );
	}

	void testOutPort_IsConnectableTo_WithGeneralTemplate_WhenInPortIsTheSameType()
	{
		const int dummyLength = 0;
		CLAM::OutPort<DummyProcessingData> out("out-port", this );
		CLAM::InPort<DummyProcessingData> in("in-port", this );

		CLAM::OutPort & baseOutPort = out;
		CLAM::InPort & baseInPort = in;

		CPPUNIT_ASSERT_EQUAL( true, baseOutPort.IsConnectableTo(baseInPort) );
	}

	void testOutPort_IsConnectableTo_WithAudioOutPort_WhenInPortIsDifferentType()
	{
		const int dummyLength = 0;
		CLAM::OutPort<CLAM::Audio> out("out-port", this );
		CLAM::InPort<DummyProcessingData> in("in-port", this );

		CLAM::OutPort & baseOutPort = out;
		CLAM::InPort & baseInPort = in;

		CPPUNIT_ASSERT_EQUAL( false, baseOutPort.IsConnectableTo(baseInPort) );
	}

	void testOutPort_IsConnectableTo_WithGeneralTemplate_WhenInPortIsDifferentType()
	{
		const int dummyLength = 0;
		CLAM::OutPort<DummyProcessingData> out("out-port", this );
		CLAM::InPort<CLAM::Audio> in("in-port", this );

		CLAM::OutPort & baseOutPort = out;
		CLAM::InPort & baseInPort = in;

		CPPUNIT_ASSERT_EQUAL( false, baseOutPort.IsConnectableTo(baseInPort) );
	}


};

} //namespace 

