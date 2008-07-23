#include <cppunit/extensions/HelperMacros.h>
#include "Processing.hxx" // CLAM

#include "InPort.hxx" // CLAM
#include "InControl.hxx" // CLAM
#include <CLAM/TypedInControl.hxx> // CLAM
#include "OutPort.hxx" // CLAM
#include "OutControl.hxx" // CLAM
#include <CLAM/TypedOutControl.hxx> // CLAM
#include "DummyProcessingData.hxx"

namespace CLAMTest
{

class ProcessingTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ProcessingTest );

class ProcessingTest : public CppUnit::TestFixture, public CLAM::Processing
{
	CPPUNIT_TEST_SUITE( ProcessingTest );

	CPPUNIT_TEST( testGetInControl_GetTheRightControl );
	CPPUNIT_TEST( testGetOutControl_GetTheRightControl );
	CPPUNIT_TEST( testGetTypedInControl_GetTheRightControl );
	CPPUNIT_TEST( testGetTypedOutControl_GetTheRightControl );
	CPPUNIT_TEST( testGetInControl_WithOutOfRangeIndexThrowException );
	CPPUNIT_TEST( testGetOutControl_WithOutOfRangeIndexThrowException );
	CPPUNIT_TEST( testGetTypedInControl_WithOutOfRangeIndexThrowException );
	CPPUNIT_TEST( testGetTypedOutControl_WithOutOfRangeIndexThrowException );
	CPPUNIT_TEST( testLinkAndSendControl_ChangesInControlState );
	CPPUNIT_TEST( testLinkAndSendControl_ChangesTypedInControlState );
	// new controls/ports interface.
	// todo: adapt the old tests:
	CPPUNIT_TEST( testInControls_GetByNumber_GetTheRightControl );
	CPPUNIT_TEST( testOutControls_GetByNumber_GetTheRightControl );
	CPPUNIT_TEST( testTypedInControls_GetByNumber_GetTheRightControl );
	CPPUNIT_TEST( testTypedOutControls_GetByNumber_GetTheRightControl );
	CPPUNIT_TEST( testOutPorts_GetByNumber_GetTheRightPort );
	CPPUNIT_TEST( testInPorts_GetByNumber_GetTheRightPort );
	CPPUNIT_TEST( testInControls_GetByName_GetTheRightControl );
	CPPUNIT_TEST( testOutControls_GetByName_GetTheRightControl );
	CPPUNIT_TEST( testOutPorts_GetByName_GetTheRightPort );
	CPPUNIT_TEST( testInPorts_GetByName_GetTheRightPort );
	CPPUNIT_TEST( testInControls_Size );
	CPPUNIT_TEST( testOutControls_Size );
	CPPUNIT_TEST( testTypedInControls_Size );
	CPPUNIT_TEST( testTypedOutControls_Size );
	CPPUNIT_TEST( testInPorts_Size );
	CPPUNIT_TEST( testOutPorts_Size );
	CPPUNIT_TEST( testOutPorts_Delete);
	CPPUNIT_TEST( testInPorts_Delete);
	CPPUNIT_TEST( testOutControls_Delete);
	CPPUNIT_TEST( testInControls_Delete);
	CPPUNIT_TEST( testUnRegistredOutPorts);
	CPPUNIT_TEST( testUnRegistredInPorts);
	CPPUNIT_TEST( testUnRegistredOutControls);
	CPPUNIT_TEST( testUnRegistredInControls);
		
	CPPUNIT_TEST( testConnectPorts_usingNames );
	CPPUNIT_TEST( testConnectPorts_usingNumbers );
	CPPUNIT_TEST( testConnectControls_usingNames );
	CPPUNIT_TEST( testConnectControls_usingNumbers );
	CPPUNIT_TEST( testConnectTypedControls_usingNames );
	CPPUNIT_TEST( testConnectTypedControls_usingNumbers );
	CPPUNIT_TEST( testHasTypedInControl_usingRegisteredControlName );
	CPPUNIT_TEST( testHasTypedInControl_usingNotRegisteredControlName );
	CPPUNIT_TEST( testHasTypedOutControl_usingRegisteredControlName );
	CPPUNIT_TEST( testHasTypedOutControl_usingNotRegisteredControlName );

	CPPUNIT_TEST( testIsSyncSource_default );
	
	CPPUNIT_TEST_SUITE_END();

public:
	template<class TControlData>
	class TypedInControl;
	template<class TControlData>
	class TypedOutControl;

	typedef CLAM::TypedInControl<int>* IntTypedInControlPointer;
	typedef CLAM::TypedOutControl<int>* IntTypedOutControlPointer;
	
	// void implementation the pure virtual methods of processing
	ProcessingTest() : 
		mInControl("in", this), 
		mInControlTmpl("in tmpl",this,&ProcessingTest::ControlHandler),
		mOutControl1("out1", this),
		mOutControl2("out2", this),
		
		mTypedInControl("TypedIn", this),
		mTypedOutControl("TypedOut", this),
		mTypedOutControl2("TypedOut2", this),
		
		mInPort(std::string("in"),this),
		mOutPort1("out1",this),
		mOutPort2("out2",this)
	{};

private:
	// using testing pattern Self Shunt (make the fixture be a Processing under test)
	bool ConcreteConfigure(const CLAM::ProcessingConfig&) { return false; }
	const CLAM::ProcessingConfig &GetConfig() const {
		return *static_cast<CLAM::ProcessingConfig*>(0);
	}
	bool Do(void){ return false; }
	const char * GetClassName() const { return "ProcessingTest"; };	
	int ControlHandler( CLAM::TControlData ) { return 0; };
	
	CLAM::InControl mInControl;
	CLAM::InControlTmpl<ProcessingTest> mInControlTmpl;
	CLAM::OutControl mOutControl1;
	CLAM::OutControl mOutControl2;
	
	CLAM::TypedInControl<int> mTypedInControl;
	CLAM::TypedOutControl<int> mTypedOutControl;
	CLAM::TypedOutControl<float> mTypedOutControl2;
	
	CLAM::InPort<DummyProcessingData> mInPort;
	CLAM::OutPort<DummyProcessingData> mOutPort1;
	CLAM::OutPort<DummyProcessingData> mOutPort2;

	void testGetInControl_GetTheRightControl()
	{
		CPPUNIT_ASSERT_EQUAL( std::string("in"), GetInControls().GetByNumber(0).GetName() );
	}
	void testGetOutControl_GetTheRightControl()
	{
		CPPUNIT_ASSERT_EQUAL( std::string("out1"), GetOutControls().GetByNumber(0).GetName() );
	}
	void testGetTypedInControl_GetTheRightControl()
	{
		CPPUNIT_ASSERT_EQUAL( std::string("TypedIn"), GetTypedInControls().GetByNumber(0).GetName() );
	}
	void testGetTypedOutControl_GetTheRightControl()
	{
		CPPUNIT_ASSERT_EQUAL( std::string("TypedOut"), GetTypedOutControls().GetByNumber(0).GetName() );
	}
	void testGetInControl_WithOutOfRangeIndexThrowException() 
	{
		try
		{
			GetInControls().GetByNumber(2); // we have two published in controls: indexs 0,1
			CPPUNIT_FAIL("std::out_of_range was expected but none was thrown");
		}
		catch( ... ) {}
	}
	void testGetOutControl_WithOutOfRangeIndexThrowException() 
	{
		try
		{
			GetOutControls().GetByNumber(2); // we have two published out controls: indexs 0,1
			CPPUNIT_FAIL("std::out_of_range was expected but none was thrown");
		}
		catch( ... ) {}
	}
	void testGetTypedInControl_WithOutOfRangeIndexThrowException() 
	{
		try
		{
			GetTypedInControls().GetByNumber(2); // we have one published in controls: index 0
			CPPUNIT_FAIL("std::out_of_range was expected but none was thrown");
		}
		catch( ... ) {}
	}
	void testGetTypedOutControl_WithOutOfRangeIndexThrowException() 
	{
		try
		{
			GetOutControls().GetByNumber(2); // we have two published out controls: indexs 0,1
			CPPUNIT_FAIL("std::out_of_range was expected but none was thrown");
		}
		catch( ... ) {}
	}
	void testLinkAndSendControl_ChangesInControlState()
	{
		const int outId=0, inId=0;
		GetOutControls().GetByNumber(outId).AddLink(GetInControls().GetByNumber(inId));
		SendFloatToOutControl(*this,outId,1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f, GetFloatFromInControl(*this,inId) );
	}
	void testLinkAndSendControl_ChangesTypedInControlState()
	{
		const int outId=0, inId=0;
		(dynamic_cast< IntTypedOutControlPointer > (&GetTypedOutControls().GetByNumber(outId)))->AddLink(*(dynamic_cast< IntTypedInControlPointer >(&GetTypedInControls().GetByNumber(inId))));
		(dynamic_cast< IntTypedOutControlPointer >(&GetTypedOutControls().GetByNumber(outId)))->SendControl(1);
		CPPUNIT_ASSERT_EQUAL( 1, (dynamic_cast< IntTypedInControlPointer >(&GetTypedInControls().GetByNumber(inId)))->GetLastValue() );
	}

	//---------------------------------------
	// new controls/ports interface tests

	void testInControls_GetByNumber_GetTheRightControl()
	{
		CLAM::InControl* returned = &GetInControls().GetByNumber(0);
		CLAM::InControl* expected = &mInControl;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testOutControls_GetByNumber_GetTheRightControl()
	{
		CLAM::OutControl* returned = &GetOutControls().GetByNumber(1); // get the second control
		CLAM::OutControl* expected = &mOutControl2;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testTypedInControls_GetByNumber_GetTheRightControl()
	{
		CLAM::BaseTypedInControl* returned = &GetTypedInControls().GetByNumber(0);
		CLAM::BaseTypedInControl* expected = &mTypedInControl;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testTypedOutControls_GetByNumber_GetTheRightControl()
	{
		CLAM::BaseTypedOutControl* returned = &GetTypedOutControls().GetByNumber(1); // get the second control
		CLAM::BaseTypedOutControl* expected = &mTypedOutControl2;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}

	void testOutPorts_GetByNumber_GetTheRightPort()
	{
		CLAM::OutPortBase* returnedPort = &GetOutPorts().GetByNumber(1); // get the second port
		CLAM::OutPortBase* expectedPort = &mOutPort2;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}
	void testInPorts_GetByNumber_GetTheRightPort()
	{
		CLAM::InPortBase* returnedPort = &GetInPorts().GetByNumber(0); // get the first port
		CLAM::InPortBase* expectedPort = &mInPort;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}
	void testInControls_GetByName_GetTheRightControl()
	{
		CLAM::InControl* returned = &GetInControls().Get("in");
		CLAM::InControl* expected = &mInControl;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}

	void testOutControls_GetByName_GetTheRightControl()
	{
		CLAM::OutControl* returned = &GetOutControls().Get("out2"); // get the second control
		CLAM::OutControl* expected = &mOutControl2;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testTypedInControls_GetByName_GetTheRightControl()
	{
		CLAM::BaseTypedInControl* returned = &GetTypedInControls().Get("TypedIn");
		CLAM::BaseTypedInControl* expected = &mTypedInControl;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}

	void testTypedOutControls_GetByName_GetTheRightControl()
	{
		CLAM::BaseTypedOutControl* returned = &GetTypedOutControls().Get("TypedOut2"); // get the second control
		CLAM::BaseTypedOutControl* expected = &mTypedOutControl2;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testOutPorts_GetByName_GetTheRightPort()
	{
		CLAM::OutPortBase* returnedPort = &GetOutPorts().Get("out2"); // get the second port
		CLAM::OutPortBase* expectedPort = &mOutPort2;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}
	void testInPorts_GetByName_GetTheRightPort()
	{
		CLAM::InPortBase* returnedPort = &GetInPorts().Get("in"); // get the first port
		CLAM::InPortBase* expectedPort = &mInPort;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}

	void testInControls_Size()
	{
		CPPUNIT_ASSERT_EQUAL( 2, GetInControls().Size() );
	}
	void testOutControls_Size()
	{
		CPPUNIT_ASSERT_EQUAL( 2, GetOutControls().Size() );
	}
	void testTypedInControls_Size()
	{
		CPPUNIT_ASSERT_EQUAL( 1, GetTypedInControls().Size() );
	}
	void testTypedOutControls_Size()
	{
		CPPUNIT_ASSERT_EQUAL( 2, GetTypedOutControls().Size() );
	}
	void testInPorts_Size()
	{
		CPPUNIT_ASSERT_EQUAL( 1, GetInPorts().Size() );
	}
	void testOutPorts_Size()
	{
		CPPUNIT_ASSERT_EQUAL( 2, GetOutPorts().Size() );
	}
	void testOutPorts_Delete()
	{
		int initialNumPorts = GetOutPorts().Size();
		CLAM::OutPort<int>* port = new CLAM::OutPort<int>( "out", this);
		CPPUNIT_ASSERT_EQUAL( initialNumPorts+1, GetOutPorts().Size() );
		delete port;
		CPPUNIT_ASSERT_EQUAL( initialNumPorts, GetOutPorts().Size() );

	}
	void testInPorts_Delete()
	{
		int initialNumPorts = GetInPorts().Size();
		CLAM::InPort<int>* port = new CLAM::InPort<int>( "in", this);
		CPPUNIT_ASSERT_EQUAL( initialNumPorts+1, GetInPorts().Size() );
		delete port;
		CPPUNIT_ASSERT_EQUAL( initialNumPorts, GetInPorts().Size() );

	}
	void testOutControls_Delete()
	{
		int initialNumControls = GetOutControls().Size();
		CLAM::OutControl* control = new CLAM::OutControl( "outControl", this);
		CPPUNIT_ASSERT_EQUAL( initialNumControls+1, GetOutControls().Size() );
		delete control;
		CPPUNIT_ASSERT_EQUAL( initialNumControls, GetOutControls().Size() );

	}
	void testInControls_Delete()
	{
		int initialNumControls = GetInControls().Size();
		CLAM::InControl* control = new CLAM::InControl( "inControl", this);
		CPPUNIT_ASSERT_EQUAL( initialNumControls+1, GetInControls().Size() );
		delete control;
		CPPUNIT_ASSERT_EQUAL( initialNumControls, GetInControls().Size() );

	}
	void testUnRegistredOutPorts()
	{	int initialNumPorts = GetOutPorts().Size();
		new CLAM::OutPort<int>( "out");
		CPPUNIT_ASSERT_EQUAL( initialNumPorts, GetOutPorts().Size() );
	}	
	void testUnRegistredInPorts()
	{	int initialNumPorts = GetInPorts().Size();
		new CLAM::InPort<int>( "in");
		CPPUNIT_ASSERT_EQUAL( initialNumPorts, GetInPorts().Size() );
	}
	void testUnRegistredOutControls()
	{	int initialNumControls = GetOutControls().Size();
		new CLAM::OutControl( "outControl");
		CPPUNIT_ASSERT_EQUAL( initialNumControls, GetOutControls().Size() );
	}	
	void testUnRegistredInControls()
	{	int initialNumControls = GetInControls().Size();
		new CLAM::InControl( "inControl");
		CPPUNIT_ASSERT_EQUAL( initialNumControls, GetInControls().Size() );
	}

	class DummyIOProcessing : public CLAM::Processing
	{
	public:
		CLAM::InPort<int> in;
		CLAM::OutPort<int> out;
		CLAM::InControl inControl;
		CLAM::OutControl outControl;
		CLAM::TypedInControl<int> typedInControl;
		CLAM::TypedOutControl<int> typedOutControl;
		
		DummyIOProcessing() : 
			in("In", this), 
			out("Out", this),
			inControl("In", this),
			outControl("Out", this),
			typedInControl("TypedIn", this),
			typedOutControl("TypedOut", this)
		{
		}
		bool Do() { return false; }
		const char* GetClassName() const { return ""; }
		bool ConcreteConfigure(const CLAM::ProcessingConfig & ) { return false; }
		const CLAM::ProcessingConfig & GetConfig() const { throw 0;}

		
	};
	void testConnectPorts_usingNames()
	{
		DummyIOProcessing sender, receiver;
		CLAM::ConnectPorts(sender, "Out", receiver, "In");
		sender.out.GetData()=2;
		sender.out.Produce();
		CPPUNIT_ASSERT_EQUAL(2, receiver.in.GetData() );
	}
	void testConnectPorts_usingNumbers()
	{
		DummyIOProcessing sender, receiver;
		CLAM::ConnectPorts(sender, 0, receiver, 0);
		sender.out.GetData()=2;
		sender.out.Produce();
		CPPUNIT_ASSERT_EQUAL(2, receiver.in.GetData() );
	}
	void testConnectControls_usingNames()
	{
		DummyIOProcessing sender, receiver;
		CLAM::ConnectControls(sender, "Out", receiver, "In");
		receiver.inControl.DoControl(666); // A previous value
		CLAM::TControlData event(1.7);
		sender.outControl.SendControl(event);
		CPPUNIT_ASSERT_EQUAL(event, receiver.inControl.GetLastValue() );
	}
	void testConnectControls_usingNumbers()
	{

		DummyIOProcessing sender, receiver;
		CLAM::ConnectControls(sender, 0, receiver, 0);
		receiver.inControl.DoControl(666); // A previous value
		CLAM::TControlData event(1.7);
		sender.outControl.SendControl(event);
		CPPUNIT_ASSERT_EQUAL(event, receiver.inControl.GetLastValue() );
	}
	void testConnectTypedControls_usingNames()
	{
		DummyIOProcessing sender, receiver;
		CLAM::ConnectTypedControls(sender, "TypedOut", receiver, "TypedIn");
		receiver.typedInControl.DoControl(666); // A previous value
		sender.typedOutControl.SendControl(1);
		CPPUNIT_ASSERT_EQUAL(1, receiver.typedInControl.GetLastValue() );
	}
	void testConnectTypedControls_usingNumbers()
	{
		DummyIOProcessing sender, receiver;
		CLAM::ConnectTypedControls(sender, 0, receiver, 0);
		receiver.typedInControl.DoControl(666); // A previous value
		sender.typedOutControl.SendControl(1);
		CPPUNIT_ASSERT_EQUAL(1, receiver.typedInControl.GetLastValue() );
	}
	
	void testHasTypedInControl_usingRegisteredControlName()
	{
		DummyIOProcessing test;
		CPPUNIT_ASSERT_EQUAL(true, test.HasTypedInControl("TypedIn"));
	}
	void testHasTypedInControl_usingNotRegisteredControlName()
	{
		DummyIOProcessing test;
		CPPUNIT_ASSERT_EQUAL(false, test.HasTypedInControl("FalseTypedIn"));
	}
	void testHasTypedOutControl_usingRegisteredControlName()
	{
		DummyIOProcessing test;
		CPPUNIT_ASSERT_EQUAL(true, test.HasTypedOutControl("TypedOut"));
	}
	void testHasTypedOutControl_usingNotRegisteredControlName()
	{
		DummyIOProcessing test;
		CPPUNIT_ASSERT_EQUAL(false, test.HasTypedOutControl("FalseTypedOut"));
	}

	void testIsSyncSource_default()
	{
		DummyIOProcessing proc;
		CPPUNIT_ASSERT_EQUAL( false, proc.IsSyncSource() );
	}


};

} // namespace CLAMTest

