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

#include "SemitoneCenterFinder.hxx"

namespace Simac
{

namespace Test
{

class SemitoneCenterFinderTest;

CPPUNIT_TEST_SUITE_REGISTRATION( SemitoneCenterFinderTest );

class SemitoneCenterFinderTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( SemitoneCenterFinderTest );
	CPPUNIT_TEST( testSilence );
	CPPUNIT_TEST( testSingleTunedPeakAtFirstPitch );
	CPPUNIT_TEST( testSingleOneBinDetunedAtFirstPitch );
	CPPUNIT_TEST( testSingleOneBinDetunedAtSecondPitch );
	CPPUNIT_TEST( testDifferentTunnings_choosesTheMostFrequent );
	CPPUNIT_TEST( testSeveralFrames );
	CPPUNIT_TEST( testBorderTunning_roundsToTheFirstDecimalUp );
	CPPUNIT_TEST( testBorderTunning_roundsToTheFirstDecimalDown );
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

	void assertFoundCenterIs(double expectedCenter,
			unsigned nPeaks, double * peakPositions, double * peakValues)
	{
		SemitoneCenterFinder centerFinder;
		centerFinder.doIt(nPeaks, peakPositions, peakValues);

		double center = centerFinder.output();

		CPPUNIT_ASSERT_EQUAL(expectedCenter, center);
	}

	void testSilence()
	{
		unsigned nPeaks=0;
		double positions[]={666666}; // ignored
		double values   []={666666}; // ignored;
		
		assertFoundCenterIs(0.0, nPeaks, positions, values);
	}
	void testSingleTunedPeakAtFirstPitch()
	{
		const unsigned nPeaks=1;
		double positions[]={0.0};
		double values   []={8.0};
		
		assertFoundCenterIs(0.0, nPeaks, positions, values);
	}
	void testSingleOneBinDetunedAtFirstPitch()
	{
		const unsigned nPeaks=1;
		double positions[]={1.0};
		double values   []={8.0};
		
		assertFoundCenterIs(1.0, nPeaks, positions, values);
	}
	void testSingleOneBinDetunedAtSecondPitch()
	{
		const unsigned nPeaks=1;
		double positions[]={4.0};
		double values   []={8.0};
		
		assertFoundCenterIs(1.0, nPeaks, positions, values);
	}
	void testDifferentTunnings_choosesTheMostFrequent()
	{
		const unsigned nPeaks=2;
		double positions[]={0.4, 3.0, 6.0};
		double values   []={8.0, 8.0, 8.0};
		
		assertFoundCenterIs(0.0, nPeaks, positions, values);
	}
	void testSeveralFrames()
	{
		const unsigned nPeaks1=3;
		double positions1[]={0.4, 3.0, 6.0};
		double values1   []={8.0, 8.0, 8.0};
		
		const unsigned nPeaks2=3;
		double positions2[]={0.4, 3.5, 6.5};
		double values2   []={8.0, 8.0, 8.0};

		const unsigned nPeaks3=3;
		double positions3[]={0.4, 3.6, 6.6};
		double values3   []={8.0, 8.0, 8.0};
		
		SemitoneCenterFinder centerFinder;
		centerFinder.doIt(nPeaks1, positions1, values1);
		centerFinder.doIt(nPeaks2, positions2, values2);
		centerFinder.doIt(nPeaks3, positions3, values3);

		double center = centerFinder.output();

		CPPUNIT_ASSERT_EQUAL(0.4, center);
	}
	void testBorderTunning_roundsToTheFirstDecimalUp()
	{
		const unsigned nPeaks=1;
		double positions[]={1.951};
		double values   []={8.0};
		
		assertFoundCenterIs(2.0, nPeaks, positions, values);
	}
	void testBorderTunning_roundsToTheFirstDecimalDown()
	{
		const unsigned nPeaks=1;
		double positions[]={1.95};
		double values   []={8.0};
		
		assertFoundCenterIs(1.9, nPeaks, positions, values);
	}
};




} // namespace Test
} // namespace Simac

