#include <cppunit/extensions/HelperMacros.h>
#include "InPort.hxx"
#include "OutPort.hxx"
#include "InPortPublisher.hxx"
#include "OutPortPublisher.hxx"

#include "AudioInPortPublisher.hxx"
#include "AudioOutPortPublisher.hxx"
#include "AudioInPort.hxx"
#include "AudioOutPort.hxx"

namespace CLAMTest {

class TestsPortsRegionsInteraction ;
CPPUNIT_TEST_SUITE_REGISTRATION( TestsPortsRegionsInteraction );

class TestsPortsRegionsInteraction : public CppUnit::TestFixture
{
public:
	CPPUNIT_TEST_SUITE( TestsPortsRegionsInteraction );

	CPPUNIT_TEST( testOutPortConnectToIn_usingBaseClass );
	CPPUNIT_TEST( testOutPortConnect );
	CPPUNIT_TEST( testOutPortConnect_whenPortsAlreadyConnected ); 
	CPPUNIT_TEST( testOutPortConnect_whenInPortAlreadyConnectedToAnotherOutPort );
	CPPUNIT_TEST( testOutPortConnect_whenMoreThanOneInPort ); 
	CPPUNIT_TEST( testOutPortConnectNotifiesInPort );
	CPPUNIT_TEST( testOutPortDisconnect_whenPortsAreConnected );
	CPPUNIT_TEST( testOutPortDisconnect_whenPortsAreConnected_usingBaseClass );
	CPPUNIT_TEST( testOutPortDisconnect_whenPortsAreNotConnected_throwsException ); 
	CPPUNIT_TEST( testOutPortDisconnect_whenMoreThanOneInPort ); 
	CPPUNIT_TEST( testOutPortDisconnectNotifiesInPort );
	CPPUNIT_TEST( testPortChangeSizeResizesRegion ); 
	CPPUNIT_TEST( testProduceAndConsume );
	CPPUNIT_TEST( testProduceAndConsume_whenMoreThanOneInPort ); 
	CPPUNIT_TEST( testOutPortGetConnectedInPorts_whenOneInPort ); 
	CPPUNIT_TEST( testOutPortGetConnectedInPorts_whenMoreThanOneInPort );
	CPPUNIT_TEST( testInPortGetConnectedOutPort );
	CPPUNIT_TEST( testOutPortDisconnectFromAll );	
	CPPUNIT_TEST( testOutPort_IsConnectableTo_WhenInPortIsTheSameType );
	CPPUNIT_TEST( testOutPort_IsConnectableTo_WhenInPortIsDifferentType );
	CPPUNIT_TEST( testConnectedPortsDestructors_whenInDeletedBefore );
	CPPUNIT_TEST( testInPortPublisher_PublishInPort_withIncorrectInPort );
	CPPUNIT_TEST( testInPortPublisher_PublishInPort_withProperInPort );
	CPPUNIT_TEST( testOutPortPublisher_PublishOutPort_withIncorrectOutPort );
	CPPUNIT_TEST( testOutPortPublisher_PublishOutPort_withProperOutPort );
	CPPUNIT_TEST( testInPortPublisher_PublishInPort_withSomeInPorts );
	CPPUNIT_TEST( testOutPort_GetConnectedInPorts_whenConnectedToInPortPublisher );
	CPPUNIT_TEST( testOutPortPublisher_SetSize_assertsWhenDoesntPublishAnyPort );
	CPPUNIT_TEST( testAudioOutPortPublisher_SetSize_assertsWhenDoesntPublishAnyPort );
	CPPUNIT_TEST( testAudioOutPort_DefaultSize );
	CPPUNIT_TEST( testAudioInPort_DefaultSize );
	
	CPPUNIT_TEST( testOutPort_IsPhysicallyConnectedToIn_withOneInPort );
	CPPUNIT_TEST( testGetLastWrittenData_whenPortIsWrongType_throwsException );
	CPPUNIT_TEST( testGetLastWrittenData_fillsWithCorrectData );
	// delete sequence tests:
	CPPUNIT_TEST( testOutPort_deleteSequence_Out_In );
	CPPUNIT_TEST( testPort_deleteSequence_In_Out );
	CPPUNIT_TEST( testPortPublisher_deleteSequence_In_InPub_Out );
	CPPUNIT_TEST( testPortPublisher_deleteSequence_Out_InPub_In );
	CPPUNIT_TEST( testAudioInPortPublisher_deleteSequence_Out_InPub_In ); 

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
	}
	void tearDown()
	{
	}
	void testOutPortConnectToIn_usingBaseClass()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;
		CLAM::OutPortBase& outBase = out;
		CLAM::InPortBase& inBase = in;

		outBase.ConnectToIn(inBase);

		CPPUNIT_ASSERT_EQUAL( true, outBase.IsDirectlyConnectedTo(inBase) );
	}
	
	void testOutPortConnect()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;
		out.ConnectToIn(in);
		//TODO check that stream has been initialized (as soon as is implemented)

		CPPUNIT_ASSERT_EQUAL( true, out.IsDirectlyConnectedTo( in ) );
	}

	void testOutPortConnect_whenMoreThanOneInPort()
	{	
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in1, in2;
		out.ConnectToIn(in1);
		out.ConnectToIn(in2);

		CPPUNIT_ASSERT_EQUAL( true, out.IsDirectlyConnectedTo(in1) );
		CPPUNIT_ASSERT_EQUAL( true, out.IsDirectlyConnectedTo(in2) );
	}

	void testOutPortConnect_whenPortsAlreadyConnected()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;
		out.ConnectToIn(in);

		try
		{
			out.ConnectToIn(in);	
			CPPUNIT_FAIL("exception should be thrown");
		}
		catch( CLAM::ErrAssertionFailed& )
		{
		}
	}
		
	void testOutPortConnect_whenInPortAlreadyConnectedToAnotherOutPort()
	{
		CLAM::OutPort<int> out, out2;
		CLAM::InPort<int> in;
		out.ConnectToIn(in);

		try
		{
			out2.ConnectToIn(in);	
			CPPUNIT_FAIL("exception should be thrown");
		}
		catch( CLAM::ErrAssertionFailed& )
		{
		}
	}

	void testOutPortConnectNotifiesInPort()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in; 
		CPPUNIT_ASSERT( 0 == in.GetAttachedOutPort() );
		out.ConnectToConcreteIn(in);
		CPPUNIT_ASSERT( &out == in.GetAttachedOutPort() );
	}
	void testOutPortDisconnect_whenPortsAreConnected()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;		
		out.ConnectToConcreteIn(in);

		out.DisconnectFromConcreteIn( in );
		CPPUNIT_ASSERT(0 ==  in.GetAttachedOutPort() );
		CPPUNIT_ASSERT( out.BeginConnectedInPorts() == out.EndConnectedInPorts() );
	}

	void testOutPortDisconnect_whenPortsAreConnected_usingBaseClass()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;
		CLAM::OutPortBase& outBase = out;
		CLAM::InPortBase& inBase = in;
		outBase.ConnectToIn(inBase);

		outBase.DisconnectFromIn(inBase);

		CPPUNIT_ASSERT_EQUAL( false, outBase.IsDirectlyConnectedTo(inBase) );
	}

	void testOutPortDisconnect_whenPortsAreNotConnected_throwsException()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;		

		try
		{
			out.DisconnectFromConcreteIn( in );
			CPPUNIT_FAIL("exception should be thrown");
		}
		catch( CLAM::ErrAssertionFailed& )
		{
		}
	}

	void testOutPortDisconnect_whenMoreThanOneInPort()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in1, in2, in3;		
		out.ConnectToConcreteIn(in1);
		out.ConnectToConcreteIn(in2);
		out.ConnectToConcreteIn(in3);

		out.DisconnectFromConcreteIn(in2);

		CPPUNIT_ASSERT_EQUAL( false, out.IsDirectlyConnectedTo(in2) );
	}

	void testOutPortDisconnectNotifiesInPort()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in; 
		out.ConnectToConcreteIn(in);

		out.DisconnectFromConcreteIn(in);

		CPPUNIT_ASSERT( 0 == in.GetAttachedOutPort() );
	}

	void testPortChangeSizeResizesRegion()
	{
		CLAM::OutPort<int> out;
		int newSize = 5;
		out.SetSize(5);

		CPPUNIT_ASSERT_EQUAL( newSize, out.GetSize() );
	}

	void testProduceAndConsume()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;
		int data = 4;

		out.ConnectToIn(in);
		out.GetData() = 4;
		out.Produce();
		
		CPPUNIT_ASSERT_EQUAL( data, in.GetData() );
	}

	void testProduceAndConsume_whenMoreThanOneInPort()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in1, in2, in3;
		int data1 = 1;
		int data2 = 2;
		int data3 = 3;

		out.ConnectToConcreteIn(in1);
		out.ConnectToConcreteIn(in2);
		out.ConnectToConcreteIn(in3);

		in1.SetSize(2);
		in3.SetSize(3);

		out.GetData() = data1;
		out.Produce();
		out.GetData() = data2;
		out.Produce();
		out.GetData() = data3;
		out.Produce();

		CPPUNIT_ASSERT_EQUAL( data1, in1.GetData() );
		CPPUNIT_ASSERT_EQUAL( data2, in1.GetData(1) );
		
		CPPUNIT_ASSERT_EQUAL( data1, in2.GetData() );
		
		CPPUNIT_ASSERT_EQUAL( data1, in3.GetData() );
		CPPUNIT_ASSERT_EQUAL( data2, in3.GetData(1) );
		CPPUNIT_ASSERT_EQUAL( data3, in3.GetData(2) );

	}
	void testOutPortGetConnectedInPorts_whenOneInPort()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;
		out.ConnectToIn( in );
		
		CLAM::OutPortBase::InPortsList::iterator it = out.BeginConnectedInPorts(); 
		CPPUNIT_ASSERT( it != out.EndConnectedInPorts() );
		CPPUNIT_ASSERT( *(it++) = &in );
		CPPUNIT_ASSERT( it == out.EndConnectedInPorts() );
		
	}
	
	void testOutPortGetConnectedInPorts_whenMoreThanOneInPort()
	{	
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in1, in2, in3;
		out.ConnectToIn( in1 );
		out.ConnectToIn( in2 );
		out.ConnectToIn( in3 );
		
		CLAM::OutPortBase::InPortsList::iterator it = out.BeginConnectedInPorts(); 
		CPPUNIT_ASSERT( it != out.EndConnectedInPorts() );
		CPPUNIT_ASSERT( *(it++) = &in1 );
		CPPUNIT_ASSERT( *(it++) = &in2 );
		CPPUNIT_ASSERT( *(it++) = &in3 );
		CPPUNIT_ASSERT( it == out.EndConnectedInPorts() );

	}
	void testInPortGetConnectedOutPort()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in1, in2;
		out.ConnectToIn( in1 );
		out.ConnectToIn( in2 );

		CPPUNIT_ASSERT( &out == in1.GetAttachedOutPort() );
		CPPUNIT_ASSERT( &out == in2.GetAttachedOutPort() );
	}

	void testOutPortDisconnectFromAll()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in1, in2, in3;
		out.ConnectToIn( in1 );
		out.ConnectToIn( in2 );
		out.ConnectToIn( in3 );

		out.DisconnectFromAll();

		CPPUNIT_ASSERT( out.BeginConnectedInPorts() == out.EndConnectedInPorts() );
		CPPUNIT_ASSERT( 0 == in1.GetProcessing() );
		CPPUNIT_ASSERT( 0 == in2.GetProcessing() );
		CPPUNIT_ASSERT( 0 == in3.GetProcessing() );
	}

	void testOutPort_IsConnectableTo_WhenInPortIsTheSameType()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;

		CLAM::OutPortBase & baseOutPort = out;
		CLAM::InPortBase & baseInPort = in;

		CPPUNIT_ASSERT_EQUAL( true, baseOutPort.IsConnectableTo(baseInPort) );
	}

	void testOutPort_IsConnectableTo_WhenInPortIsDifferentType()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<char> in;

		CLAM::OutPortBase & baseOutPort = out;
		CLAM::InPortBase & baseInPort = in;

		CPPUNIT_ASSERT_EQUAL( false, baseOutPort.IsConnectableTo(baseInPort) );
	}

	void testInPortPublisher_PublishInPort_withIncorrectInPort()
	{
		CLAM::InPort<int> in;
		CLAM::InPortPublisher<char> inPublisher;

		try
		{
			inPublisher.PublishInPort( in );
			CPPUNIT_FAIL( "Assertion should fail" ); 
		}
		catch( CLAM::ErrAssertionFailed & )
		{
		}
	}
	
	void testConnectedPortsDestructors_whenInDeletedBefore()	
	{
		CLAM::OutPort<int> *out = new CLAM::OutPort<int>();
		CLAM::InPort<int> *in = new CLAM::InPort<int>();
		CLAM::InPortPublisher<int> *inPublisher = new CLAM::InPortPublisher<int>();
		
		inPublisher->PublishInPort( *in );
		out->ConnectToIn(*inPublisher);
		
		delete inPublisher;
		delete out;		
	}
	
	void testInPortPublisher_PublishInPort_withProperInPort()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;
		CLAM::InPortPublisher<int> inPublisher;

		inPublisher.PublishInPort( in );
		int data = 4;

		out.ConnectToIn(inPublisher);
		out.GetData() = data;
		out.Produce();
		
		CPPUNIT_ASSERT_EQUAL( data, in.GetData() );

	}
	
	void testOutPortPublisher_PublishOutPort_withIncorrectOutPort()
	{
		CLAM::OutPort<int> out;
		CLAM::OutPortPublisher<char> outPublisher;

		try
		{
			outPublisher.PublishOutPort( out );
			CPPUNIT_FAIL( "Assertion should fail" ); 
		}
		catch( CLAM::ErrAssertionFailed & )
		{
		}
	}
	
	void testOutPortPublisher_PublishOutPort_withProperOutPort()
	{
		
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;
		CLAM::OutPortPublisher<int> outPublisher;
		outPublisher.PublishOutPort( out );

		int data = 4;

		outPublisher.ConnectToIn(in);
		out.GetData() = data;

		CPPUNIT_ASSERT_EQUAL( data, outPublisher.GetData() );
		
		out.Produce();
		CPPUNIT_ASSERT_EQUAL( data, in.GetData() );

	}

	void testInPortPublisher_PublishInPort_withSomeInPorts()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in;
		CLAM::InPort<int> in2;
		CLAM::InPort<int> in3;
		CLAM::InPortPublisher<int> inPublisher;

		inPublisher.PublishInPort( in );
		inPublisher.PublishInPort( in2 );
		inPublisher.PublishInPort( in3 );
		int data = 4;

		out.ConnectToIn(inPublisher);
		out.GetData() = data;
		out.Produce();
		
		CPPUNIT_ASSERT_EQUAL( data, in.GetData() );
		CPPUNIT_ASSERT_EQUAL( data, in2.GetData() );
		CPPUNIT_ASSERT_EQUAL( data, in3.GetData() );

	}

	void testOutPort_GetConnectedInPorts_whenConnectedToInPortPublisher()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> in1, in2, in3;
		CLAM::InPortPublisher<int> pubIn;
		pubIn.PublishInPort( in1 );
		pubIn.PublishInPort( in2 );
		pubIn.PublishInPort( in3 );
		out.ConnectToIn( pubIn );
		
		// let's check that the connected list of pubOut have a unique
		// entry (which is &pubIn)
		CLAM::OutPortBase::InPortsList::iterator it = out.BeginConnectedInPorts(); 
		CPPUNIT_ASSERT( it != out.EndConnectedInPorts() );
		CPPUNIT_ASSERT( *(it++) = &pubIn );
		CPPUNIT_ASSERT( it == out.EndConnectedInPorts() );
		
	}
	void testOutPortPublisher_SetSize_assertsWhenDoesntPublishAnyPort()
	{
		CLAM::OutPortPublisher<int> pubOut;
		try
		{
			pubOut.GetSize();
			CPPUNIT_FAIL("as assertion was expected from OutPortPublisher::GetSize()");
		}
		catch( CLAM::ErrAssertionFailed& )
		{
		}
		try 
		{
			pubOut.GetHop();
			CPPUNIT_FAIL("as assertion was expected from OutPortPublisher::GetHop()");
		}
		catch( CLAM::ErrAssertionFailed& )
		{
		}
	}
	void testAudioOutPortPublisher_SetSize_assertsWhenDoesntPublishAnyPort()
	{
		CLAM::AudioOutPortPublisher pubOut;
		try
		{
			pubOut.GetSize();
			CPPUNIT_FAIL("as assertion was expected from OutPortPublisher::GetSize()");
		}
		catch( CLAM::ErrAssertionFailed& )
		{
		}
		try 
		{
			pubOut.GetHop();
			CPPUNIT_FAIL("as assertion was expected from OutPortPublisher::GetHop()");
		}
		catch( CLAM::ErrAssertionFailed& )
		{
		}
	}
	void testAudioOutPort_DefaultSize()
	{
		CLAM::AudioOutPort out;
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Size not expected", 256, out.GetSize() );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Hop not expected", 256, out.GetHop() );
	}
	void testAudioInPort_DefaultSize()
	{
		CLAM::AudioInPort in;
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Size not expected", 256, in.GetSize() );
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Hop not exptected", 256, in.GetHop() );
	}
	
	void testGetLastWrittenData_whenPortIsWrongType_throwsException()
	{
		CLAM::OutPort<char> out;
		out.GetData() = 'a';
		out.Produce();

		try
		{
			int result = CLAM::OutPort<int>::GetLastWrittenData( out );
			CPPUNIT_FAIL( "Assertion should be failed" );
		}
		catch( CLAM::ErrAssertionFailed &  )
		{
		}
	}
	
	void testGetLastWrittenData_fillsWithCorrectData()
	{
		CLAM::OutPort<int> out;
		int data = 5;
		out.GetData() = data;
		out.Produce();

		int result = CLAM::OutPort<int>::GetLastWrittenData( out );

		CPPUNIT_ASSERT_EQUAL( data, result );
	}

	void testOutPort_IsPhysicallyConnectedToIn_withOneInPort() 
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> inPublished, inDirect, inNotConnected; 
		CLAM::InPortPublisher<int> publisher;

		publisher.PublishInPort( inPublished );
		out.ConnectToIn( publisher ); // so inPublished gets "physically" connected
		out.ConnectToIn( inDirect );

		CPPUNIT_ASSERT( true==out.IsPhysicallyConnectedToIn(inDirect) );
		CPPUNIT_ASSERT( true==out.IsPhysicallyConnectedToIn(inPublished) );
		CPPUNIT_ASSERT( false==out.IsPhysicallyConnectedToIn(inNotConnected) );
	}

	
	void testOutPort_deleteSequence_Out_In()
	{
		CLAM::InPort<int> * in = new CLAM::InPort<int>;
		CLAM::OutPort<int> * out = new CLAM::OutPort<int>;
		out->ConnectToIn( *in );

		delete out;
		delete in;
	}
	void testPort_deleteSequence_In_Out()
	{
		CLAM::InPort<int> * in = new CLAM::InPort<int>;
		CLAM::OutPort<int> * out = new CLAM::OutPort<int>;
		out->ConnectToIn( *in );

		delete in;
		delete out;
	}
	void testPortPublisher_deleteSequence_In_InPub_Out()
	{
		CLAM::OutPort<int> out;
		CLAM::InPort<int> *in1, *in2;
		CLAM::InPortPublisher<int> *pubIn;

		in1 = new CLAM::InPort<int>;
		in2 = new CLAM::InPort<int>;
		pubIn = new CLAM::InPortPublisher<int>;
		
		pubIn->PublishInPort( *in1 );
		pubIn->PublishInPort( *in2 );
		out.ConnectToIn( *pubIn );
		
		delete in1;
		delete in2;
		delete pubIn;

		CLAM::OutPortBase::InPortsList::iterator it = out.BeginConnectedInPorts(); 
		CPPUNIT_ASSERT(it == out.EndConnectedInPorts() );
	}
	void testPortPublisher_deleteSequence_Out_InPub_In()
	{
		CLAM::OutPort<int> *out = new CLAM::OutPort<int>;
		CLAM::InPort<int> *in = new CLAM::InPort<int>;
		CLAM::InPortPublisher<int> *pubIn = new CLAM::InPortPublisher<int>;

		pubIn->PublishInPort( *in );
		out->ConnectToIn( *pubIn );
		
		delete out;
		CPPUNIT_ASSERT( 0 == pubIn->GetAttachedOutPort() );
		delete pubIn;
		delete in;
	}
	void testAudioInPortPublisher_deleteSequence_Out_InPub_In()
	{
		CLAM::AudioInPortPublisher *pub = new CLAM::AudioInPortPublisher;
		CLAM::AudioInPort *in = new CLAM::AudioInPort;
		CLAM::AudioOutPort *out = new CLAM::AudioOutPort;

		pub->PublishInPort( *in );
		out->ConnectToIn( *pub );
		delete out;
		delete pub;
		delete in;
	}
};

} // namespace CLAMTest 

