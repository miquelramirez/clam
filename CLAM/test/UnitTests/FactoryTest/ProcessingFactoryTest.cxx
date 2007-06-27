#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include "ProcessingFactory.hxx"


namespace CLAMTest
{


class ProcessingFactoryTest;
CPPUNIT_TEST_SUITE_REGISTRATION( ProcessingFactoryTest );

class ProcessingFactoryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ProcessingFactoryTest );
	CPPUNIT_TEST( testGetListOfKeys_empty );
	CPPUNIT_TEST_SUITE_END();

protected:
	class DummyCreator : public CLAM::Factory<CLAM::Processing>::Creator
	{
		CLAM::Processing* Create()
		{
			return 0;
		}
	};
public:
	void setUp()
	{
	}
	void tearDown()
	{
	}

	// Tests definition :
protected:

	void testGetListOfKeys_empty()
	{
		CLAM::ProcessingFactory factory;
		CLAM::ProcessingFactory::Key result = factory.GetListOfKeys("category");
		CPPUNIT_ASSERT_EQUAL(0, result.size() );
	}

	void testGetListOfKeys_twoElements()
	{
		CLAM::ProcessingFactory factory;
		factory.AddCreator("the key", new DummyCreator());
	//	factory.AddAttribute("first", "category", "spectral");
	//	factory.AddAttribute("second", "category", "time-domain");
		CLAM::ProcessingFactory::Key result = factory.GetListOfKeys("category", "spectral");
		CPPUNIT_ASSERT_EQUAL(1, result.size() );
		CPPUNIT_ASSERT_EQUAL(std::string("first"), result.head() );
	}
	void testGetValuesOf_empty()
	{
	}
	/*
	 *
	 * vendors = GetValuesFor("category")
	for a in vendors
		keys = GetListOfKeys("category", a)
		*/
};



} // namespace CLAMTest

