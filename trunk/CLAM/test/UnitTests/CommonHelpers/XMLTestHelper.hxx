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

#ifndef _XMLTESTHELPER_
#define _XMLTESTHELPER_

#include <iostream>
#include <fstream>
#include <cppunit/extensions/HelperMacros.h>


#ifdef CLAM_USE_XML
#include "XMLStorage.hxx"
#include "XMLComponentAdapter.hxx"
#endif//CLAM_USE_XML

using namespace CLAM;


namespace CLAMTest {
	/**
	 * Returns true if the XML output from 'tested' and the XML output
	 * generated from an object restored from this output matches.
	 */
	template <class T>
	bool XMLInputOutputMatches(T & outObject, char * filename) {
		std::ostringstream out;
		std::ostringstream in;
		T inObject;
#ifdef CLAM_USE_XML
		{
			// Store the arg a file and on a string
			XMLStorage storage;
			storage.UseIndentation(true);
			std::ofstream fout(filename);
			storage.Dump(outObject, "Object", out);
			storage.Dump(outObject, "Object", fout);
		}
		{
			// Recover the objec on superIn
			XMLStorage storage;
			storage.Restore(inObject, filename);
		}
		{
			// Store it on a string again
			XMLStorage storage;
			storage.UseIndentation(true);
			storage.Dump(inObject, "Object", in);
		}
#endif//CLAM_USE_XML
		CPPUNIT_ASSERT_EQUAL(in.str(),out.str());
		return in.str()==out.str();
	}

	void checkXmlEqual( std::string expectedXml,
		std::string actualXml,
		CppUnit::SourceLine sourceLine );


	/// Asserts that two XML strings are equivalent.
	#define CLAMTEST_ASSERT_XML_EQUAL( expected, actual ) \
		::CLAMTest::checkXmlEqual( expected, actual,      \
		CPPUNIT_SOURCELINE() )


}
#endif//_XMLTESTHELPER_

