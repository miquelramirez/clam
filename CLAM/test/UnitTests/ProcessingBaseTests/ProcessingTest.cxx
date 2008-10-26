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
	CPPUNIT_TEST( testGetInControl_WithOutOfRangeIndexThrowException );
	CPPUNIT_TEST( testGetOutControl_WithOutOfRangeIndexThrowException );
	CPPUNIT_TEST( testLinkAndSendControl_ChangesInControlState );
	// new controls/ports interface.
	// todo: adapt the old tests:
	CPPUNIT_TEST( testInControls_GetByNumber_GetTheRightControl );
	CPPUNIT_TEST( testOutControls_GetByNumber_GetTheRightControl );
	CPPUNIT_TEST( testOutPorts_GetByNumber_GetTheRightPort );
	CPPUNIT_TEST( testInPorts_GetByNumber_GetTheRightPort );
	CPPUNIT_TEST( testInControls_GetByName_GetTheRightControl );
	CPPUNIT_TEST( testOutControls_GetByName_GetTheRightControl );
	CPPUNIT_TEST( testOutPorts_GetByName_GetTheRightPort );
	CPPUNIT_TEST( testInPorts_GetByName_GetTheRightPort );
	CPPUNIT_TEST( testInControls_Size );
	CPPUNIT_TEST( testOutControls_Size );
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
	CPPUNIT_TEST( testHasInControl_usingRegisteredControlName );
	CPPUNIT_TEST( testHasInControl_usingNotRegisteredControlName );
	CPPUNIT_TEST( testHasOutControl_usingRegisteredControlName );
	CPPUNIT_TEST( testHasOutControl_usingNotRegisteredControlName );

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
		CPPUNIT_ASSERT_EQUAL( std::string("in"), GetInControl(0).GetName() );
	}
	void testGetOutControl_GetTheRightControl()
	{
		CPPUNIT_ASSERT_EQUAL( std::string("out1"), GetOutControl(0).GetName() );
	}
	void testGetInControl_WithOutOfRangeIndexThrowException() 
	{
		try
		{
			GetInControl(2); // we have two published in controls: indexs 0,1
			CPPUNIT_FAIL("std::out_of_range was expected but none was thrown");
		}
		catch( ... ) {}
	}
	void testGetOutControl_WithOutOfRangeIndexThrowException() 
	{
		try
		{
			GetOutControl(2); // we have two published out controls: indexs 0,1
			CPPUNIT_FAIL("std::out_of_range was expected but none was thrown");
		}
		catch( ... ) {}
	}
	void testLinkAndSendControl_ChangesInControlState()
	{
		const int outId=0, inId=0;
		GetOutControl(outId).AddLink(GetInControl(inId));
		SendFloatToOutControl(*this,outId,1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f, GetFloatFromInControl(*this,inId) );
	}
	//---------------------------------------
	// new controls/ports interface tests

	void testInControls_GetByNumber_GetTheRightControl()
	{
		CLAM::InControl* returned = &GetInControl(0);
		CLAM::InControl* expected = &mInControl;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testOutControls_GetByNumber_GetTheRightControl()
	{
		CLAM::OutControl* returned = &GetOutControl(1); // get the second control
		CLAM::OutControl* expected = &mOutControl2;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testOutPorts_GetByNumber_GetTheRightPort()
	{
		CLAM::OutPortBase* returnedPort = &GetOutPort(1); // get the second port
		CLAM::OutPortBase* expectedPort = &mOutPort2;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}
	void testInPorts_GetByNumber_GetTheRightPort()
	{
		CLAM::InPortBase* returnedPort = &GetInPort(0); // get the first port
		CLAM::InPortBase* expectedPort = &mInPort;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}
	void testInControls_GetByName_GetTheRightControl()
	{
		CLAM::InControl* returned = &GetInControl("in");
		CLAM::InControl* expected = &mInControl;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}

	void testOutControls_GetByName_GetTheRightControl()
	{
		CLAM::OutControl* returned = &GetOutControl("out2"); // get the second control
		CLAM::OutControl* expected = &mOutControl2;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testOutPorts_GetByName_GetTheRightPort()
	{
		CLAM::OutPortBase* returnedPort = &GetOutPort("out2"); // get the second port
		CLAM::OutPortBase* expectedPort = &mOutPort2;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}
	void testInPorts_GetByName_GetTheRightPort()
	{
		CLAM::InPortBase* returnedPort = &GetInPort("in"); // get the first port
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
//		CLAM::TypedInControl<int> typedInControl;
//		CLAM::TypedOutControl<int> typedOutControl;
		
		DummyIOProcessing()
			: in("In", this)
			, out("Out", this)
			, inControl("In", this)
			, outControl("Out", this)
//			, typedInControl("TypedIn", this)
//			, typedOutControl("TypedOut", this)
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

	void testHasInControl_usingRegisteredControlName()
	{
		DummyIOProcessing test;
		CPPUNIT_ASSERT_EQUAL(true, test.HasInControl("In"));
	}
	void testHasInControl_usingNotRegisteredControlName()
	{
		DummyIOProcessing test;
		CPPUNIT_ASSERT_EQUAL(false, test.HasInControl("FalseIn"));
	}
	void testHasOutControl_usingRegisteredControlName()
	{
		DummyIOProcessing test;
		CPPUNIT_ASSERT_EQUAL(true, test.HasOutControl("Out"));
	}
	void testHasOutControl_usingNotRegisteredControlName()
	{
		DummyIOProcessing test;
		CPPUNIT_ASSERT_EQUAL(false, test.HasOutControl("FalseOut"));
	}

	void testIsSyncSource_default()
	{
		DummyIOProcessing proc;
		CPPUNIT_ASSERT_EQUAL( false, proc.IsSyncSource() );
	}


};

} // namespace CLAMTest

