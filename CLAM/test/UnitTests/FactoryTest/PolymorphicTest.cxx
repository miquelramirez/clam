#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "DummyProducts.hxx"

#include "Factory.hxx"
#include "Polymorphic.hxx"

#include <list>
#include <sstream>

namespace CLAMTest
{


class PolymorphicTest;
CPPUNIT_TEST_SUITE_REGISTRATION( PolymorphicTest );

class PolymorphicTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PolymorphicTest );
	CPPUNIT_TEST(testAdapte_byDefaultPointsNull);
/*
	CPPUNIT_TEST(testAdapte_createsAFooWithSuchParameter);
	CPPUNIT_TEST(testAdapte_createsABarWithSuchParameter);
	CPPUNIT_TEST(testConstGet);
	CPPUNIT_TEST(testReferenceConversor);
	CPPUNIT_TEST(testConstructorFromReference);
	CPPUNIT_TEST(testAssignFromReference);
	CPPUNIT_TEST(testGetClassName);
	CPPUNIT_TEST(testXmlDump_whenFoo);
	CPPUNIT_TEST(testXmlDump_whenBar);
	CPPUNIT_TEST(testXmlRestore_whenFoo);
	CPPUNIT_TEST(testXmlRestore_whenBar);
*/
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
	MyFactoryType::CreatorMethod FooCreator()
	{
		return MyFactoryType::Registrator<DummyProductFoo>::Create;
	}

	MyFactoryType::CreatorMethod BarCreator()
	{
		return MyFactoryType::Registrator<DummyProductBar>::Create;
	}


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

		delete expected;
	}

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
		std::string className = "Polymorphic";
		MyPolymorphicType poly;
		std::string result = poly.GetClassName();
		CPPUNIT_ASSERT_EQUAL( className , result);
	}

	void testXmlDump_whenFoo()
	{
		std::string className = "DummyProductFoo";
		MyPolymorphicType poly(className);
		std::stringstream stream;
		CLAM::XMLStorage::Dump(poly,"Poly",stream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<Poly type=\"DummyProductFoo\">"
				"FooContent"
			"</Poly>"
			),stream.str());
	}

	void testXmlDump_whenBar()
	{
		std::string className = "DummyProductBar";
		MyPolymorphicType poly(className);
		std::stringstream stream;
		CLAM::XMLStorage::Dump(poly,"Poly",stream);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"<Poly type=\"DummyProductBar\">"
				"BarContent"
			"</Poly>"
			),stream.str());
	}

	void testXmlRestore_whenFoo()
	{
		std::stringstream stream(
			"<Poly type=\"DummyProductFoo\">"
				"FooContent"
			"</Poly>");
		MyPolymorphicType poly;
		CLAM::XMLStorage::Restore(poly,stream);
		DummyProduct & adaptee = poly.Get();

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, adaptee);

//		CPPUNIT_ASSERT_EQUAL(
//			std::string("FooContent"),
//			((DummyProductFoo&)adaptee).content());
	}

	void testXmlRestore_whenBar()
	{
		std::stringstream stream(
			"<Poly type=\"DummyProductBar\">"
				"BarContent"
			"</Poly>");
		MyPolymorphicType poly;
		CLAM::XMLStorage::Restore(poly,stream);
		DummyProduct & adaptee = poly.Get();

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductBar, adaptee);

//		CPPUNIT_ASSERT_EQUAL(
//			std::string("BarContent"),
//			((DummyProductBar&)adaptee).content());
	}

};



} // namespace CLAMTest

