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

#include "ChordCorrelator.hxx"
#include <iosfwd>

namespace Simac
{

namespace Test
{

class ChordCorrelatorTest;

CPPUNIT_TEST_SUITE_REGISTRATION( ChordCorrelatorTest );

class ChordCorrelatorTest : public CppUnit::TestCase
{
	CPPUNIT_TEST_SUITE( ChordCorrelatorTest );
	CPPUNIT_TEST( testDumpChordMatrix );
	CPPUNIT_TEST( testSilence );
	CPPUNIT_TEST( testAllNotes );
	CPPUNIT_TEST( testPureCMajor );
	CPPUNIT_TEST( testPureCMinor );
	CPPUNIT_TEST( testFifth );
	CPPUNIT_TEST( testSingleNote );
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

	void assertChordCorrelationMatches(double * expected, double * input)
	{
		ChordCorrelator correlator;
		std::vector<double> inputData(input, input+12);
		correlator.doIt(inputData);

		const ChordCorrelator::ChordCorrelation & correlation = correlator.output();

		for (unsigned i=0; i<correlation.size(); i++)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expected[i], correlation[i], 1e-10);
		}
	}

	void testDumpChordMatrix()
	{
		ChordCorrelator correlator;
		std::ostringstream os;
		correlator.DumpChordPatterns(os);
		CPPUNIT_ASSERT_EQUAL(std::string(
			"1 1 1 1 1 1 1 1 1 1 1 1 12 C None\n"
			"1 0 0 0 1 0 0 1 0 0 0 0 3 C Major\n"
			"0 1 0 0 0 1 0 0 1 0 0 0 3 Db Major\n"
			"0 0 1 0 0 0 1 0 0 1 0 0 3 D Major\n"
			"0 0 0 1 0 0 0 1 0 0 1 0 3 Eb Major\n"
			"0 0 0 0 1 0 0 0 1 0 0 1 3 E Major\n"
			"1 0 0 0 0 1 0 0 0 1 0 0 3 F Major\n"
			"0 1 0 0 0 0 1 0 0 0 1 0 3 Gb Major\n"
			"0 0 1 0 0 0 0 1 0 0 0 1 3 G Major\n"
			"1 0 0 1 0 0 0 0 1 0 0 0 3 Ab Major\n"
			"0 1 0 0 1 0 0 0 0 1 0 0 3 A Major\n"
			"0 0 1 0 0 1 0 0 0 0 1 0 3 Bb Major\n"
			"0 0 0 1 0 0 1 0 0 0 0 1 3 B Major\n"
			"1 0 0 1 0 0 0 1 0 0 0 0 3 C Minor\n"
			"0 1 0 0 1 0 0 0 1 0 0 0 3 Db Minor\n"
			"0 0 1 0 0 1 0 0 0 1 0 0 3 D Minor\n"
			"0 0 0 1 0 0 1 0 0 0 1 0 3 Eb Minor\n"
			"0 0 0 0 1 0 0 1 0 0 0 1 3 E Minor\n"
			"1 0 0 0 0 1 0 0 1 0 0 0 3 F Minor\n"
			"0 1 0 0 0 0 1 0 0 1 0 0 3 Gb Minor\n"
			"0 0 1 0 0 0 0 1 0 0 1 0 3 G Minor\n"
			"0 0 0 1 0 0 0 0 1 0 0 1 3 Ab Minor\n"
			"1 0 0 0 1 0 0 0 0 1 0 0 3 A Minor\n"
			"0 1 0 0 0 1 0 0 0 0 1 0 3 Bb Minor\n"
			"0 0 1 0 0 0 1 0 0 0 0 1 3 B Minor\n"
			"1 0 0 0 1 0 0 0 1 0 0 0 3 C Augmented\n"
			"0 1 0 0 0 1 0 0 0 1 0 0 3 Db Augmented\n"
			"0 0 1 0 0 0 1 0 0 0 1 0 3 D Augmented\n"
			"0 0 0 1 0 0 0 1 0 0 0 1 3 Eb Augmented\n"
			"1 0 0 1 0 0 1 0 0 1 0 0 4 C Diminished\n"
			"0 1 0 0 1 0 0 1 0 0 1 0 4 Db Diminished\n"
			"0 0 1 0 0 1 0 0 1 0 0 1 4 D Diminished\n"
			),os.str());
	}
	void testSilence()
	{
		double pcp[12]={0,0,0,0, 0,0,0,0, 0,0,0,0};
		double chords[]={
			0,
			0,0,0,0, 0,0,0,0, 0,0,0,0, 
			0,0,0,0, 0,0,0,0, 0,0,0,0, 
			0,0,0,0, 0,0,0
		};

		assertChordCorrelationMatches(chords, pcp);
	}
	void testAllNotes()
	{
		double pcp[12]={1,1,1,1, 1,1,1,1, 1,1,1,1};
		double chords[]={
			1,
			1,1,1,1, 1,1,1,1, 1,1,1,1, 
			1,1,1,1, 1,1,1,1, 1,1,1,1, 
			1,1,1,1, 1,1,1
		};

		assertChordCorrelationMatches(chords, pcp);
	}
	void testPureCMajor()
	{
		double pcp[12]={1,0,0,0, 1,0,0,1, 0,0,0,0};
		double chords[]={
			3,
			12,0,0,4, 4,4,0,4, 4,4,0,0, 
			8,4,0,0, 8,4,0,4, 0,8,0,0, 
			8,0,0,4, 3,6,0
		};
		for (unsigned i=0; i<32; i++) chords[i]/=12;

		assertChordCorrelationMatches(chords, pcp);
	}
	void testPureCMinor()
	{
		double pcp[12]={1,0,0,1, 0,0,0,1, 0,0,0,0};
		double chords[]={
			3,
			8,0,0,8, 0,4,0,4, 8,0,0,4, 
			12,0,0,4, 4,4,0,4, 4,4,0,0, 
			4,0,0,8, 6,3,0
		};
		for (unsigned i=0; i<32; i++) chords[i]/=12;

		assertChordCorrelationMatches(chords, pcp);
	}
	void testFifth()
	{
		double pcp[12]={1,0,0,0, 0,0,0,1, 0,0,0,0};
		double chords[]={
			2,
			8,0,0,4, 0,4,0,4, 4,0,0,0, 
			8,0,0,0, 4,4,0,4, 0,4,0,0, 
			4,0,0,4, 3,3,0
		};
		for (unsigned i=0; i<32; i++) chords[i]/=12;

		assertChordCorrelationMatches(chords, pcp);
	}
	void testSingleNote()
	{
		double pcp[12]={1,0,0,0, 0,0,0,0, 0,0,0,0};
		double chords[]={
			1,
			4,0,0,0, 0,4,0,0, 4,0,0,0, 
			4,0,0,0, 0,4,0,0, 0,4,0,0, 
			4,0,0,0, 3,0,0
		};
		for (unsigned i=0; i<32; i++) chords[i]/=12;

		assertChordCorrelationMatches(chords, pcp);
	}
};




} // namespace Test
} // namespace Simac

