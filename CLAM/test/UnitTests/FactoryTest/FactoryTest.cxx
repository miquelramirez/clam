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
	CPPUNIT_TEST( NewtestCreateFooReturnsAFoo );
	CPPUNIT_TEST( testCreate_ReturnsAFoo );
	CPPUNIT_TEST( NewtestCreate_ReturnsAFoo );
	CPPUNIT_TEST( testCreateSafe_WithABadKey );
	CPPUNIT_TEST( NewtestCreateSafe_WithABadKey );
	CPPUNIT_TEST( testAddCreator_WithRepeatedKey );
	CPPUNIT_TEST( NewtestAddCreator_WithRepeatedKey );
	CPPUNIT_TEST( testAddCreatorSafe_WithRepeatedKey );
	CPPUNIT_TEST( NewtestAddCreatorSafe_WithRepeatedKey );
	CPPUNIT_TEST( testGetRegisteredNames_WithNoKeys );
	CPPUNIT_TEST( NewtestGetRegisteredNames_WithNoKeys );

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
	MyFactoryType::CreatorMethod FooCreator() {
		return MyFactoryType::Registrator<DummyProductFoo>::Create;
	}

	MyFactoryType::CreatorMethod BarCreator() {
		return MyFactoryType::Registrator<DummyProductBar>::Create;
	}

	class NewFooCreator : public MyFactoryType::Creator
	{
	public:
		DummyProduct *Create()
		{	
			return new DummyProductFoo();
		}

	};
	class NewBarCreator : public MyFactoryType::Creator
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
		DummyProduct* returned = FooCreator()();

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *returned );
		delete returned;
	}
	void NewtestCreateFooReturnsAFoo()
	{
		DummyProduct* returned = NewFooCreator().Create();
		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *returned );
		delete returned;
	}

	void testCreate_ReturnsAFoo()
	{
		mTheFactory->AddCreator( "DummyProductFoo", FooCreator() );

		DummyProduct* returned = mTheFactory->Create("DummyProductFoo");
		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *returned );

		// tear down:
		delete returned;
		mTheFactory->Clear();

	}
	void NewtestCreate_ReturnsAFoo()
	{
		mTheFactory->NewAddCreator( "DummyProductFoo", new NewFooCreator() );

		DummyProduct* returned = mTheFactory->NewCreate("DummyProductFoo");
		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *returned );

		// tear down:
		delete returned;
		mTheFactory->Clear();
	}

	void testCreateSafe_WithABadKey()
	{
		try{
			mTheFactory->CreateSafe("DummyProductFoo");
			CPPUNIT_FAIL("Should throw an exception");
		} catch ( CLAM::ErrFactory& ) {}
	}
	void NewtestCreateSafe_WithABadKey()
	{
		try{
			mTheFactory->NewCreateSafe("XXX");
			CPPUNIT_FAIL("Should throw an exception");
		} catch ( CLAM::ErrFactory& ) {}
	}


	void testAddCreator_WithRepeatedKey()
	{
		mTheFactory->AddCreator("DummyProductFoo", FooCreator() );
		try{
			mTheFactory->AddCreator("DummyProductFoo", FooCreator());
			CPPUNIT_FAIL("an assertion should happen");
		} catch ( CLAM::ErrAssertionFailed& )
		{}
	}
	void NewtestAddCreator_WithRepeatedKey()
	{
		mTheFactory->NewAddCreator("DummyProductFoo", new NewFooCreator() );
		try{
			mTheFactory->NewAddCreator("DummyProductFoo", new NewFooCreator());
			CPPUNIT_FAIL("an assertion should happen");
		} catch ( CLAM::ErrAssertionFailed& )
		{}
	}

	void testAddCreatorSafe_WithRepeatedKey()
	{
		mTheFactory->AddCreator("DummyProductFoo", BarCreator() );
		try{
			mTheFactory->AddCreatorSafe("DummyProductFoo", FooCreator());
			CPPUNIT_FAIL("an ErrFactory should be raised");
		} catch (CLAM::ErrFactory&) {

		}
	}
	void NewtestAddCreatorSafe_WithRepeatedKey()
	{
		mTheFactory->NewAddCreator("DummyProductFoo", new NewBarCreator() );
		try{
			mTheFactory->NewAddCreatorSafe("DummyProductFoo", new NewFooCreator());
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
	void NewtestGetRegisteredNames_WithNoKeys()
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
	CPPUNIT_TEST( NewtestCreateFooReturnsAFoo );
	CPPUNIT_TEST( testCreate_ReturnsAFoo );
	CPPUNIT_TEST( NewtestCreate_ReturnsAFoo );
	CPPUNIT_TEST( testCreateSafe_WithABadKey );
	CPPUNIT_TEST( NewtestCreateSafe_WithABadKey );
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

