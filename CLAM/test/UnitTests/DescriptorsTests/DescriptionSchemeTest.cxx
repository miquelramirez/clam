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

#include "DescriptionScheme.hxx" // CLAM
#include "DataTypes.hxx" // CLAM
#include "Component.hxx" // CLAM
#include "XMLStorage.hxx" // CLAM


namespace CLAMTest
{

class DescriptionSchemeTest;

CPPUNIT_TEST_SUITE_REGISTRATION( DescriptionSchemeTest );

class DescriptionSchemeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( DescriptionSchemeTest );
	CPPUNIT_TEST( testGetNScopes_withNoScopeRegistered );
	CPPUNIT_TEST( testGetNScopes_withASingleAttribute );
	CPPUNIT_TEST( testGetNScopes_withTwoAttributeOnDifferentScopes );
	CPPUNIT_TEST( testGetNScopes_withTwoAttributeOnTheSameScope );
	CPPUNIT_TEST( testGetScope_withNoScopeRegistered );
	CPPUNIT_TEST( testGetScope_withARegisteredAttribute );
	CPPUNIT_TEST( testGetScope_withTwoScopes );
	CPPUNIT_TEST( testGetScopeByIndex_withIndexOutOfBounds );
	CPPUNIT_TEST( testGetScopeIndex_withNoScopeRegistered );
	CPPUNIT_TEST( testGetScopeIndex_withARegisteredAttribute );
	CPPUNIT_TEST( testGetScopeIndex_withTwoScopes );
	CPPUNIT_TEST( testGetScopeName_withTwoScopes );
	CPPUNIT_TEST( testGetScopeName_withWrongNumber );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testGetNScopes_withNoScopeRegistered()
	{
		CLAM::DescriptionScheme scheme;
		CPPUNIT_ASSERT_EQUAL(0u, scheme.GetNScopes());
	}

	void testGetNScopes_withASingleAttribute()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::TData>("MyScope","MyAttribute");
		CPPUNIT_ASSERT_EQUAL(1u, scheme.GetNScopes());
	}

	void testGetNScopes_withTwoAttributeOnDifferentScopes()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::TData>("MyScope","MyAttribute");
		scheme.AddAttribute<CLAM::TData>("YourScope","YourAttribute");
		CPPUNIT_ASSERT_EQUAL(2u, scheme.GetNScopes());
	}

	void testGetNScopes_withTwoAttributeOnTheSameScope()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::TData>("MyScope","MyAttribute");
		scheme.AddAttribute<CLAM::TData>("MyScope","YourAttribute");
		CPPUNIT_ASSERT_EQUAL(1u, scheme.GetNScopes());
	}

	void testGetScope_withNoScopeRegistered()
	{
		CLAM::DescriptionScheme scheme;
		try
		{
			scheme.GetScope("NonExistent");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Attribute scope 'NonExistent' not found";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGetScope_withARegisteredAttribute()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::TData>("MyScope","MyAttribute");
		scheme.AddAttribute<CLAM::TData>("MyScope","MyOtherAttribute");

		const CLAM::DescriptionScope & spec = scheme.GetScope("MyScope");

		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetIndex("MyOtherAttribute"));
	}

	void testGetScope_withTwoScopes()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::TData>("MyScope","MyAttribute");
		scheme.AddAttribute<CLAM::TData>("YourScope","YourAttribute");
		scheme.AddAttribute<int>("YourScope","YourIntAttribute");

		const CLAM::DescriptionScope & myScope = scheme.GetScope("MyScope");
		const CLAM::DescriptionScope & yourScope = scheme.GetScope("YourScope");

		CPPUNIT_ASSERT_EQUAL(0u,myScope.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(0u,yourScope.GetIndex("YourAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,yourScope.GetIndex("YourIntAttribute"));
	}

	void testGetScopeByIndex_withIndexOutOfBounds()
	{
		CLAM::DescriptionScheme scheme;
		try
		{
			scheme.GetScope(0);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Accessing an illegal scope index for the description scheme";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGetScopeIndex_withNoScopeRegistered()
	{
		CLAM::DescriptionScheme scheme;
		try
		{
			scheme.GetScopeIndex("NonExistent");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Attribute scope 'NonExistent' not found";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGetScopeIndex_withARegisteredAttribute()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::TData>("MyScope","MyAttribute");
		scheme.AddAttribute<CLAM::TData>("MyScope","MyOtherAttribute");

		unsigned scopeIndex = scheme.GetScopeIndex("MyScope");
		const CLAM::DescriptionScope & scope = scheme.GetScope(scopeIndex);

		CPPUNIT_ASSERT_EQUAL(0u,scope.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,scope.GetIndex("MyOtherAttribute"));
	}

	void testGetScopeIndex_withTwoScopes()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::TData>("MyScope","MyAttribute");
		scheme.AddAttribute<CLAM::TData>("YourScope","YourAttribute");
		scheme.AddAttribute<int>("YourScope","YourIntAttribute");

		unsigned myScopeIndex = scheme.GetScopeIndex("MyScope");
		const CLAM::DescriptionScope & myScope = scheme.GetScope(myScopeIndex);
		unsigned yourScopeIndex = scheme.GetScopeIndex("YourScope");
		const CLAM::DescriptionScope & yourScope = scheme.GetScope(yourScopeIndex);

		CPPUNIT_ASSERT_EQUAL(0u,myScope.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(0u,yourScope.GetIndex("YourAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,yourScope.GetIndex("YourIntAttribute"));
	}

	void testGetScopeName_withTwoScopes()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::TData>("MyScope","MyAttribute");
		scheme.AddAttribute<CLAM::TData>("YourScope","YourAttribute");
		CPPUNIT_ASSERT_EQUAL(std::string("MyScope"), scheme.GetScopeName(0));
		CPPUNIT_ASSERT_EQUAL(std::string("YourScope"), scheme.GetScopeName(1));
	}

	void testGetScopeName_withWrongNumber()
	{
		CLAM::DescriptionScheme scheme;
		scheme.AddAttribute<CLAM::TData>("MyScope","MyAttribute");
		try
		{
			scheme.GetScopeName(1);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Accessing an illegal scope index for the description scheme";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

};






} // namespace CLAMTest

