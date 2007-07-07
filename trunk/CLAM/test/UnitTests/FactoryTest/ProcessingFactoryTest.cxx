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
	CPPUNIT_TEST( testGetListOfKeys_twoElements );
	CPPUNIT_TEST( testGetValuesFor_empty );
	CPPUNIT_TEST( testGetValuesFor_twoElements );
	CPPUNIT_TEST( testGetValuesFrom_empty );
	CPPUNIT_TEST( testGetValuesFrom_twoElements );
	CPPUNIT_TEST( testGetValuesOf_empty );
	CPPUNIT_TEST( testGetValuesOf_twoElements );
	CPPUNIT_TEST_SUITE_END();

protected:
	class DummyCreator : public CLAM::Factory<CLAM::Processing>::Creator
	{
		CLAM::Processing* Create()
		{
			return 0;
		}
	};

	CLAM::ProcessingFactory factory;
public:
	void setUp()
	{
	}
	void tearDown()
	{
		factory.Clear();
	}

	// Tests definition :
protected:

	void testGetListOfKeys_empty()
	{
		CLAM::ProcessingFactory::Keys result = factory.GetListOfKeys("category","");
		CPPUNIT_ASSERT_EQUAL(size_t(0), result.size() );
	}

	void testGetListOfKeys_twoElements()
	{
		//factory.AddCreator("the key", new DummyCreator());
		factory.AddAttribute("first", "category", "spectral");
		factory.AddAttribute("second", "category", "time-domain");
		CLAM::ProcessingFactory::Keys result = factory.GetListOfKeys("category", "spectral");
		CPPUNIT_ASSERT_EQUAL(size_t(1), result.size() );
		CPPUNIT_ASSERT_EQUAL(std::string("first"), result.front() );
	}
	void testGetValuesFor_empty()
	{
		CLAM::ProcessingFactory::Values result = factory.GetValuesFor("non-existing");
		CPPUNIT_ASSERT_EQUAL(size_t(0), result.size());
	}
	void testGetValuesFor_twoElements()
	{
		factory.AddAttribute("first", "category", "spectral");
		factory.AddAttribute("second", "category", "time-domain");
		CLAM::ProcessingFactory::Values result = factory.GetValuesFor("category");
		CPPUNIT_ASSERT_EQUAL(size_t(2), result.size());
		CPPUNIT_ASSERT_EQUAL(std::string("spectral"), result.front());
		CPPUNIT_ASSERT_EQUAL(std::string("time-domain"), result.back());
	}
	void testGetValuesFrom_empty()
	{
		CLAM::ProcessingFactory::Values result = factory.GetValuesFrom("key","category");
		CPPUNIT_ASSERT_EQUAL(size_t(0), result.size());
	}
	void testGetValuesFrom_twoElements()
	{
		factory.AddAttribute("first", "category", "spectral");
		factory.AddAttribute("first", "category", "time-domain");
		factory.AddAttribute("first", "description", "short description");
		CLAM::ProcessingFactory::Values result = factory.GetValuesFrom("first","category");
		CPPUNIT_ASSERT_EQUAL(size_t(2), result.size());
		CPPUNIT_ASSERT_EQUAL(std::string("spectral"), result.front());
		CPPUNIT_ASSERT_EQUAL(std::string("time-domain"), result.back());
	}
	void testGetValuesOf_empty()
	{
		CLAM::ProcessingFactory::Attributes result = factory.GetValuesOf("key");
		CPPUNIT_ASSERT_EQUAL(size_t(0), result.size());
	}
	void testGetValuesOf_twoElements()
	{
		factory.AddAttribute("first", "category", "spectral");
		factory.AddAttribute("first", "category", "time-domain");
		CLAM::ProcessingFactory::Attributes result = factory.GetValuesOf("first");
		CPPUNIT_ASSERT_EQUAL(size_t(2), result.size());
		CPPUNIT_ASSERT_EQUAL(std::string("spectral"), result.front().value);
		CPPUNIT_ASSERT_EQUAL(std::string("time-domain"), result.back().value);
	}
};



} // namespace CLAMTest

