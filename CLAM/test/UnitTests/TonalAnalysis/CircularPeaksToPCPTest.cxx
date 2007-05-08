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

#include "CircularPeaksToPCP.hxx"

namespace Simac
{

namespace Test
{

class CircularPeaksToPCPTest;

CPPUNIT_TEST_SUITE_REGISTRATION( CircularPeaksToPCPTest );

class CircularPeaksToPCPTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( CircularPeaksToPCPTest );
	CPPUNIT_TEST( testDo_withNoPeaks );
	CPPUNIT_TEST( testDo_withSinglePeak );
	CPPUNIT_TEST( testDo_withSingleLowPeak );
	CPPUNIT_TEST( testDo_withSinglePastLowPeak );
	CPPUNIT_TEST( testDo_withSameSemitonePeaks_getAdded );
	CPPUNIT_TEST( testDo_withPeaksOnTheBorder );
	CPPUNIT_TEST( testDo_withMultipleFrames_getCleared );
	CPPUNIT_TEST( testHammingWindowing_withCenterPoint );
	CPPUNIT_TEST( testHammingWindowing_withOtherCenterPoint );
	CPPUNIT_TEST( testHammingWindowing_withInterSemitonePosition );
	CPPUNIT_TEST( testHammingWindowing_withOtherInterSemitonePosition );
	CPPUNIT_TEST( testDo_withWindowingActivated );
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

	void assertPCPMatch(unsigned nPeaks,
			double * expectedPCP,
			double * inputPositions, double * inputValues)
	{
		unsigned int nPCPbins=12;
		CircularPeaksToPCP peaks2pcp;
		CircularPeaksToPCP::PeakList detunedPeaks;
		for (unsigned int i = 0; i<nPeaks; i++)
			detunedPeaks.push_back(std::make_pair(inputPositions[i], inputValues[i]));

		peaks2pcp.doIt(detunedPeaks);

		const CircularPeaksToPCP::PCP & pcp = peaks2pcp.output();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("Different number of peaks detected",
			nPCPbins, (unsigned int)pcp.size());
		for (unsigned i=0; i<nPCPbins; i++)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPCP[i], pcp[i], 1e-10);
		}
	}

	void testDo_withNoPeaks()
	{
		unsigned nPeaks = 0;
		double inPos [] = {6666}; 
		double inVal [] = {6666};; 
		double expectedPCP [12] = {
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0
		}; 
		assertPCPMatch(nPeaks, expectedPCP, inPos, inVal);
	}
	void testDo_withSinglePeak()
	{
		unsigned nPeaks = 1;
		double inPos [] = {2.0}; 
		double inVal [] = {8.0};; 
		double expectedPCP [12] = {
			0.0,0.0,8.0,0.0,
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0
		}; 
		assertPCPMatch(nPeaks, expectedPCP, inPos, inVal);
	}
	void testDo_withSingleLowPeak()
	{
		unsigned nPeaks = 1;
		double inPos [] = {1.51}; 
		double inVal [] = {8.0};; 
		double expectedPCP [12] = {
			0.0,0.0,8.0,0.0,
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0
		}; 
		assertPCPMatch(nPeaks, expectedPCP, inPos, inVal);
	}
	void testDo_withSinglePastLowPeak()
	{
		unsigned nPeaks = 1;
		double inPos [] = {1.49}; 
		double inVal [] = {8.0};; 
		double expectedPCP [12] = {
			0.0,8.0,0.0,0.0,
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0
		}; 
		assertPCPMatch(nPeaks, expectedPCP, inPos, inVal);
	}
	void testDo_withSameSemitonePeaks_getAdded()
	{
		unsigned nPeaks = 2;
		double inPos [] = {1.79, 2.3}; 
		double inVal [] = {8.0 , 1.0};; 
		double expectedPCP [12] = {
			0.0,0.0,9.0,0.0,
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0
		}; 
		assertPCPMatch(nPeaks, expectedPCP, inPos, inVal);
	}
	void testDo_withPeaksOnTheBorder()
	{
		unsigned nPeaks = 1;
		double inPos [] = {11.9}; 
		double inVal [] = {8.0};; 
		double expectedPCP [12] = {
			8.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0
		}; 
		assertPCPMatch(nPeaks, expectedPCP, inPos, inVal);
	}
	void testDo_withMultipleFrames_getCleared()
	{
		unsigned nPeaks = 1;
		double inPos1 [] = {1.0}; 
		double inVal1 [] = {7.0};; 
		double inPos2 [] = {2.0}; 
		double inVal2 [] = {7.0};; 
		double expectedPCP [12] = {
			0.0,0.0,7.0,0.0,
			0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0
		}; 
		unsigned int nPCPbins=12;
		CircularPeaksToPCP peaks2pcp;

		CircularPeaksToPCP::PeakList peaks1;
		for (unsigned int i = 0; i<nPeaks; i++)
			peaks1.push_back(std::make_pair(inPos1[i], inVal1[i]));
		peaks2pcp.doIt(peaks1);

		CircularPeaksToPCP::PeakList peaks2;
		for (unsigned int i = 0; i<nPeaks; i++)
			peaks2.push_back(std::make_pair(inPos2[i], inVal2[i]));
		peaks2pcp.doIt(peaks2);

		const CircularPeaksToPCP::PCP & pcp = peaks2pcp.output();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("Different number of peaks detected",
			nPCPbins, (unsigned int)pcp.size());
		for (unsigned i=0; i<nPCPbins; i++)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPCP[i], pcp[i], 1e-10);
		}
	}

	void testHammingWindowing_withCenterPoint()
	{
		double windowed = CircularPeaksToPCP::windowedValue(0.0, 10.0);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, windowed,1e-14);
	}
	void testHammingWindowing_withOtherCenterPoint()
	{
		double windowed = CircularPeaksToPCP::windowedValue(1.0, 10.0);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(10.0, windowed,1e-14);
	}
	void testHammingWindowing_withInterSemitonePosition()
	{
		double windowed = CircularPeaksToPCP::windowedValue(0.5, 10.0);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, windowed,1e-14);
	}
	void testHammingWindowing_withOtherInterSemitonePosition()
	{
		double windowed = CircularPeaksToPCP::windowedValue(1.5, 10.0);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.8, windowed,1e-14);
	}

	void testDo_withWindowingActivated()
	{
		unsigned nPeaks = 2;
		double inPos [] = {3.0, 6.5}; 
		double inVal [] = {1.0, 1.0};; 
		double expectedPCP [12] = {
			0.0,0.0,0.0,1.0,
			0.0,0.0,0.0,0.08,
			0.0,0.0,0.0,0.0
		}; 
		unsigned int nPCPbins=12;
		CircularPeaksToPCP peaks2pcp;

		peaks2pcp.activateWindowing();

		CircularPeaksToPCP::PeakList peaks;
		for (unsigned int i = 0; i<nPeaks; i++)
			peaks.push_back(std::make_pair(inPos[i], inVal[i]));
		peaks2pcp.doIt(peaks);

		const CircularPeaksToPCP::PCP & pcp = peaks2pcp.output();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("Different number of peaks detected",
			nPCPbins, (unsigned int)pcp.size());
		for (unsigned i=0; i<nPCPbins; i++)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPCP[i], pcp[i], 1e-10);
		}
	}

};




} // namespace Test
} // namespace Simac

