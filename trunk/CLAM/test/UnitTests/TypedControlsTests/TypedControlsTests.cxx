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
		 CPPUNIT_TEST_SUITE_END();
		
		void testTypedInControl_DoControl_ChangesInternalState()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			int number=1;
			in.DoControl(number);
			CPPUNIT_ASSERT_EQUAL( 1, in.GetLastValue() );
		}

		void testLinkAndSendControl_ChangesTypedInControlInternalState()
		{
			CLAM::TypedInControl<int> in("IntInControl");
			CLAM::TypedOutControl<int> out("IntOutControl");
			out.AddLink(in);
			int number=1;
			out.SendControl(number);
			CPPUNIT_ASSERT_EQUAL( 1 , in.GetLastValue() );
		}

	};
		
} // namespace
