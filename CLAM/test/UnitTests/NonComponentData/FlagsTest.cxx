/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

// TODO: Symbolic operation


#include "Flags.hxx" // CLAM
#include <cppunit/extensions/HelperMacros.h>
#include <string>
#include "XMLTestHelper.hxx"


using namespace CLAM;

/////////////////////////////////////////////////////////////////////
// Test Class
/////////////////////////////////////////////////////////////////////
namespace CLAMTest {

	class FlagsTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( FlagsTest );

	class MyFlags : public Flags<5> {
	// Construction/Destruction
	public:
		static tFlagValue sFlagValues[];
		static tValue sDefault;
		virtual Component * Species() const {
			return new MyFlags();
		}
		typedef enum {
			eFlag0=0,
			eFlag1=1,
			eFlag2=2,
			eFlag3=3,
			eFlag4=4
		} tFlag;
		MyFlags () :
			Flags<5>(sFlagValues),
			flag0(operator[](eFlag0)),
			flag1(operator[](eFlag1)),
			flag2(operator[](eFlag2)),
			flag3(operator[](eFlag3)),
			flag4(operator[](eFlag4))
		{
			flag4=true;
		};
		MyFlags (const MyFlags& someFlags) :
			Flags<5>(sFlagValues,someFlags),
			flag0(operator[](eFlag0)),
			flag1(operator[](eFlag1)),
			flag2(operator[](eFlag2)),
			flag3(operator[](eFlag3)),
			flag4(operator[](eFlag4))
		{};

		MyFlags(int i) :
		Flags<5>(sFlagValues, i),
			flag0(operator[](eFlag0)),
			flag1(operator[](eFlag1)),
			flag2(operator[](eFlag2)),
			flag3(operator[](eFlag3)),
			flag4(operator[](eFlag4))
		{};

		reference flag0;
		reference flag1;
		reference flag2;
		reference flag3;
		reference flag4;

		bool Matches(bool f0, bool f1, bool f2, bool f3, bool f4) {
			// This check is done in this weird way in order not to
			// mask type errors with phantom implicit conversions
			bool ok = FullfilsInvariant();
//			ok &= (f2? flag2: !flag2);
			if (f0!=operator[](0)) ok=ReportNotMatch(0,f0,operator[](0));
			if (f1!=operator[](1)) ok=ReportNotMatch(1,f1,operator[](1));
			if (f2!=operator[](2)) ok=ReportNotMatch(2,f2,operator[](2));
			if (f3!=operator[](3)) ok=ReportNotMatch(3,f3,operator[](3));
			if (f4!=operator[](4)) ok=ReportNotMatch(4,f4,operator[](4));
			return ok;
		}
		bool ReportNotMatch(const int place, const bool expected, const bool found) {
			const bool verbose = true;
			if (verbose) {
				std::cout
					<< "Flag at positon " << place
					<< " expected: " << expected
					<< " and found: " << found
					<< std::endl;
			}
			return false;
		}
		bool FullfilsInvariant() {
			bool ok = true;
			if (flag0!=operator[](0)) ok=ReportReferencesDoNotMatch(0,flag0,operator[](0));
			if (flag1!=operator[](1)) ok=ReportReferencesDoNotMatch(1,flag1,operator[](1));
			if (flag2!=operator[](2)) ok=ReportReferencesDoNotMatch(2,flag2,operator[](2));
			if (flag3!=operator[](3)) ok=ReportReferencesDoNotMatch(3,flag3,operator[](3));
			if (flag4!=operator[](4)) ok=ReportReferencesDoNotMatch(4,flag4,operator[](4));
			return ok;
		}
		bool ReportReferencesDoNotMatch(const int place, const bool ref, const bool op) {
			const bool verbose = true;
			if (verbose) {
				std::cout
					<< "Internal bit reference inconsistency"
					<< " at positon " << place
					<< " by reference: " << ref
					<< " by [] operator: " << op << " "
					<< std::endl;
			}
			return false;
		}
		static void TestInputOutput ();
		static void TestOperations ();
	};

	Flags<5>::tFlagValue MyFlags::sFlagValues[] = {
		{MyFlags::eFlag0, "flag0"},
		{MyFlags::eFlag1, "flag1"},
		{MyFlags::eFlag2, "flag2"},
		{MyFlags::eFlag3, "flag3"},
		{MyFlags::eFlag4, "flag4"},
		{0,NULL}
	};



	/**
	* @todo Symbolic operation
	* @todo Remove console output
	*/
	class FlagsTest : public CppUnit::TestFixture {

		CPPUNIT_TEST_SUITE (CLAMTest::FlagsTest);

		CPPUNIT_TEST (testDefaultConstructor);
		CPPUNIT_TEST (testValueConstructor);
		CPPUNIT_TEST (testEqualOperator_withEquivalentFlags);
		CPPUNIT_TEST (testEqualOperator_withDifferentFlags);
		CPPUNIT_TEST (testCopyConstructor_AdoptValuesValues);
		CPPUNIT_TEST (testCopyConstructor_DoesNotCreateAliasingOnWriteCopy);
		CPPUNIT_TEST (testCopyConstructor_DoesNotCreateAliasingOnWriteOriginal);
		CPPUNIT_TEST (testOrAssignmentOperator);
		CPPUNIT_TEST (testReset);
		CPPUNIT_TEST (testDirectFlagSetting_withTrue_whenSet);
		CPPUNIT_TEST (testDirectFlagSetting_withTrue_whenNotSet);
		CPPUNIT_TEST (testDirectFlagSetting_withFalse_whenSet);
		CPPUNIT_TEST (testDirectFlagSetting_withFalse_whenNotSet);
		CPPUNIT_TEST (testFlip_whenSet);
		CPPUNIT_TEST (testFlip_whenNotSet);
		CPPUNIT_TEST (testExtraction_withCorrectInputs);
		CPPUNIT_TEST (testExtraction_withIncorrectInputs);
		CPPUNIT_TEST (testExtraction_withResetFlags);
		CPPUNIT_TEST (testReloadingXML);
//		CPPUNIT_TEST (testStringConstructor);
//		CPPUNIT_TEST (testSetValue);
//		CPPUNIT_TEST (testSetValue_WithString);
//		CPPUNIT_TEST (testSetValueSafely_WithIllegalString);
//		CPPUNIT_TEST (testSetValueSafely_WithIllegalValue);

		CPPUNIT_TEST_SUITE_END();
	private:
		void testDefaultConstructor()
		{
			MyFlags defaultConstructedFlags;
			CPPUNIT_ASSERT_MESSAGE("All but Flag4 must be off by default",
				defaultConstructedFlags.Matches(0,0,0,0,1));
		}

		void testValueConstructor()
		{
			MyFlags flags1and2=3;
			CPPUNIT_ASSERT_MESSAGE("Only Flags0 and Flags1 must be on",
				flags1and2.Matches(1,1,0,0,0));
		}

		void testEqualOperator_withEquivalentFlags()
		{
			MyFlags flags1and2=3;
			MyFlags flags1and2sibbling=3;
			CPPUNIT_ASSERT_EQUAL(true, flags1and2 == flags1and2sibbling);
		}
		void testEqualOperator_withDifferentFlags()
		{
			MyFlags flags1and2=3;
			MyFlags onlyFlag4;
			CPPUNIT_ASSERT_EQUAL(false, flags1and2 == onlyFlag4);
		}

		void testCopyConstructor_AdoptValuesValues()
		{
			MyFlags original=7;
			MyFlags copied(original);
			CPPUNIT_ASSERT_EQUAL(original,copied);
		}

		void testCopyConstructor_DoesNotCreateAliasingOnWriteCopy()
		{
			MyFlags original=7;
			MyFlags copied(original);
			MyFlags expectedOriginal=7;
			MyFlags expectedCopied=6;
			copied.flag0=false;
			CPPUNIT_ASSERT_EQUAL(expectedOriginal, original);
			CPPUNIT_ASSERT_EQUAL(expectedCopied, copied);
		}

		void testCopyConstructor_DoesNotCreateAliasingOnWriteOriginal()
		{
			MyFlags original=7;
			MyFlags copied(original);
			MyFlags expectedOriginal=6;
			MyFlags expectedCopied=7;
			original.flag0=false;
			CPPUNIT_ASSERT_EQUAL(expectedOriginal, original);
			CPPUNIT_ASSERT_EQUAL(expectedCopied, copied);
		}

		void testOrAssignmentOperator()
		{
			MyFlags flags4=16;
			MyFlags toBeOred=3;
			MyFlags expected= (3|16);

			toBeOred |= flags4;

			CPPUNIT_ASSERT_EQUAL(expected, toBeOred);
		}

		void testReset()
		{
			MyFlags resetted=5;
			MyFlags expected=0;
			resetted.reset();
			CPPUNIT_ASSERT_EQUAL(expected, resetted);
		}

		void testDirectFlagSetting_withTrue_whenNotSet()
		{
			MyFlags toBeSet=5;
			MyFlags expected=(5|16);
			toBeSet.flag4=true;
			CPPUNIT_ASSERT_EQUAL(expected, toBeSet);
		}

		void testDirectFlagSetting_withTrue_whenSet()
		{
			MyFlags toBeSet=5;
			MyFlags expected=5;
			toBeSet.flag2=true;
			CPPUNIT_ASSERT_EQUAL(expected, toBeSet);
		}

		void testDirectFlagSetting_withFalse_whenNotSet()
		{
			MyFlags toBeSet=5;
			MyFlags expected=5;
			toBeSet.flag3=false;
			CPPUNIT_ASSERT_EQUAL(expected, toBeSet);
		}

		void testDirectFlagSetting_withFalse_whenSet()
		{
			MyFlags toBeSet=5;
			MyFlags expected=5 & ~1;
			toBeSet.flag0=false;
			CPPUNIT_ASSERT_EQUAL(expected, toBeSet);
		}

		void testFlip_whenSet()
		{
			MyFlags toBeFlipped=5;
			MyFlags expected=5 & ~4;
			toBeFlipped.flag2.flip();
			CPPUNIT_ASSERT_EQUAL(expected, toBeFlipped);
		}

		void testFlip_whenNotSet()
		{
			MyFlags toBeFlipped=5;
			MyFlags expected=5 | 16;
			toBeFlipped.flag4.flip();
			CPPUNIT_ASSERT_EQUAL(expected, toBeFlipped);
		}

		void testReloadingXML()
		{
			MyFlags A=5;
			MyFlags B=21;
			MyFlags C=0;
			CPPUNIT_ASSERT_MESSAGE(
				"Failed: The loaded version of A stores differently than the original A",
				XMLInputOutputMatches(A,"FlagA.xml"));
			CPPUNIT_ASSERT_MESSAGE(
				"Failed: The loaded version of B stores differently than the original B",
				XMLInputOutputMatches(B,"FlagB.xml"));
			CPPUNIT_ASSERT_MESSAGE(
				"Failed: The loaded version of C stores differently than the original C",
				XMLInputOutputMatches(C,"FlagC.xml"));
		}

		void testExtraction_withIncorrectInputs()
		{
			const char * inputstring[] =
			{
				"{flag0 fla2 flag3 }",
				"{fla0 flag2 flag3 }",
				NULL
			};

			for (int i=0; inputstring[i]; i++) {
				std::stringstream is(inputstring[i]);
				MyFlags flag=16;
				MyFlags expected=16;
				try {
					is >> flag;
					CPPUNIT_FAIL("Incorrect input did not fail");
				}
				catch(...) {
					CPPUNIT_ASSERT_EQUAL(expected, flag);
				}
			}
		}

		void testExtraction_withCorrectInputs() {
			const char * inputstring[] = {
				"{flag0 flag2 flag3}",
				"{ flag0 flag2 flag3 }",
				" { flag0 flag2 flag3 } ",
				" { flag0 flag2 flag3 } ",
				NULL
			};
			MyFlags expected = 13;
			for (int i=0; inputstring[i]; i++) {
				std::stringstream is(inputstring[i]);
				MyFlags read;
				is >> read;
				CPPUNIT_ASSERT_EQUAL(expected, read);
			}
		}

		void testExtraction_withResetFlags() {
			const char * inputstring[] = {
				"{}",
				" {  } ",
				" { } ",
				NULL
			};
			MyFlags expected = 0;
			for (int i=0; inputstring[i]; i++) {
				std::stringstream is(inputstring[i]);
				MyFlags read;
				is >> read;
				CPPUNIT_ASSERT_EQUAL(expected, read);
			}
		}
	};

}




