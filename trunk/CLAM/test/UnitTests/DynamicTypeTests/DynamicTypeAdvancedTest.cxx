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
#include "DynamicTypeAdvancedTest.hxx"
#include "XMLTestHelper.hxx"
#include <sstream>

namespace CLAMTest {

CPPUNIT_TEST_SUITE_REGISTRATION( DynamicTypeAdvancedTest );


void DynamicTypeAdvancedTest::SimpleXMLSupport()
{
	Dyn p;
	p.Populate(); p.Modify( 4 );
	bool match = XMLInputOutputMatches( p,__FILE__"Dyn.xml" );
	CPPUNIT_ASSERT( match );
}

void DynamicTypeAdvancedTest::XMLCustomizationByRedefinition()
{
	DynAlt o;
	o.AddAll(); o.UpdateData();
	Dyn & p = o.GetSubDyn();
	p.AddAll(); p.UpdateData();
	bool match = XMLInputOutputMatches( o,__FILE__"DynAlt.xml" );
	CPPUNIT_ASSERT_MESSAGE("this test should be reworked (pau)", match );
}

void DynamicTypeAdvancedTest::XMLCustomizationBySubclassing()
{
	DynAlt o;
	o.AddAll(); o.UpdateData();
	Dyn & p = o.GetSubDyn();
	p.AddAll(); p.UpdateData();
	bool match = XMLInputOutputMatches(o,__FILE__"DynAlt.xml");
	CPPUNIT_ASSERT( match );
}

void DynamicTypeAdvancedTest::XMLWithArrayAttribute()
{
	DynWithArrays o;
	o.AddAll(); o.UpdateData();
	Array<float> & af = o.GetMyFloatArray();
	af.AddElem(7);
	af.AddElem(6);
	af.AddElem(5);
	af.AddElem(4);
	af.AddElem(3);
	af.AddElem(2);
	af.AddElem(1);
	af.AddElem(0);
	Array<Dyn> & ad = o.GetMyDynArray();
	Dyn d1; d1.AddAll(); d1.UpdateData();
	ad.AddElem(d1);
	ad.AddElem(d1);
	ad.AddElem(d1);
	ad.AddElem(d1);
	bool match = XMLInputOutputMatches( o,__FILE__"DynWithArrays.xml" );
	CPPUNIT_ASSERT( match );

}

void DynamicTypeAdvancedTest::XMLWithIterableAttribute()
{
	DynWithIterables o;
	o.Populate();
	o.Modify(5);
	bool match = XMLInputOutputMatches(o,__FILE__"DynWithIterables.xml");
	CPPUNIT_ASSERT( match );
}

class Loggable
{
public:
	std::string GetLog() { return log.str(); }
protected:
	std::ostringstream log;
};

class MyVisitorToInt : public Loggable
{
public:
	template <typename T>
	void Accept(const char *name, T &value) {
		Accept(name, value, value);
	}
	template <typename T>
	void Accept(const char *name, int foo, T& value) {
		log << "ToInt Visiting '" << name << "' Type int Value: " << value << std::endl;
		
	}
	template <typename T>
	void Accept(const char *name, Component &foo, T&value) {
		log << "ToInt Visiting '" << name << "' Component" << std::endl;
	}
	template <typename T>
	void Accept(const char *name, DynamicType &foo, T&value) {
		log << "ToInt Visiting '" << name << "' Dynamic Type" << std::endl;
		value.VisitAll(*this);
	}
};

class MyVisitorToFloat : public Loggable
{
	public:
	void Accept(const char *name, float value) {
		log << "ToFloat Visiting '" << name << "' Type float Value: " << value << std::endl;
	}
	void Accept(const char *name, Component &value) {
		log << "ToFloat Visiting '" << name << "' Component" << std::endl;
	}
};
	
void DynamicTypeAdvancedTest::VisitorsToPlainDTs() 
{
	Dyn p;
	p.Populate(); p.Modify(4);
	MyVisitorToInt visitorToInt;
	p.VisitAll(visitorToInt);

	CPPUNIT_ASSERT_EQUAL( std::string(
		"ToInt Visiting 'Int' Type int Value: 4\n"
		"ToInt Visiting 'MyA' Component\n"
		"ToInt Visiting 'MyB' Component\n"
		"ToInt Visiting 'MoreInt' Type int Value: 104\n"
		"ToInt Visiting 'SubDyn' Dynamic Type\n" ),
		visitorToInt.GetLog() );

	MyVisitorToFloat visitorToFloat;
	p.VisitAll(visitorToFloat);	
	
	CPPUNIT_ASSERT_EQUAL( std::string(
		"ToFloat Visiting 'Int' Type float Value: 4\n" //Notice: float
		"ToFloat Visiting 'MyA' Component\n"
		"ToFloat Visiting 'MyB' Component\n"
		"ToFloat Visiting 'MoreInt' Type float Value: 104\n" //Notice: float
		"ToFloat Visiting 'SubDyn' Component\n" ), //Notice: Component
		visitorToFloat.GetLog() );


}
void DynamicTypeAdvancedTest::VisitorsToDTWithArrays()
{
	DynWithArrays o;
	o.Populate();
	o.Modify(4);
	MyVisitorToInt visitorToInt;
	o.VisitAll(visitorToInt);	
	
	CPPUNIT_ASSERT_EQUAL( std::string(
		"ToInt Visiting 'MyFloat' Type int Value: 4\n"
		"ToInt Visiting 'MyFloatArray' Component\n"
		"ToInt Visiting 'MyDynArray' Component\n" ),
		visitorToInt.GetLog() );
	
	MyVisitorToFloat visitorToFloat;
	o.VisitAll(visitorToFloat);	

	CPPUNIT_ASSERT_EQUAL( std::string(
		"ToFloat Visiting 'MyFloat' Type float Value: 4\n" //Notice: float
		"ToFloat Visiting 'MyFloatArray' Component\n"
		"ToFloat Visiting 'MyDynArray' Component\n" ),
		visitorToFloat.GetLog() );
}

void DynamicTypeAdvancedTest::VisitorsToDTTrees()
{
	DynAlt o;
	o.Populate();
	Dyn & p = o.GetSubDyn();
	p.Populate();
	p.Modify(5);
	

	MyVisitorToInt visitorToInt;
	o.VisitAll(visitorToInt);

	CPPUNIT_ASSERT_EQUAL( std::string(
		"ToInt Visiting 'Int' Type int Value: 0\n"
		"ToInt Visiting 'MyA' Component\n"
		"ToInt Visiting 'MyB' Component\n"
		"ToInt Visiting 'MoreInt' Type int Value: 100\n"
		"ToInt Visiting 'SubDyn' Dynamic Type\n"
		"ToInt Visiting 'Int' Type int Value: 5\n"
		"ToInt Visiting 'MyA' Component\n"
		"ToInt Visiting 'MyB' Component\n"
		"ToInt Visiting 'MoreInt' Type int Value: 105\n"
		"ToInt Visiting 'SubDyn' Dynamic Type\n" ),
		visitorToInt.GetLog() );

	MyVisitorToFloat visitorToFloat;
	o.VisitAll(visitorToFloat);	

	CPPUNIT_ASSERT_EQUAL( std::string(
		"ToFloat Visiting 'Int' Type float Value: 0\n"
		"ToFloat Visiting 'MyA' Component\n"
		"ToFloat Visiting 'MyB' Component\n"
		"ToFloat Visiting 'MoreInt' Type float Value: 100\n"
		"ToFloat Visiting 'SubDyn' Component\n" ),
		visitorToFloat.GetLog() );

}

} //namespace

