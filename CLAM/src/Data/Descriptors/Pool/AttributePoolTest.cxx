#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "AttributePool.hxx"
#include "DataTypes.hxx"
#include "Component.hxx"


namespace CLAMTest
{

class AttributePoolTest;

CPPUNIT_TEST_SUITE_REGISTRATION( AttributePoolTest );

class AttributePoolTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( AttributePoolTest );
	CPPUNIT_TEST(testGetData_returnsNullByDefault);
	CPPUNIT_TEST(testAllocatePool);
	CPPUNIT_TEST(testDeallocatePool);
	CPPUNIT_TEST(testDeallocatePool_whenNoPreviousAllocation);
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testGetData_returnsNullByDefault()
	{
		CLAM::AttributePool pool;
		void * result = pool.GetData();
		CPPUNIT_ASSERT_EQUAL((void*)0x0, result);
	}

	void testAllocatePool()
	{
		CLAM::Attribute<int> definition("MyAttribute");
		CLAM::AttributePool pool;
		pool.SetDefinition(definition);
		pool.Allocate(4);
		const void * data = pool.GetData();
		CPPUNIT_ASSERT(data!=0);

		delete[] (int*) data;
	}
	void testDeallocatePool()
	{
		CLAM::Attribute<int> definition("MyAttribute");
		CLAM::AttributePool pool;
		pool.SetDefinition(definition);
		pool.Allocate(4);
		pool.Deallocate();
		const void * data = pool.GetData();
		CPPUNIT_ASSERT_EQUAL((const void*)0, data);
	}

	void testDeallocatePool_whenNoPreviousAllocation()
	{
		CLAM::Attribute<int> definition("MyAttribute");
		CLAM::AttributePool pool;
		pool.SetDefinition(definition);
		pool.Deallocate();
		const void * data = pool.GetData();
		CPPUNIT_ASSERT_EQUAL((const void*)0, data);
	}


};






} // namespace CLAMTest
