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
#include "cppUnitHelper.hxx" // needed for assertion_traits<bool>

#include "InPort.hxx"
#include "InPortTmpl.hxx"
#include "OutPort.hxx"
#include "OutPortTmpl.hxx"
#include "InPort.hxx"
#include "Processing.hxx"
#include "DummyProcessingData.hxx"
#include "NodeTmpl.hxx"
#include "CircularStreamImpl.hxx"
#include "Processing.hxx"

namespace CLAMTest {

class PortsAndNodesTest;
CPPUNIT_TEST_SUITE_REGISTRATION( PortsAndNodesTest );


class PortsAndNodesTest : public CppUnit::TestFixture, public CLAM::Processing
{
	CPPUNIT_TEST_SUITE( PortsAndNodesTest );
	// Tests for the Port base interface and Node:
	
	
	CPPUNIT_TEST( testAttachPortsToNode_WithGeneralTemplatePorts_GetsAttachedToConcreteNode );
	CPPUNIT_TEST( testAttachOutPortToNode_WithWrongNodeTypeAsserts );
	CPPUNIT_TEST( testAttachPortsToNode_WithAudioPorts_GetsAttachedToConcreteNode );
	CPPUNIT_TEST( testAttachPortsAndGetData_WithGeneralTemplatePorts_ReadsTheWrittenData );
	CPPUNIT_TEST( testAttachPortsAndGetData_WithAudioPorts_ReadsTheWrittenData );
	
	// tests for IsAttached / AreConnected methods using Nodes
	CPPUNIT_TEST( testIsAttached_WithAudioOutPort_AfterAttachWithNode );
	CPPUNIT_TEST( testIsAttached_WithAudioInPort_AfterAttachWithNode );
	CPPUNIT_TEST( testIsAttached_WithGeneralTemplateOutPort_AfterAttachWithNode );
	CPPUNIT_TEST( testIsAttached_WithGeneralTemplateInPort_AfterAttachWithNode );

	CPPUNIT_TEST( testOutPort_IsConnectedTo_WithAudioPorts_WhenPortsAreConnectedWithNode );
	CPPUNIT_TEST( testOutPort_IsConnectedTo_WithGeneralTemplatePorts_WhenPortsAreConnectedWithNode );
	CPPUNIT_TEST( testOutPort_IsConnectedTo_WithAudioPorts_WhenPortsAreNotConnectedWithNode );
	CPPUNIT_TEST( testOutPort_IsConnectedTo_WithGeneralTemplatePorts_WhenPortsAreNotConnectedWithNode );

	// tests related to IsReadyForReading / IsReadyForWriting methods of Inport/OutPort classes
	/*
	CPPUNIT_TEST( testInPort_IsReadyForReading_WithGeneralTemplatePorts_WhenThereIsDataReady );
	CPPUNIT_TEST( testInPort_IsReadyForReading_WithGeneralTemplatePorts_WhenThereIsntDataReady );
	CPPUNIT_TEST( testOutPort_IsReadyForWriting_WithGeneralTemplatePorts_WhenThereIsDataReady );
	CPPUNIT_TEST( testOutPort_IsReadyForWriting_WithGeneralTemplatePorts_WhenThereIsntDataReady );
	*/	
	CPPUNIT_TEST( testInPort_IsReadyForReading_WithAudioPorts_WhenThereIsDataReady );
	CPPUNIT_TEST( testInPort_IsReadyForReading_WithAudioPorts_WhenThereIsntDataReady );
	CPPUNIT_TEST( testOutPort_IsReadyForWriting_WithAudioPorts_WhenThereIsDataReady );
	CPPUNIT_TEST( testOutPort_IsReadyForWriting_WithAudioPorts_WhenThereIsntDataReady );

	CPPUNIT_TEST( testInPort_IsReadyForReading_WithAudioPorts_WhenIsNotAttachedToNode );
	CPPUNIT_TEST( testOutPort_IsReadyForWriting_WithAudioPorts_WhenIsNotAttachedToNode );
	CPPUNIT_TEST( testInPort_IsReadyForReading_WithGeneralTemplatePorts_WhenIsNotAttachedToNode );
	CPPUNIT_TEST( testOutPort_IsReadyForWriting_WithGeneralTemplatePorts_WhenIsNotAttachedToNode );
	
	CPPUNIT_TEST( testOutPort_IsReadyForWriting_WithAudioPorts_ConnectedToNodeWithoutReaders );


	CPPUNIT_TEST( testUnattach_WithAudioInPort_AfterAttach );
	CPPUNIT_TEST( testUnattach_WithGeneralTemplateInPort_AfterAttach );
	CPPUNIT_TEST( testAttach_WithAudioInPort_AfterUnattach );
	CPPUNIT_TEST( testAttach_WithGeneralTemplateInPort_AfterUnattach );

	CPPUNIT_TEST_SUITE_END();

	// Testing pattern: Self Shunt
	// Processing interface:
	const char* GetClassName() const { return "for testing"; }
	bool Do() { return false; }
	const CLAM::ProcessingConfig& GetConfig() const { throw 0; }
	bool ConcreteConfigure( const CLAM::ProcessingConfig& ) { return false; }
	

	void testAttachPortsToNode_WithGeneralTemplatePorts_GetsAttachedToConcreteNode()
	{
		const int dummyLength = 1;
		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;
		CLAM::InPortTmpl<DummyProcessingData> concreteInPort("in", this, dummyLength );
		CLAM::InPort& in = concreteInPort;

		CLAM::OutPortTmpl<DummyProcessingData> concreteOutPort("out", this, dummyLength );
		CLAM::OutPort& out = concreteOutPort;

		out.Attach(concreteNode);
		in.Attach(concreteNode);

		CLAM::NodeBase* theNode = &concreteNode;
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Node not attached", theNode, in.GetNode() );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Node not attached", theNode, out.GetNode() );
	}
	
	void testAttachOutPortToNode_WithWrongNodeTypeAsserts()
	{
		const int dummyLength = 1;
		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;
		CLAM::OutPortTmpl<CLAM::Audio> audioOutPort("out", this, dummyLength );
		CLAM::OutPort& out = audioOutPort;
		
		try {
			out.Attach(concreteNode);
			CPPUNIT_FAIL("Assert expected but nothing happened");
		} catch ( CLAM::ErrAssertionFailed& ) {}
	}

	void testAttachPortsToNode_WithAudioPorts_GetsAttachedToConcreteNode()
	{
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode;

		const int dummyLength=1; 
		CLAM::InPortTmpl<CLAM::Audio> concreteInPort("in", this, dummyLength);
		CLAM::InPort& in = concreteInPort;

		CLAM::OutPortTmpl<CLAM::Audio> concreteOutPort("out", this, dummyLength);
		CLAM::OutPort& out = concreteOutPort;

        out.Attach(concreteNode);
		in.Attach(concreteNode);

		CLAM::NodeBase* theNode = &concreteNode;
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Node not attached", theNode, in.GetNode() );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Node not attached", theNode, out.GetNode() );
	}
	
	void testAttachPortsAndGetData_WithGeneralTemplatePorts_ReadsTheWrittenData()
	{
		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;

		const int length=1; 
		CLAM::InPortTmpl<DummyProcessingData> concreteIn("in", this, length);
		CLAM::InPort& in = concreteIn;
	
		CLAM::OutPortTmpl<DummyProcessingData> concreteOut("out", this, length);
		CLAM::OutPort& out = concreteOut;
		
		// attach using generic interface
		out.Attach(concreteNode);
		in.Attach(concreteNode);
		
		// configure node before using its data
		concreteNode.Configure(length);

		// write with out port
		concreteOut.GetData().SetState(1);
		concreteOut.LeaveData();

		DummyProcessingData& returned = concreteIn.GetData();
		CPPUNIT_ASSERT_EQUAL( 1, returned.GetState() );
	}

	void testAttachPortsAndGetData_WithAudioPorts_ReadsTheWrittenData()
	{
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode;

		const int length=1; 
		CLAM::InPortTmpl<CLAM::Audio> concreteIn("in", this, length);
		CLAM::InPort& in = concreteIn;
	
		CLAM::OutPortTmpl<CLAM::Audio> concreteOut("out", this, length);
		CLAM::OutPort& out = concreteOut;
		
		// attach using generic interface
		out.Attach(concreteNode);
		in.Attach(concreteNode);
		
		// configure node before using its data
		concreteNode.Configure(length);

		// write with out port

		concreteOut.GetData().GetBuffer()[0]=1;
		concreteOut.LeaveData();

		CLAM::Audio returned = concreteIn.GetData();
		CPPUNIT_ASSERT_EQUAL( CLAM::TData(1), returned.GetBuffer()[0] );
		CPPUNIT_ASSERT_EQUAL( 1, returned.GetSize() );
	}

	void testIsAttached_WithAudioOutPort_AfterAttachWithNode()
	{
		const int length=1; 
		CLAM::OutPortTmpl<CLAM::Audio> concreteOut("out", this, length);

		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode;

		concreteOut.Attach(concreteNode);
		CPPUNIT_ASSERT_EQUAL( true, concreteOut.IsAttached() );
	}

	void testIsAttached_WithAudioInPort_AfterAttachWithNode()
	{

		const int length=1; 
		CLAM::OutPortTmpl<CLAM::Audio> concreteOut("out", this, length);
		CLAM::InPortTmpl<CLAM::Audio> concreteIn("out", this, length);

		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode;
	
		concreteOut.Attach(concreteNode);
		concreteIn.Attach(concreteNode);
		CPPUNIT_ASSERT_EQUAL( true, concreteIn.IsAttached() );
	}

	void testIsAttached_WithGeneralTemplateOutPort_AfterAttachWithNode()
	{
		const int length=1; 
		CLAM::OutPortTmpl<DummyProcessingData> concreteOut("out", this, length);
		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;

		concreteOut.Attach(concreteNode);
		CPPUNIT_ASSERT_EQUAL( true, concreteOut.IsAttached() );
	}

	void testIsAttached_WithGeneralTemplateInPort_AfterAttachWithNode()
	{
		const int length=1; 

		CLAM::OutPortTmpl<DummyProcessingData> concreteOut("out", this, length);
		CLAM::InPortTmpl<DummyProcessingData> concreteIn("out", this, length);

		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;

		concreteOut.Attach(concreteNode);
		concreteIn.Attach(concreteNode);
		CPPUNIT_ASSERT_EQUAL( true, concreteIn.IsAttached() );
	}

	void testOutPort_IsConnectedTo_WithAudioPorts_WhenPortsAreConnectedWithNode()
	{
		const int length=1; 
		CLAM::OutPortTmpl<CLAM::Audio> concreteOut("out", this, length);
		CLAM::InPortTmpl<CLAM::Audio> concreteIn("out", this, length);

		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode;

		concreteOut.Attach(concreteNode);
		concreteIn.Attach(concreteNode);
		CPPUNIT_ASSERT_EQUAL( true, concreteOut.IsConnectedTo(concreteIn) );
	}

	void testOutPort_IsConnectedTo_WithGeneralTemplatePorts_WhenPortsAreConnectedWithNode()
	{
		const int length=1; 
		CLAM::OutPortTmpl<DummyProcessingData> concreteOut("out", this, length);
		CLAM::InPortTmpl<DummyProcessingData> concreteIn("out", this, length);

		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;

		concreteOut.Attach(concreteNode);
		concreteIn.Attach(concreteNode);
		CPPUNIT_ASSERT_EQUAL( true, concreteOut.IsConnectedTo(concreteIn) );
	}

	void testOutPort_IsConnectedTo_WithAudioPorts_WhenPortsAreNotConnectedWithNode()
	{
		const int length=1; 
		CLAM::OutPortTmpl<CLAM::Audio> concreteOut("out", this, length);
		CLAM::InPortTmpl<CLAM::Audio> concreteIn("out", this, length);

		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode;

		concreteOut.Attach(concreteNode);
		CPPUNIT_ASSERT_EQUAL( false, concreteOut.IsConnectedTo(concreteIn) );
	}

	void testOutPort_IsConnectedTo_WithGeneralTemplatePorts_WhenPortsAreNotConnectedWithNode()
	{
		const int length=1; 
		CLAM::OutPortTmpl<DummyProcessingData> concreteOut("out", this, length);
		CLAM::InPortTmpl<DummyProcessingData> concreteIn("out", this, length);
		CPPUNIT_ASSERT_EQUAL( false, concreteOut.IsConnectedTo(concreteIn) );
	}
	
	void testInPort_IsReadyForReading_WithAudioPorts_WhenIsNotAttachedToNode()
	{
		const int length=1;
		CLAM::InPortTmpl< CLAM::Audio > in("in", this, length );
		CPPUNIT_ASSERT_EQUAL( false, in.IsReadyForReading() );
	}

	void testOutPort_IsReadyForWriting_WithAudioPorts_WhenIsNotAttachedToNode()
	{
		const int length=1;
		CLAM::OutPortTmpl< DummyProcessingData > out("out", this, length );
		CPPUNIT_ASSERT_EQUAL( false, out.IsReadyForWriting() );
	}

	void testInPort_IsReadyForReading_WithGeneralTemplatePorts_WhenIsNotAttachedToNode()
	{
		const int length=1;
		CLAM::InPortTmpl< CLAM::Audio > in("in", this, length );
		CPPUNIT_ASSERT_EQUAL( false, in.IsReadyForReading() );
	}

	void testOutPort_IsReadyForWriting_WithGeneralTemplatePorts_WhenIsNotAttachedToNode()
	{
		const int length=1;
		CLAM::OutPortTmpl< DummyProcessingData > out("out", this, length );
		CPPUNIT_ASSERT_EQUAL( false, out.IsReadyForWriting() );
	}

	void testInPort_IsReadyForReading_WithAudioPorts_WhenThereIsDataReady()
	{
		const int length=1; 
		CLAM::OutPortTmpl<CLAM::Audio> out("out", this, length);
		CLAM::InPortTmpl<CLAM::Audio> in("out", this, length);
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> >  node;
		
		out.Attach(node);
		in.Attach(node);
		node.Configure(length);

		out.GetData();
		out.LeaveData();
		
		CPPUNIT_ASSERT_EQUAL( true, in.IsReadyForReading() );
	}

	void testInPort_IsReadyForReading_WithAudioPorts_WhenThereIsntDataReady()
	{
		const int length=1; 
		CLAM::OutPortTmpl<CLAM::Audio> out("out", this, length);
		CLAM::InPortTmpl<CLAM::Audio> in("out", this, length);
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> >  node;
		
		out.Attach(node);
		in.Attach(node);
		node.Configure(length);
	
		CPPUNIT_ASSERT_EQUAL( false, in.IsReadyForReading() );
	}

	void testOutPort_IsReadyForWriting_WithAudioPorts_WhenThereIsDataReady()
	{
		const int length=1; 
		CLAM::OutPortTmpl<CLAM::Audio> out("out", this, length);
		CLAM::InPortTmpl<CLAM::Audio> in("out", this, length);
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> >  node;
		
		out.Attach(node);
		in.Attach(node);
		node.Configure(length);
	
		CPPUNIT_ASSERT_EQUAL( true, out.IsReadyForWriting() );
	}

	void testOutPort_IsReadyForWriting_WithAudioPorts_WhenThereIsntDataReady()
	{
		const int length=1; 
		CLAM::OutPortTmpl<CLAM::Audio> out("out", this, length);
		CLAM::InPortTmpl<CLAM::Audio> in("out", this, length);
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> >  node;
		
		out.Attach(node);
		in.Attach(node);
		node.Configure(length);

		out.GetData();
		out.LeaveData();
	
		CPPUNIT_ASSERT_EQUAL_MESSAGE( 
			"shouldn't be ReadyForWriting since node is size==1, and port already have LeaveData once",
			false, out.IsReadyForWriting() );
	}

	void testOutPort_IsReadyForWriting_WithAudioPorts_ConnectedToNodeWithoutReaders()
	{
		const int length=1; 
		CLAM::OutPortTmpl<CLAM::Audio> out("out", this, length);
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> >  node;
		out.Attach(node);
		node.Configure(length);
		CPPUNIT_ASSERT_EQUAL_MESSAGE( "nodes are allowed to have source and no readers. But no the other way around",
			true, out.IsReadyForWriting() );
	}

//-------------------------------------------------------------------- 

	void testUnattach_WithAudioInPort_AfterAttach()
	{
		const int samples=1; 
		CLAM::OutPortTmpl<CLAM::Audio> concreteOut("out", this, samples);
		CLAM::InPortTmpl<CLAM::Audio> concreteIn("in", this, samples);
		
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode;

		concreteOut.Attach(concreteNode);
		concreteIn.Attach(concreteNode);
		concreteIn.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, concreteOut.IsConnectedTo(concreteIn) );
	}
	
	void testUnattach_WithGeneralTemplateInPort_AfterAttach()
	{		
		const int samples=1; 
		CLAM::OutPortTmpl<DummyProcessingData> concreteOut("out", this, samples);
		CLAM::InPortTmpl<DummyProcessingData> concreteIn("in", this, samples);
		
		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;

		concreteOut.Attach(concreteNode);
		concreteIn.Attach(concreteNode);
		concreteIn.Unattach();
		CPPUNIT_ASSERT_EQUAL( false, concreteOut.IsConnectedTo(concreteIn) );

	}

	void testAttach_WithAudioInPort_AfterUnattach()
	{
		const int samples=1; 
		CLAM::OutPortTmpl<CLAM::Audio> concreteOut("out", this, samples);
		CLAM::OutPortTmpl<CLAM::Audio> concreteOut2("out2", this, samples);
		CLAM::InPortTmpl<CLAM::Audio> concreteIn("in", this, samples);
		
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode;
		CLAM::NodeTmpl<CLAM::Audio, CLAM::CircularStreamImpl<CLAM::TData> > 
			concreteNode2;

		concreteOut.Attach(concreteNode);
		concreteIn.Attach(concreteNode);
		concreteIn.Unattach();
		concreteOut2.Attach(concreteNode2);
		concreteIn.Attach(concreteNode2);
		CPPUNIT_ASSERT_EQUAL( true, concreteOut2.IsConnectedTo(concreteIn) );	
	}


	void testAttach_WithGeneralTemplateInPort_AfterUnattach()
	{
		const int samples=1; 
		CLAM::OutPortTmpl<DummyProcessingData> concreteOut("out", this, samples);
		CLAM::OutPortTmpl<DummyProcessingData> concreteOut2("out2", this, samples);
		CLAM::InPortTmpl<DummyProcessingData> concreteIn("in", this, samples);
		
		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode;
		CLAM::NodeTmpl<DummyProcessingData, CLAM::CircularStreamImpl<DummyProcessingData> > 
			concreteNode2;

		concreteOut.Attach(concreteNode);
		concreteIn.Attach(concreteNode);
		concreteIn.Unattach();
		concreteOut2.Attach(concreteNode2);
		concreteIn.Attach(concreteNode2);
		CPPUNIT_ASSERT_EQUAL( true, concreteOut2.IsConnectedTo(concreteIn) );	
	}

	
};

} // namespace
