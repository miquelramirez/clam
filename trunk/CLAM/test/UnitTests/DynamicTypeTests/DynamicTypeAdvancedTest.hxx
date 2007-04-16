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
#ifndef _DynamicTypeAdvancedTest_hxx_
#define _DynamicTypeAdvancedTest_hxx_

#include <cppunit/extensions/HelperMacros.h>

#include "DynamicType.hxx" // CLAM
#include "DummyDynamicTypes.hxx"

namespace CLAMTest
{

class DynamicTypeAdvancedTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( DynamicTypeAdvancedTest );
	CPPUNIT_TEST( CheckCLAM_USE_XML );
	CPPUNIT_TEST( SimpleXMLSupport );
	CPPUNIT_TEST( XMLCustomizationByRedefinition );
	CPPUNIT_TEST( XMLCustomizationBySubclassing );
	CPPUNIT_TEST( XMLWithArrayAttribute );
	CPPUNIT_TEST( XMLWithIterableAttribute );
	CPPUNIT_TEST( VisitorsToPlainDTs );
	CPPUNIT_TEST( VisitorsToDTWithArrays );
	CPPUNIT_TEST( VisitorsToDTTrees );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp() {};
	void tearDown() {};

private:
	void CheckCLAM_USE_XML();
	void SimpleXMLSupport();
	void XMLCustomizationByRedefinition();
	void XMLCustomizationBySubclassing();
	void XMLWithArrayAttribute();
	void XMLWithIterableAttribute();
	void VisitorsToPlainDTs();
	void VisitorsToDTWithArrays();
	void VisitorsToDTTrees();
};

} // namespace

#endif

