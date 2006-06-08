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

#include "Enum.hxx" // CLAM
#include <cppunit/extensions/HelperMacros.h>


using namespace CLAM;

/////////////////////////////////////////////////////////////////////
// Test Class
/////////////////////////////////////////////////////////////////////
namespace CLAMTest {
	
	class EnumTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( EnumTest );

	class MyEnum : public Enum {
	public:
		static tValue sDefault;
		MyEnum() : Enum(EnumValues(), sDefault) {}
		MyEnum(tValue v) : Enum(EnumValues(), v) {};
		MyEnum(std::string s) : Enum(EnumValues(), s) {};
		virtual ~MyEnum() {};
		Component * Species() const { return new MyEnum;};

		typedef enum {
			zero=0,
			dos=2,
			cent=100
		} tEnum;
		static Enum::tEnumValue * EnumValues()
		{
			static Enum::tEnumValue sEnumValues[] = {
				{MyEnum::zero,"zero"},
				{MyEnum::dos,"dos"},
				{MyEnum::cent,"cent"},
				{0,NULL}
			};
			return sEnumValues;
		}
	};

	Enum::tValue MyEnum::sDefault = MyEnum::dos;

	class EnumTest : public CppUnit::TestFixture {

		CPPUNIT_TEST_SUITE (CLAMTest::EnumTest);

		CPPUNIT_TEST (testDefaultConstructor);
		CPPUNIT_TEST (testValueConstructor);
		CPPUNIT_TEST (testStringConstructor);
		CPPUNIT_TEST (testSetValue);
		CPPUNIT_TEST (testSetValue_WithString);
		CPPUNIT_TEST (testSetValueSafely_WithIllegalString);
		CPPUNIT_TEST (testSetValueSafely_WithIllegalValue);

		CPPUNIT_TEST_SUITE_END();
	private:
		void testDefaultConstructor()
		{
			MyEnum e;
			CPPUNIT_ASSERT_EQUAL_MESSAGE("Default constructor didn't get the expected value 'dos'",
					std::string("dos"),e.GetString());
		}
		void testValueConstructor()
		{
			MyEnum e(MyEnum::cent);
			CLAM_ASSERT (e.GetString()=="cent", "Value constructor didn't get the expected value 'cent'");
		}

		void testStringConstructor()
		{
			MyEnum e("cent");
			CLAM_ASSERT (e.GetString()=="cent", "String constructor didn't get the expected value 'cent'");
		}

		void testSetValue()
		{
			MyEnum e;
			e.SetValue(0);
			CLAM_ASSERT (e.GetString()=="zero", "SetValue(enum) didn't change the value to 'zero'");
		}
		void testSetValue_WithString()
		{
			MyEnum e;
			e.SetValue("dos");
			CLAM_ASSERT (e.GetString()=="dos", "SetValue(string) didn't change the value to 'dos'");
		}
		void testSetValueSafely_WithIllegalString()
		{
			MyEnum e;
			try {
				e.SetValueSafely("dros");
				CLAM_ASSERT (false, "Exception not thrown, when setting an illegal string symbol");
			} 
			catch (IllegalValue e) {
				// That's ok
			}
		}
		void testSetValueSafely_WithIllegalValue()
		{
			MyEnum e;
			try {
				e.SetValueSafely(4);
				CLAM_ASSERT (false, "Exception not thrown, when setting an illegal integer value");
			} 
			catch (IllegalValue e) {
				// That's ok
			}
		}
	};


}




