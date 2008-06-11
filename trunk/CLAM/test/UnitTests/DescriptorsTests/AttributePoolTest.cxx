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

#include "AttributePool.hxx" // CLAM
#include "DataTypes.hxx" // CLAM
#include "Component.hxx" // CLAM


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

