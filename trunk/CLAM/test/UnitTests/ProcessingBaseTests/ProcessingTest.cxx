#include <cppunit/extensions/HelperMacros.h>
#include "Processing.hxx"
#include "InPortTmpl.hxx"
#include "OutPortTmpl.hxx"
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

	CPPUNIT_TEST( testLinkOutWithInControl_ChangesInControlState );
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
		
	
	CPPUNIT_TEST_SUITE_END();

public:
	// void implementation the pure virtual methods of processing
	ProcessingTest() : 
		mInControl("in", this), 
		mInControlTmpl("in tmpl",this,&ProcessingTest::ControlHandler),
		mOutControl1("out1", this),
		mOutControl2("out2", this),
		
		mInPort(std::string("in"),this,0/*length*/),
		mOutPort1("out1",this,0/*length*/),
		mOutPort2("out2",this,0/*length*/)
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

	CLAM::InPortTmpl<DummyProcessingData> mInPort;
	CLAM::OutPortTmpl<DummyProcessingData> mOutPort1;
	CLAM::OutPortTmpl<DummyProcessingData> mOutPort2;

	void testGetInControl_GetTheRightControl()
	{
		CPPUNIT_ASSERT_EQUAL( std::string("in"), GetInControl(0)->GetName() );
	}
	void testGetOutControl_GetTheRightControl()
	{
		CPPUNIT_ASSERT_EQUAL( std::string("out1"), GetOutControl(0)->GetName() );
	}
	void testGetInControl_WithOutOfRangeIndexThrowException() 
	{
#		ifdef HAVE_STANDARD_VECTOR_AT // otherwise cannot be tested
			try
			{
				GetInControl(2); // we have two published in controls: indexs 0,1
				CPPUNIT_FAIL("std::out_of_range was expected but none was thrown");
			}
			catch( std::out_of_range&) {}
#		endif
	}
	void testGetOutControl_WithOutOfRangeIndexThrowException() 
	{
#		ifdef HAVE_STANDARD_VECTOR_AT // otherwise cannot be tested
			try
			{
				GetOutControl(2); // we have two published out controls: indexs 0,1
				CPPUNIT_FAIL("std::out_of_range was expected but none was thrown");
			}
			catch( std::out_of_range&) {}
#		endif
	}
	void testLinkAndSendControl_ChangesInControlState()
	{
		const int outId=0, inId=0;
		LinkOutWithInControl(outId, this, inId);
		SendControl(outId, 1.f);
		CPPUNIT_ASSERT_EQUAL( 1.f, GetInControl(inId)->GetLastValue() );
	}

	// free method LinkOutWithInControl()
	
	void testLinkOutWithInControl_ChangesInControlState()
	{
		CLAM::LinkOutWithInControl( this, "out1",this, "in" );
		GetOutControls().Get( "out1" ).SendControl( 1.f );
		CPPUNIT_ASSERT_EQUAL( 1.f, GetInControls().Get("in").GetLastValue() );
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
	void testOutPorts_GetByNumber_GetTheRightPort()
	{
		CLAM::OutPort* returnedPort = &GetOutPorts().GetByNumber(1); // get the second port
		CLAM::OutPort* expectedPort = &mOutPort2;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}
	void testInPorts_GetByNumber_GetTheRightPort()
	{
		CLAM::InPort* returnedPort = &GetInPorts().GetByNumber(0); // get the first port
		CLAM::InPort* expectedPort = &mInPort;
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
	void testOutPorts_GetByName_GetTheRightPort()
	{
		CLAM::OutPort* returnedPort = &GetOutPorts().Get("out2"); // get the second port
		CLAM::OutPort* expectedPort = &mOutPort2;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}
	void testInPorts_GetByName_GetTheRightPort()
	{
		CLAM::InPort* returnedPort = &GetInPorts().Get("in"); // get the first port
		CLAM::InPort* expectedPort = &mInPort;
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
	
	



};

} // namespace CLAMTest
