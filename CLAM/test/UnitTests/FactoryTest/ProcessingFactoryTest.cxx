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
//	CPPUNIT_TEST( testAddAttribute_nonExistingKey );
//	CPPUNIT_TEST( testGetValueFor_nonExistingKey );
//	CPPUNIT_TEST( testGetValueFor_nonExistingAttribute );
//	CPPUNIT_TEST( testGetValueFor_existingAttribute );
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
		factory.AddCreator("first", new DummyCreator());
		factory.AddCreator("second", new DummyCreator());
		factory.AddAttribute("first", "category", "spectral");
		factory.AddAttribute("second", "category", "time-domain");
		CLAM::ProcessingFactory::Keys result = factory.GetListOfKeys("category", "spectral");
		CPPUNIT_ASSERT_EQUAL(size_t(1), result.size() );
		CPPUNIT_ASSERT_EQUAL(std::string("first"), result.front() );
	}
	void testGetValuesFor_empty()
	{
		factory.AddCreator("non-existing", new DummyCreator());
		CLAM::ProcessingFactory::Values result = factory.GetValuesFor("non-existing");
		CPPUNIT_ASSERT_EQUAL(size_t(0), result.size());
	}
	void testGetValuesFor_twoElements()
	{
		factory.AddCreator("first", new DummyCreator());
		factory.AddCreator("second", new DummyCreator());
		factory.AddAttribute("first", "category", "spectral");
		factory.AddAttribute("second", "category", "time-domain");
		CLAM::ProcessingFactory::Values result = factory.GetValuesFor("category");
		CPPUNIT_ASSERT_EQUAL(size_t(2), result.size());
		CPPUNIT_ASSERT_EQUAL(std::string("spectral"), result.front());
		CPPUNIT_ASSERT_EQUAL(std::string("time-domain"), result.back());
	}
	void testGetValuesFrom_empty()
	{
		factory.AddCreator("key", new DummyCreator());
		CLAM::ProcessingFactory::Values result = factory.GetValuesFrom("key","category");
		CPPUNIT_ASSERT_EQUAL(size_t(0), result.size());
	}
	void testGetValuesFrom_twoElements()
	{
		factory.AddCreator("first", new DummyCreator());
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
		factory.AddCreator("key", new DummyCreator());
		CLAM::ProcessingFactory::Attributes result = factory.GetValuesOf("key");
		CPPUNIT_ASSERT_EQUAL(size_t(0), result.size());
	}
	void testGetValuesOf_twoElements()
	{
		factory.AddCreator("first", new DummyCreator());
		factory.AddAttribute("first", "category", "spectral");
		factory.AddAttribute("first", "category", "time-domain");
		CLAM::ProcessingFactory::Attributes result = factory.GetValuesOf("first");
		CPPUNIT_ASSERT_EQUAL(size_t(2), result.size());
		CPPUNIT_ASSERT_EQUAL(std::string("spectral"), result.front().value);
		CPPUNIT_ASSERT_EQUAL(std::string("time-domain"), result.back().value);
	}

/*	void testAddAttribute_nonExistingKey()
	{
		CLAM::ProcessingFactory factory;
		try
		{
			factory.AddAttribute("non-existing-key", "attr");
		} 
		catch (CLAM::AssertFailedErr& e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("ProcessingFactory::AddAttribute: Adding an attribute with an non existing key "), 
				std::string(e.what())
				);
		}
	}
	void testGetValueFor_nonExistingKey()
	{
		CLAM::ProcessingFactory factory;
		try
		{
			factory.GetValueFor("non-existing-key", "attr");
		} 
		catch (CLAM::AssertFailedErr& e)
		{
			CPPUNIT_ASSERT_EQUAL(
				std::string("ProcessingFactory::GetValueFor: Retrieving a value of an non existing key "), 
				std::string(e.what())
				);
		}
	}

	void testGetValueFor_nonExistingAttribute()
	{
		CLAM::ProcessingFactory factory;
		factory.AddCreator("first", new DummyCreator());
		factory.AddAttribute("first", "category", "spectral");
		CPPUNIT_ASSERT_EQUAL( std::string("--undefined--"), factory.GetValueFor("first", "non existing attr") );
	}

	void testGetValueFor_existingAttribute()
	{
		CLAM::ProcessingFactory factory;
		factory.AddCreator("first", new DummyCreator());
		factory.AddAttribute("first", "category", "spectral");
		CPPUNIT_ASSERT_EQUAL( std::string("spectral"), factory.GetValueFor("category") );

	}
*/
};



} // namespace CLAMTest

