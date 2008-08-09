#include <CLAM/TypedInControl.hxx>
#include <CLAM/TypedInControlRegistry.hxx>
#include <CLAM/TypedOutControl.hxx>
#include <CLAM/TypedOutControlRegistry.hxx>
#include <CLAM/Processing.hxx>
#include <cppunit/extensions/HelperMacros.h>
#include "BaseLoggable.hxx"
#include <string>

namespace CLAMTest {
	class TypedControlsTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( TypedControlsTest );
	
	class TypedControlsTest : public CppUnit::TestFixture, public BaseLoggable, public CLAM::Processing
	{
		CPPUNIT_TEST_SUITE( TypedControlsTest );

		// testing TypedInControl and TypedOutControl
		CPPUNIT_TEST( testTypedInControl_DoControl_ChangesInternalState );
		CPPUNIT_TEST( testAddLinkAndSendControl_ChangesTypedInControlInternalState );
	
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
		CPPUNIT_TEST( testAddLink_withSameTypedControls );
		CPPUNIT_TEST( testAddLink_withDifferentTypedControls );
		CPPUNIT_TEST( testIsConnected_WithBaseTypedOutControl_AfterConnection );
		CPPUNIT_TEST( testIsConnected_WithBaseTypedOutControl_WithNoConnection );
		CPPUNIT_TEST( testIsConnectedTo_WithBaseTypedOutControl_WhenControlsAreConnected );
		CPPUNIT_TEST( testIsConnectedTo_WithBaseTypedOutControl_WhenControlsAreNotConnected );

		// testing CascadingTypedInControl
		CPPUNIT_TEST( testCascadingTypedInControl_DoControl_ChangesInternalState );
		CPPUNIT_TEST( testLinkAndSendWithCascadingTypedInControl_CallbackMethodGetsCalled );
		CPPUNIT_TEST( testControlHandlerId_WritesToLog );
		CPPUNIT_TEST( testLinkAndSendWithCascadingTypedInControl_CallbackWithIdMethodGetsCalled );
		
		// Typed*ControlRegistry Tests
		CPPUNIT_TEST( testTypedInControlRegistry_ProcessingInterface_Register_ChangesInternalState );
		CPPUNIT_TEST( testTypedOutControlRegistry_ProcessingInterface_Register_ChangesInternalState );
		CPPUNIT_TEST( testTypedInControlRegistry_Has_loadingControlWithSameName );
		CPPUNIT_TEST( testTypedOutControlRegistry_Has_loadingControlWithSameName );
		CPPUNIT_TEST( testTypedInControlRegistry_Has_withNoControls );
		CPPUNIT_TEST( testTypedOutControlRegistry_Has_withNoControls );
		
		// Testing bounds
		CPPUNIT_TEST( testTypedInControl_doesntHaveBoundsBydefault );
		CPPUNIT_TEST( testTypedInControl_defaultBounds );
		CPPUNIT_TEST( testTypedInControl_settingBounds );
		CPPUNIT_TEST( testTypedInControl_boundedDefaultValue );
		CPPUNIT_TEST( testTypedInControl_isBoundedWhenTrue );

		// Testing default value
		CPPUNIT_TEST( testTypedInControl_setDefaultprevailstoBounds );

		CPPUNIT_TEST_SUITE_END();
		
		// Processing interface:
		const char* GetClassName() const { return "for testing"; }
		bool Do() { return false; }
		
		// testing TypedInControl and TypedOutControl
		void testTypedInControl_DoControl_ChangesInternalState()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			in.DoControl(1);
			CPPUNIT_ASSERT_EQUAL( 1, in.GetLastValue() );
		}

		void testAddLinkAndSendControl_ChangesTypedInControlInternalState()
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

		void testAddLink_withDifferentTypedControls()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<int> concreteOut("Concrete Out");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::BaseTypedOutControl & out = concreteOut;
			try {
				out.AddLink(in);
				CPPUNIT_FAIL("assertion failed expected, but nothing happened");
			} catch(CLAM::ErrAssertionFailed& )	{}
		}

		void testAddLink_withSameTypedControls()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::BaseTypedOutControl & out = concreteOut;
			out.AddLink(in);
			CPPUNIT_ASSERT_EQUAL(true, concreteOut.IsConnectedTo(concreteIn));
		}
		
		void testIsConnected_WithBaseTypedOutControl_AfterConnection()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::BaseTypedOutControl & out = concreteOut;
			out.AddLink(in);
			CPPUNIT_ASSERT_EQUAL(true, out.IsConnected());
		}

		void testIsConnected_WithBaseTypedOutControl_WithNoConnection()
		{
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::BaseTypedOutControl & out = concreteOut;
			CPPUNIT_ASSERT_EQUAL(false, out.IsConnected());
		}

		void testIsConnectedTo_WithBaseTypedOutControl_WhenControlsAreConnected()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::BaseTypedOutControl & out = concreteOut;
			out.AddLink(in);
			CPPUNIT_ASSERT_EQUAL(true, out.IsConnectedTo(in));
		}
		
		void testIsConnectedTo_WithBaseTypedOutControl_WhenControlsAreNotConnected()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::BaseTypedOutControl & out = concreteOut;
			CPPUNIT_ASSERT_EQUAL(false, out.IsConnectedTo(in));
		}
		
		// testing CascadingTypedInControl
		void testCascadingTypedInControl_DoControl_ChangesInternalState()
		{
			CLAM::CascadingTypedInControl<int, TypedControlsTest> in("I'm a cascading typed in control", this);
			in.DoControl(1);
			CPPUNIT_ASSERT_EQUAL( 1, in.GetLastValue() );
		}
		// helper method used for handling incoming control
		int ControlHandler(int val) {
			ToLog() << "ControlHandler called with: " << val;
			return 0;
		}
		void testLinkAndSendWithCascadingTypedInControl_CallbackMethodGetsCalled()
		{
			CLAM::CascadingTypedInControl<int, TypedControlsTest> 
				in("in", this, &TypedControlsTest::ControlHandler);
			ClearLog();
			in.DoControl(1);
			CPPUNIT_ASSERT_EQUAL( std::string("ControlHandler called with: 1"), GetLog() );
		}

		// helper method for handling incoming control plus incontrol ID
		int ControlHandlerId(int id, int val) {
			ToLog() << "ControlHandler called with id : " << id << " and value : " << val;
			return 0;
		}
		void testControlHandlerId_WritesToLog()
		{
			ControlHandlerId(0, 1);
			CPPUNIT_ASSERT_EQUAL( std::string("ControlHandler called with id : 0 and value : 1"), GetLog() );
		}

		void testLinkAndSendWithCascadingTypedInControl_CallbackWithIdMethodGetsCalled()
		{
			const int controlId=2;
			CLAM::CascadingTypedInControl<int, TypedControlsTest> 
				in( controlId, "in", this, &TypedControlsTest::ControlHandlerId );

			in.DoControl( 1 );
			CPPUNIT_ASSERT_EQUAL( 
				GetLog(), 
				std::string("ControlHandler called with id : 2 and value : 1") );
			    // note that controlId == 2
		}
		
		// Typed*ControlRegistry Tests
		// testing TypedInControl and TypedOutControl
		void testTypedInControlRegistry_ProcessingInterface_Register_ChangesInternalState()
		{
			CLAM::TypedInControl<int> concreteIn("IntInControl");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::TypedInControlRegistry inRegistry;
			inRegistry.ProcessingInterface_Register(&in);
			CPPUNIT_ASSERT_EQUAL( &in, &inRegistry.Get("IntInControl") );
		}

		void testTypedOutControlRegistry_ProcessingInterface_Register_ChangesInternalState()
		{
			CLAM::TypedOutControl<int> concreteOut("IntOutControl");
			CLAM::BaseTypedOutControl & out = concreteOut;
			CLAM::TypedOutControlRegistry outRegistry;
			outRegistry.ProcessingInterface_Register(&out);
			CPPUNIT_ASSERT_EQUAL( &out, &outRegistry.Get("IntOutControl") );
		}

		void testTypedInControlRegistry_Has_loadingControlWithSameName()
		{
			CLAM::TypedInControl<int> concreteIn("IntInControl");
			CLAM::BaseTypedInControl & in = concreteIn;
			CLAM::TypedInControlRegistry inRegistry;
			inRegistry.ProcessingInterface_Register(&in);
			CPPUNIT_ASSERT_EQUAL( true, inRegistry.Has("IntInControl") );
		}

		void testTypedOutControlRegistry_Has_loadingControlWithSameName()
		{
			CLAM::TypedOutControl<int> concreteOut("IntOutControl");
			CLAM::BaseTypedOutControl & out = concreteOut;
			CLAM::TypedOutControlRegistry outRegistry;
			outRegistry.ProcessingInterface_Register(&out);
			CPPUNIT_ASSERT_EQUAL( true, outRegistry.Has("IntOutControl") );
		}

		void testTypedInControlRegistry_Has_withNoControls()
		{
			CLAM::TypedInControlRegistry inRegistry;
			CPPUNIT_ASSERT_EQUAL( false, inRegistry.Has("IntInControl") );
		}

		void testTypedOutControlRegistry_Has_withNoControls()
		{
			CLAM::TypedOutControlRegistry outRegistry;
			CPPUNIT_ASSERT_EQUAL( false, outRegistry.Has("IntOutControl") );
		}

	void testTypedInControl_doesntHaveBoundsBydefault()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::BaseTypedInControl & inControl = concreteIn;
		CPPUNIT_ASSERT_EQUAL( false, inControl.IsBounded() );
	}
	void testTypedInControl_defaultBounds()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::BaseTypedInControl & inControl = concreteIn;
		CPPUNIT_ASSERT_EQUAL( 0.0f, inControl.LowerBound() );
		CPPUNIT_ASSERT_EQUAL( 1.0f, inControl.UpperBound() );
	}
	void testTypedInControl_settingBounds()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::BaseTypedInControl & inControl = concreteIn;
		inControl.SetBounds(-1.f, 2.f);
		CPPUNIT_ASSERT_EQUAL( -1.f, inControl.LowerBound() );
		CPPUNIT_ASSERT_EQUAL( 2.f, inControl.UpperBound() );
	}
	void testTypedInControl_boundedDefaultValue()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::BaseTypedInControl & inControl = concreteIn;
		inControl.SetBounds(0.f, 10.f);
		CPPUNIT_ASSERT_EQUAL( 5.f, inControl.DefaultValue() );
	}
	void testTypedInControl_isBoundedWhenTrue()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::BaseTypedInControl & inControl = concreteIn;
		inControl.SetBounds(0.f, 0.f);
		CPPUNIT_ASSERT_EQUAL( true, inControl.IsBounded() );
	}

	void testTypedInControl_setDefaultprevailstoBounds()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::BaseTypedInControl & inControl = concreteIn;
		inControl.SetBounds(0.f, 10.f);
		inControl.SetDefaultValue(0.0f);
		CPPUNIT_ASSERT_EQUAL( 0.0f, inControl.DefaultValue() );
	}
	};
} // namespace
