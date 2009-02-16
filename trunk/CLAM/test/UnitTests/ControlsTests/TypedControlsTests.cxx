#include <CLAM/TypedInControl.hxx>
#include <CLAM/TypedOutControl.hxx>
#include <CLAM/OutControlRegistry.hxx>
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
	CPPUNIT_TEST( testInControl_DoControl_ChangesInternalState );
	CPPUNIT_TEST( testAddLinkAndSendControl_ChangesInControlInternalState );

	// tests for IsConnected / IsConnectedTo
	CPPUNIT_TEST( testIsConnected_WithOutControl_AfterConnection );
	CPPUNIT_TEST( testIsConnected_WithOutControl_WithoutConnection );
	CPPUNIT_TEST( testIsConnectedTo_WithOutControl_WhenControlsAreConnected );
	CPPUNIT_TEST( testIsConnectedTo_WithOutControl_WhenControlsAreNotConnected );
	CPPUNIT_TEST( testInControlIsConnected_byDefault );
	CPPUNIT_TEST( testInControlIsConnected_whenConnected );
	CPPUNIT_TEST( testInControlIsConnected_whenDisconnected );
	
	// Destructor disconnections
	CPPUNIT_TEST( testInControlDestructor_disconnectsOutControl );
	CPPUNIT_TEST( testOutControlDestructor_disconnectsInControl );

	// 1 Out - Many Ins Test
	CPPUNIT_TEST( testOneOutControlManyInControls_DoControl_ChangesInternalState );

	// Template Link Tests
	CPPUNIT_TEST( testIsLinkable_withDifferentControls );
	CPPUNIT_TEST( testIsLinkable_withSameControls );
	CPPUNIT_TEST( testAddLink_withSameControls );
	CPPUNIT_TEST( testAddLink_withDifferentControls );
	CPPUNIT_TEST( testIsConnected_WithOutControl_AfterConnection_withFloat );
	CPPUNIT_TEST( testIsConnected_WithOutControl_WithNoConnection_withFloat );
	CPPUNIT_TEST( testIsConnectedTo_WithOutControl_WhenControlsAreConnected_withFloat );
	CPPUNIT_TEST( testIsConnectedTo_WithOutControl_WhenControlsAreNotConnected_withFloat );

	// testing InControl Callbacks
	CPPUNIT_TEST( testInControlWithCallback_DoControl_ChangesInternalState );
	CPPUNIT_TEST( testLinkAndSendWithInControl_CallbackMethodGetsCalled );
	CPPUNIT_TEST( testControlHandlerId_WritesToLog );
	CPPUNIT_TEST( testLinkAndSendWithInControl_CallbackWithIdMethodGetsCalled );
	
	// *ControlRegistry Tests
	CPPUNIT_TEST( testInControlRegistry_ProcessingInterface_Register_ChangesInternalState );
	CPPUNIT_TEST( testOutControlRegistry_ProcessingInterface_Register_ChangesInternalState );
	CPPUNIT_TEST( testInControlRegistry_Has_loadingControlWithSameName );
	CPPUNIT_TEST( testOutControlRegistry_Has_loadingControlWithSameName );
	CPPUNIT_TEST( testInControlRegistry_Has_withNoControls );
	CPPUNIT_TEST( testOutControlRegistry_Has_withNoControls );
	
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
	void tearDown()
	{
		ClearLog();
	}
		
	// testing InControl and OutControl
	void testInControl_DoControl_ChangesInternalState()
	{
		CLAM::TypedInControl<int> in("IntInControl");
		in.DoControl(1);
		CPPUNIT_ASSERT_EQUAL( 1, in.GetLastValue() );
	}

	void testAddLinkAndSendControl_ChangesInControlInternalState()
	{
		CLAM::TypedInControl<int> in("IntInControl");
		CLAM::TypedOutControl<int> out("IntOutControl");
		out.AddLink(in);
		out.SendControl(1);
		CPPUNIT_ASSERT_EQUAL( 1 , in.GetLastValue() );
	}
	// tests for IsConnected / IsConnectedTo
	void testIsConnected_WithOutControl_AfterConnection()
	{	
		CLAM::TypedInControl<int> in("IntInControl");
		CLAM::TypedOutControl<int> out("IntOutControl");
		out.AddLink(in);
		CPPUNIT_ASSERT_EQUAL( true, out.IsConnected() );
	}

	void testIsConnected_WithOutControl_WithoutConnection()
	{	
		CLAM::TypedOutControl<int> out("IntOutControl");
		CPPUNIT_ASSERT_EQUAL(  false, out.IsConnected() );		
	}

	void testIsConnectedTo_WithOutControl_WhenControlsAreConnected()
	{
		CLAM::TypedInControl<int> in("IntInControl");
		CLAM::TypedOutControl<int> out("IntOutControl");
		out.AddLink(in);
		CPPUNIT_ASSERT_EQUAL( true, out.IsConnectedTo(in) );
	}

	void testIsConnectedTo_WithOutControl_WhenControlsAreNotConnected()
	{
		CLAM::TypedInControl<int> in("IntInControl");
		CLAM::TypedOutControl<int> out("IntOutControl");
		CPPUNIT_ASSERT_EQUAL( false, out.IsConnectedTo(in) );
	}

	void testInControlIsConnected_byDefault()
	{
		CLAM::TypedInControl<int> in("IntInControl");
		CPPUNIT_ASSERT_EQUAL(false, in.IsConnected());
	}
	void testInControlIsConnected_whenConnected()
	{
		CLAM::TypedInControl<int> in("IntInControl");
		CLAM::TypedOutControl<int> out("IntOutControl");
		out.AddLink(in);
		CPPUNIT_ASSERT_EQUAL(true, in.IsConnected());
	}
	void testInControlIsConnected_whenDisconnected()
	{
		CLAM::TypedInControl<int> in("IntInControl");
		CLAM::TypedOutControl<int> out("IntOutControl");
		out.AddLink(in);
		out.RemoveLink(in);
		CPPUNIT_ASSERT_EQUAL(false, in.IsConnected());
	}
	
	// Destructor disconnections
	void testInControlDestructor_disconnectsOutControl()
	{
		CLAM::TypedOutControl<int> out("IntOutControl");
		{
			CLAM::TypedInControl<int> in("IntInControl");
			out.AddLink(in);
		}
		CPPUNIT_ASSERT_EQUAL(false, out.IsConnected());
	}
	void testOutControlDestructor_disconnectsInControl()
	{
		CLAM::TypedInControl<int> in("IntInControl");
		{
			CLAM::TypedOutControl<int> out("IntOutControl");
			out.AddLink(in);
		}
		CPPUNIT_ASSERT_EQUAL(false, in.IsConnected());
	}
	
	// 1 Out - Many Ins Test
	void testOneOutControlManyInControls_DoControl_ChangesInternalState()
	{
		CLAM::TypedOutControl<int> out("IntOutControl");
		CLAM::TypedInControl<int> in1("IntInControl 1");
		CLAM::TypedInControl<int> in2("IntInControl 2");
		out.AddLink(in1);
		out.AddLink(in2);
		out.SendControl(1);
		CPPUNIT_ASSERT_EQUAL( 1 , in1.GetLastValue() );
		CPPUNIT_ASSERT_EQUAL( 1 , in2.GetLastValue() );
	}

	// Template Link Tests
	void testIsLinkable_withDifferentControls()
	{
		CLAM::TypedInControl<int> concreteIn("Concrete In");
		CLAM::TypedOutControl<float> concreteOut("Concrete Out");
		CLAM::InControlBase & in = concreteIn;
		CLAM::OutControlBase & out = concreteOut;
		CPPUNIT_ASSERT_EQUAL(false, out.IsLinkable(in));
	}

		void testIsLinkable_withSameControls()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::InControlBase & in = concreteIn;
			CLAM::OutControlBase & out = concreteOut;
			CPPUNIT_ASSERT_EQUAL(true, out.IsLinkable(in));
		}

		void testAddLink_withDifferentControls()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<int> concreteOut("Concrete Out");
			CLAM::InControlBase & in = concreteIn;
			CLAM::OutControlBase & out = concreteOut;
			try {
				out.AddLink(in);
				CPPUNIT_FAIL("assertion failed expected, but nothing happened");
			} catch(CLAM::ErrAssertionFailed& )	{}
		}

		void testAddLink_withSameControls()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::InControlBase & in = concreteIn;
			CLAM::OutControlBase & out = concreteOut;
			out.AddLink(in);
			CPPUNIT_ASSERT_EQUAL(true, concreteOut.IsConnectedTo(concreteIn));
		}
		
		void testIsConnected_WithOutControl_AfterConnection_withFloat()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::InControlBase & in = concreteIn;
			CLAM::OutControlBase & out = concreteOut;
			out.AddLink(in);
			CPPUNIT_ASSERT_EQUAL(true, out.IsConnected());
		}

		void testIsConnected_WithOutControl_WithNoConnection_withFloat()
		{
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::OutControlBase & out = concreteOut;
			CPPUNIT_ASSERT_EQUAL(false, out.IsConnected());
		}

		void testIsConnectedTo_WithOutControl_WhenControlsAreConnected_withFloat()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::InControlBase & in = concreteIn;
			CLAM::OutControlBase & out = concreteOut;
			out.AddLink(in);
			CPPUNIT_ASSERT_EQUAL(true, out.IsConnectedTo(in));
		}
		
		void testIsConnectedTo_WithOutControl_WhenControlsAreNotConnected_withFloat()
		{
			CLAM::TypedInControl<float> concreteIn("Concrete In");
			CLAM::TypedOutControl<float> concreteOut("Concrete Out");
			CLAM::InControlBase & in = concreteIn;
			CLAM::OutControlBase & out = concreteOut;
			CPPUNIT_ASSERT_EQUAL(false, out.IsConnectedTo(in));
		}
		
		// testing CascadingTypedInControl
		void testInControlWithCallback_DoControl_ChangesInternalState()
		{
			CLAM::TypedInControl<int> 
				in("in", this, &TypedControlsTest::ControlHandler);
			in.DoControl(1);
			CPPUNIT_ASSERT_EQUAL( 1, in.GetLastValue() );
		}
		// helper method used for handling incoming control
		void ControlHandler(int val) {
			ToLog() << "ControlHandler called with: " << val;
		}

		void testLinkAndSendWithInControl_CallbackMethodGetsCalled()
		{
			CLAM::TypedInControl<int> 
				in("in", this, &TypedControlsTest::ControlHandler);
			in.DoControl(1);
			CPPUNIT_ASSERT_EQUAL( std::string("ControlHandler called with: 1"), GetLog() );
		}

		// helper method for handling incoming control plus incontrol ID
		void ControlHandlerId(unsigned id, int val) {
			ToLog() << "ControlHandler called with id : " << id << " and value : " << val;
		}
		void testControlHandlerId_WritesToLog()
		{
			ControlHandlerId(0, 1);
			CPPUNIT_ASSERT_EQUAL( std::string("ControlHandler called with id : 0 and value : 1"), GetLog() );
		}

	void testLinkAndSendWithInControl_CallbackWithIdMethodGetsCalled()
	{
		const unsigned controlId=2;
		CLAM::TypedInControl<int> 
			in( controlId, "in", this, &TypedControlsTest::ControlHandlerId );

		in.DoControl( 1 );
		CPPUNIT_ASSERT_EQUAL( 
			GetLog(), 
			std::string("ControlHandler called with id : 2 and value : 1") );
			// note that controlId == 2
	}
	
	// *ControlRegistry Tests
	// testing InControl and OutControl
	void testInControlRegistry_ProcessingInterface_Register_ChangesInternalState()
	{
		CLAM::TypedInControl<int> concreteIn("IntInControl");
		CLAM::InControlBase & in = concreteIn;
		CLAM::InControlRegistry inRegistry;
		inRegistry.ProcessingInterface_Register(&in);
		CPPUNIT_ASSERT_EQUAL( &in, &inRegistry.Get("IntInControl") );
	}

	void testOutControlRegistry_ProcessingInterface_Register_ChangesInternalState()
	{
		CLAM::TypedOutControl<int> concreteOut("IntOutControl");
		CLAM::OutControlBase & out = concreteOut;
		CLAM::OutControlRegistry outRegistry;
		outRegistry.ProcessingInterface_Register(&out);
		CPPUNIT_ASSERT_EQUAL( &out, &outRegistry.Get("IntOutControl") );
	}

	void testInControlRegistry_Has_loadingControlWithSameName()
	{
		CLAM::TypedInControl<int> concreteIn("IntInControl");
		CLAM::InControlBase & in = concreteIn;
		CLAM::InControlRegistry inRegistry;
		inRegistry.ProcessingInterface_Register(&in);
		CPPUNIT_ASSERT_EQUAL( true, inRegistry.Has("IntInControl") );
	}

	void testOutControlRegistry_Has_loadingControlWithSameName()
	{
		CLAM::TypedOutControl<int> concreteOut("IntOutControl");
		CLAM::OutControlBase & out = concreteOut;
		CLAM::OutControlRegistry outRegistry;
		outRegistry.ProcessingInterface_Register(&out);
		CPPUNIT_ASSERT_EQUAL( true, outRegistry.Has("IntOutControl") );
	}

	void testInControlRegistry_Has_withNoControls()
	{
		CLAM::InControlRegistry inRegistry;
		CPPUNIT_ASSERT_EQUAL( false, inRegistry.Has("IntInControl") );
	}

	void testOutControlRegistry_Has_withNoControls()
	{
		CLAM::OutControlRegistry outRegistry;
		CPPUNIT_ASSERT_EQUAL( false, outRegistry.Has("IntOutControl") );
	}

	void testInControl_doesntHaveBoundsBydefault()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::InControlBase & inControl = concreteIn;
		CPPUNIT_ASSERT_EQUAL( false, inControl.IsBounded() );
	}
	void testInControl_defaultBounds()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::InControlBase & inControl = concreteIn;
		CPPUNIT_ASSERT_EQUAL( 0.0f, inControl.LowerBound() );
		CPPUNIT_ASSERT_EQUAL( 1.0f, inControl.UpperBound() );
	}
	void testInControl_settingBounds()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::InControlBase & inControl = concreteIn;
		inControl.SetBounds(-1.f, 2.f);
		CPPUNIT_ASSERT_EQUAL( -1.f, inControl.LowerBound() );
		CPPUNIT_ASSERT_EQUAL( 2.f, inControl.UpperBound() );
	}
	void testInControl_boundedDefaultValue()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::InControlBase & inControl = concreteIn;
		inControl.SetBounds(0.f, 10.f);
		CPPUNIT_ASSERT_EQUAL( 5.f, inControl.DefaultValue() );
	}
	void testInControl_isBoundedWhenTrue()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::InControlBase & inControl = concreteIn;
		inControl.SetBounds(0.f, 0.f);
		CPPUNIT_ASSERT_EQUAL( true, inControl.IsBounded() );
	}

	void testInControl_setDefaultprevailstoBounds()
	{
		CLAM::TypedInControl<float> concreteIn("InControl");
		CLAM::InControlBase & inControl = concreteIn;
		inControl.SetBounds(0.f, 10.f);
		inControl.SetDefaultValue(0.0f);
		CPPUNIT_ASSERT_EQUAL( 0.0f, inControl.DefaultValue() );
	}
};


} // namespace

