#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "DummyProducts.hxx"

#include "Factory.hxx" // CLAM
#include "FactoryOnOtherTranslationUnits.hxx"

namespace CLAMTest
{


typedef CLAM::Factory< A > FactoryOfAs;

class FactoryRegistratorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( FactoryRegistratorTest );

class FactoryRegistratorTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FactoryRegistratorTest );
	CPPUNIT_TEST( testCreate_ReturnsAnObjectOfTheTemplateType );
	CPPUNIT_TEST( testConstructorPassingFactory_RegistersCreator );
	CPPUNIT_TEST( testConstructorPassingKeyAndFactory_RegistersCreator );
	CPPUNIT_TEST( testConstructorPassingKey_RegistersCreator );
	CPPUNIT_TEST( testDefaultConstructor_RegistersCreator );
	CPPUNIT_TEST( testRegistratorsAsStaticObjects_FactoryUnicity );
	CPPUNIT_TEST( testRegistratorsAsStaticObjects_ProductsRegistered );
	CPPUNIT_TEST_SUITE_END();

protected:
	typedef CLAM::Factory<DummyProduct> MyFactoryType;

private:
	void testCreate_ReturnsAnObjectOfTheTemplateType()
	{
		 DummyProduct* created =
			 MyFactoryType::Registrator<DummyProductFoo>::ConcreteCreator().Create();

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *created );
		delete created;
	}

	void testConstructorPassingKeyAndFactory_RegistersCreator()
	{
		MyFactoryType fact;
		const char* fooClassName = "DummyProductFoo";

		MyFactoryType::Registrator<DummyProductFoo> regt( fooClassName, fact );

		DummyProduct *created =
			fact.Create( fooClassName );

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *created );
		delete created;
	}

	void testConstructorPassingFactory_RegistersCreator()
	{
		DummyProductFoo dummy; //needed for calling GetClassName()
		// here we use the name just for creating. Not for registrating
		const char* fooClassName = dummy.GetClassName();

		MyFactoryType fact;
		// passing just the factory to the constructor
		MyFactoryType::Registrator<DummyProductFoo> regt( fact );

		DummyProduct *created =
			fact.Create( fooClassName );

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *created );
		delete created;
	}

	void testConstructorPassingKey_RegistersCreator()
	{
		MyFactoryType &theFactory = MyFactoryType::GetInstance();
		theFactory.Clear();

		DummyProductFoo dummy;
		MyFactoryType::Registrator<DummyProductFoo> regt( dummy.GetClassName() );

		DummyProduct *created =
			theFactory.Create( dummy.GetClassName() );

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *created );

		// tear down
		delete created;
		theFactory.Clear();

	}

	void testDefaultConstructor_RegistersCreator()
	{
		MyFactoryType &theFactory = MyFactoryType::GetInstance();
		theFactory.Clear();

		MyFactoryType::Registrator<DummyProductFoo> DummyRegt;

		DummyProductFoo dummy;
		DummyProduct *created =
			theFactory.Create( dummy.GetClassName() );

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *created );

		// tear down
		delete created;
		theFactory.Clear();
	}

	void testRegistratorsAsStaticObjects_ProductsRegistered()
	{
		FactoryOfAs& theFactory = FactoryOfAs::GetInstance();
		std::list< std::string > keysInFactory;

		theFactory.GetRegisteredNames( keysInFactory );

		CPPUNIT_ASSERT_EQUAL( false, keysInFactory.empty() );
	}

	void testRegistratorsAsStaticObjects_FactoryUnicity()
	{
		FactoryOfAs& theFactory = FactoryOfAs::GetInstance();

		CPPUNIT_ASSERT( RegistratorStub::StaticInstance().AddressOfFactory() 
				== &theFactory);
	}

};



} // namespace CLAMTest

