#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "DummyProducts.hxx"

#include "Factory.hxx"
#include "Polymorphic.hxx"

#include <list>

namespace CLAMTest
{


class PolymorphicTest;
CPPUNIT_TEST_SUITE_REGISTRATION( PolymorphicTest );

class PolymorphicTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PolymorphicTest );
	CPPUNIT_TEST(testAdapte_byDefaultPointsNull);
	CPPUNIT_TEST(testAdapte_createsAFooWithSuchParameter);
	CPPUNIT_TEST(testAdapte_createsABarWithSuchParameter);
	CPPUNIT_TEST(testConstGet);
	CPPUNIT_TEST(testReferenceConversor);
	CPPUNIT_TEST(testConstructorFromReference);
//	CPPUNIT_TEST(testConstructorFromConstReference);
	CPPUNIT_TEST(testAssignFromReference);
	CPPUNIT_TEST(testGetClassName);

	CPPUNIT_TEST_SUITE_END();

protected:
	typedef CLAM::Factory<DummyProduct> MyFactoryType;
	typedef CLAM::Polymorphic<MyFactoryType> MyPolymorphicType;

	MyFactoryType* mTheFactory;

public:
	void setUp()
	{
		mTheFactory = &MyFactoryType::GetInstance();
		mTheFactory->Clear();
		MyFactoryType::Registrator<DummyProductFoo> dummyRegtFoo;
		MyFactoryType::Registrator<DummyProductBar> dummyRegtBar;
	}

	void tearDown()
	{
		mTheFactory->Clear();
	}

private:

	// helper methods:
	MyFactoryType::CreatorMethod FooCreator() {
		return MyFactoryType::Registrator<DummyProductFoo>::Create;
	}

	MyFactoryType::CreatorMethod BarCreator() {
		return MyFactoryType::Registrator<DummyProductBar>::Create;
	}


	// Tests definition :
protected:

	void testAdapte_byDefaultPointsNull()
	{
		MyPolymorphicType poly;
		try
		{
			DummyProduct & adaptee = poly.Get();
			CPPUNIT_FAIL("Should have failed an assertion");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("Derreferencing a null polymorph pointer"),
				std::string(err.what()));
		}
	}

	void testAdapte_createsAFooWithSuchParameter()
	{
		MyPolymorphicType poly("DummyProductFoo");
		DummyProduct & adaptee = poly.Get();
		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, adaptee);
	}

	void testAdapte_createsABarWithSuchParameter()
	{
		MyPolymorphicType poly("DummyProductBar");
		DummyProduct & adaptee = poly.Get();
		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductBar, adaptee);
	}

	void testConstGet()
	{
		const MyPolymorphicType poly("DummyProductBar");
		const DummyProduct & adaptee = poly.Get();
		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductBar, adaptee);
	}

	void testReferenceConversor()
	{
		MyPolymorphicType poly("DummyProductFoo");
		DummyProduct & expected = poly.Get();
		DummyProduct & result = poly;
		CPPUNIT_ASSERT_EQUAL( &expected , &result);
	}

	void testConstReferenceConversor()
	{
		const MyPolymorphicType poly("DummyProductFoo");
		const DummyProduct & expected = poly.Get();
		const DummyProduct & result = poly;
		CPPUNIT_ASSERT_EQUAL( &expected , &result);
	}

	void testConstructorFromReference()
	{
		DummyProduct * expected = BarCreator()();
		MyPolymorphicType poly(*expected);
		DummyProduct & result = poly;
		CPPUNIT_ASSERT_EQUAL( expected , &result);
	}
/*
	void testConstructorFromConstReference()
	{
		const DummyProduct * expected = BarCreator()();
		const MyPolymorphicType poly(*expected);
		const DummyProduct & result = poly;
		CPPUNIT_ASSERT_EQUAL( expected , &result);
	}
*/
	void testAssignFromReference()
	{
		DummyProduct * expected = BarCreator()();
		MyPolymorphicType poly;
		poly = *expected;
		DummyProduct & result = poly;
		CPPUNIT_ASSERT_EQUAL( expected , &result);
	}

	void testGetClassName()
	{
		std::string className = "DummyProductBar";
		MyPolymorphicType poly(className);
		std::string result = poly.GetClassName();
		CPPUNIT_ASSERT_EQUAL( className , result);
	}




	/////////// The edge

	void testCreateFooReturnsAFoo()
	{
		DummyProduct* returned = FooCreator()();

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

	void testCreateSafe_WithABadKey()
	{
		try{
			mTheFactory->CreateSafe("DummyProductFoo");
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

	void testAddCreatorSafe_WithRepeatedKey()
	{
		mTheFactory->AddCreator("DummyProductFoo", BarCreator() );
		try{
			mTheFactory->AddCreatorSafe("DummyProductFoo", FooCreator());
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



} // namespace CLAMTest
