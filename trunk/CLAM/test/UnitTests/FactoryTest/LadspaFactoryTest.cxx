#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"
#include <string>

namespace CLAMTest
{

class LadspaFactoryTest;
CPPUNIT_TEST_SUITE_REGISTRATION( LadspaFactoryTest );

class LadspaFactoryTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( LadspaFactoryTest );
	CPPUNIT_TEST( testCreate_existing );
	CPPUNIT_TEST_SUITE_END();
protected:
	class Processing //TODO delete. it should be CLAM::Processing 
	{
	protected:
		virtual ~Processing() {};
	};
	class LadspaWrapper : public Processing //TODO move to CLAM/src/Processing/Plugins (in namespace CLAM)
	{
	};
	class AbstractProductCreator //TODO move next to Factory
	{
	public:
		virtual Processing* Create() = 0; //TODO integrate with Factory so it returns AbstractProduct
		virtual ~AbstractProductCreator() {};
	};
	class LadspaWrapperCreator : public AbstractProductCreator //TODO move to CLAM/src/Processing/Plugins
	{
	public:
		Processing* Create()
		{
			return new LadspaWrapper;
		}
	};
	class LadspaFactory
	{
		std::string _key;
		AbstractProductCreator* _creator;
	public:
		Processing* Create(const std::string& key)
		{
			return _creator->Create();
		}
		void AddCreator( const std::string& key, AbstractProductCreator* creator )
		{
			_key = key;
			_creator = creator;
		}
	};
	void testCreate_existing()
	{
		LadspaFactory factory;
		factory.AddCreator( "foo", new LadspaWrapperCreator );
		Processing* product = factory.Create("foo");
		CLAMTEST_ASSERT_EQUAL_RTTYPES( LadspaWrapper, *product );
	}
//	void testCreate_nonExistingKey_throws()
//	void testCreate_withTwoCreators()
	
};

} //namespace


