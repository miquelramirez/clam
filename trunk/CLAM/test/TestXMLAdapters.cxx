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
#include "XMLTestHelper.hxx"

#include <iostream>
#include <fstream>
#include <vector>

using namespace CLAM;

namespace CLAMTest {

/**
 * This a component which simulates containing some XML adapters TestCases.
 * It check the effectiveness of the Store/Load pair for the XML adapter.
 * It will store default constructed TestCases, and then will load
 * over a non-default constructed TestCases that can be compared
 * to the default one.
 */
template <class TestCase>
class GenericAdaptersTester : public Component {
	std::string mId;
public:
	GenericAdaptersTester() {
		mId=TestCase::kind();
	}
	void StoreOn(Storage & storer) {
		TestCase a, b, c;
		a.AdaptToStore(storer,true,false); // Content
		b.AdaptToStore(storer,false,false); // Attribute
		c.AdaptToStore(storer,false,true); // Element
	}
	void LoadFrom(Storage & storer) 
	{
		// For each insertion mode (attribute-element-content)
		{
			TestCase a(false), b;
			a.AdaptToLoad(storer,true,false); // Content
			std::string context("Loading Content "+mId);
			bool failed = a.DiferenceCause(b,context);
			CLAM_ASSERT(!failed, context.c_str());
		}
		{
			TestCase a(false), b;
			a.AdaptToLoad(storer,false,false); // Attribute
			std::string context("Loading Attribute "+mId);
			bool failed = a.DiferenceCause(b,context);
			CLAM_ASSERT(!failed, context.c_str());
		}
		{
			TestCase a(false), b;
			a.AdaptToLoad(storer,false,true); // Element
			std::string context("Loading Element "+mId);
			bool failed = a.DiferenceCause(b,context);
			CLAM_ASSERT(!failed, context.c_str());
		}
	}
};

class SimpleAdapterTestCase {
public:
	int i;
	double d;
	char c;
	std::string s;
	static char * kind() {return "Simple Adapter";}
	SimpleAdapterTestCase() {
		i = 3;
		d = 3.5;
		c = 'a';
		s = "Hola";
	}
	SimpleAdapterTestCase(bool b) {
		i = 6;
		d = 6.5;
		c = 'b';
		s = "Adios";
	}
	bool DiferenceCause(const SimpleAdapterTestCase & other, std::string &context) const {
		if (i!=other.i) {
			context+=": Int value mismatch";
			return true;
		}
		if (d!=other.d) {
			context+=": Double value mismatch";
			return true;
		}
		if (c!=other.c) {
			context+=": Char value mismatch";
			return true;
		}
		if (std::string(s)!=std::string(other.s)) {
			context+=": String value mismatch";
			return true;
		}
		return false;
	}
	void AdaptToStore(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		XMLAdapter<int> intAdapter(i, asContent?0:"myInt", asElement);
		storer.Store(&intAdapter);
		XMLAdapter<double> doubleAdapter(d, asContent?0:"myDouble", asElement);
		storer.Store(&doubleAdapter);
		XMLAdapter<char> charAdapter(c, asContent?0:"myChar", asElement);
		storer.Store(&charAdapter);
		XMLAdapter<std::string> strAdapter(s, asContent?0:"myString", asElement);
		storer.Store(&strAdapter);
	}
	void AdaptToLoad(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		XMLAdapter<int> intAdapter(i, asContent?0:"myInt", asElement);
		storer.Load(&intAdapter);
		XMLAdapter<double> doubleAdapter(d, asContent?0:"myDouble", asElement);
		storer.Load(&doubleAdapter);
		XMLAdapter<char> charAdapter(c, asContent?0:"myChar", asElement);
		storer.Load(&charAdapter);
		XMLAdapter<std::string> strAdapter(s, asContent?0:"myString", asElement);
		storer.Load(&strAdapter);
	}
};
	
class ArrayAdapterTestCase {
protected:
	int *iarray;
	double *darray;
	char *carray;
	std::string *sarray;
	unsigned isize, dsize, csize, ssize;
public:
	static char * kind() {return "Array of Simple type Adapter";}
	template <class T> 
	void Fill(T*& target, unsigned & targetsize, T* source, unsigned size) {
		target=new T[size];
		targetsize = size;
		for (unsigned j = 0; j<size; j++) {
			target[j]=source[j];
		}
	}

	ArrayAdapterTestCase() 
	{
		int iArray[] = {2,359,4,5,32,432};
		double dArray[] = {2.67,359.67,46574.56,565,32.5,432};
		char cArray[] = {'e','#','<','>','"','b','c','&'};
		std::string sArray[] = {"Hi,you", "HowAreYou?", "VeryWell,thanks,&you?"};
		Fill(iarray,isize,iArray,sizeof(iArray)/sizeof(int));
		Fill(darray,dsize,dArray,sizeof(dArray)/sizeof(double));
		Fill(carray,csize,cArray,sizeof(cArray)/sizeof(char));
		Fill(sarray,ssize,sArray,sizeof(sArray)/sizeof(std::string));
	}
	ArrayAdapterTestCase(bool b) {
		int iArray[] = {0,0,0,0,0,0};
		double dArray[] = {6.66,6.66,6.66,6.66,6.66,6.66};
		char cArray[] = {'=','=','=','=','=','=','=','='};
		std::string sArray[] = {"Empty1", "Empty2", "Empty3"};
		Fill(iarray,isize,iArray,sizeof(iArray)/sizeof(int));
		Fill(darray,dsize,dArray,sizeof(dArray)/sizeof(double));
		Fill(carray,csize,cArray,sizeof(cArray)/sizeof(char));
		Fill(sarray,ssize,sArray,sizeof(sArray)/sizeof(std::string));
	}
	~ArrayAdapterTestCase() {
		delete[] iarray;
		delete[] darray;
		delete[] carray;
		delete[] sarray;
	}
	bool DiferenceCause(const ArrayAdapterTestCase & other, std::string &context) const {
		for (unsigned j=0; j<isize; j++) {
			if (iarray[j]!=other.iarray[j]) {
				std::cout << "Found " << iarray[j] << " expected " << other.iarray[j] << " pos: " << j << std::endl;
				context+=": Int value mismatch";
				return true;
			}
		}
		for (unsigned j=0; j<dsize; j++) {
			// TODO: To do this check better
			double difference=darray[j]-other.darray[j];
			difference*=difference;
			if (difference>0.1) {
				std::cout << "Found " << darray[j] << " expected " << other.darray[j] << " pos: " << j << std::endl;
				context+=": Double value mismatch";
				return true;
			}
		}
		for (unsigned j=0; j<csize; j++)
			if (carray[j]!=other.carray[j]) {
				std::cout << "Found " << carray[j] << " expected " << other.carray[j] << " pos: " << j << std::endl;
				context+=": Char value mismatch";
				return true;
			}
		for (unsigned j=0; j<ssize; j++) {
			if (sarray[j]!=other.sarray[j]) {
				std::cout << "Found " << sarray[j] << " expected " << other.sarray[j] << " pos: " << j << std::endl;
				context+=": String value mismatch";
				return true;
			}
		}
		return false;
	}
	void AdaptToStore(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		XMLArrayAdapter<int> intAdapter(iarray, isize, asContent?0:"myInt", asElement);
		storer.Store(&intAdapter);
		XMLArrayAdapter<double> doubleAdapter(darray, dsize, asContent?0:"myDouble", asElement);
		storer.Store(&doubleAdapter);
		XMLArrayAdapter<char> charAdapter(carray, csize, asContent?0:"myChar", asElement);
		storer.Store(&charAdapter);
		XMLArrayAdapter<std::string> strAdapter(sarray, ssize, asContent?0:"myString", asElement);
		storer.Store(&strAdapter);
	}
	void AdaptToLoad(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		XMLArrayAdapter<int> intAdapter(iarray, isize, asContent?0:"myInt", asElement);
		storer.Load(&intAdapter);
		XMLArrayAdapter<double> doubleAdapter(darray, dsize, asContent?0:"myDouble", asElement);
		storer.Load(&doubleAdapter);
		XMLArrayAdapter<char> charAdapter(carray, csize, asContent?0:"myChar", asElement);
		storer.Load(&charAdapter);
		XMLArrayAdapter<std::string> strAdapter(sarray, ssize, asContent?0:"myString", asElement);
		storer.Load(&strAdapter);
	}
};

/**
 * A Component that is a test case for testing XMLStaticAdapters of
 * some bassic types.
 * This one is much like the SimpleAdapterTestCase but the
 * Store procedure use static adapters.
 * @attention Static adapters cannot be uses for loading so we use 
 * the non static adapters for this purpose.
 */
class StaticAdapterTestCase : public SimpleAdapterTestCase {
public:
	StaticAdapterTestCase() : SimpleAdapterTestCase() {}
	StaticAdapterTestCase(bool b) : SimpleAdapterTestCase(b) {}
	static char * kind() {return "Simple type Static Adapter";}
	void AdaptToStore(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		XMLStaticAdapter intAdapter(i, asContent?0:"myInt", asElement);
		storer.Store(&intAdapter);
		XMLStaticAdapter doubleAdapter(d, asContent?0:"myDouble", asElement);
		storer.Store(&doubleAdapter);
		XMLStaticAdapter charAdapter(c, asContent?0:"myChar", asElement);
		storer.Store(&charAdapter);
		XMLStaticAdapter strAdapter(s, asContent?0:"myString", asElement);
		storer.Store(&strAdapter);
	}
};

/**
 * A Component that is a test case for testing IterableAdapters of
 * some bassic types.
 * This one is much like the SimpleAdapterTestCase but the
 * Store procedure use static adapters.
 */
class IterableAdapterTestCase : public ArrayAdapterTestCase {
	std::vector<int> iv;
	std::vector<double> dv;
	std::vector<char> cv;
	std::vector<std::string> sv;
public:
	IterableAdapterTestCase() : ArrayAdapterTestCase() {
		CopyCArrays();
	}
	IterableAdapterTestCase(bool b) : ArrayAdapterTestCase(b) {
		CopyCArrays();
	}
	void CopyCArrays() {
		iv.assign(iarray, iarray+isize);
		dv.assign(darray, darray+dsize);
		cv.assign(carray, carray+csize);
		sv.assign(sarray, sarray+ssize);
	}
	static char * kind() {return "XMLIterableAdapter";}
	void AdaptToStore(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		XMLStaticAdapter separator("", "Separator", true);
		XMLIterableAdapter<std::vector<int> > intAdapter(iv, "elemi", asContent?0:"myInt", asElement);
		storer.Store(&intAdapter);
		if (asContent) storer.Store(&separator);
		XMLIterableAdapter<std::vector<double> > doubleAdapter(dv, "elemd", asContent?0:"myDouble", asElement);
		storer.Store(&doubleAdapter);
		if (asContent) storer.Store(&separator);
		XMLIterableAdapter<std::vector<char> > charAdapter(cv, "elemc", asContent?0:"myChar", asElement);
		storer.Store(&charAdapter);
		if (asContent) storer.Store(&separator);
		XMLIterableAdapter<std::vector<std::string> > strAdapter(sv, "elems",  asContent?0:"myString", asElement);
		storer.Store(&strAdapter);
	}
	void AdaptToLoad(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToLoad");
		std::string dummy;
		XMLAdapter<std::string> separator(dummy, "Separator", true);
		XMLIterableAdapter<std::vector<int> > intAdapter(iv, "elemi", asContent?0:"myInt", asElement);
		storer.Load(&intAdapter);
		if (asContent) storer.Load(&separator);
		XMLIterableAdapter<std::vector<double> > doubleAdapter(dv, "elemd", asContent?0:"myDouble", asElement);
		storer.Load(&doubleAdapter);
		if (asContent) storer.Load(&separator);
		XMLIterableAdapter<std::vector<char> > charAdapter(cv, "elemc", asContent?0:"myChar", asElement);
		storer.Load(&charAdapter);
		if (asContent) storer.Load(&separator);
		XMLIterableAdapter<std::vector<std::string> > strAdapter(sv, "elems",  asContent?0:"myString", asElement);
		storer.Load(&strAdapter);
	}
	bool DiferenceCause(const IterableAdapterTestCase & other, std::string &context) const {
		for (unsigned j=0; j<isize; j++) {
			if (iv[j]!=other.iv[j]) {
				std::cout << "Found " << iv[j] << " expected " << other.iv[j] << std::endl;
				context+=": Int value mismatch";
				return true;
			}
		}
		for (unsigned j=0; j<dsize; j++) {
			// TODO: To do this check better
			double difference=dv[j]-other.dv[j];
			difference*=difference;
			if (difference>0.1) {
				std::cout << "Found " << dv[j] << " expected " << other.dv[j] << " dif " << difference << std::endl;
				context+=": Double value mismatch";
				return true;
			}
		}
		for (unsigned j=0; j<csize; j++)
			if (cv[j]!=other.cv[j]) {
				std::cout << "Found " << cv[j] << " expected " << other.cv[j] << " pos: " << j << std::endl;
				context+=": Char value mismatch";
				return true;
			}
		for (unsigned j=0; j<ssize; j++) {
			if (sv[j]!=other.sv[j]) {
				std::cout << "Found " << sv[j] << " expected " << other.sv[j] << " pos: " << j << std::endl;
				context+=": String value mismatch";
				return true;
			}
		}
		return false;
	}
};


/**
 * Tests the XMLAdapter class
 */
void XMLAdapterClassTest() {
	std::cout << "-- Testing XMLAdapter" << std::endl;
	{
		GenericAdaptersTester<SimpleAdapterTestCase> tester;
		bool match = XMLInputOutputMatches(tester,__FILE__"Simple.xml");
		CLAM_ASSERT(match, "Store/Load mismatch using basic types adapters");
	}
}

/**
 * Tests the XMLArrayAdapter class
 */
void XMLArrayAdapterClassTest() {
	std::cout << "-- Testing XMLArrayAdapter" << std::endl;
	{
		GenericAdaptersTester<ArrayAdapterTestCase> tester;
		bool match = XMLInputOutputMatches(tester,__FILE__"Array.xml");
		CLAM_ASSERT(match, "Store/Load mismatch using basic type arrays adapters");
	}
}

/**
 * Tests the deprecated XMLStaticAdapter class
 */
void XMLStaticAdapterClassTest() {
	std::cout << "-- Testing XMLStaticAdapter" << std::endl;
	{
		GenericAdaptersTester<StaticAdapterTestCase> tester;
		bool match = XMLInputOutputMatches(tester,__FILE__"Array.xml");
		CLAM_ASSERT(match, "Store/Load mismatch using basic type static adapters");
	}
}

/**
 * Tests the XMLIterableAdapter class
 */
void XMLIterableAdapterClassTest() {
	std::cout << "-- Testing XMLIterableAdapter" << std::endl;
	{
		GenericAdaptersTester<IterableAdapterTestCase> tester;
		bool match = XMLInputOutputMatches(tester,__FILE__"Array.xml");
		CLAM_ASSERT(match, "Store/Load mismatch using basic type iterable adapters");
	}
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
		virtual const char * GetClassName() {
			return "MyComponent";
		}
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
