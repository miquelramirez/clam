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
		_inControl("in", this), 
		_inControlTmpl("in tmpl",this,&ProcessingTest::ControlHandler),
		_outControl1("out1", this),
		_outControl2("out2", this),
		
		_inPort(std::string("in"),this,0/*length*/),
		_outPort1("out1",this,0/*length*/),
		_outPort2("out2",this,0/*length*/)
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
	
	CLAM::InControl _inControl;
	CLAM::InControlTmpl<ProcessingTest> _inControlTmpl;
	CLAM::OutControl _outControl1;
	CLAM::OutControl _outControl2;

	CLAM::InPortTmpl<DummyProcessingData> _inPort;
	CLAM::OutPortTmpl<DummyProcessingData> _outPort1;
	CLAM::OutPortTmpl<DummyProcessingData> _outPort2;

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
		CLAM::InControl* expected = &_inControl;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testOutControls_GetByNumber_GetTheRightControl()
	{
		CLAM::OutControl* returned = &GetOutControls().GetByNumber(1); // get the second control
		CLAM::OutControl* expected = &_outControl2;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testOutPorts_GetByNumber_GetTheRightPort()
	{
		CLAM::OutPort* returnedPort = &GetOutPorts().GetByNumber(1); // get the second port
		CLAM::OutPort* expectedPort = &_outPort2;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}
	void testInPorts_GetByNumber_GetTheRightPort()
	{
		CLAM::InPort* returnedPort = &GetInPorts().GetByNumber(0); // get the first port
		CLAM::InPort* expectedPort = &_inPort;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}

	void testInControls_GetByName_GetTheRightControl()
	{
		CLAM::InControl* returned = &GetInControls().Get("in");
		CLAM::InControl* expected = &_inControl;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}

	void testOutControls_GetByName_GetTheRightControl()
	{
		CLAM::OutControl* returned = &GetOutControls().Get("out2"); // get the second control
		CLAM::OutControl* expected = &_outControl2;
		CPPUNIT_ASSERT_EQUAL( expected, returned );
	}
	void testOutPorts_GetByName_GetTheRightPort()
	{
		CLAM::OutPort* returnedPort = &GetOutPorts().Get("out2"); // get the second port
		CLAM::OutPort* expectedPort = &_outPort2;
		CPPUNIT_ASSERT_EQUAL( expectedPort,  returnedPort );
	}
	void testInPorts_GetByName_GetTheRightPort()
	{
		CLAM::InPort* returnedPort = &GetInPorts().Get("in"); // get the first port
		CLAM::InPort* expectedPort = &_inPort;
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
