#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "DummyProducts.hxx"

#include "Factory.hxx" // CLAM
#include "FactoryOnOtherTranslationUnits.hxx"

namespace CLAMTest
{


class FactoryRegistratorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( FactoryRegistratorTest );

class FactoryRegistratorTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( FactoryRegistratorTest );
	CPPUNIT_TEST( testCreate_ReturnsAnObjectOfTheTemplateType );
	CPPUNIT_TEST( testConstructorPassingFactory_RegistersCreator );
	CPPUNIT_TEST( testConstructorPassingKeyAndFactory_RegistersCreator );
	CPPUNIT_TEST( testConstructorPassingMetadata_AssertsWhenNoKey );
	CPPUNIT_TEST( testConstructorPassingMetadata_RegistersCreator );
	CPPUNIT_TEST( testConstructorPassingKey_RegistersCreator );
//	CPPUNIT_TEST( testDefaultConstructor_RegistersCreator ); //deprecated functionality
	CPPUNIT_TEST( testRegistratorsAsStaticObjects_FactoryUnicity );
	CPPUNIT_TEST( testRegistratorsAsStaticObjects_ProductsRegistered );
	CPPUNIT_TEST_SUITE_END();

protected:
	class MyFactoryType : public CLAM::Factory<DummyProduct>
	{
		public:
		static MyFactoryType& GetInstance()
		{
			static MyFactoryType theInstance;
			return theInstance;
		}

	};
private:
	void testCreate_ReturnsAnObjectOfTheTemplateType()
	{
		 DummyProduct* created =
			 CLAM::FactoryRegistrator<MyFactoryType, DummyProductFoo>::ConcreteCreator().Create();

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *created );
		delete created;
	}

	void testConstructorPassingMetadata_AssertsWhenNoKey()
	{
		MyFactoryType & fact = MyFactoryType::GetInstance();
		fact.Clear();
		const char* metadata[] = {
			"some attribute", "DummyProductFoo",
			0
		};
		try 
		{
			CLAM::FactoryRegistrator<MyFactoryType, DummyProductFoo> regt = metadata;
		}
		catch ( CLAM::ErrAssertionFailed & err )
		{
			CPPUNIT_ASSERT_EQUAL(std::string("FactoryRegistrator: first char* metadata should be 'key'"), std::string(err.what()) );
		}

	}
	void testConstructorPassingMetadata_RegistersCreator()
	{
		MyFactoryType & fact = MyFactoryType::GetInstance();
		fact.Clear();

		const char* metadata[] = {
			"key", "DummyProductFoo",
			0
		};

		CLAM::FactoryRegistrator<MyFactoryType, DummyProductFoo> regt = metadata;

		DummyProduct *created = fact.Create( "DummyProductFoo" );

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *created );
		delete created;
	}

	void testConstructorPassingKeyAndFactory_RegistersCreator()
	{
		MyFactoryType fact;
		const char* fooClassName = "DummyProductFoo";

		CLAM::FactoryRegistrator<MyFactoryType, DummyProductFoo> regt( fooClassName, fact );

		DummyProduct *created =	fact.Create( fooClassName );

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
		CLAM::FactoryRegistrator<MyFactoryType, DummyProductFoo> regt( fact );

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
		CLAM::FactoryRegistrator<MyFactoryType, DummyProductFoo> regt( dummy.GetClassName() );

		DummyProduct *created =
			theFactory.Create( dummy.GetClassName() );

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *created );

		// tear down
		delete created;
		theFactory.Clear();

	}
/*
	//deprecated functionality (Pau)
	void testDefaultConstructor_RegistersCreator()
	{
		MyFactoryType &theFactory = MyFactoryType::GetInstance();
		theFactory.Clear();

		CLAM::FactoryRegistrator<MyFactoryType, DummyProductFoo> DummyRegt;

		DummyProductFoo dummy;
		DummyProduct *created =
			theFactory.Create( dummy.GetClassName() );

		CLAMTEST_ASSERT_EQUAL_RTTYPES( DummyProductFoo, *created );

		// tear down
		delete created;
		theFactory.Clear();
	}
*/
	void testRegistratorsAsStaticObjects_ProductsRegistered()
	{
		FactoryOfAs& theFactory = FactoryOfAs::GetInstance();
		std::list< std::string > keysInFactory;

		theFactory.GetRegisteredNames( keysInFactory );

		CPPUNIT_ASSERT( false==keysInFactory.empty() );
	}

	void testRegistratorsAsStaticObjects_FactoryUnicity()
	{
		FactoryOfAs& theFactory = FactoryOfAs::GetInstance();

		CPPUNIT_ASSERT( RegistratorStub::StaticInstance().AddressOfFactory() 
				== &theFactory);
	}

};



} // namespace CLAMTest

