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

#include "CircularPeakPicking.hxx"

namespace Simac
{

namespace Test
{

class CircularPeakPickingTest;

CPPUNIT_TEST_SUITE_REGISTRATION( CircularPeakPickingTest );

class CircularPeakPickingTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( CircularPeakPickingTest );
	CPPUNIT_TEST( testSilence );
	CPPUNIT_TEST( testSingleTunedPeak );
	CPPUNIT_TEST( testSingleTunedPeakAtTheFront );
	CPPUNIT_TEST( testSingleTunedPeakAtTheBack );
	CPPUNIT_TEST( testSingleTunedPeakAtThePreviousToBack );
	CPPUNIT_TEST( testInterpolatedMaximum_withSimetricPoints );
	CPPUNIT_TEST( testInterpolatedMaximum_withLowUnbalancedPoints );
	CPPUNIT_TEST( testInterpolatedMaximum_withHighUnbalancedPoints );
	CPPUNIT_TEST( testSingleLowDetunedPeak );
	CPPUNIT_TEST( testSingleHighDetunedPeak );
	CPPUNIT_TEST( testTwoDetunedPeaks );
	CPPUNIT_TEST( testSingleLowDetunedPeaksAtTheFront );
	CPPUNIT_TEST( testSingleLowDetunedPeaksAtTheBack );
	CPPUNIT_TEST( testSingleLowDetunedPeaksAtThePreviousToTheBack );
	CPPUNIT_TEST( testSingleHighDetunedPeaksAtTheFront );
	CPPUNIT_TEST( testSingleHighDetunedPeaksAtTheBack );
	CPPUNIT_TEST( testSingleHighDetunedPeaksAtThePreviousToTheBack );
	CPPUNIT_TEST( testInterpolatedMaximum_withInterBinPeak );
	CPPUNIT_TEST( testSingleInterBinPeak );
	CPPUNIT_TEST( testSinglePeak_withNonOneBinSize );
	CPPUNIT_TEST( testSinglePeak_withNonZeroOffset );
	CPPUNIT_TEST( testSinglePeak_withOffsetAndBinSize );
	CPPUNIT_TEST( testSuccesiveCallsClearsPreviousResults );
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

	void assertPeaksMatches(unsigned dataSize, double * data,
			unsigned nPeaks, double * peakPositions, double * peakValues)
	{
		CircularPeakPicking peakPicker(dataSize);
		std::vector<double> chromagram(data, data+dataSize);
		peakPicker.doIt(chromagram);

		const CircularPeakPicking::PeakList & peakList = peakPicker.output();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("Different number of peaks detected",
			nPeaks, (unsigned int)peakList.size());
		for (unsigned i=0; i<nPeaks; i++)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(peakPositions[i], peakList[i].first, 1e-10);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(peakValues[i], peakList[i].second, 1e-10);
		}
	}


	void testSilence()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,0,0,0};
		double positions[]={666666}; // ignored
		double values   []={666666}; // ignored;
		
		assertPeaksMatches(chromagramSize, chromagram, 0, positions, values);
	}
	void testSingleTunedPeak()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,8,0,0,0};
		double positions[]={2.0};
		double values   []={8.0};
		
		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testSingleTunedPeakAtTheFront()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={8,0,0,0,0,0};
		double positions[]={0.0};
		double values   []={8.0};
		
		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testSingleTunedPeakAtTheBack()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,0,0,8};
		double positions[]={5.0};
		double values   []={8.0};
		
		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testSingleTunedPeakAtThePreviousToBack()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,0,8,0};
		double positions[]={4.0};
		double values   []={8.0};
		
		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testInterpolatedMaximum_withSimetricPoints()
	{
		CircularPeakPicking peakPicker(4);
		
		std::pair<double,double> peak = peakPicker.interpolate(0,8,0);
		
		CPPUNIT_ASSERT_EQUAL(1.0, peak.first);
		CPPUNIT_ASSERT_EQUAL(8.0, peak.second);
	}
	void testInterpolatedMaximum_withLowUnbalancedPoints()
	{
		CircularPeakPicking peakPicker(4);
		
		std::pair<double,double> peak = peakPicker.interpolate(6,8,0);
		
		CPPUNIT_ASSERT_EQUAL(0.7, peak.first);
		CPPUNIT_ASSERT_EQUAL(8.450, peak.second);
	}
	void testInterpolatedMaximum_withHighUnbalancedPoints()
	{
		CircularPeakPicking peakPicker(4);
		
		std::pair<double,double> peak = peakPicker.interpolate(0,8,6);
		
		CPPUNIT_ASSERT_EQUAL(1.3, peak.first);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(8.450, peak.second, 0.00000001);
	}
	void testSingleLowDetunedPeak()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,6,8,0,0};
		double positions[]={2.7};
		double values   []={8.450};

		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testSingleHighDetunedPeak()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,8,6,0};
		double positions[]={3.3};
		double values   []={8.450};
		
		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testTwoDetunedPeaks()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,6,8,0,8,6};
		double positions[]={1.7, 4.3};
		double values   []={8.450, 8.450};

		assertPeaksMatches(chromagramSize, chromagram, 2, positions, values);
	}
	void testSingleLowDetunedPeaksAtTheFront()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={8,0,0,0,0,6};
		double positions[]={5.7};
		double values   []={8.450};

		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testSingleLowDetunedPeaksAtTheBack()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,0,6,8};
		double positions[]={4.7};
		double values   []={8.450};

		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testSingleLowDetunedPeaksAtThePreviousToTheBack()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,6,8,0};
		double positions[]={3.7};
		double values   []={8.450};

		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testSingleHighDetunedPeaksAtTheFront()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={8,6,0,0,0,0};
		double positions[]={0.3};
		double values   []={8.450};

		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testSingleHighDetunedPeaksAtTheBack()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={6,0,0,0,0,8};
		double positions[]={5.3};
		double values   []={8.450};

		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testSingleHighDetunedPeaksAtThePreviousToTheBack()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,0,8,6};
		double positions[]={4.3};
		double values   []={8.450};

		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testInterpolatedMaximum_withInterBinPeak()
	{
		CircularPeakPicking peakPicker(4);
		
		std::pair<double,double> peak = peakPicker.interpolate(8,8,0);
		
		CPPUNIT_ASSERT_EQUAL(0.5, peak.first);
		CPPUNIT_ASSERT_EQUAL(9.0, peak.second);
	}
	void testSingleInterBinPeak()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,8,8,0};
		double positions[]={3.5};
		double values   []={9.0};

		assertPeaksMatches(chromagramSize, chromagram, 1, positions, values);
	}
	void testSinglePeak_withNonOneBinSize()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,8,0,0};
		double positions[]={3.0/3.0};
		double values   []={8.0};
		const unsigned nPeaks=1;

		CircularPeakPicking peakPicker(chromagramSize,1.0/3);
		std::vector<double> chromagramVector(chromagram, chromagram+chromagramSize);
		peakPicker.doIt(chromagramVector);

		const CircularPeakPicking::PeakList & peakList = peakPicker.output();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("Different number of peaks detected",
			nPeaks, (unsigned int)peakList.size());
		for (unsigned i=0; i<nPeaks; i++)
		{
			CPPUNIT_ASSERT_EQUAL(positions[i], peakList[i].first);
			CPPUNIT_ASSERT_EQUAL(values[i], peakList[i].second);
		}
	}
	void testSinglePeak_withNonZeroOffset()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,8,0,0};
		double positions[]={33.0};
		double values   []={8.0};
		const unsigned nPeaks=1;

		CircularPeakPicking peakPicker(chromagramSize,1.0,30.0);
		std::vector<double> chromagramVector(chromagram, chromagram+chromagramSize);
		peakPicker.doIt(chromagramVector);

		const CircularPeakPicking::PeakList & peakList = peakPicker.output();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("Different number of peaks detected",
			nPeaks, (unsigned int)peakList.size());
		for (unsigned i=0; i<nPeaks; i++)
		{
			CPPUNIT_ASSERT_EQUAL(positions[i], peakList[i].first);
			CPPUNIT_ASSERT_EQUAL(values[i], peakList[i].second);
		}
	}
	void testSinglePeak_withOffsetAndBinSize()
	{
		const unsigned chromagramSize=6;
		double chromagram[chromagramSize]={0,0,0,8,0,0};
		double positions[]={30.0+3.0/3.0};
		double values   []={8.0};
		const unsigned nPeaks=1;

		CircularPeakPicking peakPicker(chromagramSize,1.0/3.0,30.0);
		std::vector<double> chromagramVector(chromagram, chromagram+chromagramSize);
		peakPicker.doIt(chromagramVector);

		const CircularPeakPicking::PeakList & peakList = peakPicker.output();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("Different number of peaks detected",
			nPeaks, (unsigned int)peakList.size());
		for (unsigned i=0; i<nPeaks; i++)
		{
			CPPUNIT_ASSERT_EQUAL(positions[i], peakList[i].first);
			CPPUNIT_ASSERT_EQUAL(values[i], peakList[i].second);
		}
	}
	void testSuccesiveCallsClearsPreviousResults()
	{
		const unsigned chromagramSize=6;
		double previousChromagram[chromagramSize]={8,0,8,0,8,0};
		double chromagram[chromagramSize]={0,0,0,8,0,0};
		double positions[]={30.0+3.0/3.0};
		double values   []={8.0};
		const unsigned nPeaks=1;

		CircularPeakPicking peakPicker(chromagramSize,1.0/3.0,30.0);
		std::vector<double> chromagramVector(previousChromagram, previousChromagram+chromagramSize);
		peakPicker.doIt(chromagramVector);
		chromagramVector.assign(chromagram, chromagram+chromagramSize);
		peakPicker.doIt(chromagramVector);

		const CircularPeakPicking::PeakList & peakList = peakPicker.output();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("Different number of peaks detected",
			nPeaks, (unsigned int)peakList.size());
		for (unsigned i=0; i<nPeaks; i++)
		{
			CPPUNIT_ASSERT_EQUAL(positions[i], peakList[i].first);
			CPPUNIT_ASSERT_EQUAL(values[i], peakList[i].second);
		}
	}
};




} // namespace Test
} // namespace Simac

