#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "DescriptionScope.hxx"
#include "DataTypes.hxx"

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
			std::string expected = "Accessing an unexisting attribute inside a scope";
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
			std::string expected = "Accessing an unexisting attribute inside a scope";
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
			std::string expected = "DescriptionScope::Add, Attribute already present";
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
		std::string expected = "Type Missmatch using a pool";
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");
		try
		{
			spec.CheckType(0,(CLAM::TIndex*)0);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
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
