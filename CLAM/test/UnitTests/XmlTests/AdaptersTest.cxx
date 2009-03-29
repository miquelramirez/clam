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

/**
 * @file
 * This file contains tests that checks that the XMLAdapters do their work,
 * that is to adapt objects to something a XMLStorage can manage for Loading
 * and Storing.
 * @todo Refactor code duplication
 * @todo Array of basic objects XML test
 * @todo List of basic objects XML test
 */

#include "XMLAdapter.hxx" // CLAM
#include "XMLArrayAdapter.hxx" // CLAM
#include "XMLComponentAdapter.hxx" // CLAM
#include "XMLIterableAdapter.hxx" // CLAM
#include "XMLStorage.hxx" // CLAM
#include "Array.hxx" // CLAM
#include "List.hxx" // CLAM
#include "XMLTestHelper.hxx"

#include <cppunit/extensions/HelperMacros.h>
#include <string>

#include <sstream>
//#include <fstream>
#include <vector>

using namespace CLAM;

namespace CLAMTest {


	class XmlAdaptersTest;
	CPPUNIT_TEST_SUITE_REGISTRATION( XmlAdaptersTest );



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
	void StoreOn(Storage & storer) const {
		TestCase a(true), b(true), c(true);
		a.AdaptToStore(storer,true,false); // Content
		b.AdaptToStore(storer,false,false); // Attribute
		c.AdaptToStore(storer,false,true); // Element
	}
	void LoadFrom(Storage & storer)
	{
		// For each insertion mode (attribute-element-content)
		{
			TestCase a, b(true);
			a.AdaptToLoad(storer,true,false); // Content
			std::string context("Loading Content "+mId);
			bool failed = a.DiferenceCause(b,context);
			CLAM_ASSERT(!failed, context.c_str());
		}
		{
			TestCase a, b(true);
			a.AdaptToLoad(storer,false,false); // Attribute
			std::string context("Loading Attribute "+mId);
			bool failed = a.DiferenceCause(b,context);
			CLAM_ASSERT(!failed, context.c_str());
		}
		{
			TestCase a, b(true);
			a.AdaptToLoad(storer,false,true); // Element
			std::string context("Loading Element "+mId);
			bool failed = a.DiferenceCause(b,context);
			CLAM_ASSERT(!failed, context.c_str());
		}
	}
	const char * GetClassName() const {
		return "GenericAdaptersTester";
	}
};

class SimpleAdapterTestCase {
public:
	int i;
	double d;
	char c;
	std::string s;
	static const char * kind() {return "Simple Adapter";}
	SimpleAdapterTestCase(bool b) {
		i = 3;
		d = 3.5;
		c = 'a';
		s = "Hola";
	}
	SimpleAdapterTestCase() {
		i = 6;
		d = 6.5;
		c = 'b';
		s = "Adios";
	}
	bool DiferenceCause(const SimpleAdapterTestCase & other, std::string &context) const {
		bool areDifferent = false;
		std::stringstream out;
		if (i!=other.i) {
			out 
				<< "Int value mismatch:" 
				<< " Found " << i
				<< " Expected " << other.i << std::endl;
			areDifferent = true;
		}
		if (d!=other.d) {
			out 
				<< "Double value mismatch:" 
				<< " Found " << d
				<< " Expected " << other.d << std::endl;
			areDifferent = true;
		}
		if (c!=other.c) {
			out 
				<< "Char value mismatch:" 
				<< " Found " << c
				<< " Expected " << other.c << std::endl;
			areDifferent = true;
		}
		if (std::string(s)!=std::string(other.s)) {
			out 
				<< "String value mismatch:" 
				<< " Found " << s
				<< " Expected " << other.s << std::endl;
			areDifferent = true;
		}
		context += out.str();
		return areDifferent;
	}
	void AdaptToStore(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		XMLAdapter<int> intAdapter(i, asContent?0:"myInt", asElement);
		storer.Store(intAdapter);
		XMLAdapter<double> doubleAdapter(d, asContent?0:"myDouble", asElement);
		storer.Store(doubleAdapter);
		XMLAdapter<char> charAdapter(c, asContent?0:"myChar", asElement);
		storer.Store(charAdapter);
		XMLAdapter<std::string> strAdapter(s, asContent?0:"myString", asElement);
		storer.Store(strAdapter);
	}
	void AdaptToLoad(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		XMLAdapter<int> intAdapter(i, asContent?0:"myInt", asElement);
		storer.Load(intAdapter);
		XMLAdapter<double> doubleAdapter(d, asContent?0:"myDouble", asElement);
		storer.Load(doubleAdapter);
		XMLAdapter<char> charAdapter(c, asContent?0:"myChar", asElement);
		storer.Load(charAdapter);
		XMLAdapter<std::string> strAdapter(s, asContent?0:"myString", asElement);
		storer.Load(strAdapter);
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
	static const char * kind() {return "Array of Simple type Adapter";}
	template <class T>
	void Fill(T*& target, unsigned & targetsize, T* source, unsigned size) {
		target=new T[size];
		targetsize = size;
		for (unsigned j = 0; j<size; j++) {
			target[j]=source[j];
		}
	}

	ArrayAdapterTestCase(bool b)
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
	ArrayAdapterTestCase() {
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
		bool areDifferent = false;
		std::stringstream out;
		for (unsigned j=0; j<isize; j++) 
		{
			if (iarray[j]==other.iarray[j]) continue;
			out 
				<< "Int value mismatch: pos: " << j 
				<< " Found " << iarray[j] 
				<< " expected " << other.iarray[j] << std::endl;
			areDifferent = true;
		}
		for (unsigned j=0; j<dsize; j++) 
		{
			// TODO: To do this check better
			double difference=darray[j]-other.darray[j];
			difference*=difference;
			if (difference<0.1) continue;
			out 
				<< "Double value mismatch: pos: " << j 
				<< " Found " << darray[j] 
				<< " expected " << other.darray[j] << std::endl;
			areDifferent = true;
		}
		for (unsigned j=0; j<csize; j++)
		{
			if (carray[j]==other.carray[j]) continue;
			out 
				<< "Char value mismatch: pos: " << j 
				<< " Found " << carray[j] 
				<< " expected " << other.carray[j] << std::endl;
			areDifferent = true;
		}
		for (unsigned j=0; j<ssize; j++)
		{
			if (sarray[j]==other.sarray[j])continue;
			out 
				<< "String value mismatch: pos: " << j 
				<< " Found " << sarray[j] 
				<< " expected " << other.sarray[j] << std::endl;
			areDifferent = true;
		}
		context+=out.str();
		return areDifferent;
	}
	void AdaptToStore(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		XMLArrayAdapter<int> intAdapter(iarray, isize, asContent?0:"myInt", asElement);
		storer.Store(intAdapter);
		XMLArrayAdapter<double> doubleAdapter(darray, dsize, asContent?0:"myDouble", asElement);
		storer.Store(doubleAdapter);
		XMLArrayAdapter<char> charAdapter(carray, csize, asContent?0:"myChar", asElement);
		storer.Store(charAdapter);
		XMLArrayAdapter<std::string> strAdapter(sarray, ssize, asContent?0:"myString", asElement);
		storer.Store(strAdapter);
	}
	void AdaptToLoad(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		XMLArrayAdapter<int> intAdapter(iarray, isize, asContent?0:"myInt", asElement);
		storer.Load(intAdapter);
		XMLArrayAdapter<double> doubleAdapter(darray, dsize, asContent?0:"myDouble", asElement);
		storer.Load(doubleAdapter);
		XMLArrayAdapter<char> charAdapter(carray, csize, asContent?0:"myChar", asElement);
		storer.Load(charAdapter);
		XMLArrayAdapter<std::string> strAdapter(sarray, ssize, asContent?0:"myString", asElement);
		storer.Load(strAdapter);
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
	IterableAdapterTestCase(bool b) : ArrayAdapterTestCase(b) {
		CopyCArrays();
	}
	IterableAdapterTestCase() : ArrayAdapterTestCase() {
		CopyCArrays();
	}
	void CopyCArrays() {
		iv.assign(iarray, iarray+isize);
		dv.assign(darray, darray+dsize);
		cv.assign(carray, carray+csize);
		sv.assign(sarray, sarray+ssize);
	}
	static const char * kind() {return "XMLIterableAdapter";}
	void AdaptToStore(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");
		std::string dummy;
		XMLAdapter<std::string> separator(dummy, "Separator", true);
		XMLIterableAdapter<std::vector<int> > intAdapter(iv, "elemi", asContent?0:"myInt", asElement);
		storer.Store(intAdapter);
		if (asContent) storer.Store(separator);
		XMLIterableAdapter<std::vector<double> > doubleAdapter(dv, "elemd", asContent?0:"myDouble", asElement);
		storer.Store(doubleAdapter);
		if (asContent) storer.Store(separator);
		XMLIterableAdapter<std::vector<char> > charAdapter(cv, "elemc", asContent?0:"myChar", asElement);
		storer.Store(charAdapter);
		if (asContent) storer.Store(separator);
		XMLIterableAdapter<std::vector<std::string> > strAdapter(sv, "elems",  asContent?0:"myString", asElement);
		storer.Store(strAdapter);
	}
	void AdaptToLoad(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToLoad");
		std::string dummy;
		XMLAdapter<std::string> separator(dummy, "Separator", true);
		XMLIterableAdapter<std::vector<int> > intAdapter(iv, "elemi", asContent?0:"myInt", asElement);
		storer.Load(intAdapter);
		if (asContent) storer.Load(separator);
		XMLIterableAdapter<std::vector<double> > doubleAdapter(dv, "elemd", asContent?0:"myDouble", asElement);
		storer.Load(doubleAdapter);
		if (asContent) storer.Load(separator);
		XMLIterableAdapter<std::vector<char> > charAdapter(cv, "elemc", asContent?0:"myChar", asElement);
		storer.Load(charAdapter);
		if (asContent) storer.Load(separator);
		XMLIterableAdapter<std::vector<std::string> > strAdapter(sv, "elems",  asContent?0:"myString", asElement);
		storer.Load(strAdapter);
	}
	bool DiferenceCause(const IterableAdapterTestCase & other, std::string &context) const {
		bool areDifferent = false;
		std::stringstream out;
		for (unsigned j=0; j<isize; j++) 
		{
			if (iv[j]==other.iv[j]) continue;
			out 
				<< "Int value mismatch: pos: " << j 
				<< " Found " << iv[j] 
				<< " expected " << other.iv[j] << std::endl;
			areDifferent = true;
		}
		for (unsigned j=0; j<dsize; j++) 
		{
			// TODO: To do this check better
			double difference=dv[j]-other.dv[j];
			difference*=difference;
			if (difference<0.1) continue;
			out 
				<< "Double value mismatch: pos: " << j 
				<< " Found " << dv[j] 
				<< " expected " << other.dv[j] << std::endl;
			areDifferent = true;
		}
		for (unsigned j=0; j<csize; j++)
		{
			if (cv[j]==other.cv[j]) continue;
			out 
				<< "Char value mismatch: pos: " << j 
				<< " Found " << cv[j] 
				<< " expected " << other.cv[j] << std::endl;
			areDifferent = true;
		}
		for (unsigned j=0; j<ssize; j++)
		{
			if (sv[j]==other.sv[j])continue;
			out 
				<< "String value mismatch: pos: " << j 
				<< " Found " << sv[j] 
				<< " expected " << other.sv[j] << std::endl;
			areDifferent = true;
		}
		context+=out.str();
		return areDifferent;
	}
};


/**
 * A Component class having three basic attributes
 * that are stored each as element, attribute and plain.
 */
class ComponentAdapterTestHelper : public Component {
// Construction/Destruction
public:
	ComponentAdapterTestHelper(char c) {
		plain=std::string("simpleContent-")+c;
		attribute=std::string("attributeContent-")+c;
		element=std::string("elementContent-")+c;
	}
	virtual ~ComponentAdapterTestHelper() {

	};
	const char * GetClassName() const {
		return "ComponentAdapterTestHelper";
	}
// Operations
public:
	void modify () {
		plain+="-Modified";
		attribute+="-Modified";
		element+="-Modified";
	}
	bool operator== (const ComponentAdapterTestHelper&c) const {
		return c.plain==plain && c.attribute==attribute && c.element==element;
	}
	void print(std::ostream & out) const {
		out << plain << '\n' << attribute << '\n' << element << std::endl;
	}
// Operations (Component interface)
public:
	virtual void StoreOn (Storage & store) const {
		XMLAdapter<std::string> myAdapter1(plain);
		store.Store(myAdapter1);
		XMLAdapter<std::string> myAdapter2(attribute, "mySubItem");
		store.Store(myAdapter2);
		XMLAdapter<std::string> myAdapter3(element, "mySubItem", true);
		store.Store(myAdapter3);
	}
	virtual void LoadFrom (Storage & store) {
		XMLAdapter<std::string> myAdapter1(plain);
		store.Load(myAdapter1);
		XMLAdapter<std::string> myAdapter2(attribute, "mySubItem");
		store.Load(myAdapter2);
		XMLAdapter<std::string> myAdapter3(element, "mySubItem", true);
		store.Load(myAdapter3);
	}
// Attributes
private:
	std::string plain;
	std::string attribute;
	std::string element;
};


class ComponentAdapterTestCase {
public:
	ComponentAdapterTestCase(char c) : mComponent(c) {
	}
	ComponentAdapterTestCase(char c, bool b) : mComponent(c) {
		mComponent.modify();
	}
	static const char * kind() {return "XMLComponentAdapter";}
	void AdaptToStore(Storage & storer, bool asContent, bool asElement) const {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToStore");

		CLAM_ASSERT(asContent || asElement,
			"Attributes not aplicable for XMLComponentAdapters");

		XMLComponentAdapter adapter(mComponent, asContent?0:"AsElement", asElement);
		storer.Store(adapter);
	}
	void AdaptToLoad(Storage & storer, bool asContent, bool asElement) {
		CLAM_ASSERT(!(asContent && asElement),
			"Testing logic calling in AdaptToLoad");
		CLAM_ASSERT(asContent || asElement,
			"Attributes not aplicable for XMLComponentAdapters");

		XMLComponentAdapter adapter(mComponent, asContent?0:"AsElement", asElement);
		storer.Load(adapter);
	}
	bool DiferenceCause(const ComponentAdapterTestCase & other, std::string &context) const {
		if (mComponent==other.mComponent) return false;
		std::ostringstream out;
		out << "Found: " << std::endl;
		mComponent.print(out);
		out << "Expected: " << std::endl;
		other.mComponent.print(out);
		context = out.str();
		return true;
	}
// Attributes
private:
	ComponentAdapterTestHelper mComponent;
};


class ComponentAdaptersTester : public Component {
public:
	ComponentAdaptersTester() {
		mId="ComponentAdapter";
	}
	const char * GetClassName() const {
		return "ComponentAdaptersTester";
	}
	void StoreOn(Storage & storer) const {
		ComponentAdapterTestCase a('C'), b('E');
		a.AdaptToStore(storer,true,false); // Content
		b.AdaptToStore(storer,false,true); // Element
	}
	void LoadFrom(Storage & storer) {
		{
			ComponentAdapterTestCase a('C',false), b('C');
			a.AdaptToLoad(storer,true,false); // Content
			std::string context("Loading Content "+mId);
			bool failed = a.DiferenceCause(b,context);
			CLAM_ASSERT(!failed, context.c_str());
		}
		{
			ComponentAdapterTestCase a('E',false), b('E');
			a.AdaptToLoad(storer,false,true); // Element
			std::string context("Loading Element "+mId);
			bool failed = a.DiferenceCause(b,context);
			CLAM_ASSERT(!failed, context.c_str());
		}
	}
private:
	std::string mId;
};


/**
 * A Component having only one parameter
 */
class MyComponent : public CLAM::Component {
		std::string mTag;
	public:
		MyComponent() {mTag="DefaultContent";};
		MyComponent(const MyComponent &c) {mTag=c.mTag;};
		MyComponent(std::string tag) {mTag=tag;};
		const char * GetClassName() const {
			return "MyComponent";
		}
		virtual ~MyComponent() {};
		void StoreOn(CLAM::Storage & s) const {
			CLAM::XMLAdapter<std::string> adapt(mTag);
			s.Store(adapt);
		}
		void LoadFrom(CLAM::Storage & s) {
			std::string text;
			CLAM::XMLAdapter<std::string> adapt(mTag);
			s.Load(adapt);
		}
};



class XmlAdaptersTest : public CppUnit::TestFixture {

public:

	CPPUNIT_TEST_SUITE (CLAMTest::XmlAdaptersTest);
	CPPUNIT_TEST (testXmlAdapter);
	CPPUNIT_TEST (testXmlArrayAdapter);
	CPPUNIT_TEST (testXmlIterableAdapter);
	CPPUNIT_TEST (testXmlComponentAdapter);
	CPPUNIT_TEST (testXmlComponentArray);
	CPPUNIT_TEST (testXmlComponentList);
	CPPUNIT_TEST_SUITE_END();

private:

/**
 * Tests the XMLAdapter class
 */
	void testXmlAdapter()
	{
		GenericAdaptersTester<SimpleAdapterTestCase> tester;
		bool match = XMLInputOutputMatches(tester,__FILE__"Simple.xml");
		CLAM_ASSERT(match, "Store/Load mismatch using basic types adapters");
	}

/**
 * Tests the XMLArrayAdapter class
 */
	void testXmlArrayAdapter()
	{
		GenericAdaptersTester<ArrayAdapterTestCase> tester;
		bool match = XMLInputOutputMatches(tester,__FILE__"Array.xml");
		CLAM_ASSERT(match, "Store/Load mismatch using basic type arrays adapters");
	}

/**
 * Tests the XMLIterableAdapter class
 */
	void testXmlIterableAdapter()
	{
		GenericAdaptersTester<IterableAdapterTestCase> tester;
		bool match = XMLInputOutputMatches(tester,__FILE__"Array.xml");
		CLAM_ASSERT(match, "Store/Load mismatch using basic type iterable adapters");
	}

/**
 * Tests the XMLComponentAdapter class
 */
	void testXmlComponentAdapter()
	{
		ComponentAdaptersTester tester;
		bool match = XMLInputOutputMatches(tester,__FILE__"Component.xml");
		CLAM_ASSERT(match, "Store/Load mismatch using component adapters");
	}

	void testXmlComponentArray()
	{
		CLAM::Array<MyComponent> a;
		a.AddElem(MyComponent("One"));
		a.AddElem(MyComponent("Two"));
		a.AddElem(MyComponent("Three"));
		a.AddElem(MyComponent("Four"));
		bool match = XMLInputOutputMatches(a,__FILE__"ComponentArray.xml");
		CLAM_ASSERT(match, "Store/Load mismatch using array of components");
	}

	void testXmlComponentList()
	{
		CLAM::List<MyComponent> a;
		a.AddElem(MyComponent("One"));
		a.AddElem(MyComponent("Two"));
		a.AddElem(MyComponent("Three"));
		a.AddElem(MyComponent("Four"));
		bool match = XMLInputOutputMatches(a,__FILE__"ComponentListy.xml");
		CLAM_ASSERT(match, "Store/Load mismatch using list of components");
	}

};


}

