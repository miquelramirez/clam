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

#ifdef CLAM_USE_XML
#include "XMLStorage.hxx"
#include "XMLStaticAdapter.hxx"
#endif//CLAM_USE_XML
#include "Component.hxx"

using namespace CLAM;

namespace CLAMTest {

	
class A : public Component
{
public:
	int a;
	float b;
	double c;
	void f() {};
	A() : a(8), b(8), c(8)
	{
		std::cout << "Constructiong A at : " << this << std::endl;
	}
	A(const A& arg) : a(arg.a), b(arg.b), c(arg.c)	
	{
			std::cout << "Constructiong A by copy at "<< this <<" from argument at : " << &arg << std::endl;
	}
	~A()
	{
		std::cout << "Destroying A...... at : "<< this << std::endl;
	}

	void StoreOn(Storage & storage)
	{
#ifdef CLAM_USE_XML
		if (dynamic_cast < XMLStorage* > (&storage))
		{
			XMLable * adaptera = new XMLStaticAdapter(a, "Aa", true);
			XMLable * adapterb = new XMLStaticAdapter(b, "Ab", true);
			XMLable * adapterc = new XMLStaticAdapter(c, "Ac", true);

			storage.Store(adaptera);
			storage.Store(adapterb);
			storage.Store(adapterc);
			delete adaptera;
			delete adapterb;
			delete adapterc;
		}
#endif//CLAM_USE_XML
	}



};

class B : public Component
{
public:
	int a;
	float b;
	double c;
	void f() {};
	virtual ~B()
	{
		std::cout << "Destroying B (virtual dest)...... at : "<< this << std::endl;
	}
	B (const B& arg) : a(arg.a), b(arg.b), c(arg.c)	
	{
			std::cout << "Constructiong B by copy at "<< this <<" from argument at : " << &arg << std::endl;
	}
	B() : a(9), b(9), c(9)
	{
		std::cout << "Constructiong B at : " << this << std::endl;
	}

	void StoreOn(Storage & storage)
	{
#ifdef CLAM_USE_XML
		if (dynamic_cast < XMLStorage* > (&storage))
		{
			XMLable * adaptera = new XMLStaticAdapter(a, "Ba", true);
			XMLable * adapterb = new XMLStaticAdapter(b, "Bb", true);
			XMLable * adapterc = new XMLStaticAdapter(c, "Bc", true);

			storage.Store(adaptera);
			storage.Store(adapterb);
			storage.Store(adapterc);
			delete adaptera;
			delete adapterb;
			delete adapterc;
		}
#endif //CLAM_USE_XML
	}


};



class Dyn : public DynamicType
{
	DYNAMIC_TYPE( Dyn, 3 );
public:

	DYN_ATTRIBUTE(0, public, int, Int);
	DYN_ATTRIBUTE(1, public, A, myA );
	DYN_ATTRIBUTE(2, public, B, myB );

	//DYN_ATTRIBUTE(3, public, Dyn*, p );
	//DYN_ATTRIBUTE(4, public, Dyn*, pDyn );

	virtual ~Dyn() 
	{
		std::cout << "Deleting a Dyn at : " << this << std::endl;
	} 

};


class SuperDyn : public DynamicType
{
public:
	DYNAMIC_TYPE( SuperDyn, 3 );
	DYN_ATTRIBUTE(0, public, Dyn, myDyn );
	DYN_ATTRIBUTE(1, public, A, myA );
	DYN_ATTRIBUTE(2, public, B, myB );
	//DYN_ATTRIBUTE(3, public, Dyn*, p );

	virtual ~SuperDyn() 
	{
		std::cout << "Deleting a SuperDyn at : " << this << std::endl;
	}

};

}//namespace

using namespace CLAMTest;

void testBasic()
{
	Dyn d, d2, d3, d4, *p; 
	A a;
	B b, bb;

	d.AddInt();
	d.AddInt();
//	d.Addp();
	d.AddmyA();
//	d.Addp();
	d.AddmyB();
	d.AddmyB();
//	d.AddpDyn();
	d.UpdateData();
//	if (!d.HasmyB() || !d.HasmyA() || !d.HasInt() || !d.Hasp() ) throw Err("Safe Add* malfunctioning");

	if (!d.HasmyB() || !d.HasmyA() || !d.HasInt()  ) throw Err("Safe Add* malfunctioning");


	d.SetInt(999);

	a.a=999; 
	a.b=999.999f; 
	a.c=999.99999999f;
	d.SetmyA(a);

	b.a=888; 
	b.b=888.888f; 
	b.c=888.88888888f;
	d.SetmyB(b);

//	d.SetpDyn(&d);
//	d.RemovepDyn(); // it removes only the pointer. otherwise this would crash.

	if (d.GetInt() != 999 ||
		d.GetmyA().c != 999.99999999f ||
		d.GetmyB().c != 888.88888888f  )
		throw Err("testBasic(): simple set/get check failed.");

	d.RemovemyA();
	A(); //temporaty
//	d.Removep();
	
	// a complete deletion
	p = new Dyn();
	p->AddInt();
//	p->Addp();
	p->AddmyA();
	p->AddmyB();
//	p->AddpDyn();
	p->UpdateData();

	delete p;

	// deletion of a not-all-informed DT
	SuperDyn* sd = new SuperDyn();
	sd->AddmyDyn();
	sd->UpdateData();
	sd->AddmyA();

	d.UpdateData();
	sd->SetmyDyn(d);
	if (sd->GetmyDyn().GetInt() != 999 ||
		sd->GetmyDyn().GetmyB().c != 888.88888888f  )
		throw Err("testBasic(): nested set/get check failed.");

	delete sd;

}


void testDeepCopy()
{
	Dyn d;
	Dyn *dp2;
	A a, aa;
	B b, bb;
	
	d.AddInt();
	d.AddmyA();
//	d.Addp();
	d.UpdateData();
	d.AddmyB();
//	d.Setp(0);
	d.Debug();
	d.UpdateData();
	
	int i = d.GetInt();
	a = d.GetmyA();
	b = d.GetmyB();
//	Dyn* p = d.Getp();

	a.a=99; a.b= 88.8f; a.c=767.767;
	b.b=11; b.b= 11.1f; b.c= 282.822;
	
	d.SetInt(8);
	d.SetmyA(a);
	d.SetmyB(b);
	dp2 = new Dyn();
//	d.Setp(dp2);
	d.Debug();

	Dyn* pd = dynamic_cast<Dyn*>(d.DeepCopy());
	pd->Debug();

	i = pd->GetInt();
	aa = pd->GetmyA();
	bb = pd->GetmyB();
//	p = pd->Getp();

	/*
	if (i != 8 ||
		aa.c != 767.767 ||
		bb.c != 282.822 ||
		pd->Getp() == dp2   )
		throw Err("testDeepCopy(): Explicit DeepCopy failed.");
	*/
	if (i != 8 ||
		aa.c != 767.767 ||
		bb.c != 282.822   )
		throw Err("testDeepCopy(): Explicit DeepCopy failed.");


//	delete pd->Getp();
//	pd->Setp(0);

	delete pd;

	pd = dynamic_cast<Dyn*>(d.ShallowCopy());
	pd->Debug();

 	i = pd->GetInt();
	aa = pd->GetmyA();
	bb = pd->GetmyB();
//	p = pd->Getp();

/*	if (i != 8 ||
		aa.c != 767.767 ||
		bb.c != 282.822 ||
		pd->Getp() != dp2   )
		throw Err("testDeepCopy(): Explicit ShallowCopy failed.");
*/
	if (i != 8 ||
		aa.c != 767.767 ||
		bb.c != 282.822  )
		throw Err("testDeepCopy(): Explicit ShallowCopy failed.");


	delete pd;

 	i = d.GetInt();
	aa = d.GetmyA();
	bb = d.GetmyB();
//	p = d.Getp();

	if (i != 8 ||
		aa.c != 767.767 ||
		bb.c != 282.822 )
		throw Err("testDeepCopy(): Original data changed after copy operation.");

//	delete p;
//	delete pd->Getp();  Would be error because the pd is the result of a Shallow copy.
}

void testMoreDeepCopy()
{
	Dyn *d = new Dyn();
	d->AddInt();
	d->AddmyA();
	d->AddmyB();
//	d->Addp();
	d->UpdateData();
//	d->Setp(new Dyn);
//	d->Getp()->AddInt();
//	d->Getp()->UpdateData();
//	d->Getp()->SetInt(99999);

	delete d;

	d = new Dyn();
	d->AddInt();
	d->AddmyA();
	d->AddmyB();
//	d->Addp();
	d->UpdateData();
//	d->Setp(new Dyn);
//	d->Getp()->AddInt();
//	d->Getp()->UpdateData();
//	d->Getp()->SetInt(99999);
	
	SuperDyn *sd = new SuperDyn();
	sd->AddmyDyn();
	sd->AddmyA();
	sd->AddmyB();
	sd->UpdateData();
	delete sd;

	sd = new SuperDyn();
	sd->AddmyDyn();
	sd->AddmyA();
	sd->AddmyB();
	sd->UpdateData();
	
	A a; B b;
	a.a=99; a.b= 88.8f; a.c=767.767;
	b.b=11; b.b= 11.1f; b.c= 282.822;

	sd->SetmyA(a);
	sd->SetmyB(b);
	sd->SetmyDyn(*d);
	sd->Debug();
	sd->GetmyDyn().Debug();

	delete d;

	/*
	if (sd->GetmyDyn().Getp()->GetInt() != 99999)
		throw Err("testMoreDeepCopy(): First copy operation differs.");
	*/

	SuperDyn *copy = new SuperDyn();
	copy->Debug();
	*copy=*sd;
	copy->GetmyDyn().Debug();

	delete sd;
/*
	if (copy->GetmyDyn().Getp()->GetInt() != 99999)
		throw Err("testMoreDeepCopy(): Second copy operation differs.");
*/	
	{
	SuperDyn copy2(*copy);
	copy2.Debug();
/*	if (copy2.GetmyDyn().Getp()->GetInt() != 99999)
		throw Err("testMoreDeepCopy(): Third copy operation differs.");
*/
	}

	copy->Debug();
	copy->GetmyDyn().Debug();
/*	if (copy->GetmyDyn().Getp()->GetInt() != 99999)
		throw Err("testMoreDeepCopy(): Fourth copy operation differs.");
*/
	delete copy;

}

void testAssignation()
{
	Dyn d; A a, aa;
	B b, bb;
	
	d.AddInt();
	d.AddmyA();
	d.AddmyB();
	d.UpdateData();
	a.a=99; a.b= 88.8f; a.c=767.767;
	b.b=11; b.b= 11.1f; b.c= 282.822;
	d.SetInt(8);
	d.SetmyA(a);
	d.SetmyB(b);
	d.Debug();


	Dyn copy;
	copy = d;
	copy.Debug();
	if ( copy.GetInt() != 8 ||
		 copy.GetmyA().c != 767.767 ||
		 copy.GetmyB().c != 282.822)
		throw Err("testAssignation(): Direct asignment differs.");

	Dyn copy2(d);
	copy2.Debug();
	if ( copy2.GetInt() != 8 ||
		 copy2.GetmyA().c != 767.767 ||
		 copy2.GetmyB().c != 282.822)
		throw Err("testAssignation(): Copy constructor differs.");

}

class AudioConfig: public CLAM::ProcessingDataConfig
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (AudioConfig, 3, CLAM::ProcessingDataConfig);
	void DefaultInit() 
	{
		AddSampleRate();
		AddSize();
		AddRawFormat();
		UpdateData();
	}

	DYN_ATTRIBUTE (0, public, int, SampleRate );  // Hz
	DYN_ATTRIBUTE (1, public, int, Size );        // Samples/channel
	DYN_ATTRIBUTE (2, public, int, RawFormat );      // EDataFormat
//	DYN_ATTRIBUTE (3, Type,AudioTypeFlags);
};


void testInterface()
{
	AudioConfig myConfig;
	if (!myConfig.HasSize()) throw Err("Error in ExistAttr");
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
			throw Err("testRealCases(): First SetType failed.");


		dummy.FullfilsInvariant();
		
		flags.bPolar=1;
		flags.bComplex=0;
		dummy.SetType(flags); // set to mag phase
		
		if (dummy.HasComplexArray() ||
			!dummy.HasPolarArray())
			throw Err("testRealCases(): Second SetType failed.");

		dummy.RemoveComplexArray();
		dummy.UpdateData();
		dummy.Debug();

		dummy.GetType(flags);
		if (flags.bComplex)
			throw Err("testRealCases(): GetConfig failed.");
		
		flags.bPolar=0;
		flags.bMagPhase=1;
		flags.bComplex=1;
		dummy.SetType(flags); // again add complex

		if (dummy.HasPolarArray()   ||
			!dummy.HasMagBuffer()   ||
			!dummy.HasPhaseBuffer() ||
			!dummy.HasComplexArray())
			throw Err("testRealCases(): Third SetType failed.");

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
	
	for (i=0; i<1000; i++) {
		*pS = dummySpec;
	}
	delete pS;
}

void testCopiesOf2DimSpecArray()
{
	const int size=50;
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

void testDataSharing()
{
	Dyn d1;

	d1.AddInt();
	d1.AddmyA();
	d1.AddmyB();
	d1.UpdateData();

	Dyn d2(d1);
	Dyn d3(d2);

	if (!d1.HasInt() || !d2.HasInt() || !d3.HasInt() ||
		!d1.HasmyA() || !d2.HasmyA() || !d3.HasmyA() ||
		!d1.HasmyB() || !d2.HasmyB() || !d3.HasmyB() )
		throw(Err("testDataSharing(): Copy failed"));

//	d1.Addp();
//	d1.AddpDyn();

	d1.UpdateData();
	d2.UpdateData();
	d3.UpdateData();
/*
	if (d2.Hasp() || d3.Hasp() ||
		d2.HaspDyn() || d3.HaspDyn() )
		throw(Err("testDataSharing(): Sibilings changed after adding attributes."));
*/
	d2.RemoveInt();
	d2.RemovemyA();

	d1.UpdateData();
	d2.UpdateData();
	d3.UpdateData();

	if (!d1.HasInt()  || !d3.HasInt() ||
		!d1.HasmyA()  || !d3.HasmyA() )
		throw(Err("testDataSharing(): Sibilings changed after removing attributes."));

	d3.RemovemyB();
//	d3.AddpDyn();

	d1.UpdateData();
	d2.UpdateData();
	d3.UpdateData();

/*	if (!d1.HasmyB() || d2.HaspDyn())
		throw(Err("testDataSharing(): Sibilings changed after adding and removing attributes."));
*/
	if (!d1.HasmyB() )
		throw(Err("testDataSharing(): Sibilings changed after adding and removing attributes."));


	A a;
	B b;
	a.a=99; a.b= 88.8f; a.c=767.767;
	b.b=11; b.b= 11.1f; b.c= 282.822;
	

	d3.AddmyB();
//	d3.Addp();
	d3.UpdateData();

	d3.SetInt(8);
	d3.SetmyA(a);
	d3.SetmyB(b);
	Dyn *dp = new Dyn();
//	d3.Setp(dp);

	bool ShareData=true;
	Dyn d4(d3,ShareData);
	if (&d3.GetmyB() != &d4.GetmyB()) 
		throw(Err("testDataSharing(): Attributes are not the same after copy with share."));

	d4.RemoveInt();
	d4.UpdateData();

	if (d4.GetmyA().c != 767.767 ||
		d4.GetmyB().c != 282.822 )
		throw(Err("testDataSharing(): UpdateData on an object with shared memory failed"));

}

int main(void)
{
	try{
		Spectrum dummy;
		AudioConfig myConfig;
		if (!myConfig.HasSize() ) throw Err (" error in ExistAttr ");
	 
		for (int i=0; i<50; i++) // loop for memory leaks checking
		{	
			
			testBasic();

			CLAM_CHECK_HEAP( "No corruption at testBasic" );

			testDeepCopy();

			CLAM_CHECK_HEAP( "No corruption at testDeepCopy" );


			testMoreDeepCopy();

			CLAM_CHECK_HEAP( "No corruption at testMoreDeepCopy" );


			testExtensiveDeepCopies();

			CLAM_CHECK_HEAP( "No corruption at testExtensiveDeepCopies" );

			testCopiesOf2DimSpecArray();

			CLAM_CHECK_HEAP( "No corruption at testCopiesOf2DimSpecArray" );

			testAssignation();

			CLAM_CHECK_HEAP( "No corruption at testAssignation" );

			testInterface();

			CLAM_CHECK_HEAP( "No corruption at testInterface" );

			testRealCases();

			CLAM_CHECK_HEAP( "No corruption at testRealCases" );

			testDataSharing();

			CLAM_CHECK_HEAP( "No corruption at testDataSharing" );

			std::cout << ".";
			
		}

	} catch (Err e)
	{
			e.Print();
			std::cout << "Failed!" << std::endl;
			return 1;
	}
	std::cout << "Passed." << std::endl;
	return 0;	
} 



