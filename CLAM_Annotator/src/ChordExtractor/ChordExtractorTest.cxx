/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "ChordExtractor.hxx"
#include <iosfwd>

namespace Simac
{

namespace Test
{

class ChordExtractorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ChordExtractorTest );

class ChordExtractorTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( ChordExtractorTest );
	CPPUNIT_TEST( testSilence );
	CPPUNIT_TEST_SUITE_END();

public:
	/// Common initialization, executed before each test method
	void setUp() 
	{
	}

	/// Common clean up, executed after each test method
	void tearDown()
	{
	}

private:
	void testSilence()
	{
		ChordExtractor estimator;
		std::vector<float> inputData(30000);
		estimator.doIt(&inputData[0]);
		CPPUNIT_ASSERT_EQUAL(std::string("None"), estimator.chordEstimation());
	}
};




} // namespace Test
} // namespace Simac

