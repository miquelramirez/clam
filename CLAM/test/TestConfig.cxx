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

#include "HeapDbg.hxx"
#include "DynamicType.hxx"

#include "mtgsstream.h" // An alias for <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "XMLTestHelper.hxx"
#include "Component.hxx"
#include "TypeInfo.hxx"
#include "TypeInfoStd.hxx"

#include "DummyObjects.hxx"
#include "Enum.hxx"
#include "Processing.hxx"

namespace CLAMTest {

void Testing(const std::string & s) {
	std::cout << "*** Testing: " << s << std::endl;
}

// Dummy Enum

class EDummy : public Enum {
public:
	static tEnumValue sEnumValues[];
	static tValue sDefault;
	EDummy() : Enum(sEnumValues, sDefault) {}
	EDummy(tValue v) : Enum(sEnumValues, v) {};
	EDummy(std::string s) : Enum(sEnumValues, s) {};
	virtual ~EDummy() {};
	Component * Species() const { return new EDummy;};

	typedef enum {
		zero=0,
		dos=2,
		cent=100
	} tEnum;
	static void TestClass ();
};
Enum::tEnumValue EDummy::sEnumValues[] = {
	{EDummy::zero,"zero"},
	{EDummy::dos,"dos"},
	{EDummy::cent,"cent"},
	{0,NULL}
};

Enum::tValue EDummy::sDefault = EDummy::dos;


// Dummy Config

class DummySubConfig : public ProcessingConfig
{

	DYNAMIC_TYPE_USING_INTERFACE (DummySubConfig,6,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
	DYN_ATTRIBUTE(1,public,std::string,ThatIsAString);
	DYN_ATTRIBUTE(2,public,TData,ThatIsATData);
	DYN_ATTRIBUTE(3,public,TSize,ThatIsATSize);
	DYN_ATTRIBUTE(4,public,EDummy, ThatIsAEDummy);
	DYN_ATTRIBUTE(5,public,bool, ThatIsABool);
private:

	void DefaultInit() {
		AddAll();
		UpdateData();
		DefaultValues();
	}
	void DefaultValues() {
	}

public:
	~DummySubConfig(){};
};

class DummyConfig : public ProcessingConfig
{

	DYNAMIC_TYPE_USING_INTERFACE (DummyConfig,7,ProcessingConfig);
	DYN_ATTRIBUTE(0,public,std::string,Name);
	DYN_ATTRIBUTE(1,public,std::string,ThisisAString);
	DYN_ATTRIBUTE(2,public,TData,ThisIsATData);
	DYN_ATTRIBUTE(3,public,TSize,ThisIsATSize);
	DYN_ATTRIBUTE(4,public,EDummy, ThisIsAEDummy);
	DYN_ATTRIBUTE(5,public,bool, ThisIsABool);
	DYN_ATTRIBUTE(6,public,DummySubConfig, ThisIsASubConfig);
private:

	void DefaultInit() {
		AddAll();
		UpdateData();
		DefaultValues();
	}
	void DefaultValues() {
	}
public:
	~DummyConfig(){};
};


class DummyVisitor {
	public:
	template <typename T>
	void Accept(const char *name, T &value) {
		Accept(name, &value, value);
	}
	template <typename T>
	void Accept(const char *name, void *foo, T& value) {
		std::cout << "Visiting Uneditable type '" << name << "'" << std::endl;
	}
	template <typename T>
	void Accept(const char *name, std::string *foo, T& value) {
		std::cout << "Visiting '" << name << "' Type string Value: '" << value << "'" << std::endl;
	}
	template <typename T>
	void Accept(const char *name, TData *foo, T& value) {
		std::cout << "Visiting '" << name << "' Type TData Value: " << value << std::endl;
	}
	template <typename T>
	void Accept(const char *name, TSize *foo, T& value) {
		std::cout << "Visiting '" << name << "' Type TSize Value: " << value << std::endl;
	}
	template <typename T>
	void Accept(const char *name, bool *foo, T& value) {
		std::cout << "Visiting '" << name << "' Type bool Value: " << (value?"true":"false") << std::endl;
	}
	template <typename T>
	void Accept(const char *name, Enum *foo, T&value) {
		std::cout << "Visiting '" << name << "' Type Enum Value: [ " ;
		const Enum::tEnumValue * mapping = value.GetSymbolMap();
		for (unsigned i = 0; mapping[i].name; i++) {
			if (mapping[i].value==value.GetValue()) {
				std::cout << "*'" << value.GetString() << "'* ";
			}
			else {
				std::cout << "'" << mapping[i].name << "' ";
			}
		}
		std::cout << "]" << std::endl;
	}
	template <typename T>
	void Accept(const char *name, DynamicType *foo, T&value) {
		std::cout << "Visiting '" << name << "' Dynamic Type" << std::endl;
		value.VisitAll(*this);
	}
};

class MyVisitorToFloat {
	public:
	void Accept(const char *name, float value) {
		std::cout << "ToFloat Visiting '" << name << "' Type float Value: " << value << std::endl;
	}
	void Accept(const char *name, Component &value) {
		std::cout << "ToFloat Visiting '" << name << "' Component" << std::endl;
	}
};
void testVisitors() {
	{
		DummyConfig config;
		DummyVisitor visitor;
		config.VisitAll(visitor);
	}
}

}
static const bool quiet=false;
static const bool verbose=false;

bool CLAMTest::CompWithBasics8::sTraceStatus= verbose || (!quiet && false);
bool CLAMTest::CompWithBasics9::sTraceStatus= verbose || (!quiet && false);
bool CLAMTest::Dyn::sTraceStatus= verbose || (!quiet && false);
bool CLAMTest::SuperDyn::sTraceStatus= verbose || (!quiet && true);
bool CLAMTest::SuperDynAlt::sTraceStatus= verbose || (!quiet && false);
/*
using namespace CLAM;
// We will play with classes
// A: A Component with storable members (int float and double) with value 8
// B: A clone of A but with values 9
// Dyn: A dynamicType contaning A and B as DynAttributes
// SuperDyn: A dynamicType contaning Dyn, A and B as DynAttributes
*/

using namespace CLAMTest;

int main(void)
{
	try{
		for (int i=0; i<50; i++) // loop for memory leaks checking
		{	

			CLAM_CHECK_HEAP( " Corruption at testXML() " );

			testVisitors();

			CLAM_CHECK_HEAP( " Corruption at testVisitors() " );

			std::cout << ".";
			
		}

	} catch (CLAM::Err e)
	{
		e.Print();
		std::cout << "Failed!" << std::endl;
		return 1;
	} catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	} catch (...)
	{
		std::cout << "catch (...)" << std::endl;
	}

	std::cout << "Passed." << std::endl;
	return 0;	
} 



