#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Pool.hxx"
#include "DataTypes.hxx"
#include "Component.hxx"


namespace CLAMTest
{

class PoolTest;
class AttributeScopeTest;
class PoolBuilderTest;

CPPUNIT_TEST_SUITE_REGISTRATION( AttributeScopeTest );
CPPUNIT_TEST_SUITE_REGISTRATION( PoolTest );
CPPUNIT_TEST_SUITE_REGISTRATION( PoolBuilderTest );

class AttributeScopeTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( AttributeScopeTest );
	CPPUNIT_TEST( testGetIndex_whenEmpty );
	CPPUNIT_TEST( testGetIndex_withOneInserted );
	CPPUNIT_TEST( testGetIndex_withAWrongName );
	CPPUNIT_TEST( testGetIndex_withSecondInsertedArray );
	CPPUNIT_TEST( testAddAttribute_whenNameAlreadyAdded );
	CPPUNIT_TEST( testAdding_DifferentTypes );
	CPPUNIT_TEST( testCheckType_withOtherType );
	CPPUNIT_TEST( testCheckType_withSameType );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testGetIndex_whenEmpty()
	{
		CLAM::AttributeScope spec;
		try
		{
			spec.GetIndex("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			std::string expected = "Not such descriptor name on this descriptor scope";
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetNAttributes());
	}

	void testGetIndex_withOneInserted()
	{
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetNAttributes());
	}

	void testGetIndex_withAWrongName()
	{
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");
		try
		{
			spec.GetIndex("Foo");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			std::string expected = "Not such descriptor name on this descriptor scope";
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetNAttributes());
	}

	void testGetIndex_withSecondInsertedArray()
	{
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");
		spec.Add<CLAM::TData>("Foo");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetIndex("Foo"));
		CPPUNIT_ASSERT_EQUAL(2u,spec.GetNAttributes());
	}

	void testAddAttribute_whenNameAlreadyAdded()
	{
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");
		try
		{
			spec.Add<CLAM::TData>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			std::string expected = "ScopeSpec::Add, Attribute already present";
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetNAttributes());
	}

	void testAdding_DifferentTypes()
	{
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");
		spec.Add<CLAM::TIndex>("Foo");
		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetIndex("Foo"));
		CPPUNIT_ASSERT_EQUAL(2u,spec.GetNAttributes());
	}

	void testCheckType_withOtherType()
	{
		std::string expected = "Type Missmatch using a pool";
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");
		try
		{
			spec.CheckType(0,(CLAM::TIndex*)0);
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			CPPUNIT_ASSERT_EQUAL(
				expected,
				std::string(err.what()));
		}
	}

	void testCheckType_withSameType()
	{
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");
		spec.CheckType(0,(CLAM::TData*)0);
	}

};





class PoolTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PoolTest );
	CPPUNIT_TEST( testGet_ReturnsSameMemory );
	CPPUNIT_TEST( testGet_ReturnsConstMemory );
	CPPUNIT_TEST( testGet_withStrings );
	CPPUNIT_TEST( testGet_withWrongType );
	CPPUNIT_TEST( testGet_withWrongTypeAndConst );
	CPPUNIT_TEST( testConstruction_withoutSize );
	CPPUNIT_TEST( testConstruction_withoutSizeGettingNoConst );
	CPPUNIT_TEST( testConstruction_givesSizeZeroByDefault );
	CPPUNIT_TEST( testSetSize_overAZeroSizePool );
	CPPUNIT_TEST( testSetSize_overANonZeroSizePool );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testGet_ReturnsSameMemory()
	{
		const unsigned poolSize=5;
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");
		
		CLAM::Pool pool(spec,poolSize);
		CLAM::TData * data = pool.Get<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.Get<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}

	void testGet_ReturnsConstMemory()
	{
		const unsigned poolSize=5;
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::Pool pool(spec,poolSize);
		CLAM::TData * data = pool.Get<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		const CLAM::Pool & pool2 = pool;
		const CLAM::TData * data2 = pool2.Get<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(const_cast<const CLAM::TData*>(data),data2);
	}

	void testGet_withStrings()
	{
		const unsigned poolSize=5;
		CLAM::AttributeScope spec;
		spec.Add<std::string>("MyAttribute");

		CLAM::Pool pool(spec,poolSize);
		std::string * data = pool.Get<std::string>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
		{
			std::ostringstream os;
			os << "Hola " << i*i;
			data[i] += os.str();
		}
		const CLAM::Pool & pool2 = pool;
		const std::string * data2 = pool2.Get<std::string>("MyAttribute");
		const std::string expected = "Hola 16";
		CPPUNIT_ASSERT_EQUAL(expected,data2[4]);
	}

	void testGet_withWrongType()
	{
		const unsigned poolSize=5;
		CLAM::AttributeScope spec;
		spec.Add<std::string>("MyAttribute");

		CLAM::Pool pool(spec,poolSize);
		try
		{
			int * data = pool.Get<int>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Type Missmatch using a pool";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGet_withWrongTypeAndConst()
	{
		const unsigned poolSize=5;
		CLAM::AttributeScope spec;
		spec.Add<std::string>("MyAttribute");

		CLAM::Pool pool(spec,poolSize);
		const CLAM::Pool & pool2 = pool;
		try
		{
			const int * data = pool2.Get<int>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Type Missmatch using a pool";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testConstruction_withoutSize()
	{
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::Pool pool(spec);
		const CLAM::Pool & constPool = pool;

		try
		{
			const CLAM::TData * data = constPool.Get<CLAM::TData>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Getting an attribute from a zero size pool";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testConstruction_withoutSizeGettingNoConst()
	{
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::Pool pool(spec);

		try
		{
			CLAM::TData * data = pool.Get<CLAM::TData>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Getting an attribute from a zero size pool";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testConstruction_givesSizeZeroByDefault()
	{
		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::Pool pool(spec);
		CPPUNIT_ASSERT_EQUAL(0u,pool.GetSize());

	}

	void testSetSize_overAZeroSizePool()
	{
		const unsigned poolSize=5;

		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::Pool pool(spec);

		pool.SetSize(poolSize);

		CLAM::TData * data = pool.Get<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.Get<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}

	void testSetSize_overANonZeroSizePool()
	{
		const unsigned poolSize=5;

		CLAM::AttributeScope spec;
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::Pool pool(spec,7);

		pool.SetSize(poolSize);

		CLAM::TData * data = pool.Get<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.Get<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}


};

class PoolBuilderTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( PoolBuilderTest );
	CPPUNIT_TEST( testPoolBuilder_withNoScopeRegistered );
	CPPUNIT_TEST( testPoolBuilder_withARegisteredAttribute );
	CPPUNIT_TEST( testPoolBuilder_withTwoScopes );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testPoolBuilder_withNoScopeRegistered()
	{
		CLAM::ScopeRegistry registry;
		try
		{
			registry.GetSpec("NonExistent");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "No scope registered with that name";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testPoolBuilder_withARegisteredAttribute()
	{
		CLAM::ScopeRegistry registry;
		registry.AddAttribute< CLAM::Attribute<CLAM::TData> >("MyScope","MyAttribute");
		registry.AddAttribute< CLAM::Attribute<CLAM::TData> >("MyScope","MyOtherAttribute");

		const CLAM::AttributeScope & spec = registry.GetSpec("MyScope");

		CPPUNIT_ASSERT_EQUAL(0u,spec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,spec.GetIndex("MyOtherAttribute"));
	}

	void testPoolBuilder_withTwoScopes()
	{
		CLAM::ScopeRegistry registry;
		registry.AddAttribute< CLAM::Attribute<CLAM::TData> >("MyScope","MyAttribute");
		registry.AddAttribute< CLAM::Attribute<CLAM::TData> >("YourScope","YourAttribute");
		registry.AddAttribute< CLAM::Attribute<int> >("YourScope","YourIntAttribute");

		const CLAM::AttributeScope & mySpec = registry.GetSpec("MyScope");
		const CLAM::AttributeScope & yourSpec = registry.GetSpec("YourScope");

		CPPUNIT_ASSERT_EQUAL(0u,mySpec.GetIndex("MyAttribute"));
		CPPUNIT_ASSERT_EQUAL(0u,yourSpec.GetIndex("YourAttribute"));
		CPPUNIT_ASSERT_EQUAL(1u,yourSpec.GetIndex("YourIntAttribute"));
	}

};






} // namespace CLAMTest
