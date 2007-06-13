#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "DummyProducts.hxx"

#include "Factory.hxx" // CLAM

#include <list>

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
	CPPUNIT_TEST( testGetRegisteredNames_WithNoKeys );

	CPPUNIT_TEST_SUITE_END();

protected:
	typedef CLAM::Factory<DummyProduct> MyFactoryType;

	MyFactoryType* mTheFactory;

public:
	void setUp()
	{
		mTheFactory = new MyFactoryType;
	}
	void tearDown()
	{
		delete mTheFactory;
	}

	// helper methods:
	class FooCreator : public MyFactoryType::Creator
	{
	public:
		DummyProduct *Create()
		{	
			return new DummyProductFoo();
		}

	};
	class BarCreator : public MyFactoryType::Creator
	{
	public:
		DummyProduct *Create()
		{	
			return new DummyProductBar();
		}

	};
	// Tests definition :
protected:
	void testCreateFooReturnsAFoo()
	{
		DummyProduct* returned = FooCreator().Create();
		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *returned );
		delete returned;
	}

	void testCreate_ReturnsAFoo()
	{
		mTheFactory->AddCreator( "DummyProductFoo", new FooCreator() );

		DummyProduct* returned = mTheFactory->Create("DummyProductFoo");
		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *returned );

		// tear down:
		delete returned;
		mTheFactory->Clear();
	}

	void testCreateSafe_WithABadKey()
	{
		try{
			mTheFactory->CreateSafe("XXX");
			CPPUNIT_FAIL("Should throw an exception");
		} catch ( CLAM::ErrFactory& ) {}
	}


	void testAddCreator_WithRepeatedKey()
	{
		mTheFactory->AddCreator("DummyProductFoo", new FooCreator() );
		try{
			mTheFactory->AddCreator("DummyProductFoo", new FooCreator());
			CPPUNIT_FAIL("an assertion should happen");
		} catch ( CLAM::ErrAssertionFailed& )
		{}
	}

	void testAddCreatorSafe_WithRepeatedKey()
	{
		mTheFactory->AddCreator("DummyProductFoo", new BarCreator() );
		try{
			mTheFactory->AddCreatorSafe("DummyProductFoo", new FooCreator());
			CPPUNIT_FAIL("an ErrFactory should be raised");
		} catch (CLAM::ErrFactory&) {

		}
	}

	void testGetRegisteredNames_WithNoKeys()
	{
		std::list<std::string> registeredNames;

		mTheFactory->GetRegisteredNames( registeredNames );

		CPPUNIT_ASSERT_EQUAL( true, registeredNames.empty() );
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
		mTheFactory = &MyFactoryType::GetInstance();
	}

	void tearDown()
	{
		mTheFactory->Clear();
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

