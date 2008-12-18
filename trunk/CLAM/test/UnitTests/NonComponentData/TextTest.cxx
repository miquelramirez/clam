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

#include "Text.hxx" // CLAM
#include "DynamicType.hxx" // CLAM
#include "XMLStorage.hxx" // CLAM
#include "XMLTestHelper.hxx"
#include <cppunit/extensions/HelperMacros.h>


using namespace CLAM;

/////////////////////////////////////////////////////////////////////
// Test Class
/////////////////////////////////////////////////////////////////////
namespace CLAMTest {
	
	class TextTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( TextTest );


	class TextTest : public CppUnit::TestFixture {

		CPPUNIT_TEST_SUITE (CLAMTest::TextTest);

		CPPUNIT_TEST (testDefaultConstructor);
		CPPUNIT_TEST (testStringComparation_whenEqual);
		CPPUNIT_TEST (testStringComparation_whenDiffers);
		CPPUNIT_TEST (testInitializationFromCharPointer);
		CPPUNIT_TEST (testStreamExtraction_withSpaces);
		CPPUNIT_TEST (testStreamExtraction_withTabs);
		CPPUNIT_TEST (testStreamExtraction_clearsPreviousContent);
		CPPUNIT_TEST (testXml_output);
		CPPUNIT_TEST (testXml_inputOutputAreIdempotent);

		CPPUNIT_TEST_SUITE_END();

	private:
		class TextContainerComponent : public CLAM::DynamicType
		{
		public:
			DYNAMIC_TYPE(TextContainerComponent, 1);
			DYN_ATTRIBUTE(0, public, CLAM::Text, Text);
		};
	private:
		void testDefaultConstructor()
		{
			CLAM::Text text;

			CPPUNIT_ASSERT_EQUAL(CLAM::Text(""), text);
		}

		void testStringComparation_whenEqual()
		{
			CLAM::Text text("Hola");
			std::string str("Hola");

			bool result = (text==str);

			CPPUNIT_ASSERT_EQUAL(true, result);
		}
		void testStringComparation_whenDiffers()
		{
			CLAM::Text text("Hola");
			std::string str("Adios");

			bool result = (text==str);

			CPPUNIT_ASSERT_EQUAL(false, result);
		}
		void testInitializationFromCharPointer()
		{
			CLAM::Text text = "Hola";

			std::string & str = text;

			CPPUNIT_ASSERT_EQUAL(std::string("Hola"), str);
		}

		void testStreamExtraction_withSpaces()
		{
			std::string toberead("Hola tu");
			std::istringstream myistream (toberead);
			CLAM::Text extracted;
			myistream >> extracted;
			CLAM::Text expected("Hola tu");

			CPPUNIT_ASSERT_EQUAL(expected, extracted);
			
		}
		void testStreamExtraction_withTabs()
		{
			std::string toberead("Hola\t tu");
			std::istringstream myistream (toberead);
			CLAM::Text extracted;
			myistream >> extracted;
			CLAM::Text expected("Hola\t tu");

			CPPUNIT_ASSERT_EQUAL(expected, extracted);
			
		}
		void testStreamExtraction_clearsPreviousContent()
		{
			std::string toberead("Hola");
			std::istringstream myistream (toberead);
			CLAM::Text extracted("Previous content");
			myistream >> extracted;
			CLAM::Text expected("Hola");

			CPPUNIT_ASSERT_EQUAL(expected, extracted);
			
		}

		void testXml_inputOutputAreIdempotent()
		{
			TextContainerComponent component;
			component.AddText();
			component.UpdateData();
			component.SetText("La cadena esperada");

			bool match = XMLInputOutputMatches(component,__FILE__"TextContainer.xml");
			CLAM_ASSERT(match, "Text Store/Load mismatch");
		}

		void testXml_output()
		{
			TextContainerComponent component;
			component.AddText();
			component.UpdateData();
			component.SetText("La cadena esperada");

			std::stringstream stream;
			XMLStorage storage;
			storage.Dump(component, "object", stream, false);
			CLAMTEST_ASSERT_XML_EQUAL(
				"<object><Text>La cadena esperada</Text></object>\n", stream.str());

		}

	};


}




