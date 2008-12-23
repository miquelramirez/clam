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


#include "XMLStorage.hxx" // CLAM

using namespace CLAM;


namespace CLAMTest {
	/**
	 * Returns true if the XML output from 'tested' and the XML output
	 * generated from an object restored from this output matches.
	 */
	template <class T>
	bool XMLInputOutputMatches(T & outObject, const char * filename)
	{
		std::stringstream out;
		std::ostringstream in;
		T inObject;
#ifdef CLAM_USE_XML
		{
			// Store the arg a file and on a string
			XMLStorage::Dump(outObject, "Object", out);
		}
		{
			// Recover the objec on superIn
			XMLStorage::Restore(inObject, out);
		}
		{
			// Store it on a string again
			XMLStorage::Dump(inObject, "Object", in);
		}
#endif//CLAM_USE_XML
		CPPUNIT_ASSERT_EQUAL(out.str(),in.str());
		return in.str()==out.str();
	}

	void checkXmlEqual( const std::string & expectedXml,
		const std::string & actualXml,
		CppUnit::SourceLine sourceLine );


	/// Asserts that two XML strings are equivalent.
	#define CLAMTEST_ASSERT_XML_EQUAL( expected, actual ) \
		::CLAMTest::checkXmlEqual( expected, actual,      \
		CPPUNIT_SOURCELINE() )

	inline std::string xmlHeader()
	{
		return
		#ifdef USE_XERCES
		"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>";
		#else
		"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		#endif
	}
	inline std::string xmlFooter()
	{
		return
		#ifdef USE_XERCES
		"";
		#else
		"\n";
		#endif
	}
}
#endif//_XMLTESTHELPER_

