#ifndef USE_LADSPA
#warning "LADSPA tests disabled by CLAM linking options"
#else

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include <string>
#include "Processing.hxx"
#include "LadspaFactory.hxx"
#include "LadspaWrapper.hxx"
#include "LadspaWrapperCreator.hxx"

namespace CLAMTest
{

class LadspaFactoryTest;
CPPUNIT_TEST_SUITE_REGISTRATION( LadspaFactoryTest );

class LadspaFactoryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( LadspaFactoryTest );
	CPPUNIT_TEST( testCreate_existing );
	CPPUNIT_TEST( testCreate_nonExistingKey_throws );
	CPPUNIT_TEST( testCreate_nonExistingFileName_throws );
	CPPUNIT_TEST( testCreate_withTwoCreators );
	CPPUNIT_TEST( testCreate_emptyCreatorMap_throws );
	CPPUNIT_TEST_SUITE_END();
protected:
	void testCreate_existing()
	{
		CLAM::LadspaFactory factory;
		const std::string libName = "factoryID";
		factory.AddCreator( "sine", new CLAM::LadspaWrapperCreator("/usr/lib/ladspa/sine.so", 1, libName) ) ;
		CLAM::Processing* processing = factory.CreateSafe("sine");
		CLAM::LadspaWrapper* wrapper = dynamic_cast<CLAM::LadspaWrapper*>(processing);
		CPPUNIT_ASSERT_EQUAL( libName, std::string(wrapper->GetClassName()) );
	}
	void testCreate_nonExistingKey_throws()
	{
		CLAM::LadspaFactory factory;
		factory.AddCreator( "foo", new CLAM::LadspaWrapperCreator("lib.so", 1, "factoryID") );
		try
		{
			factory.CreateSafe("non-existing");
			CPPUNIT_FAIL("expected an exception");
		} catch ( CLAM::ErrFactory& ) {}
	}
	void testCreate_nonExistingFileName_throws()
	{
		CLAM::LadspaFactory factory;
		factory.AddCreator( "foo", new CLAM::LadspaWrapperCreator("lib.so", 1, "factoryID") );
		try
		{
			factory.CreateSafe("non-existing");
			CPPUNIT_FAIL("expected an exception");
		} catch ( CLAM::ErrFactory& ) {}
	}
	void testCreate_withTwoCreators()
	{
		CLAM::LadspaFactory factory;
		factory.AddCreator( "noise", new CLAM::LadspaWrapperCreator("/usr/lib/ladspa/noise.so", 1, "noiseLibName") );
		factory.AddCreator( "sine", new CLAM::LadspaWrapperCreator("/usr/lib/ladspa/sine.so", 1, "sineLibName") );
		CLAM::Processing* processing = factory.CreateSafe("sine");
		CLAM::LadspaWrapper* wrapper = dynamic_cast<CLAM::LadspaWrapper*>(processing);
		CPPUNIT_ASSERT_EQUAL( std::string("sineLibName"), std::string(wrapper->GetClassName()) );
		
	}
	void testCreate_emptyCreatorMap_throws()
	{
		CLAM::LadspaFactory factory;
		try {
			factory.CreateSafe("non-existing");
			CPPUNIT_FAIL("expected an exception");
		} catch ( CLAM::ErrFactory& ) {}
	}

	
};

} //namespace

#endif

