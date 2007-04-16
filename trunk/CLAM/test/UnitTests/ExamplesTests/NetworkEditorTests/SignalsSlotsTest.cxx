
#include <cppunit/extensions/HelperMacros.h>
#include "BaseLoggable.hxx" // also includes <sstream>
#include <string>
#include "Signalv1.hxx"
#include "Slotv1.hxx"


class A
{
public:
	SigSlot::Signalv1< const std::string & > SignalSendB;
	SigSlot::Slotv1< const std::string & > SlotReceiveA;

	void ReceiveA( const std::string & )
	{
	}

	A()
	{
		SlotReceiveA.Wrap( this, &A::ReceiveA ); 
	}
		
	};

class B
{
public:
	SigSlot::Signalv1< const std::string & > SignalSendA;
	SigSlot::Slotv1< const std::string & > SlotReceiveB;

	void ReceiveB( const std::string & ) 
	{
	}

	B()
	{
		SlotReceiveB.Wrap( this, &B::ReceiveB ); 
	}
		
	void Attach( A & a )
	{
		SignalSendA.Connect( a.SlotReceiveA );
		a.SignalSendB.Connect( SlotReceiveB );
	}
};

namespace CLAMTest 
{

class SignalsSlotsTest;
CPPUNIT_TEST_SUITE_REGISTRATION( SignalsSlotsTest );


class SignalsSlotsTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( SignalsSlotsTest );
	
	CPPUNIT_TEST( testDoubleConnection );
	
	CPPUNIT_TEST_SUITE_END();

	void testDoubleConnection()
	{
		A * a = new A;
		B * b = new B;
		
		b->Attach( *a );

		delete b;
		delete a;
//		CPPUNIT_FAIL("oho");
	}
};

} // namespace CLAMTest

