#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "DummyProducts.hxx"

#include "Factory.hxx"


namespace CLAMTest
{


class FactoryTest;
CPPUNIT_TEST_SUITE_REGISTRATION( FactoryTest );

class FactoryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FactoryTest );
	CPPUNIT_TEST( testCreateFooReturnsAFoo );
	CPPUNIT_TEST( testCreate_ReturnsAFoo );
	CPPUNIT_TEST( testCreateSafe_WithABadKey );
	CPPUNIT_TEST( testAddCreator_WithRepeatedKey );
	CPPUNIT_TEST( testAddCreatorSafe_WithRepeatedKey );

	CPPUNIT_TEST_SUITE_END();

protected:
	typedef CLAM::Factory<DummyProduct> MyFactoryType;

	MyFactoryType* _theFactory;

public:
	void setUp()
	{
		_theFactory = new MyFactoryType;
	}
	void tearDown()
	{
		delete _theFactory;
	}

	// helper methods:
	MyFactoryType::CreatorMethod FooCreator() {
		return MyFactoryType::Registrator<DummyProductFoo>::Create;
	}

	MyFactoryType::CreatorMethod BarCreator() {
		return MyFactoryType::Registrator<DummyProductBar>::Create;
	}
	// Tests definition :
protected:
	void testCreateFooReturnsAFoo()
	{
		DummyProduct* returned = FooCreator()();

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *returned );
		delete returned;
	}

	void testCreate_ReturnsAFoo()
	{
		_theFactory->AddCreator( "DummyProductFoo", FooCreator() );

		DummyProduct* returned = _theFactory->Create("DummyProductFoo");
		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *returned );

		// tear down:
		delete returned;
		_theFactory->Clear();

	}

	void testCreateSafe_WithABadKey()
	{
		try{
			_theFactory->CreateSafe("DummyProductFoo");
			CPPUNIT_FAIL("Should throw an exception");
		} catch ( CLAM::ErrFactory& ) {}
	}


	void testAddCreator_WithRepeatedKey()
	{
		_theFactory->AddCreator("DummyProductFoo", FooCreator() );
		try{
			_theFactory->AddCreator("DummyProductFoo", FooCreator());
			CPPUNIT_FAIL("an assertion should happen");
		} catch ( CLAM::ErrAssertionFailed& )
		{}
	}

	void testAddCreatorSafe_WithRepeatedKey()
	{
		_theFactory->AddCreator("DummyProductFoo", BarCreator() );
		try{
			_theFactory->AddCreatorSafe("DummyProductFoo", FooCreator());
			CPPUNIT_FAIL("an ErrFactory should be rised");
		} catch (CLAM::ErrFactory&) {

		}
	}

};



///////////////////////////////////////////////////////////////////////

class FactorySingletonTest;

CPPUNIT_TEST_SUITE_REGISTRATION( FactorySingletonTest );

class FactorySingletonTest : public FactoryTest
{
	CPPUNIT_TEST_SUITE( FactorySingletonTest );
	CPPUNIT_TEST( testCreateFooReturnsAFoo );
	CPPUNIT_TEST( testCreate_ReturnsAFoo );
	CPPUNIT_TEST( testCreateSafe_WithABadKey );
	CPPUNIT_TEST( testFactoryIsSingleton );

	CPPUNIT_TEST_SUITE_END();

public:

	void setUp()
	{
		_theFactory = &MyFactoryType::GetInstance();
	}

	void tearDown()
	{
		_theFactory->Clear();
	}

private:
	void testFactoryIsSingleton()
	{
		MyFactoryType &ref1 = MyFactoryType::GetInstance();
		MyFactoryType &ref2 = MyFactoryType::GetInstance();

		CPPUNIT_ASSERT_MESSAGE(
			"Both Factory refs should point the same object ",
			&ref1 == &ref2);

	}
};




} // namespace CLAMTest
