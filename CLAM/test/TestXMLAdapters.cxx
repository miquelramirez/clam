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

#ifdef CLAM_USE_XML
#include "XMLAdapter.hxx"
#include "XMLStaticAdapter.hxx"
#include "XMLArrayAdapter.hxx"
#include "XMLComponentAdapter.hxx"
#include "XMLIterableAdapter.hxx"
#include "XMLStorage.hxx"
#endif//CLAM_USE_XML
#include "Array.hxx"
#include <list>

#include <iostream>
#include <fstream>
#include <vector>

using namespace CLAM;

namespace CLAMTest {
	
/**
 * Tests the XMLAdapter class
 */
void XMLAdapterClassTest() {
	std::cout << "-- Testing XMLAdapter" << std::endl;
	{
		int i = 3;
		double d = 3.5;
		char c = 'a';
		char * s = "Hola";
		XMLStorage storer("MyPrueba");
		{ 
			XMLAdapter<int> intAdapter(i);
			storer.Store(&intAdapter);
			XMLAdapter<double> doubleAdapter(d);
			storer.Store(&doubleAdapter);
			XMLAdapter<char> charAdapter(c);
			storer.Store(&charAdapter);
			XMLAdapter<char*> strAdapter(s);
			storer.Store(&strAdapter);
		}
		{
			XMLAdapter<int> intAdapter(i, "myInt");
			storer.Store(&intAdapter);
			XMLAdapter<double> doubleAdapter(d, "myDouble");
			storer.Store(&doubleAdapter);
			XMLAdapter<char> charAdapter(c, "myChar");
			storer.Store(&charAdapter);
			XMLAdapter<char*> strAdapter(s, "myString");
			storer.Store(&strAdapter);
		}
		{ 
			XMLAdapter<int> intAdapter(i, "myInt", true);
			storer.Store(&intAdapter);
			XMLAdapter<double> doubleAdapter(d, "myDouble", true);
			storer.Store(&doubleAdapter);
			XMLAdapter<char> charAdapter(c, "myChar", true);
			storer.Store(&charAdapter);
			XMLAdapter<char*> strAdapter(s, "myString", true);
			storer.Store(&strAdapter);
		}
		storer.dumpOn(std::cout);
		std::ofstream f("testxml.xml");
		storer.dumpOn(f);
	}
	{
		XMLStorage storer("MyPrueba");
		storer._restoreFrom("testxml.xml");
		{
			int i = 6;
			double d = 6.214;
			char c = 'b';
			char buffer[512] = "Adios";
			char *s = buffer;
			std::cout << "Reading content" << std::endl;
			std::cout << "i:" << i << " d:" << d << " c:'" << c << "' s:\"" << s << "\"" << std::endl;
			{
				XMLAdapter<int> intAdapter(i);
				storer.Load(&intAdapter);
				XMLAdapter<double> doubleAdapter(d);
				storer.Load(&doubleAdapter);
				XMLAdapter<char> charAdapter(c);
				storer.Load(&charAdapter);
				XMLAdapter<char*> strAdapter(s);
				storer.Load(&strAdapter);
			}
			std::cout << "i:" << i << " d:" << d << " c:'" << c << "' s:\"" << s << "\"" << std::endl;
		}
		{
			int i = 6;
			double d = 6.214;
			char c = 'b';
			char buffer[512] = "Adios";
			char *s = buffer;
			std::cout << "Reading attributes" << std::endl;
			std::cout << "i:" << i << " d:" << d << " c:'" << c << "' s:\"" << s << "\"" << std::endl;
			{
				XMLAdapter<int> intAdapter(i, "myInt");
				storer.Load(&intAdapter);
				XMLAdapter<double> doubleAdapter(d, "myDouble");
				storer.Load(&doubleAdapter);
				XMLAdapter<char> charAdapter(c, "myChar");
				storer.Load(&charAdapter);
				XMLAdapter<char*> strAdapter(s, "myString");
				storer.Load(&strAdapter);
			}
			std::cout << "i:" << i << " d:" << d << " c:'" << c << "' s:\"" << s << "\"" << std::endl;
		}
		{
			int i = 6;
			double d = 6.214;
			char c = 'b';
			char buffer[512] = "Adios";
			char *s = buffer;
			std::cout << "Reading elements" << std::endl;
			std::cout << "i:" << i << " d:" << d << " c:'" << c << "' s:\"" << s << "\"" << std::endl;
			{ 
				XMLAdapter<int> intAdapter(i, "myInt", true);
				storer.Load(&intAdapter);
				XMLAdapter<double> doubleAdapter(d, "myDouble", true);
				storer.Load(&doubleAdapter);
				XMLAdapter<char> charAdapter(c, "myChar", true);
				storer.Load(&charAdapter);
				XMLAdapter<char*> strAdapter(s, "myString", true);
				storer.Load(&strAdapter);
			}
			std::cout << "i:" << i << " d:" << d << " c:'" << c << "' s:\"" << s << "\"" << std::endl;
		}
	}
}

/**
 * Test the XMLArrayAdapter class
 */
void XMLArrayAdapterClassTest() {
	std::cout << "-- Testing XMLArrayAdapter" << std::endl;
	int intArray[] = {2,359,4,5,32,432};
	double doubleArray[] = {2.67,359.67,46574.56,565,32.5,432};
	char charArray[] = {'e','#','<','>','"','b','c','&'};
	char *stringArray[] = {"Hi,you", "HowAreYou?", "VeryWell,thanks,&you?"};
	XMLStorage storer("MyPrueba");
	{
		XMLArrayAdapter<int> intAdapter(intArray, sizeof(intArray)/sizeof(int));
		storer.Store(&intAdapter);
		XMLArrayAdapter<double> doubleAdapter(doubleArray, sizeof(doubleArray)/sizeof(double));
		storer.Store(&doubleAdapter);
		XMLArrayAdapter<char> charAdapter(charArray, sizeof(charArray)/sizeof(char));
		storer.Store(&charAdapter);
		XMLArrayAdapter<char*> strAdapter(stringArray, sizeof(stringArray)/sizeof(char*));
		storer.Store(&strAdapter);
	}
	{ 
		XMLArrayAdapter<int> intAdapter(intArray, sizeof(intArray)/sizeof(int), "myInt");
		storer.Store(&intAdapter);
		XMLArrayAdapter<double> doubleAdapter(doubleArray, sizeof(doubleArray)/sizeof(double), "myDouble");
		storer.Store(&doubleAdapter);
		XMLArrayAdapter<char> charAdapter(charArray, sizeof(charArray)/sizeof(char), "myChar");
		storer.Store(&charAdapter);
		XMLArrayAdapter<char*> strAdapter(stringArray, sizeof(stringArray)/sizeof(char*), "myString");
		storer.Store(&strAdapter);
	}
	{ 
		XMLArrayAdapter<int> intAdapter(intArray, sizeof(intArray)/sizeof(int), "myInt", true);
		storer.Store(&intAdapter);
		XMLArrayAdapter<double> doubleAdapter(doubleArray, sizeof(doubleArray)/sizeof(double), "myDouble", true);
		storer.Store(&doubleAdapter);
		XMLArrayAdapter<char> charAdapter(charArray, sizeof(charArray)/sizeof(char), "myChar", true);
		storer.Store(&charAdapter);
		XMLArrayAdapter<char*> strAdapter(stringArray, sizeof(stringArray)/sizeof(char*), "myString", true);
		storer.Store(&strAdapter);
	}
	storer.dumpOn(std::cout);
	std::ofstream f("testxml.xml");
	storer.dumpOn(f);
	{
		XMLStorage storer("MyPrueba");
		storer._restoreFrom("testxml.xml");
		{
			int intArray[] = {0,0,0,0,0,0};
			double doubleArray[] = {6.66,6.66,6.66,6.66,6.66,6.66};
			char charArray[] = {'=','=','=','=','=','=','=','='};
			std::string stringArray[] = {"Empty1", "Empty2", "Empty3"};
			std::cout << "Reading content" << std::endl;
			XMLArrayAdapter<int> intAdapter(intArray, sizeof(intArray)/sizeof(int));
			storer.Load(&intAdapter);
			XMLArrayAdapter<double> doubleAdapter(doubleArray, sizeof(doubleArray)/sizeof(double));
			storer.Load(&doubleAdapter);
			XMLArrayAdapter<char> charAdapter(charArray, sizeof(charArray)/sizeof(char));
			storer.Load(&charAdapter);
			XMLArrayAdapter<std::string> strAdapter(stringArray, sizeof(stringArray)/sizeof(std::string));
			storer.Load(&strAdapter);

			unsigned int i;
			for ( i=0; i<sizeof(intArray)/sizeof(int); i++) std::cout << intArray[i] << " "; std::cout << std::endl;
			for ( i=0; i<sizeof(doubleArray)/sizeof(double); i++) std::cout << doubleArray[i] << " "; std::cout << std::endl;
			for ( i=0; i<sizeof(charArray)/sizeof(char); i++) std::cout << charArray[i] << " "; std::cout << std::endl;
			for ( i=0; i<sizeof(stringArray)/sizeof(std::string); i++) std::cout << stringArray[i] << " "; std::cout << std::endl;
		}
		{ 
			int intArray[] = {0,0,0,0,0,0};
			double doubleArray[] = {6.66,6.66,6.66,6.66,6.66,6.66};
			char charArray[] = {'=','=','=','=','=','=','=','='};
			std::string stringArray[] = {"Empty1", "Empty2", "Empty3"};
			std::cout << "Reading attributes" << std::endl;
			XMLArrayAdapter<int> intAdapter(intArray, sizeof(intArray)/sizeof(int), "myInt");
			storer.Load(&intAdapter);
			XMLArrayAdapter<double> doubleAdapter(doubleArray, sizeof(doubleArray)/sizeof(double), "myDouble");
			storer.Load(&doubleAdapter);
			XMLArrayAdapter<char> charAdapter(charArray, sizeof(charArray)/sizeof(char), "myChar");
			storer.Load(&charAdapter);
			XMLArrayAdapter<std::string> strAdapter(stringArray, sizeof(stringArray)/sizeof(std::string), "myString");
			storer.Load(&strAdapter);
			unsigned int i;
			for ( i=0; i<sizeof(intArray)/sizeof(int); i++) std::cout << intArray[i] << " "; std::cout << std::endl;
			for ( i=0; i<sizeof(doubleArray)/sizeof(double); i++) std::cout << doubleArray[i] << " "; std::cout << std::endl;
			for ( i=0; i<sizeof(charArray)/sizeof(char); i++) std::cout << charArray[i] << " "; std::cout << std::endl;
			for ( i=0; i<sizeof(stringArray)/sizeof(std::string); i++) std::cout << stringArray[i] << " "; std::cout << std::endl;
		}
		{ 
			int intArray[] = {0,0,0,0,0,0};
			double doubleArray[] = {6.66,6.66,6.66,6.66,6.66,6.66};
			char charArray[] = {'=','=','=','=','=','=','=','='};
			std::string stringArray[] = {"Empty1", "Empty2", "Empty3"};
			std::cout << "Reading elements" << std::endl;
			XMLArrayAdapter<int> intAdapter(intArray, sizeof(intArray)/sizeof(int), "myInt", true);
			storer.Load(&intAdapter);
			XMLArrayAdapter<double> doubleAdapter(doubleArray, sizeof(doubleArray)/sizeof(double), "myDouble", true);
			storer.Load(&doubleAdapter);
			XMLArrayAdapter<char> charAdapter(charArray, sizeof(charArray)/sizeof(char), "myChar", true);
			storer.Load(&charAdapter);
			XMLArrayAdapter<std::string> strAdapter(stringArray, sizeof(stringArray)/sizeof(std::string), "myString", true);
			storer.Load(&strAdapter);
			unsigned int i;
			for ( i=0; i<sizeof(intArray)/sizeof(int); i++) std::cout << intArray[i] << " "; std::cout << std::endl;
			for ( i=0; i<sizeof(doubleArray)/sizeof(double); i++) std::cout << doubleArray[i] << " "; std::cout << std::endl;
			for ( i=0; i<sizeof(charArray)/sizeof(char); i++) std::cout << charArray[i] << " "; std::cout << std::endl;
			for ( i=0; i<sizeof(stringArray)/sizeof(std::string); i++) std::cout << stringArray[i] << " "; std::cout << std::endl;
		}
	}
}

/**
 * Tests the deprecated XMLStaticAdapter class
 */
void XMLStaticAdapterClassTest() {
	std::cout << "-- Testing XMLStaticAdapter" << std::endl;
	int i = 3;
	double d = 3.5;
	char c = 'a';
	char * s = "Hola";
	XMLStorage storer("MyPrueba");
	{ 
		XMLStaticAdapter intAdapter(i);
		storer.Store(&intAdapter);
		XMLStaticAdapter doubleAdapter(d);
		storer.Store(&doubleAdapter);
		XMLStaticAdapter charAdapter(c);
		storer.Store(&charAdapter);
		XMLStaticAdapter strAdapter(s);
		storer.Store(&strAdapter);
	}
	{ 
		XMLStaticAdapter intAdapter(i, "myInt");
		storer.Store(&intAdapter);
		XMLStaticAdapter doubleAdapter(d, "myDouble");
		storer.Store(&doubleAdapter);
		XMLStaticAdapter charAdapter(c, "myChar");
		storer.Store(&charAdapter);
		XMLStaticAdapter strAdapter(s, "myString");
		storer.Store(&strAdapter);
	}
	{ 
		XMLStaticAdapter intAdapter(i, "myInt", true);
		storer.Store(&intAdapter);
		XMLStaticAdapter doubleAdapter(d, "myDouble", true);
		storer.Store(&doubleAdapter);
		XMLStaticAdapter charAdapter(c, "myChar", true);
		storer.Store(&charAdapter);
		XMLStaticAdapter strAdapter(s, "myString", true);
		storer.Store(&strAdapter);
	}
	storer.dumpOn(std::cout);
}

/**
 * Tests the XMLIterableAdapter class
 */
void XMLIterableAdapterClassTest() {
	std::cout << "-- Testing XMLIterableAdapter" << std::endl;
	int intArray[] = {2,359,4,5,32,432};
	double doubleArray[] = {2.67,359.67,46574.56,565,32.5,432};
	char charArray[] = {'e','#','<','>','"','b','c','&'};
	char *stringArray[] = {"Hi,you", "HowAreYou?", "VeryWell,thanks,&you?"};
	const unsigned intArraySize = sizeof(intArray)/sizeof(int);
	const unsigned doubleArraySize = sizeof(doubleArray)/sizeof(double);
	const unsigned charArraySize = sizeof(charArray)/sizeof(char);
	const unsigned stringArraySize = sizeof(stringArray)/sizeof(char*);
	std::vector<int> intVector(intArray, intArray+intArraySize);
	std::vector<double> doubleVector(doubleArray, doubleArray+doubleArraySize);
	std::vector<char> charVector(charArray, charArray+charArraySize);
	std::vector<char*> stringVector(stringArray, stringArray+stringArraySize);
	XMLStorage storer("MyPrueba");

	{
		XMLIterableAdapter<std::vector<int> > intAdapter(intVector, "elemi");
		storer.Store(&intAdapter);
		XMLIterableAdapter<std::vector<double> > doubleAdapter(doubleVector, "elemd");
		storer.Store(&doubleAdapter);
		XMLIterableAdapter<std::vector<char> > charAdapter(charVector, "elemc");
		storer.Store(&charAdapter);
		XMLIterableAdapter<std::vector<char*> > strAdapter(stringVector, "elems");
		storer.Store(&strAdapter);
	}
	{ 
		XMLIterableAdapter<std::vector<int> > intAdapter(intVector, "elemi", "myInt");
		storer.Store(&intAdapter);
		XMLIterableAdapter<std::vector<double> > doubleAdapter(doubleVector, "elemd", "myDouble");
		storer.Store(&doubleAdapter);
		XMLIterableAdapter<std::vector<char> > charAdapter(charVector, "elemc", "myChar");
		storer.Store(&charAdapter);
		XMLIterableAdapter<std::vector<char*> > strAdapter(stringVector, "elems", "myString");
		storer.Store(&strAdapter);
	}
	{ 
		XMLIterableAdapter<std::vector<int> > intAdapter(intVector, "elemi", "myInt", true);
		storer.Store(&intAdapter);
		XMLIterableAdapter<std::vector<double> > doubleAdapter(doubleVector, "elemd", "myDouble", true);
		storer.Store(&doubleAdapter);
		XMLIterableAdapter<std::vector<char> > charAdapter(charVector, "elemc", "myChar", true);
		storer.Store(&charAdapter);
		XMLIterableAdapter<std::vector<char*> > strAdapter(stringVector, "elems", "myString", true);
		storer.Store(&strAdapter);
	}
	storer.dumpOn(std::cout);
	std::ofstream f("testxml.xml");
	storer.dumpOn(f);
/*
	{
		XMLStorage storer("MyPrueba");
		storer._restoreFrom("testxml.xml");
		{
			int intArray[] = {0,0,0,0,0,0};
			double doubleArray[] = {6.66,6.66,6.66,6.66,6.66,6.66};
			char charArray[] = {'=','=','=','=','=','=','=','='};
			std::string stringArray[] = {"Empty1", "Empty2", "Empty3"};
			std::cout << "Reading content" << std::endl;
			XMLIterableAdapter<std::vector<int> > intAdapter(intArray, "elemi");
			storer.Load(&intAdapter);
			XMLIterableAdapter<std::vector<double> > doubleAdapter(doubleArray, "elemd");
			storer.Load(&doubleAdapter);
			XMLIterableAdapter<std::vector<char> > charAdapter(charArray, "elemc");
			storer.Load(&charAdapter);
			XMLIterableAdapter<std::vector<std::string> > strAdapter(stringArray, "elems");
			storer.Load(&strAdapter);
			for (unsigned int i=0; i<sizeof(intArray)/sizeof(int); i++) std::cout << intArray[i] << " "; std::cout << std::endl;
			for (unsigned int i=0; i<sizeof(doubleArray)/sizeof(double); i++) std::cout << doubleArray[i] << " "; std::cout << std::endl;
			for (unsigned int i=0; i<sizeof(charArray)/sizeof(char); i++) std::cout << charArray[i] << " "; std::cout << std::endl;
			for (unsigned int i=0; i<sizeof(stringArray)/sizeof(std::string); i++) std::cout << stringArray[i] << " "; std::cout << std::endl;
		}
		{ 
			int intArray[] = {0,0,0,0,0,0};
			double doubleArray[] = {6.66,6.66,6.66,6.66,6.66,6.66};
			char charArray[] = {'=','=','=','=','=','=','=','='};
			std::string stringArray[] = {"Empty1", "Empty2", "Empty3"};
			std::cout << "Reading attributes" << std::endl;
			XMLIterableAdapter<std::vector<int> > intAdapter(intArray, "elemi", "myInt");
			storer.Load(&intAdapter);
			XMLIterableAdapter<std::vector<double> > doubleAdapter(doubleArray, "elemd", "myDouble");
			storer.Load(&doubleAdapter);
			XMLIterableAdapter<std::vector<char> > charAdapter(charArray, "elemc", "myChar");
			storer.Load(&charAdapter);
			XMLIterableAdapter<std::vector<std::string> > strAdapter(stringArray, "elems", "myString");
			storer.Load(&strAdapter);
			for (unsigned int i=0; i<sizeof(intArray)/sizeof(int); i++) std::cout << intArray[i] << " "; std::cout << std::endl;
			for (unsigned int i=0; i<sizeof(doubleArray)/sizeof(double); i++) std::cout << doubleArray[i] << " "; std::cout << std::endl;
			for (unsigned int i=0; i<sizeof(charArray)/sizeof(char); i++) std::cout << charArray[i] << " "; std::cout << std::endl;
			for (unsigned int i=0; i<sizeof(stringArray)/sizeof(std::string); i++) std::cout << stringArray[i] << " "; std::cout << std::endl;
		}
		{ 
			int intArray[] = {0,0,0,0,0,0};
			double doubleArray[] = {6.66,6.66,6.66,6.66,6.66,6.66};
			char charArray[] = {'=','=','=','=','=','=','=','='};
			std::string stringArray[] = {"Empty1", "Empty2", "Empty3"};
			std::cout << "Reading elements" << std::endl;
			XMLIterableAdapter<std::vector<int> > intAdapter(intArray, "elemi", "myInt", true);
			storer.Load(&intAdapter);
			XMLIterableAdapter<std::vector<double> > doubleAdapter(doubleArray, "elemd", "myDouble", true);
			storer.Load(&doubleAdapter);
			XMLIterableAdapter<std::vector<char> > charAdapter(charArray, "elemc", "myChar", true);
			storer.Load(&charAdapter);
			XMLIterableAdapter<std::vector<std::string> > strAdapter(stringArray, "elems", "myString", true);
			storer.Load(&strAdapter);
			for (unsigned int i=0; i<sizeof(intArray)/sizeof(int); i++) std::cout << intArray[i] << " "; std::cout << std::endl;
			for (unsigned int i=0; i<sizeof(doubleArray)/sizeof(double); i++) std::cout << doubleArray[i] << " "; std::cout << std::endl;
			for (unsigned int i=0; i<sizeof(charArray)/sizeof(char); i++) std::cout << charArray[i] << " "; std::cout << std::endl;
			for (unsigned int i=0; i<sizeof(stringArray)/sizeof(std::string); i++) std::cout << stringArray[i] << " "; std::cout << std::endl;
		}
	}
	*/
}

/** 
 * A Component class having three basic attributes
 * that are stored each as element, attribute and plain.
 */
class XMLComponentAdapterTester : public Component {
// Construction/Destruction
public:
	XMLComponentAdapterTester(std::string c) {
		plain="simpleContent-"+c;
		attribute="attributeContent-"+c;
		element="elementContent-"+c;
	}
	virtual ~XMLComponentAdapterTester() {
		
	};
// Operations
public:
	void modify () {
		plain+="Modified";
		attribute+="Modified";
		element+="Modified";
	}
	bool operator== (XMLComponentAdapterTester&c) {
		return c.plain==plain && c.attribute==attribute && c.element==element;
	}
	bool operator!= (XMLComponentAdapterTester&c) {
		return c.plain!=plain || c.attribute!=attribute || c.element!=element;
	}
	void print() {
		std::cout << plain << '\n' << attribute << '\n' << element << std::endl;
	}
// Operations (Component interface)
public:
	virtual void StoreOn (Storage & store) {
		XMLAdapter<std::string> myAdapter1(plain);
		store.Store(&myAdapter1);
		XMLAdapter<std::string> myAdapter2(attribute, "mySubItem");
		store.Store(&myAdapter2);
		XMLAdapter<std::string> myAdapter3(element, "mySubItem", true);
		store.Store(&myAdapter3);
	}
	virtual void LoadFrom (Storage & store) {
		std::cout << "Loading Component tester"<<std::endl;
		XMLAdapter<std::string> myAdapter1(plain);
		store.Load(&myAdapter1);
		XMLAdapter<std::string> myAdapter2(attribute, "mySubItem");
		store.Load(&myAdapter2);
		XMLAdapter<std::string> myAdapter3(element, "mySubItem", true);
		store.Load(&myAdapter3);
	}
// Attributes
private:
	std::string plain;
	std::string attribute;
	std::string element;
};


/**
 * Tests the XMLComponentAdapter class
 */
void XMLComponentAdapterClassTest() {
	std::cout << "-- Testing XMLComponentAdapter" << std::endl;
	CLAMTest::XMLComponentAdapterTester myAdapteeC("C");
	CLAMTest::XMLComponentAdapterTester myAdapteeA("A");
	CLAMTest::XMLComponentAdapterTester myAdapteeE("E");
	XMLStorage storer("MyPrueba");
	{
		// Testing as a simple content
		XMLComponentAdapter componentAdapter(myAdapteeC);
		storer.Store(&componentAdapter);
	}
	{
		// Testing as an element content
		XMLComponentAdapter componentAdapter(myAdapteeE, "myComponentAsElem", true);
		storer.Store(&componentAdapter);
	}
	storer.dumpOn(std::cout);
	std::ofstream f("testxml.xml");
	storer.dumpOn(f);
	{
		XMLStorage storer("MyPrueba");
		storer._restoreFrom("testxml.xml");
		CLAMTest::XMLComponentAdapterTester myLoadedAdaptee("T");
		{
			// Testing as a simple content
			XMLComponentAdapter componentAdapter(myLoadedAdaptee);
			storer.Load(&componentAdapter);
			if (myLoadedAdaptee!=myAdapteeC) {
				std::cerr << "Difference" << std::endl;
				myLoadedAdaptee.print();
				myAdapteeC.print();
			}
		}
		{
			// Testing as an element content
			XMLComponentAdapter componentAdapter(myLoadedAdaptee, "myComponentAsElem", true);
			storer.Load(&componentAdapter);
			if (myLoadedAdaptee!=myAdapteeE) {
				std::cerr << "Difference" << std::endl;
				myLoadedAdaptee.print();
				myAdapteeE.print();
			}
		}
	}
};




/**
 * A Component having only one parameter
 */
class MyComponent : public CLAM::Component {
	public:
		virtual ~MyComponent() {};
		void StoreOn(CLAM::Storage & s) {
			std::string text("Content");
			#ifdef CLAM_USE_XML
			CLAM::XMLAdapter<std::string> adapt(text);
			s.Store(&adapt);
			#endif//CLAM_USE_XML
		}
		void LoadFrom(CLAM::Storage & s) {
			#ifdef CLAM_USE_XML
			std::string text;
			CLAM::XMLAdapter<std::string> adapt(text);
			s.Load(&adapt);
			CLAM_ASSERT(text=="Content","Loading do not match");
			#endif//CLAM_USE_XML
		}
};



}

int main (void) {
	#ifdef CLAM_USE_XML
	CLAMTest::XMLAdapterClassTest();
	CLAMTest::XMLStaticAdapterClassTest();
	CLAMTest::XMLArrayAdapterClassTest();
	CLAMTest::XMLComponentAdapterClassTest();
	CLAMTest::XMLIterableAdapterClassTest();
	#endif//CLAM_USE_XML
	CLAMTest::MyComponent c;
	CLAM::Array<CLAMTest::MyComponent> a;
	a.AddElem(c);
	a.AddElem(c);
	a.AddElem(c);
	a.AddElem(c);
	#ifdef CLAM_USE_XML
	CLAM::XMLStorage storage("Prueba");
	storage.Dump(a,"UnArray",std::cout);
	#endif//CLAM_USE_XML
	// TODO: Test Array loading

	return 0;

}
