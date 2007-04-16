/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "DescriptionScope.hxx" // CLAM
#include "DataTypes.hxx" // CLAM

namespace CLAMTest
{

class DescriptionScopeTest;

CPPUNIT_TEST_SUITE_REGISTRATION( DescriptionScopeTest );

class DescriptionScopeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( DescriptionScopeTest );
	CPPUNIT_TEST( testGetIndex_whenEmpty );
	CPPUNIT_TEST( testGetIndex_withOneInserted );
	CPPUNIT_TEST( testGetIndex_withAWrongName );
	CPPUNIT_TEST( testGetIndex_withSecondInsertedArray );
	CPPUNIT_TEST( testAddAttribute_whenNameAlreadyAdded );
	CPPUNIT_TEST( testAdding_DifferentTypes );
	CPPUNIT_TEST( testCheckType_withOtherType );
	CPPUNIT_TEST( testCheckType_withSameType );
	CPPUNIT_TEST( testGetAttributeName_withTwoScopes );
	CPPUNIT_TEST( testGetAttributeName_withWrongNumber );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testGetIndex_whenEmpty()
	{
		CLAM::DescriptionScope spec("TestScope");
		try
		{
			spec.GetIndex("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			std::string expected = "Accessing an unexisting attribute 'TestScope':'MyAttribute'";
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetNAttributes());
	}

	void testGetIndex_withOneInserted()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetNAttributes());
	}

	void testGetIndex_withAWrongName()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");
		try
		{
			spec.GetIndex("Foo");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			std::string expected = "Accessing an unexisting attribute 'TestScope':'Foo'";
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetNAttributes());
	}

	void testGetIndex_withSecondInsertedArray()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");
		spec.Add<CLAM::TData>("Foo");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetIndex("Foo"));
		CPPUNIT_ASSERT_EQUAL(2u,spec.GetNAttributes());
	}

	void testAddAttribute_whenNameAlreadyAdded()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");
		try
		{
			spec.Add<CLAM::TData>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			std::string expected = "Couldn't add attribute 'TestScope:MyAttribute', already present in the scope";
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetNAttributes());
	}

	void testAdding_DifferentTypes()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");
		spec.Add<CLAM::TIndex>("Foo");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetIndex("Foo"));
		CPPUNIT_ASSERT_EQUAL(2u,spec.GetNAttributes());
	}

	void testCheckType_withOtherType()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");
		try
		{
			spec.CheckType(0,(CLAM::TIndex*)0);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			std::string expected = std::string("Attribute 'TestScope:MyAttribute' has been used as type '")
				+ typeid(int).name() + "' but it really was of type '" + typeid(float).name() + "'";
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
	}

	void testCheckType_withSameType()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");
		spec.CheckType(0,(CLAM::TData*)0);
	}

	void testGetAttributeName_withTwoScopes()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add< CLAM::TData >("MyAttribute");
		scope.Add< CLAM::TData >("YourAttribute");
		CPPUNIT_ASSERT_EQUAL(std::string("MyAttribute"), scope.GetAttributeName(0));
		CPPUNIT_ASSERT_EQUAL(std::string("YourAttribute"), scope.GetAttributeName(1));
	}

	void testGetAttributeName_withWrongNumber()
	{
		CLAM::DescriptionScope scope("TestScope");
		scope.Add< CLAM::TData >("MyAttribute");
		try
		{
			scope.GetAttributeName(1);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "GetAttributeName: Using a wrong index to look up an attribute name";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}
};


} // namespace CLAMTest

