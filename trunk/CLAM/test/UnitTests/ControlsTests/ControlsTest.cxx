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

#include "InControl.hxx"
#include "OutControl.hxx"
#include "OutControlPublisher.hxx"
#include "InControlPublisher.hxx"
#include "InControlArray.hxx"
#include "BaseLoggable.hxx"
#include "Processing.hxx"
#include <cppunit/extensions/HelperMacros.h>

namespace CLAMTest {

using CLAM::TControlData;

class ControlsTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ControlsTest );


class ControlsTest : public CppUnit::TestFixture, public BaseLoggable, public CLAM::Processing
{
	CPPUNIT_TEST_SUITE( ControlsTest );
	// testing InControl and OutControl :
	CPPUNIT_TEST( testInControl_DoControl_ChangesInternalState );
	CPPUNIT_TEST( testLinkAndSendControl_ChangesInControlInternalState );
	CPPUNIT_TEST( testInControlTmpl_DoControl_ChangesInternalState );
	CPPUNIT_TEST( testLinkAndSendWithInControlTmpl_CallbackMethodGetsCalled );
	CPPUNIT_TEST( testControlHandlerId_WritesToLog );
	CPPUNIT_TEST( testLinkAndSendWithInControlTmpl_CallbackWithIdMethodGetsCalled );
	CPPUNIT_TEST( testInControl_GetName_ChangesInteralState );
	CPPUNIT_TEST( testOutControl_GetName_ChangesInteralState );
	// testing InControlArray :
	CPPUNIT_TEST( testInControlArray_Constructor_GeneratesCorrectName );
	CPPUNIT_TEST( testInControlTmplArray_Constructor_GeneratesCorrectName );
	CPPUNIT_TEST( testInControlTmplArray_ReceivesControl_HandlerReceivesControlAndId );

	// tests for IsConnected / IsConnectedTo
	CPPUNIT_TEST( testIsConnected_WithOutControl_AfterConnection );
	CPPUNIT_TEST( testIsConnected_WithOutControl_WithoutConnection );
	CPPUNIT_TEST( testIsConnectedTo_WithOutControl_WhenControlsAreConnected );
	CPPUNIT_TEST( testIsConnectedTo_WithOutControl_WhenControlsAreNotConnected );
	CPPUNIT_TEST( testIsConnectedTo_WithInControl_WhenControlsAreConnected );
	CPPUNIT_TEST( testIsConnectedTo_WithInControl_WhenControlsAreNotConnected );

	// tests for Control Publishers
//TODO	CPPUNIT_TEST( testOutControlPublisher );
	CPPUNIT_TEST( testOutControlPublisher_GetsRegisteredToAProcessing );
	CPPUNIT_TEST( testOutControlPublisher_ConnectControlsFromPublisher );
	CPPUNIT_TEST( testOutControlPublisher_SendControlWhenNothingPublished );
	

	CPPUNIT_TEST( testInControlPublisher );
	CPPUNIT_TEST( testInControlPublisher_GetsRegisteredToAProcessing );
	CPPUNIT_TEST( testInControlPublisher_ConnectControlsFromPublisher );
	CPPUNIT_TEST( testInControlPublisher_DoControlWhenNothingPublished );

	// Backward reference
	CPPUNIT_TEST( testInControlIsConnected_byDefault );
	CPPUNIT_TEST( testInControlIsConnected_whenConnected );
	CPPUNIT_TEST( testInControlIsConnected_whenDisconnected );
	// Destructor disconnections
	CPPUNIT_TEST( testInControlDestructor_disconnectsOutControl );
	CPPUNIT_TEST( testOutControlDestructor_disconnectsInControl );

	// Testing bounds
	CPPUNIT_TEST( testInControl_doesntHaveBoundsBydefault );
	CPPUNIT_TEST( testInControl_defaultBounds );
	CPPUNIT_TEST( testInControl_settingBounds );
	CPPUNIT_TEST( testInControl_boundedDefaultValue );
	CPPUNIT_TEST( testInControl_isBoundedWhenTrue );

	// Testing default value
	CPPUNIT_TEST( testInControl_setDefaultprevailstoBounds );

	CPPUNIT_TEST_SUITE_END();

	// Testing pattern: Self Shunt
	// Processing interface:
	const char* GetClassName() const { return "for testing"; }
	bool Do() { return false; }

public:
	void tearDown() {
		ClearLog();
	}

private:
	void testInControl_DoControl_ChangesInternalState()
	{
		CLAM::FloatInControl in("i'm an in control");
		in.DoControl(1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f, in.GetLastValue() );
	}

	void testLinkAndSendControl_ChangesInControlInternalState()
	{
		CLAM::FloatInControl in("in");
		CLAM::FloatOutControl out("out");
		out.AddLink(in);
		out.SendControl(1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f , in.GetLastValue() );
	}
	// this method is used by the CLAM::InControlTmpl<T>
	// here we are simulating that this class is the parent processing object
public:
	void PublishInControl(CLAM::InControlBase* c) 
	{
		ToLog() << "InControl published\n";
	}	

private:
	void testInControlTmpl_DoControl_ChangesInternalState()
	{
		CLAM::FloatInControl in("I'm an in ctrl template", this);// calls this->PublishInControl
		in.DoControl(1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f, in.GetLastValue() );
	}
	// helper method used for handling incoming control
	void ControlHandler(CLAM::TControlData val) {
		ToLog() << "ControlHandler called with: " << val;
	}
	void testLinkAndSendWithInControlTmpl_CallbackMethodGetsCalled()
	{
		CLAM::FloatInControl 
			in("in", this, &ControlsTest::ControlHandler); // calls this->PublishInControl
		
		ClearLog();
		in.DoControl(1.f);
		CPPUNIT_ASSERT_EQUAL( std::string("ControlHandler called with: 1"), GetLog() );
	}

	// helper method for handling incoming control plus incontrol ID
	void ControlHandlerId(unsigned id, CLAM::TControlData val) {
		ToLog() << "ControlHandler called with id : " << id << " and value : " << val;
	}
	void testControlHandlerId_WritesToLog()
	{
		ControlHandlerId(0, 1.0);
		CPPUNIT_ASSERT_EQUAL( std::string("ControlHandler called with id : 0 and value : 1"), GetLog() );
	}

	void testLinkAndSendWithInControlTmpl_CallbackWithIdMethodGetsCalled()
	{
		const int controlId=2;
		CLAM::FloatInControl 
			in( controlId, "in", this, &ControlsTest::ControlHandlerId ); // calls this->PublishInControl
				
		in.DoControl( 1.f );
		CPPUNIT_ASSERT_EQUAL( 
			GetLog(), 
			std::string("ControlHandler called with id : 2 and value : 1") );
		    // note that controlId == 2
	}

	void testInControl_GetName_ChangesInteralState()
	{
		CLAM::FloatInControl in("in name");
		CPPUNIT_ASSERT_EQUAL(std::string("in name"), in.GetName() );
	}
	void testOutControl_GetName_ChangesInteralState()
	{
		CLAM::FloatOutControl out("out name");
		CPPUNIT_ASSERT_EQUAL(std::string("out name"), out.GetName() );
	}

	void testInControlArray_Constructor_GeneratesCorrectName()
	{
		CLAM::InControlArray inControls (4, "name",this);
		CPPUNIT_ASSERT_EQUAL( 
			std::string("name_0"),
			inControls[0].GetName() );
		

	}

	void testInControlTmplArray_Constructor_GeneratesCorrectName()
	{
		CLAM::InControlArray
			inControls(4, "in", this, &ControlsTest::ControlHandlerId);

		CPPUNIT_ASSERT_EQUAL( 
			std::string("in_3"), 
			inControls[3].GetName() );
	}

	void testInControlTmplArray_ReceivesControl_HandlerReceivesControlAndId()
	{
		CLAM::InControlArray
			ins( 2, /*num controls*/
				"in", 
				this, /*parent to publish (the fixture class is impersonating a processing)*/ 
				&ControlsTest::ControlHandlerId ); // calls this->PublishInControl
				
		ins[1].DoControl( -1.0 );
		CPPUNIT_ASSERT_EQUAL( 
			std::string( "ControlHandler called with id : 1 and value : -1" ),
			GetLog() );
	}


	void testIsConnected_WithOutControl_AfterConnection()
	{	
		CLAM::FloatInControl in("in");
		CLAM::FloatOutControl out("out");
		out.AddLink(in);
		CPPUNIT_ASSERT_EQUAL( true, out.IsConnected() );
	}

	void testIsConnected_WithOutControl_WithoutConnection()
	{	
		CLAM::FloatOutControl out("out");
		CPPUNIT_ASSERT_EQUAL(  false, out.IsConnected() );		
	}


	void testIsConnectedTo_WithInControl_WhenControlsAreConnected()
	{
		CLAM::FloatInControl in("in");
		CLAM::FloatOutControl out("out");
		out.AddLink(in);
		CPPUNIT_ASSERT_EQUAL( true, in.IsConnectedTo(out) );

	}

	void testIsConnectedTo_WithInControl_WhenControlsAreNotConnected()
	{
		CLAM::FloatInControl in("in");
		CLAM::FloatOutControl out("out");
		CPPUNIT_ASSERT_EQUAL( false, in.IsConnectedTo(out) );

	}



	void testIsConnectedTo_WithOutControl_WhenControlsAreConnected()
	{
		CLAM::FloatInControl in("in");
		CLAM::FloatOutControl out("out");
		out.AddLink(in);
		CPPUNIT_ASSERT_EQUAL( true, out.IsConnectedTo(in) );
	}

	void testIsConnectedTo_WithOutControl_WhenControlsAreNotConnected()
	{
		CLAM::FloatInControl in("in");
		CLAM::FloatOutControl out("out");
		CPPUNIT_ASSERT_EQUAL( false, out.IsConnectedTo(in) );
	}

	void testOutControlPublisher()
	{
		CLAM::FloatInControl in("in");
		CLAM::FloatOutControl out("out");
		CLAM::OutControlPublisher outPublisher;
		outPublisher.PublishOutControl( out );
		CLAM::FloatOutControl& publisherBaseRef = outPublisher;
		publisherBaseRef.AddLink(in);
		out.SendControl( 1.f );
		CPPUNIT_ASSERT_EQUAL( 1.f, in.GetLastValue() );
	}
	
	void testInControlPublisher()
	{
		CLAM::FloatInControl in("in");
		CLAM::FloatOutControl out("out");
		CLAM::InControlPublisher inPublisher;
		inPublisher.PublishInControl( in );
		CLAM::FloatInControl& publisherBaseRef = inPublisher;
		out.AddLink( publisherBaseRef );
		out.SendControl( 1.f );
		CPPUNIT_ASSERT_EQUAL( 1.f, in.GetLastValue() );
	}
	
	
	class DummyProcessing : public CLAM::Processing
	{
		public:

			CLAM::OutControlPublisher outControlPublisher;
			CLAM::InControlPublisher inControlPublisher;

			DummyProcessing()
				: outControlPublisher( "PublisherTestOut", this )
				, inControlPublisher( "PublisherTestIn", this )
			 {}

			const char* GetClassName() const { return "dummy processing"; }
			bool Do() { return false; }
			const CLAM::ProcessingConfig& GetConfig() const { throw 0; }
			bool ConcreteConfigure( const CLAM::ProcessingConfig& ) { return false; }
	};

	void testOutControlPublisher_GetsRegisteredToAProcessing()
	{
		DummyProcessing proc;
		CPPUNIT_ASSERT( &proc.outControlPublisher == &(proc.GetOutControl("PublisherTestOut")) );
	}
	
	void testInControlPublisher_GetsRegisteredToAProcessing()
	{
		DummyProcessing proc;
		CPPUNIT_ASSERT( &proc.inControlPublisher == &(proc.GetInControl("PublisherTestIn")) );
	}

	void testOutControlPublisher_ConnectControlsFromPublisher()
	{
		DummyProcessing proc;
		CLAM::FloatOutControl innerControlPublished("published"); // does the role of a "hidden" inner control
		CLAM::FloatInControl receiver("receiver");
		proc.outControlPublisher.PublishOutControl( innerControlPublished );
		proc.outControlPublisher.AddLink( receiver );
		innerControlPublished.SendControl( 1.f );
		CPPUNIT_ASSERT_EQUAL( 1.f, receiver.GetLastValue() );
	}

	void testInControlPublisher_ConnectControlsFromPublisher()
	{
		DummyProcessing proc;
		CLAM::FloatInControl innerControlPublished("published"); // does the role of a "hidden" inner control
		CLAM::FloatOutControl sender("sender");
		proc.inControlPublisher.PublishInControl( innerControlPublished );
		sender.AddLink( proc.inControlPublisher );
		sender.SendControl( 1.f );
		CPPUNIT_ASSERT_EQUAL( 1.f, innerControlPublished.GetLastValue() );
	}

	void testOutControlPublisher_SendControlWhenNothingPublished()
	{
		CLAM::OutControlPublisher publisher;
		CLAM::FloatInControl receiver("receiver");
		publisher.AddLink(receiver);
		publisher.SendControl(1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f, receiver.GetLastValue() );		
	}
	
	void testInControlPublisher_DoControlWhenNothingPublished()
	{
		CLAM::InControlPublisher publisher;
		publisher.DoControl(1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f, publisher.GetLastValue() );
	}
	void testInControlIsConnected_byDefault()
	{
		CLAM::FloatInControl in("Receiver");
		CPPUNIT_ASSERT_EQUAL(false, in.IsConnected());
	}
	void testInControlIsConnected_whenConnected()
	{
		CLAM::FloatInControl in("Receiver");
		CLAM::FloatOutControl out("Sender");
		out.AddLink(in);
		CPPUNIT_ASSERT_EQUAL(true, in.IsConnected());
	}
	void testInControlIsConnected_whenDisconnected()
	{
		CLAM::FloatInControl in("Receiver");
		CLAM::FloatOutControl out("Sender");
		out.AddLink(in);
		out.RemoveLink(in);
		CPPUNIT_ASSERT_EQUAL(false, in.IsConnected());
	}
	void testInControlDestructor_disconnectsOutControl()
	{
		CLAM::FloatOutControl out("Sender");
		{
			CLAM::FloatInControl in("Receiver");
			out.AddLink(in);
		}
		CPPUNIT_ASSERT_EQUAL(false, out.IsConnected());
	}
	void testOutControlDestructor_disconnectsInControl()
	{
		CLAM::FloatInControl in("Receiver");
		{
			CLAM::FloatOutControl out("Sender");
			out.AddLink(in);
		}
		CPPUNIT_ASSERT_EQUAL(false, in.IsConnected());
	}

	void testInControl_doesntHaveBoundsBydefault()
	{
		CLAM::FloatInControl inControl("inControl");
		CPPUNIT_ASSERT_EQUAL( false, inControl.IsBounded() );
	}
	void testInControl_defaultBounds()
	{
		CLAM::FloatInControl inControl("inControl");
		CPPUNIT_ASSERT_EQUAL( 0.0f, inControl.LowerBound() );
		CPPUNIT_ASSERT_EQUAL( 1.0f, inControl.UpperBound() );
	}
	void testInControl_settingBounds()
	{
		CLAM::FloatInControl inControl("inControl");
		inControl.SetBounds(-1.f, 2.f);
		CPPUNIT_ASSERT_EQUAL( -1.f, inControl.LowerBound() );
		CPPUNIT_ASSERT_EQUAL( 2.f, inControl.UpperBound() );
	}
	void testInControl_boundedDefaultValue()
	{
		CLAM::FloatInControl inControl("inControl");
		inControl.SetBounds(0.f, 10.f);
		CPPUNIT_ASSERT_EQUAL( 5.f, inControl.DefaultValue() );
	}
	void testInControl_isBoundedWhenTrue()
	{
		CLAM::FloatInControl inControl("inControl");
		inControl.SetBounds(0.f, 0.f);
		CPPUNIT_ASSERT_EQUAL( true, inControl.IsBounded() );
	}

	void testInControl_setDefaultprevailstoBounds()
	{
		CLAM::FloatInControl inControl("inControl");
		inControl.SetBounds(0.f, 10.f);
		inControl.SetDefaultValue(0.0f);
		CPPUNIT_ASSERT_EQUAL( 0.0f, inControl.DefaultValue() );
	}
};


} // namespace

