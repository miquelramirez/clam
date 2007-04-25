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

#include "InstantTunningEstimator.hxx"

namespace Simac
{

namespace Test
{

class InstantTunningEstimatorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( InstantTunningEstimatorTest );

class InstantTunningEstimatorTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( InstantTunningEstimatorTest );
	CPPUNIT_TEST( testSilence );
	CPPUNIT_TEST( testSingleTunedPeakAtFirstPitch );
	CPPUNIT_TEST( testSingleTunedPeakAtSecondPitch );
	CPPUNIT_TEST( testSingleDetunedPeakAtSecondPitch );
	CPPUNIT_TEST( testTwoTunedPeaksAtFirstSecondPitch );
	CPPUNIT_TEST( testTwoOrthogonalTunnings );
	CPPUNIT_TEST( testTwoOpposedTunnings );
	CPPUNIT_TEST( testTwoDetunnedPeaks );
	CPPUNIT_TEST( testSeveralFrames_withoutInertia );
	CPPUNIT_TEST( testSeveralFrames_withInertia );
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
	void assertFoundCenterIs(double expectedCenter, double expectedStrength,
			unsigned nPeaks, double * peakPositions, double * peakValues)
	{
		InstantTunningEstimator tunningEstimator;
		tunningEstimator.doIt(nPeaks, peakPositions, peakValues);

		std::pair<double,double> centerAndStrenght = tunningEstimator.output();

		CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedCenter, centerAndStrenght.first,1e-14);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedStrength, centerAndStrenght.second,1e-14);
	}

	void testSilence()
	{
		unsigned nPeaks=0;
		double positions[]={666666}; // ignored
		double values   []={666666}; // ignored;
		
		assertFoundCenterIs(0.0, 0.0, nPeaks, positions, values);
	}
	void testSingleTunedPeakAtFirstPitch()
	{
		const unsigned nPeaks=1;
		double positions[]={0.0};
		double values   []={2.0};
		
		assertFoundCenterIs(0.0, 2.0, nPeaks, positions, values);
	}
	void testSingleTunedPeakAtSecondPitch()
	{
		const unsigned nPeaks=1;
		double positions[]={3.0};
		double values   []={2.0};
		
		assertFoundCenterIs(0.0, 2.0, nPeaks, positions, values);
	}
	void testSingleDetunedPeakAtSecondPitch()
	{
		const unsigned nPeaks=1;
		double positions[]={3.1};
		double values   []={2.0};
		
		assertFoundCenterIs(0.1, 2.0, nPeaks, positions, values);
	}
	void testTwoTunedPeaksAtFirstSecondPitch()
	{
		const unsigned nPeaks=2;
		double positions[]={0.0, 3.0};
		double values   []={2.0, 2.0};
		
		assertFoundCenterIs(0.0, 4.0, nPeaks, positions, values);
	}
	void testTwoOrthogonalTunnings()
	{
		const unsigned nPeaks=2;
		double positions[]={0.0, 3.0/4.0};
		double values   []={4.0, 4.0};
		
		assertFoundCenterIs(3.0/8.0, 4.0*std::sqrt(2.0), nPeaks, positions, values);
	}
	void testTwoDetunnedPeaks()
	{
		const unsigned nPeaks=2;
		double positions[]={1.5, 4.5};
		double values   []={4.0, 4.0};
		
		assertFoundCenterIs(1.5, 8.0, nPeaks, positions, values);
	}
	void testTwoOpposedTunnings()
	{
		const unsigned nPeaks=2;
		double positions[]={0.0, 1.5};
		double values   []={4.0, 4.0};
		// because strength is 0 .75 is pointless, it is just the value it gives on my machine.
		assertFoundCenterIs(.75, 0.0, nPeaks, positions, values);
	}
	void testSeveralFrames_withoutInertia()
	{
		const unsigned nPeaks1=2;
		double positions1[]={0.0, 3.0};
		double values1   []={8.0, 8.0};
		
		const unsigned nPeaks2=2;
		double positions2[]={1.5, 4.5};
		double values2   []={8.0, 8.0};

		InstantTunningEstimator tunningEstimator;
		tunningEstimator.doIt(nPeaks1, positions1, values1);
		tunningEstimator.doIt(nPeaks2, positions2, values2);

		std::pair<double,double> centerAndStrength = tunningEstimator.output();

		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, centerAndStrength.first, 1e-14);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(16.0, centerAndStrength.second, 1e-14);
	}
	void testSeveralFrames_withInertia()
	{
		const unsigned nPeaks1=2;
		double positions1[]={0.0, 3.0};
		double values1   []={8.0, 8.0};
		
		const unsigned nPeaks2=2;
		double positions2[]={1.5, 4.5};
		double values2   []={8.0, 8.0};

		InstantTunningEstimator tunningEstimator;
		tunningEstimator.setInertia(.4);
		tunningEstimator.doIt(nPeaks1, positions1, values1);
		tunningEstimator.doIt(nPeaks2, positions2, values2);

		std::pair<double,double> centerAndStrength = tunningEstimator.output();

		CPPUNIT_ASSERT_DOUBLES_EQUAL(1.5, centerAndStrength.first, 1e-14);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(9.44, centerAndStrength.second, 1e-14);
	}
};




} // namespace Test
} // namespace Simac

