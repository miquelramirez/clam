#include <cppunit/extensions/HelperMacros.h>
#include <typeinfo>
#include "cppUnitHelper.hxx"

#include "DummyProducts.hxx"

#include "Factory.hxx" // CLAM

namespace CLAMTest
{



class FactoryRegistryTest;
CPPUNIT_TEST_SUITE_REGISTRATION( FactoryRegistryTest );

class FactoryRegistryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FactoryRegistryTest );
	CPPUNIT_TEST( testGetCreator_WhenIsEmpty );
	CPPUNIT_TEST( testGetCreatorSafe_WhenIsEmpty );
	CPPUNIT_TEST( testGetCreator_WrongKeyWithASingleCreator );
	CPPUNIT_TEST( testGetCreatorSafe_WrongKeyWithASingleCreator );
	CPPUNIT_TEST( testGetCreator_CorrectKeyWithASingleCreator );
	CPPUNIT_TEST( testGetCreatorSafe_CorrectKeyWithASingleCreator );
	CPPUNIT_TEST( testGetCreator_CorrectKeyWithTwoCreators );
	CPPUNIT_TEST( testGetCreatorSafe_CorrectKeyWithTwoCreators );
	CPPUNIT_TEST( testAddCreator_RepeatedKey );
	CPPUNIT_TEST( testAddCreatorSafe_RepeatedKey );
	CPPUNIT_TEST( testRemoveCreators_WhenIsEmpty );
	CPPUNIT_TEST( testRemoveCreators_WhenNotEmtpy );
	CPPUNIT_TEST( testCount_WhenEmpty );
	CPPUNIT_TEST( testCount_WithTwoCreators );
	CPPUNIT_TEST( testGetRegisteredNames_WhenEmpty );
	CPPUNIT_TEST( testGetRegisteredNames_WhenNotEmpty );
	CPPUNIT_TEST_SUITE_END();

protected:
	typedef CLAM::Factory<DummyProduct> MyFactoryType;

private:

	// helper methods:
	MyFactoryType::CreatorMethod FooCreator() {
		return MyFactoryType::Registrator<DummyProductFoo>::Create;
	}

	MyFactoryType::CreatorMethod BarCreator() {
		return MyFactoryType::Registrator<DummyProductBar>::Create;
	}

	// tests definition
	void testGetCreator_WhenIsEmpty()
	{
		MyFactoryType::Registry reg; // an empty factory register
		try {
			reg.GetCreator("bla");
			CPPUNIT_FAIL( "Assert was expected to happen");

		} catch( CLAM::ErrAssertionFailed& expected ) {
			CPPUNIT_ASSERT_EQUAL(
				std::string("the Factory Registry shouldn't be empty"),
				std::string( expected.what() ) );
		}
	}

	void testGetCreatorSafe_WhenIsEmpty()
	{
		MyFactoryType::Registry reg; // an  empty factor y register
		try {
			reg.GetCreatorSafe("foo");
			CPPUNIT_FAIL( "it was expected to catch a CLAM::ErrFactory" );
		} catch (CLAM::ErrFactory& expected) {
			CPPUNIT_ASSERT_EQUAL_MESSAGE("In ErrFactory message:",
				std::string("GetCreatorSafe invoked on an empty registry"),
				std::string( expected.what() ) );
		}
	}

	void testGetCreator_WrongKeyWithASingleCreator()
	{
		MyFactoryType::Registry reg;
		// set up:
		reg.AddCreator( "DummyProductFoo", FooCreator() );

		try{
			reg.GetCreator("non existent key");
			CPPUNIT_FAIL( "Assertion should happen" );
		} catch (CLAM::ErrAssertionFailed& ) {}
	}

	void testGetCreatorSafe_WrongKeyWithASingleCreator()
	{
		MyFactoryType::Registry reg;
		// set up:
		reg.AddCreator( "DummyProductFoo", FooCreator() );

		try{
			reg.GetCreatorSafe("non existent key");
			CPPUNIT_FAIL( "ErrFactory expected" );
		} catch (CLAM::ErrFactory& ) {}
	}

	void testGetCreator_CorrectKeyWithASingleCreator()
	{
		MyFactoryType::Registry reg;
		// set up:
		MyFactoryType::CreatorMethod inserted;
		inserted = FooCreator();

		reg.AddCreator( "DummyProductFoo", inserted);
		CPPUNIT_ASSERT( inserted == reg.GetCreator("DummyProductFoo") );
	}

	void testGetCreatorSafe_CorrectKeyWithASingleCreator()
	{
		MyFactoryType::Registry reg;
		// set up:
		MyFactoryType::CreatorMethod inserted;
		inserted = FooCreator();

		reg.AddCreator( "DummyProductFoo", inserted);
		CPPUNIT_ASSERT( inserted == reg.GetCreatorSafe("DummyProductFoo") );
	}

	void testGetCreator_CorrectKeyWithTwoCreators()
	{
		MyFactoryType::Registry reg;
		// set up
		reg.AddCreator( "DummyProductFoo", FooCreator() );
		reg.AddCreator( "DummyProductBar", BarCreator() );

		MyFactoryType::CreatorMethod fooCreator = FooCreator();
		CPPUNIT_ASSERT( fooCreator == reg.GetCreator("DummyProductFoo") );
	}

	void testGetCreatorSafe_CorrectKeyWithTwoCreators()
	{
		MyFactoryType::Registry reg;
		// set up
		reg.AddCreator( "DummyProductFoo", FooCreator() );
		reg.AddCreator( "DummyProductBar", BarCreator() );

		MyFactoryType::CreatorMethod fooCreator = FooCreator();
		CPPUNIT_ASSERT( fooCreator == reg.GetCreatorSafe("DummyProductFoo") );
	}

	void testGetCreator_WrongKeyWithTwoCreators()
	{
		MyFactoryType::Registry reg;
		// set up
		reg.AddCreator( "DummyProductFoo", FooCreator() );
		reg.AddCreator( "DummyProductBar", BarCreator() );
		try{
			reg.GetCreator("wrong name");
			CPPUNIT_FAIL( "Assert expected to happen" );
		} catch (CLAM::ErrAssertionFailed& ) {}
	}

	void testGetCreatorSafe_WrongKeyWithTwoCreators()
	{
		MyFactoryType::Registry reg;
		// set up
		reg.AddCreator( "DummyProductFoo", FooCreator() );
		reg.AddCreator( "DummyProductBar", BarCreator() );

		try{
			reg.GetCreatorSafe("incorrect as well");
			CPPUNIT_FAIL( "CLAM::ErrFactory exptected" );
		} catch (CLAM::ErrFactory&) {}
	}

	void testAddCreator_RepeatedKey()
	{
		MyFactoryType::Registry reg;
		reg.AddCreator( "DummyProductFoo", FooCreator() );

		try {
			reg.AddCreator( "DummyProductFoo", BarCreator() );
			CPPUNIT_FAIL( "Assert expected to happen" );

		} catch (CLAM::ErrAssertionFailed& ) {}
	}

	void testAddCreatorSafe_RepeatedKey()
	{
		MyFactoryType::Registry reg;
		reg.AddCreator( "DummyProductFoo", FooCreator() );

		try {
			reg.AddCreatorSafe( "DummyProductFoo", BarCreator() );
			CPPUNIT_FAIL( "CLAM::ErrFactory expected" );

		} catch (CLAM::ErrFactory& expected) {
			CPPUNIT_ASSERT_EQUAL_MESSAGE("In ErrFactory message:",
				std::string("A repeated key was passed"),
				std::string( expected.what() ) );
		}
	}

	void testRemoveCreators_WhenIsEmpty()
	{
		MyFactoryType::Registry reg;
		reg.RemoveAllCreators();
		CPPUNIT_ASSERT_EQUAL_MESSAGE(
			"Count() should be 0 after RemoveAllCreators()",
			std::size_t(0), reg.Count() );
	}

	void testRemoveCreators_WhenNotEmtpy()
	{
		MyFactoryType::Registry reg;
		reg.AddCreator("foo", FooCreator() );
		reg.AddCreator("bar", BarCreator() );

		reg.RemoveAllCreators();
		CPPUNIT_ASSERT_EQUAL_MESSAGE(
			"Count() should be 0 after RemoveAllCreators()",
			std::size_t(0), reg.Count() );
	}

	void testCount_WhenEmpty()
	{
		MyFactoryType::Registry reg;
		CPPUNIT_ASSERT_EQUAL( std::size_t(0), reg.Count() );
	}

	void testCount_WithTwoCreators()
	{
		MyFactoryType::Registry reg;
		reg.AddCreator("foo", FooCreator() );
		reg.AddCreator("bar", BarCreator() );
		CPPUNIT_ASSERT_EQUAL( std::size_t(2), reg.Count() );
	}


	void testGetRegisteredNames_WhenEmpty()
	{
		MyFactoryType::Registry reg;
		std::list<std::string> namesList;

		reg.GetRegisteredNames( namesList );

		CPPUNIT_ASSERT_EQUAL( std::size_t(0), namesList.size() );
	}

	void testGetRegisteredNames_WhenNotEmpty()
	{
		MyFactoryType::Registry reg;
		std::list<MyFactoryType::RegistryKey> namesList;

		reg.AddCreator( "foo", FooCreator() );
		reg.AddCreator( "bar", BarCreator() );

		reg.GetRegisteredNames( namesList );

		CPPUNIT_ASSERT_EQUAL( reg.Count(), namesList.size() );
	}

};




} // namespace CLAMTest

