#include <CLAM/TypedInControl.hxx>
#include <CLAM/TypedOutControl.hxx>
#include <cppunit/extensions/HelperMacros.h>
#include <string>

namespace CLAMTest {
	class TypedControlsTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( TypedControlsTest );
	
	class TypedControlsTest : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( TypedControlsTest );

		// testing TypedInControl and TypedOutControl
		 CPPUNIT_TEST( testTypedInControl_DoControl_ChangesInternalState );
		 CPPUNIT_TEST( testLinkAndSendControl_ChangesTypedInControlInternalState );
		
		// tests for IsConnected / IsConnectedTo
		CPPUNIT_TEST( testIsConnected_WithTypedOutControl_AfterConnection );
		CPPUNIT_TEST( testIsConnected_WithTypedOutControl_WithoutConnection );
		CPPUNIT_TEST( testIsConnectedTo_WithTypedOutControl_WhenControlsAreConnected );
		CPPUNIT_TEST( testIsConnectedTo_WithTypedOutControl_WhenControlsAreNotConnected );
		CPPUNIT_TEST( testTypedInControlIsConnected_byDefault );
		CPPUNIT_TEST( testTypedInControlIsConnected_whenConnected );
		CPPUNIT_TEST( testTypedInControlIsConnected_whenDisconnected );
		
		// Destructor disconnections
		CPPUNIT_TEST( testTypedInControlDestructor_disconnectsOutControl );
		CPPUNIT_TEST( testTypedOutControlDestructor_disconnectsInControl );

		// 1 Out - Many Ins Test
		CPPUNIT_TEST( testOneTypedOutControlManyTypedInControls_DoControl_ChangesInternalState );

		// Template Link Tests
		CPPUNIT_TEST( testIsLinkable_withDifferentTypedControls );
		CPPUNIT_TEST( testIsLinkable_withSameTypedControls );
		CPPUNIT_TEST( testLink_withSameTypedControls );
		CPPUNIT_TEST( testLink_withDifferentTypedControls );
				
		CPPUNIT_TEST_SUITE_END();
		
		// testing TypedInControl and TypedOutControl
		void testTypedInControl_DoControl_ChangesInternalState()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			in.DoControl(1);
			CPPUNIT_ASSERT_EQUAL( 1, in.GetLastValue() );
		}

		void testLinkAndSendControl_ChangesTypedInControlInternalState()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			CLAM::TypedOutControl<int> out("IntOutControl");
			out.AddLink(in);
			out.SendControl(1);
			CPPUNIT_ASSERT_EQUAL( 1 , in.GetLastValue() );
		}

		// tests for IsConnected / IsConnectedTo
		void testIsConnected_WithTypedOutControl_AfterConnection()
		{	
			CLAM::TypedInControl<int> in("IntInControl");
			CLAM::TypedOutControl<int> out("IntOutControl");
			out.AddLink(in);
			CPPUNIT_ASSERT_EQUAL( true, out.IsConnected() );
		}

		void testIsConnected_WithTypedOutControl_WithoutConnection()
		{	
			CLAM::TypedOutControl<int> out("IntOutControl");
			CPPUNIT_ASSERT_EQUAL(  false, out.IsConnected() );		
		}

		void testIsConnectedTo_WithTypedOutControl_WhenControlsAreConnected()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			CLAM::TypedOutControl<int> out("IntOutControl");
			out.AddLink(in);
			CPPUNIT_ASSERT_EQUAL( true, out.IsConnectedTo(in) );
		}

		void testIsConnectedTo_WithTypedOutControl_WhenControlsAreNotConnected()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			CLAM::TypedOutControl<int> out("IntOutControl");
			CPPUNIT_ASSERT_EQUAL( false, out.IsConnectedTo(in) );
		}

		void testTypedInControlIsConnected_byDefault()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			CPPUNIT_ASSERT_EQUAL(false, in.IsConnected());
		}
		void testTypedInControlIsConnected_whenConnected()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			CLAM::TypedOutControl<int> out("IntOutControl");
			out.AddLink(in);
			CPPUNIT_ASSERT_EQUAL(true, in.IsConnected());
		}
		void testTypedInControlIsConnected_whenDisconnected()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			CLAM::TypedOutControl<int> out("IntOutControl");
			out.AddLink(in);
			out.RemoveLink(in);
			CPPUNIT_ASSERT_EQUAL(false, in.IsConnected());
		}
		
		// Destructor disconnections
		void testTypedInControlDestructor_disconnectsOutControl()
		{
			CLAM::TypedOutControl<int> out("IntOutControl");
			{
				CLAM::TypedInControl<int> in("IntInControl");
				out.AddLink(in);
			}
			CPPUNIT_ASSERT_EQUAL(false, out.IsConnected());
		}
		void testTypedOutControlDestructor_disconnectsInControl()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			{
				CLAM::TypedOutControl<int> out("IntOutControl");
				out.AddLink(in);
			}
			CPPUNIT_ASSERT_EQUAL(false, in.IsConnected());
		}
		
		// 1 Out - Many Ins Test
		void testOneTypedOutControlManyTypedInControls_DoControl_ChangesInternalState()
		{
			CLAM::TypedOutControl<int> out("IntTypedOutControl");
			CLAM::TypedInControl<int> in1("IntTypedInControl 1");
			CLAM::TypedInControl<int> in2("IntTypedInControl 2");
			out.AddLink(in1);
			out.AddLink(in2);
			out.SendControl(1);
			CPPUNIT_ASSERT_EQUAL( 1 , in1.GetLastValue() );
			CPPUNIT_ASSERT_EQUAL( 1 , in2.GetLastValue() );
		}

		// Template Link Tests
		void testIsLinkable_withDifferentTypedControls()
		{
			CLAM::TypedInControl<int> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::BaseTypedOutControl & out = concreteOut;
			CPPUNIT_ASSERT_EQUAL(false, out.IsLinkable(in));
		}

		void testIsLinkable_withSameTypedControls()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::BaseTypedOutControl & out = concreteOut;
			CPPUNIT_ASSERT_EQUAL(true, out.IsLinkable(in));
		}

		void testLink_withDifferentTypedControls()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<int> concreteOut("Concrete Out");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::BaseTypedOutControl & out = concreteOut;
			try {
				out.Link(in);
				CPPUNIT_FAIL("assertion failed expected, but nothing happened");
			} catch(CLAM::ErrAssertionFailed& )	{}
		}

		void testLink_withSameTypedControls()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::BaseTypedOutControl & out = concreteOut;
			out.Link(in);
			CPPUNIT_ASSERT_EQUAL(true, concreteOut.IsConnectedTo(concreteIn));
		}


	};
} // namespace
