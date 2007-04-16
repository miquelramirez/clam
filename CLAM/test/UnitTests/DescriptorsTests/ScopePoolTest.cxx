/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <cppunit/extensions/HelperMacros.h>
#include "cppUnitHelper.hxx"

#include "Pool.hxx" // CLAM
#include "DataTypes.hxx" // CLAM


namespace CLAMTest
{

class ScopePoolTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ScopePoolTest );

class ScopePoolTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( ScopePoolTest );
	CPPUNIT_TEST( testGetWritePool_ReturnsSameMemory );
	CPPUNIT_TEST( testGetReadPool_ReturnsConstMemory );
	CPPUNIT_TEST( testGetWritePool_withStrings );
	CPPUNIT_TEST( testGetWritePool_withWrongType );
	CPPUNIT_TEST( testGetReadPool_withWrongType );
	CPPUNIT_TEST( testGetReadPool_withoutGetWritePoolFirst );
	CPPUNIT_TEST( testConstruction_givesSizeZeroByDefault );
	CPPUNIT_TEST( testSetSize_overAZeroSizePool );
	CPPUNIT_TEST( testSetSize_overANonZeroSizePool );
	CPPUNIT_TEST( testInsert );
	CPPUNIT_TEST( testRemove );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() { }

	/// Common clean up, executed after each test method
	void tearDown() { }

private:
	void testGetWritePool_ReturnsSameMemory()
	{
		const unsigned poolSize=5;
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");
		
		CLAM::ScopePool pool(spec,poolSize);
		CLAM::TData * data = pool.GetWritePool<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.GetWritePool<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}

	void testGetReadPool_ReturnsConstMemory()
	{
		const unsigned poolSize=5;
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec,poolSize);
		CLAM::TData * data = pool.GetWritePool<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		const CLAM::ScopePool & pool2 = pool;
		const CLAM::TData * data2 = pool2.GetReadPool<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(const_cast<const CLAM::TData*>(data),data2);
	}

	void testGetWritePool_withStrings()
	{
		const unsigned poolSize=5;
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<std::string>("MyAttribute");

		CLAM::ScopePool pool(spec,poolSize);
		std::string * data = pool.GetWritePool<std::string>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
		{
			std::ostringstream os;
			os << "Hola " << i*i;
			data[i] += os.str();
		}
		const CLAM::ScopePool & pool2 = pool;
		const std::string * data2 = pool2.GetReadPool<std::string>("MyAttribute");
		const std::string expected = "Hola 16";
		CPPUNIT_ASSERT_EQUAL(expected,data2[4]);
	}

	void testGetWritePool_withWrongType()
	{
		const unsigned poolSize=5;
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<std::string>("MyAttribute");

		CLAM::ScopePool pool(spec,poolSize);
		try
		{
			pool.GetWritePool<int>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = std::string("Attribute 'TestScope:MyAttribute' has been used as type '")
					+ typeid(int).name() + "' but it really was of type '" + typeid(std::string).name() + "'";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGetReadPool_withWrongType()
	{
		const unsigned poolSize=5;
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<std::string>("MyAttribute");

		CLAM::ScopePool pool(spec,poolSize);
		const CLAM::ScopePool & pool2 = pool;
		try
		{
			pool2.GetReadPool<int>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = std::string("Attribute 'TestScope:MyAttribute' has been used as type '")
					+ typeid(int).name() + "' but it really was of type '" + typeid(std::string).name() + "'";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testGetReadPool_withoutGetWritePoolFirst()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec,20);
		const CLAM::ScopePool & constPool = pool;

		try
		{
			constPool.GetReadPool<CLAM::TData>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Getting data from a non instanciated attribute 'TestScope':'MyAttribute'";
			CPPUNIT_ASSERT_EQUAL(expected, std::string(err.what()));
		}
	}

	void testConstruction_givesSizeZeroByDefault()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec);
		CPPUNIT_ASSERT_EQUAL(0u,pool.GetSize());

	}

	void testSetSize_overAZeroSizePool()
	{
		const unsigned poolSize=5;

		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec);

		pool.SetSize(poolSize);

		CLAM::TData * data = pool.GetWritePool<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.GetWritePool<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}

	void testSetSize_overANonZeroSizePool()
	{
		const unsigned poolSize=5;

		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec,7);

		pool.SetSize(poolSize);

		CLAM::TData * data = pool.GetWritePool<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < poolSize; i++)
			data[i] = i*i;
		CLAM::TData * data2 = pool.GetWritePool<CLAM::TData>("MyAttribute");
		CPPUNIT_ASSERT_EQUAL(data,data2);
	}

	void testInsert()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec,3);

		CLAM::TData * data = pool.GetWritePool<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < pool.GetSize(); i++)
			data[i] = 1000 + i*i;

		pool.Insert(2);

		unsigned expectedSize = 4;
		CLAM::TData expected[]={1000,1001,0,1004};
		CPPUNIT_ASSERT_EQUAL(expectedSize,pool.GetSize());
		const CLAM::TData * result = pool.GetReadPool<CLAM::TData>("MyAttribute");
		for (unsigned i=0; i<pool.GetSize(); i++)
			CPPUNIT_ASSERT_EQUAL(expected[i],result[i]);
	}
	void testRemove()
	{
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec,3);

		CLAM::TData * data = pool.GetWritePool<CLAM::TData>("MyAttribute");
		for (unsigned i = 0; i < pool.GetSize(); i++)
			data[i] = 1000 + i*i;

		pool.Remove(1);

		unsigned expectedSize = 2;
		CLAM::TData expected[]={1000,1004};
		CPPUNIT_ASSERT_EQUAL(expectedSize,pool.GetSize());
		const CLAM::TData * result = pool.GetReadPool<CLAM::TData>("MyAttribute");
		for (unsigned i=0; i<pool.GetSize(); i++)
			CPPUNIT_ASSERT_EQUAL(expected[i],result[i]);
	}

	// TODO: Insert outside bounds
	// TODO: Remove outside bounds

};






} // namespace CLAMTest

