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
#include "cppUnitHelper.hxx" // necessary for the custom assert

#include "Pool.hxx"
#include "DataTypes.hxx"


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
			int * data = pool.GetWritePool<int>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Type Missmatch using a pool";
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
			const int * data = pool2.GetReadPool<int>("MyAttribute");
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
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec);
		const CLAM::ScopePool & constPool = pool;

		try
		{
			const CLAM::TData * data = constPool.GetReadPool<CLAM::TData>("MyAttribute");
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
		CLAM::DescriptionScope spec("TestScope");
		spec.Add<CLAM::TData>("MyAttribute");

		CLAM::ScopePool pool(spec);

		try
		{
			CLAM::TData * data = pool.GetWritePool<CLAM::TData>("MyAttribute");
			CPPUNIT_FAIL("Should have thrown an exception");
		}
		catch (CLAM::ErrAssertionFailed & err)
		{
			const std::string expected = "Getting an attribute from a zero size pool";
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
			const CLAM::TData * data = constPool.GetReadPool<CLAM::TData>("MyAttribute");
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


};






} // namespace CLAMTest
