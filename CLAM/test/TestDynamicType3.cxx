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

#include "ProcessingDataConfig.hxx"
#include "Spectrum.hxx"

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
namespace CLAMTest {

void Testing(const std::string & s) {
	std::cout << "*** Testing: " << s << std::endl;
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

/**
 * This tests does the following tests:
 * 
 */


void testBasic()
{

	Dyn d, *p; 

	Testing("Adding attributes once");
	d.AddInt();
	d.AddMyA();
	d.UpdateData();
	if (d.HasMyB() || !d.HasMyA() || !d.HasInt() ) 
		throw CLAM::Err("TestFailed: Simple AddX");
	d.FullfilsInvariant();

	Testing("Removing all attributes with uninformed attributes");
	d.RemoveAll();
	d.UpdateData();
	if (d.HasMyB() || d.HasMyA() || d.HasInt() ) 
		throw CLAM::Err("TestFailed: RemoveAll");
	d.FullfilsInvariant();

	Testing("Repetitive adds before Update");
	d.AddInt();
	d.AddInt();
	d.AddMyA();
	d.AddMyB();
	d.AddMyB();
	d.AddMyA();
	d.UpdateData();
	if (!d.HasMyB() || !d.HasMyA() || !d.HasInt() ) 
		throw Err("Test failed: Repetitive AddX");
	d.FullfilsInvariant();

	Testing("Mixing adds and removes");
	d.AddInt();
	d.AddMyB();
	d.AddMyA();
	d.RemoveMyB();
	d.AddMyB();
	d.RemoveMyB();
	d.RemoveMyA();
	d.AddMyA();
	d.FullfilsInvariant();
	d.UpdateData();
	if (d.HasMyB() || !d.HasMyA() || !d.HasInt() ) 
		throw CLAM::Err("Test failed: Combining AddX and RemoveX");
	d.FullfilsInvariant();


	Testing("Non updated Adds and Removes");
	d.RemoveAll();
	d.AddMyA();
	d.UpdateData();
	d.AddMyB();
	d.RemoveMyA();
	if (d.HasMyB() || d.HasMyA() || d.HasInt() ) 
		throw CLAM::Err("Test failed: Non updated AddX and RemoveX");
	d.FullfilsInvariant();

	CLAM_BREAKPOINT
	Testing("Adding All attributes");
	d.AddAll();
	d.UpdateData();
	if (!d.HasMyB() || !d.HasMyA() || !d.HasInt() ) 
		throw CLAM::Err("Test failed: AddAll");
	d.FullfilsInvariant();
		
	Testing("Removing All attributes");
	d.RemoveAll();
	d.UpdateData();
	if (d.HasMyB() || d.HasMyA() || d.HasInt() ) 
		throw CLAM::Err("Test failed: RemoveAll");
	d.FullfilsInvariant();
		
	Testing("Simple Set and Get");

	d.AddAll();
	d.UpdateData();
	
	d.SetInt(999);

	CompWithBasics8 a;
	a.mA=999; 
	a.mB=999.999f; 
	a.mC=999.99999999f;
	d.SetMyA(a);

	CompWithBasics9 b, bb;
	b.mA=888; 
	b.mB=888.888f; 
	b.mC=888.88888888f;
	d.SetMyB(b);

	if (d.GetInt() != 999 || 
	    d.GetMyA().mC != 999.99999999f || 
	    d.GetMyB().mC != 888.88888888f  
	   )
		throw CLAM::Err("testBasic(): simple set/get check failed.");
	d.FullfilsInvariant();

	Testing("Dynamic usage");
	p = new Dyn();
	p->AddInt();
	p->AddMyB();
	p->UpdateData();
	delete p;

	Testing("Deletion of a not-all-informed DT");
	SuperDyn* sd = new SuperDyn();
	sd->AddMyDyn();
	sd->UpdateData();
	sd->AddMyA();
	delete sd;
	
	Testing("Inserting a DT inside other");
	sd = new SuperDyn();
	sd->AddAll();
	sd->UpdateData();
	d.AddAll();
	d.UpdateData();
	sd->SetMyDyn(d);
	if (sd->GetMyDyn().GetInt() != 999 ||
		sd->GetMyDyn().GetMyB().mC != 888.88888888f  )
		throw CLAM::Err("testBasic(): nested set/get check failed.");

	delete sd;

}


void testDeepCopy()
{
	Dyn d;
	CompWithBasics8 a, aa;
	CompWithBasics9 b, bb;
	
	d.AddInt();
	d.AddMyA();
	d.UpdateData();
	d.AddMyB();
	d.Debug();
	d.UpdateData();
	
	int i = d.GetInt();
	a = d.GetMyA();
	b = d.GetMyB();

	a.mA=99; a.mB= 88.8f; a.mC=767.767;
	b.mA=11; b.mB= 11.1f; b.mC= 282.822;
	
	d.SetInt(8);
	d.SetMyA(a);
	d.SetMyB(b);
	d.Debug();

	Dyn* pd = dynamic_cast<Dyn*>(d.DeepCopy());
	pd->Debug();

	i = pd->GetInt();
	aa = pd->GetMyA();
	bb = pd->GetMyB();

	if (i != 8 ||
		aa.mC != 767.767 ||
		bb.mC != 282.822 )
		throw CLAM::Err("testDeepCopy(): Explicit DeepCopy failed.");



	delete pd;

	pd = dynamic_cast<Dyn*>(d.ShallowCopy());
	pd->Debug();

 	i = pd->GetInt();
	aa = pd->GetMyA();
	bb = pd->GetMyB();

	if (i != 8 ||
		aa.mC != 767.767 ||
		bb.mC != 282.822   )
		throw CLAM::Err("testDeepCopy(): Explicit ShallowCopy failed.");

	delete pd;

 	i = d.GetInt();
	aa = d.GetMyA();
	bb = d.GetMyB();
	
	if (i != 8 ||
		aa.mC != 767.767 ||
		bb.mC != 282.822 )
		throw CLAM::Err("testDeepCopy(): Original data changed after copy operation.");


//	delete pd->Getp();  Would be error because the pd is the result of a Shallow copy.
}

void testMoreDeepCopy()
{
	Dyn *d = new Dyn();
	d->AddInt();
	d->AddMyA();
	d->AddMyB();
	d->UpdateData();


	delete d;

	d = new Dyn();
	d->AddInt();
	d->AddMyA();
	d->AddMyB();
	d->AddSubDyn();
	d->UpdateData();
	d->SetSubDyn(*new Dyn); 
	d->SetInt(0);
	d->GetSubDyn().AddInt();
	d->GetSubDyn().UpdateData();
	d->GetSubDyn().SetInt(99999);
	
	SuperDyn *sd = new SuperDyn();
	sd->AddMyDyn();
	sd->AddMyA();
	sd->AddMyB();
	sd->UpdateData();
	delete sd;

	sd = new SuperDyn();
	sd->AddMyDyn();
	sd->AddMyA();
	sd->AddMyB();
	sd->UpdateData();
	
	CompWithBasics8 a; 
	CompWithBasics9 b;
	a.mA=99; a.mB= 88.8f; a.mC=767.767;
	b.mB=11; b.mB= 11.1f; b.mC= 282.822;

	sd->SetMyA(a);
	sd->SetMyB(b);
	sd->SetMyDyn(*d);
	sd->Debug();
	sd->GetMyDyn().Debug();

	delete d;

	if (sd->GetMyDyn().GetSubDyn().GetInt() != 99999)
		throw CLAM::Err("testMoreDeepCopy(): First copy operation differs.");


	SuperDyn *copy = new SuperDyn();
	copy->Debug();
	*copy=*sd;
	copy->GetMyDyn().Debug();

	delete sd;

	if (copy->GetMyDyn().GetSubDyn().GetInt() != 99999)
		throw CLAM::Err("testMoreDeepCopy(): Second copy operation differs.");
	
	{
	SuperDyn copy2(*copy);
	copy2.Debug();
	if (copy2.GetMyDyn().GetSubDyn().GetInt() != 99999)
		throw CLAM::Err("testMoreDeepCopy(): Third copy operation differs.");
	}

	copy->Debug();
	copy->GetMyDyn().Debug();
	if (copy->GetMyDyn().GetSubDyn().GetInt() != 99999)
		throw CLAM::Err("testMoreDeepCopy(): Fourth copy operation differs.");
	delete copy;

}

void testAssignation()
{
	Dyn d; 
	CompWithBasics8 a, aa;
	CompWithBasics9 b, bb;
	
	d.AddInt();
	d.AddMyA();
	d.AddMyB();
	d.UpdateData();
	a.mA=99; a.mB= 88.8f; a.mC=767.767;
	b.mA=11; b.mB= 11.1f; b.mC= 282.822;
	d.SetInt(8);
	d.SetMyA(a);
	d.SetMyB(b);
	d.Debug();


	Dyn copy;
	copy = d;
	copy.Debug();
	if ( copy.GetInt() != 8 ||
	     copy.GetMyA().mC != 767.767 ||
	     copy.GetMyB().mC != 282.822)
		throw CLAM::Err("testAssignation(): Direct asignment differs.");

	Dyn copy2(d);
	copy2.Debug();
	if ( copy2.GetInt() != 8 ||
	     copy2.GetMyA().mC != 767.767 ||
	     copy2.GetMyB().mC != 282.822)
		throw CLAM::Err("testAssignation(): Copy constructor differs.");

}


class AudioConfig: public CLAM::ProcessingDataConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (AudioConfig, 3, CLAM::ProcessingDataConfig);
	DYN_ATTRIBUTE (0, public, int, SampleRate);  // Hz
	DYN_ATTRIBUTE (1, public, int, Size);        // Samples/channel
	DYN_ATTRIBUTE (2, public, int, RawFormat);      // EDataFormat
//	DYN_ATTRIBUTE (3, public, AudioTypeFlags, Type);
private:
	void DefaultInit() 
	{
		AddSampleRate();
		AddSize();
		AddRawFormat();
		UpdateData();
	}

};


void testInterface()
{
	AudioConfig myConfig;
	if (!myConfig.HasSize()) throw CLAM::Err("Error in ExistAttr");
}

void testRealCases()
{

		SpecTypeFlags flags;
		Spectrum dummy;

		dummy.SetSize(100);
		dummy.GetType(flags);
		flags.bComplex=1;
		dummy.SetType(flags);  // add complex
		
		if (!dummy.HasComplexArray())
			throw CLAM::Err("testRealCases(): First SetType failed.");


		dummy.FullfilsInvariant();
		
		flags.bPolar=1;
		flags.bComplex=0;
		dummy.SetType(flags); // set to mag phase
		
		if (dummy.HasComplexArray() ||
			!dummy.HasPolarArray())
			throw CLAM::Err("testRealCases(): Second SetType failed.");

		dummy.RemoveComplexArray();
		dummy.UpdateData();
		dummy.Debug();

		dummy.GetType(flags);
		if (flags.bComplex)
			throw CLAM::Err("testRealCases(): GetConfig failed.");
		
		flags.bPolar=0;
		flags.bMagPhase=1;
		flags.bComplex=1;
		dummy.SetType(flags); // again add complex

		if (dummy.HasPolarArray()   ||
			!dummy.HasMagBuffer()   ||
			!dummy.HasPhaseBuffer() ||
			!dummy.HasComplexArray())
			throw CLAM::Err("testRealCases(): Third SetType failed.");

		dummy.FullfilsInvariant();

}


void testExtensiveDeepCopies()
{
	int i;
	Spectrum dummySpec;
	dummySpec.SetSize(100);
	DataArray& mag = dummySpec.GetMagBuffer();
	for (i=0; i<99; i++) mag[i]=i;
	Spectrum* pS = new Spectrum();
	
	for (i=0; i<100; i++) {
		*pS = dummySpec;
	}
	delete pS;
}

void testCopiesOf2DimSpecArray()
{
	const int size=25;
	int i;

	Spectrum dummySpec;
	dummySpec.SetSize(100);
	DataArray& mag = dummySpec.GetMagBuffer();
	for (i=0; i<100; i++) mag[i]=i;
	
	Array<Array<Spectrum> > superArray;
	superArray.Resize(size);
	superArray.SetSize(size);
	
	for (i=0; i<size; i++) {
		superArray[i].Resize(size);
		superArray[i].SetSize(size);
		
	}

	for (i=0; i<size; i++)
		for(int j=0; j<size; j++) superArray[i][j] = dummySpec;
}

void testXML() 
{
	// TODO: Testing simple DT without redefining anything
	{
		std::cout << "-- Testing simple XML support" << std::endl;
		Dyn p;
		p.Populate(); p.Modify(4);
		bool match = XMLInputOutputMatches(p,__FILE__"Dyn.xml");
		CLAM_ASSERT(match, "Failed to store/load Dyn");
	}
	{
		std::cout << "-- Testing XML customization by redefinition" << std::endl;
		SuperDyn o;
		o.AddAll(); o.UpdateData();
		Dyn & p = o.GetP();
		p.AddAll(); p.UpdateData();
		bool match = XMLInputOutputMatches(o,__FILE__"SuperDyn.xml");
		CLAM_ASSERT(match, "Failed to store/load SuperDyn");
	}
	{
		std::cout << "-- Testing XML customization by subclassing" << std::endl;
		SuperDynAlt o;
		o.AddAll(); o.UpdateData();
		Dyn & p = o.GetP();
		p.AddAll(); p.UpdateData();
		bool match = XMLInputOutputMatches(o,__FILE__"SuperDynAlt.xml");
		CLAM_ASSERT(match, "Failed to store/load SuperDynAlt");
	}
	{
		std::cout << "-- Testing XML with Array attributes" << std::endl;
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
		bool match = XMLInputOutputMatches(o,__FILE__"DynWithArrays.xml");
		CLAM_ASSERT(match, "Failed to store/load DynWithArrays");
	}
	{
		std::cout << "-- Testing XML with Array attribute" << std::endl;
		DynWithArrays o;
		o.Populate();
		o.Modify(4);
		bool match = XMLInputOutputMatches(o,__FILE__"DynWithArrays.xml");
		CLAM_ASSERT(match, "Failed to store/load DynWithArrays");
	}
	{
		std::cout << "-- Testing XML with iterable attribute" << std::endl;
		DynWithIterables o;
		o.Populate();
		o.Modify(5);
		bool match = XMLInputOutputMatches(o,__FILE__"DynWithIterables.xml");
		CLAM_ASSERT(match, "Failed to store/load DynWithArrays");
	}
}


void testDataSharing()
{
	Dyn d1;

	d1.AddInt();
	d1.AddMyA();
	d1.AddMyB();
	d1.UpdateData();

	Dyn d2(d1);
	Dyn d3(d2);

	if (!d1.HasInt() || !d2.HasInt() || !d3.HasInt() ||
		!d1.HasMyA() || !d2.HasMyA() || !d3.HasMyA() ||
		!d1.HasMyB() || !d2.HasMyB() || !d3.HasMyB() )
		throw(CLAM::Err("testDataSharing(): Copy failed"));

	d1.AddSubDyn();
	
	d1.UpdateData();
	d2.UpdateData();
	d3.UpdateData();

	if (d2.HasSubDyn() || d3.HasSubDyn())
		throw(CLAM::Err("testDataSharing(): Sibilings changed after adding attributes."));

	d2.RemoveInt();
	d2.RemoveMyA();

	d1.UpdateData();
	d2.UpdateData();
	d3.UpdateData();

	if (!d1.HasInt()  || !d3.HasInt() ||
		!d1.HasMyA()  || !d3.HasMyA() )
		throw(CLAM::Err("testDataSharing(): Sibilings changed after removing attributes."));

	d3.RemoveMyB();

	d1.UpdateData();
	d2.UpdateData();
	d3.UpdateData();

	if (!d1.HasMyB())
		throw(CLAM::Err("testDataSharing(): Sibilings changed after adding and removing attributes."));


	CompWithBasics8 a;
	CompWithBasics9 b;
	a.mA=99; a.mB= 88.8f; a.mC=767.767;
	b.mB=11; b.mB= 11.1f; b.mC= 282.822;
	

	d3.AddMyB();
	d3.AddSubDyn();
	d3.UpdateData();

	d3.SetInt(8);
	d3.SetMyA(a);
	d3.SetMyB(b);
	Dyn *dp = new Dyn();
	d3.SetSubDyn(*dp);

	bool ShareData=true;
	Dyn d4(d3,ShareData);
	if (&d3.GetMyB() != &d4.GetMyB()) 
		throw(CLAM::Err("testDataSharing(): Attributes are not the same after copy with share."));

	d4.RemoveInt();
	d4.UpdateData();

	if (d4.GetMyA().mC != 767.767 ||
		d4.GetMyB().mC != 282.822 )
		throw(CLAM::Err("testDataSharing(): UpdateData on an object with shared memory failed"));

}

class MyVisitorToInt {
	public:
	template <typename T>
	void Accept(const char *name, T &value) {
		Accept(name, value, value);
	}
	template <typename T>
	void Accept(const char *name, int foo, T& value) {
		std::cout << "ToInt Visiting '" << name << "' Type int Value: " << value << std::endl;
	}
	template <typename T>
	void Accept(const char *name, Component &foo, T&value) {
		std::cout << "ToInt Visiting '" << name << "' Component" << std::endl;
	}
	template <typename T>
	void Accept(const char *name, DynamicType &foo, T&value) {
		std::cout << "ToInt Visiting '" << name << "' Dynamic Type" << std::endl;
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
		Dyn p;
		p.Populate(); p.Modify(4);
		MyVisitorToInt visitorToInt;
		p.VisitAll(visitorToInt);	
		MyVisitorToFloat visitorToFloat;
		p.VisitAll(visitorToFloat);	
	}
	{
		DynWithArrays o;
		o.Populate();
		o.Modify(4);
		MyVisitorToInt visitorToInt;
		o.VisitAll(visitorToInt);	
		MyVisitorToFloat visitorToFloat;
		o.VisitAll(visitorToFloat);	
	}
	{
		SuperDynAlt o;
		o.AddAll(); o.UpdateData();
		Dyn & p = o.GetP();
		p.AddAll(); p.UpdateData();

		MyVisitorToInt visitorToInt;
		o.VisitAll(visitorToInt);	
		MyVisitorToFloat visitorToFloat;
		o.VisitAll(visitorToFloat);	
	}
}

int main(void)
{
	try{
	
		AudioConfig myConfig;
		if (!myConfig.HasSize() ) throw CLAM::Err (" error in ExistAttr ");
	
	 
		for (int i=0; i<50; i++) // loop for memory leaks checking
		{	
			
			testBasic();

			CLAM_CHECK_HEAP( " Corruption at testBasic() " );

			testDeepCopy();

			CLAM_CHECK_HEAP( " Corruption at testDeepCopy() " );

			testMoreDeepCopy();

			CLAM_CHECK_HEAP( " Corruption at testMoreDeepCopy() " );

			testExtensiveDeepCopies();

			CLAM_CHECK_HEAP( " Corruption at testExtensiveDeepCopies() " );

			testCopiesOf2DimSpecArray();

			CLAM_CHECK_HEAP( " Corruption at testCopiesOf2DimSpecArray() " );

			testAssignation();

			CLAM_CHECK_HEAP( " Corruption at testAssignation() " );

			testInterface();

			CLAM_CHECK_HEAP( " Corruption at testInterface() " );

			testRealCases();

			CLAM_CHECK_HEAP( " Corruption at testRealCases() " );

			testDataSharing();

			CLAM_CHECK_HEAP( " Corruption at testDataSharing() " );
			
			testXML();

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



