#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "DescriptionScheme.hxx"
#include "DataTypes.hxx"
#include "Component.hxx"


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
			const std::string expected = "No scope registered with that name";
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
			const std::string expected = "No scope registered with that name";
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
