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

#include "CircularPeakTunner.hxx"

namespace Simac
{

namespace Test
{

class CircularPeakTunnerTest;

CPPUNIT_TEST_SUITE_REGISTRATION( CircularPeakTunnerTest );

class CircularPeakTunnerTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( CircularPeakTunnerTest );
	CPPUNIT_TEST( testTunningShift_withNoDetunning );
	CPPUNIT_TEST( testTunningShift_withSemitoneTranspose );
	CPPUNIT_TEST( testTunningShift_withDetunning );
	CPPUNIT_TEST( testTunningShift_withReferenceAndDetunning );
	CPPUNIT_TEST( testTunningShift_near1ToneUpTransposition );
	CPPUNIT_TEST( testTunningShift_past1ToneUpTransposition );
	CPPUNIT_TEST( testTunningShift_near1ToneDownTransposition );
	CPPUNIT_TEST( testTunningShift_past1ToneDownTransposition );
	CPPUNIT_TEST( testTunningShift_aboveQuarterTone_near1ToneUpTransposition );
	CPPUNIT_TEST( testTunningShift_aboveQuarterTone_past1ToneUpTransposition );
	CPPUNIT_TEST( testTunningShift_aboveQuarterTone_near1ToneDownTransposition );
	CPPUNIT_TEST( testTunningShift_aboveQuarterTone_past1ToneDownTransposition );
	CPPUNIT_TEST( testTunningShift_maximumShift );
	CPPUNIT_TEST( testTunningShift_minimumShift );
	CPPUNIT_TEST( testTune_withNoDetunning );
	CPPUNIT_TEST( testTune_withDetunning );
	CPPUNIT_TEST( testTune_withDetunning_underTheSemitone );
	CPPUNIT_TEST( testTune_withDetunning_twiceUnderTheSemitone );
	CPPUNIT_TEST( testTune_withDetunning_twiceOverTheSemitone );
	CPPUNIT_TEST( testDo_withNoPeaks );
	CPPUNIT_TEST( testDo_withSinglePeak_noDetunning );
	CPPUNIT_TEST( testDo_withSinglePeak_detunning );
	CPPUNIT_TEST( testDo_withSinglePeak_referenceMoved );
	CPPUNIT_TEST( testDo_withSeveralPeaks_referenceMoved );
	CPPUNIT_TEST( testChromagramToSemitone_withNoDetunning_Tuned );
	CPPUNIT_TEST( testChromagramToSemitone_withNoDetunning_UpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withNoDetunning_PastUpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withNoDetunning_LowerBound );
	CPPUNIT_TEST( testChromagramToSemitone_withNoDetunning_PastLowerBound );
	CPPUNIT_TEST( testChromagramToSemitone_withDetunning_UpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withDetunning_PastUpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withOverSemitoneDetunning_UpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withOverSemitoneDetunning_PastUpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withDifferentReference_Detunning_UpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withDifferentReference_Detunning_PastUpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withDifferentReference_OverDetunning_UpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withDifferentReference_OverDetunning_PastUpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withDifferentReference_UnderDetunning_LowerBound );
	CPPUNIT_TEST( testChromagramToSemitone_withDifferentReference_UnderDetunning_PastLowerBound );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_Tunned );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_UpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_PastUpperBound );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_UpperBound_Detunned );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_PastUpperBound_Detunned );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_UpperBound_OverDetunned );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_PastUpperBound_OverDetunned );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_LowerBound_OverDetunned );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_PastLowerBound_OverDetunned );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_LowerBound_UnderDetunned );
	CPPUNIT_TEST( testChromagramToSemitone_withTransposedReference_PastLowerBound_UnderDetunned );
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
	// For all tunningShift tests any 12 class congruent numbers should be ok
	// Anyway the assert is done with the concrete number to detech such changes
	void testTunningShift_withNoDetunning()
	{
		double shift = CircularPeakTunner::tunningShift(0.0,0.0);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0,shift,1e-14);
	}
	void testTunningShift_withSemitoneTranspose()
	{
		double shift = CircularPeakTunner::tunningShift(1.0,0.0);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.0,shift,1e-14);
	}
	void testTunningShift_withDetunning()
	{
		double shift = CircularPeakTunner::tunningShift(0.0,0.2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.2,shift,1e-14);
	}
	void testTunningShift_withReferenceAndDetunning()
	{
		double shift = CircularPeakTunner::tunningShift(0.1,0.2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.2,shift,1e-14);
	}
	void testTunningShift_near1ToneUpTransposition()
	{
		double shift = CircularPeakTunner::tunningShift(0.69,0.2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.2,shift,1e-14);
	}
	void testTunningShift_past1ToneUpTransposition()
	{
		double shift = CircularPeakTunner::tunningShift(0.7,0.2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-1.2,shift,1e-14);
	}
	void testTunningShift_near1ToneDownTransposition()
	{
		double shift = CircularPeakTunner::tunningShift(11.69,0.2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-11.2,shift,1e-14);
	}
	void testTunningShift_past1ToneDownTransposition()
	{
		double shift = CircularPeakTunner::tunningShift(11.7,0.2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-12.2,shift,1e-14);
	}
	void testTunningShift_aboveQuarterTone_near1ToneUpTransposition()
	{
		double shift = CircularPeakTunner::tunningShift(0.19,0.7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3,shift,1e-14);
	}
	void testTunningShift_aboveQuarterTone_past1ToneUpTransposition()
	{
		double shift = CircularPeakTunner::tunningShift(0.2,0.7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-0.7,shift,1e-14);
	}
	void testTunningShift_aboveQuarterTone_near1ToneDownTransposition()
	{
		double shift = CircularPeakTunner::tunningShift(11.19,0.7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-10.7,shift,1e-14);
	}
	void testTunningShift_aboveQuarterTone_past1ToneDownTransposition()
	{
		double shift = CircularPeakTunner::tunningShift(11.2,0.7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-11.7,shift,1e-14);
	}
	void testTunningShift_maximumShift()
	{
		double shift = CircularPeakTunner::tunningShift(12.0,0.5);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(-12.5,shift,1e-14);
	}
	void testTunningShift_minimumShift()
	{
		double shift = CircularPeakTunner::tunningShift(0.0,0.50000001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(+0.49999999,shift,1e-14);
	}

	void testTune_withNoDetunning()
	{
		double tunned = CircularPeakTunner::tune(0.5,0.0);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, tunned,1e-14);
	}
	void testTune_withDetunning()
	{
		double tunned = CircularPeakTunner::tune(0.5,-0.2);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.3, tunned,1e-14);
	}
	void testTune_withDetunning_underTheSemitone()
	{
		double tunned = CircularPeakTunner::tune(0.5,-0.7);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(11.8, tunned,1e-14);
	}
	void testTune_withDetunning_twiceUnderTheSemitone()
	{
		double tunned = CircularPeakTunner::tune(0.2,-12.5);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(11.7, tunned,1e-14);
	}
	void testTune_withDetunning_twiceOverTheSemitone()
	{
		double tunned = CircularPeakTunner::tune(11.999,12.5);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.499, tunned,1e-14);
	}

	void assertTunedPeaksMatch(unsigned nPeaks, double center, double reference,
			double * expectedPositions, double * expectedValues,
			double * inputPositions, double * inputValues)
	{
		CircularPeakTunner tunner(reference);
		CircularPeakTunner::PeakList detunedPeaks;
		for (unsigned int i = 0; i<nPeaks; i++)
			detunedPeaks.push_back(std::make_pair(inputPositions[i], inputValues[i]));

		tunner.doIt(center, detunedPeaks);

		const CircularPeakTunner::PeakList & tunnedPeaks = tunner.output();

		CPPUNIT_ASSERT_EQUAL_MESSAGE("Different number of peaks detected",
			nPeaks, (unsigned int)tunnedPeaks.size());
		for (unsigned i=0; i<nPeaks; i++)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedPositions[i], tunnedPeaks[i].first, 1e-10);
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedValues[i], tunnedPeaks[i].second, 1e-10);
		}
	}

	void testDo_withNoPeaks()
	{
		unsigned nPeaks = 0;
		double inPos [] = {6666}; 
		double inVal [] = {6666};; 
		double expectedPos [] = {6666}; 
		double expectedVal [] = {6666};; 
		double reference = 0.0;
		double center = 0.0;
		assertTunedPeaksMatch(nPeaks, center, reference, expectedPos, expectedVal, inPos, inVal);
	}
	void testDo_withSinglePeak_noDetunning()
	{
		unsigned nPeaks = 1;
		double inPos [] = {1}; 
		double inVal [] = {7};; 
		double expectedPos [] = {1}; 
		double expectedVal [] = {7}; 
		double reference = 0.0;
		double center = 0.0;
		assertTunedPeaksMatch(nPeaks, center, reference, expectedPos, expectedVal, inPos, inVal);
	}
	void testDo_withSinglePeak_detunning()
	{
		unsigned nPeaks = 1;
		double inPos [] = {1}; 
		double inVal [] = {7};; 
		double expectedPos [] = {0.6}; 
		double expectedVal [] = {7}; 
		double reference = 0.0;
		double center = 0.4;
		assertTunedPeaksMatch(nPeaks, center, reference, expectedPos, expectedVal, inPos, inVal);
	}
	void testDo_withSinglePeak_referenceMoved()
	{
		unsigned nPeaks = 1;
		double inPos [] = {1}; 
		double inVal [] = {7};; 
		double expectedPos [] = {9.6}; 
		double expectedVal [] = {7}; 
		double reference = 3.3;
		double center = 0.4;
		assertTunedPeaksMatch(nPeaks, center, reference, expectedPos, expectedVal, inPos, inVal);
	}
	void testDo_withSeveralPeaks_referenceMoved()
	{
		unsigned nPeaks = 2;
		double inPos [] = {1, 4}; 
		double inVal [] = {7, 5};; 
		double expectedPos [] = {9.6, 0.6}; 
		double expectedVal [] = {7, 5}; 
		double reference = 3.3;
		double center = 0.4;
		assertTunedPeaksMatch(nPeaks, center, reference, expectedPos, expectedVal, inPos, inVal);
	}


	void testChromagramToSemitone_withNoDetunning_Tuned()
	{
		CircularPeakTunner tunner(1.0);
		double tunned = tunner.chromagramToSemitone(1.0, 1.0);
		CPPUNIT_ASSERT_EQUAL(0.0,tunned);
	}
	void testChromagramToSemitone_withNoDetunning_UpperBound()
	{
		CircularPeakTunner tunner(1.0);
		double tunned = tunner.chromagramToSemitone(1.0, 2.49);
		CPPUNIT_ASSERT_EQUAL(0.0,tunned);
	}
	void testChromagramToSemitone_withNoDetunning_PastUpperBound()
	{
		CircularPeakTunner tunner(1.0);
		double tunned = tunner.chromagramToSemitone(1.0, 2.501);
		CPPUNIT_ASSERT_EQUAL(1.0,tunned);
	}
	void testChromagramToSemitone_withNoDetunning_LowerBound()
	{
		CircularPeakTunner tunner(1.0);
		double tunned = tunner.chromagramToSemitone(1.0, 35.50);
		CPPUNIT_ASSERT_EQUAL(0.0,tunned);
	}
	void testChromagramToSemitone_withNoDetunning_PastLowerBound()
	{
		CircularPeakTunner tunner(1.0);
		double tunned = tunner.chromagramToSemitone(1.0, 35.49);
		CPPUNIT_ASSERT_EQUAL(11.0,tunned);
	}
	void testChromagramToSemitone_withDetunning_UpperBound()
	{
		CircularPeakTunner tunner(1.0);
		double tunned = tunner.chromagramToSemitone(1.3, 2.79);
		CPPUNIT_ASSERT_EQUAL(0.0,tunned);
	}
	void testChromagramToSemitone_withDetunning_PastUpperBound()
	{
		CircularPeakTunner tunner(1.0);
		double tunned = tunner.chromagramToSemitone(1.3, 2.81);
		CPPUNIT_ASSERT_EQUAL(1.0,tunned);
	}
	void testChromagramToSemitone_withOverSemitoneDetunning_UpperBound()
	{
		CircularPeakTunner tunner(1.0);
		double tunned = tunner.chromagramToSemitone(2.6, 0.99);
		CPPUNIT_ASSERT_EQUAL(0.0,tunned);
	}
	void testChromagramToSemitone_withOverSemitoneDetunning_PastUpperBound()
	{
		CircularPeakTunner tunner(1.0);
		double tunned = tunner.chromagramToSemitone(2.6, 1.1001);
		CPPUNIT_ASSERT_EQUAL(1.0,tunned);
	}
	void testChromagramToSemitone_withDifferentReference_Detunning_UpperBound()
	{
		CircularPeakTunner tunner(0.5);
		double tunned = tunner.chromagramToSemitone(1.0, 2.49);
		CPPUNIT_ASSERT_EQUAL(0.0,tunned);
	}
	void testChromagramToSemitone_withDifferentReference_Detunning_PastUpperBound()
	{
		CircularPeakTunner tunner(0.5);
		double tunned = tunner.chromagramToSemitone(1.0, 2.501);
		CPPUNIT_ASSERT_EQUAL(1.0,tunned);
	}
	void testChromagramToSemitone_withDifferentReference_OverDetunning_UpperBound()
	{
		CircularPeakTunner tunner(0.5);
		double tunned = tunner.chromagramToSemitone(2.1, 0.59);
		CPPUNIT_ASSERT_EQUAL(0.0,tunned);
	}
	void testChromagramToSemitone_withDifferentReference_OverDetunning_PastUpperBound()
	{
		CircularPeakTunner tunner(0.5);
		double tunned = tunner.chromagramToSemitone(2.1, 0.601);
		CPPUNIT_ASSERT_EQUAL(1.0,tunned);
	}
	void testChromagramToSemitone_withDifferentReference_UnderDetunning_LowerBound()
	{
		CircularPeakTunner tunner(2.0);
		double tunned = tunner.chromagramToSemitone(0.4, 1.89);
		CPPUNIT_ASSERT_EQUAL(11.0,tunned);
	}
	void testChromagramToSemitone_withDifferentReference_UnderDetunning_PastLowerBound()
	{
		CircularPeakTunner tunner(2.0);
		double tunned = tunner.chromagramToSemitone(0.4, 1.901);
		CPPUNIT_ASSERT_EQUAL(0.0,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_Tunned()
	{
		CircularPeakTunner tunner(7.0);
		unsigned tunned = tunner.chromagramToSemitone(1.0, 7.0);
		CPPUNIT_ASSERT_EQUAL(0u,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_UpperBound()
	{
		CircularPeakTunner tunner(7.0);
		unsigned tunned = tunner.chromagramToSemitone(1.0, 8.49);
		CPPUNIT_ASSERT_EQUAL(0u,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_PastUpperBound()
	{
		CircularPeakTunner tunner(7.0);
		unsigned tunned = tunner.chromagramToSemitone(1.0, 8.51);
		CPPUNIT_ASSERT_EQUAL(1u,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_UpperBound_Detunned()
	{
		CircularPeakTunner tunner(7.0);
		unsigned tunned = tunner.chromagramToSemitone(1.1, 8.59);
		CPPUNIT_ASSERT_EQUAL(0u,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_PastUpperBound_Detunned()
	{
		CircularPeakTunner tunner(7.0);
		unsigned tunned = tunner.chromagramToSemitone(1.1, 8.61);
		CPPUNIT_ASSERT_EQUAL(1u,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_UpperBound_OverDetunned()
	{
		CircularPeakTunner tunner(7.0);
		unsigned tunned = tunner.chromagramToSemitone(2.6, 7.09);
		CPPUNIT_ASSERT_EQUAL(0u,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_PastUpperBound_OverDetunned()
	{
		CircularPeakTunner tunner(7.0);
		unsigned tunned = tunner.chromagramToSemitone(2.6, 7.11);
		CPPUNIT_ASSERT_EQUAL(1u,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_LowerBound_OverDetunned()
	{
		CircularPeakTunner tunner(7.0);
		unsigned tunned = tunner.chromagramToSemitone(2.6, 4.11);
		CPPUNIT_ASSERT_EQUAL(0u,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_PastLowerBound_OverDetunned()
	{
		CircularPeakTunner tunner(7.0);
		unsigned tunned = tunner.chromagramToSemitone(2.6, 4.09);
		CPPUNIT_ASSERT_EQUAL(11u,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_LowerBound_UnderDetunned()
	{
		CircularPeakTunner tunner(8.0);
		unsigned tunned = tunner.chromagramToSemitone(0.4, 7.91);
		CPPUNIT_ASSERT_EQUAL(0u,tunned);
	}
	void testChromagramToSemitone_withTransposedReference_PastLowerBound_UnderDetunned()
	{
		CircularPeakTunner tunner(8.0);
		unsigned tunned = tunner.chromagramToSemitone(0.4, 7.89);
		CPPUNIT_ASSERT_EQUAL(11u,tunned);
	}
};




} // namespace Test
} // namespace Simac

