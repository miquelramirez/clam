/*
* Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
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
#ifndef _DynamicTypeBasicTest_hxx_
#define _DynamicTypeBasicTest_hxx_

#include <cppunit/extensions/HelperMacros.h>

#include "DynamicType.hxx"
#include "DummyDynamicTypes.hxx"

namespace CLAMTest
{

class DynamicTypeBasicTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( DynamicTypeBasicTest );
	CPPUNIT_TEST( InstantiateAttributes );
	CPPUNIT_TEST( RepetitiveOperations );
	CPPUNIT_TEST( NonUpdatedAddsAndRemoves );
	CPPUNIT_TEST( ChainedMethods );
	CPPUNIT_TEST( ShallowDataAccess );
	CPPUNIT_TEST( ShallowDeletion );
	CPPUNIT_TEST( DeepDataAccess );
//  We would like to have the following test, but valid
//  accessors are managed with breakpoints, not exceptions.
//	CPPUNIT_TEST_EXCEPTION( InvalidAccessors, Err );
	CPPUNIT_TEST( CopyConstructor );
	CPPUNIT_TEST( DeepCopy );
	CPPUNIT_TEST( Assignation );
	CPPUNIT_TEST_SUITE_END();

public:
	// implementation of TestFixture interface
	void setUp();
	void tearDown();

private:
	void CheckCLAM_EXTRA_CHECKS_ON_DT();
	void InstantiateAttributes();
	void RepetitiveOperations();
	void NonUpdatedAddsAndRemoves();
	void ChainedMethods();
	void ShallowDataAccess();
	void ShallowDeletion();
	void DeepDataAccess();
	void CopyConstructor();
	void DeepCopy();
	void Assignation();

	Dyn * dyn;
	

};

}; //namespace

#endif
