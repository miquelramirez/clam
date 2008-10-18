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

#include <iostream>
#include "Array.hxx"
#include "Complex.hxx"
#include "Component.hxx"
#include "DynamicType.hxx"
#include "XMLAdapter.hxx"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <typeinfo>

using namespace CLAM;

static int result = 0;

class SelfRefering {
	int data;
	SelfRefering *me;
public:
	// Default constructor: initializes self-reference.
	int Data() const {return data;}
	void Data(int i) {data=i;}
	SelfRefering(int i=0)
		{
			data = i;
			me = this;
		}
	// Copy constructor: must be implemented to keep the self-reference consistent.
	SelfRefering(const SelfRefering& src) {
		data=src.data;
		me = this;
	}
	// Copy operator: same as for the constructor.
	SelfRefering &operator = (const SelfRefering& src) {
		data=src.data;
		me = this;
		return *this;
	}
	// The destructor: is everything ok?
	~SelfRefering() {
		if (me != this)
			throw(CLAM::Err("SelfRefering error: invalid self reference"));
	}

	bool operator ==(const SelfRefering& src) const {
		return Data() == src.Data();
	}

	bool operator !=(const SelfRefering& src) const {
		return Data() != src.Data();
	}

};

class A : public CLAM::Component
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
	const char * GetClassName() const {return "A";}

	void LoadFrom (CLAM::Storage & store)
	{
	}

	void StoreOn(CLAM::Storage & storage) const
	{
		XMLAdapter<int> adaptera(a, "Aa", true);
		XMLAdapter<float> adapterb(b, "Ab", true);
		XMLAdapter<double> adapterc(c, "Ac", true);

		storage.Store(adaptera);
		storage.Store(adapterb);
		storage.Store(adapterc);
	}



};

class B : public CLAM::Component
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

	const char * GetClassName() const {return "A";}

	void LoadFrom (CLAM::Storage & store)
	{
	}

	void StoreOn(CLAM::Storage & storage) const
	{
		XMLAdapter<int> adaptera(a, "Ba", true);
		XMLAdapter<float> adapterb(b, "Bb", true);
		XMLAdapter<double> adapterc(c, "Bc", true);

		storage.Store(adaptera);
		storage.Store(adapterb);
		storage.Store(adapterc);
	}


};



class Dyn : public DynamicType
{
public:
	DYNAMIC_TYPE(Dyn, 3);
	DYN_ATTRIBUTE(0, public, Array<int>, IntegerList);
	DYN_ATTRIBUTE(1, public, A, MyA);
	DYN_ATTRIBUTE(2, public, Array<B>, MyBList );
	
	virtual ~Dyn() 
	{
		std::cout << "Deleting a Dyn at : " << this << std::endl;
	} 

};


std::istream &operator>>(std::istream &in, SelfRefering &d)
{
	unsigned int i;
	in >> i;
	d.Data(i);
	return in;
}

std::ostream &operator<<(std::ostream &out, const SelfRefering &d)
{
	out << d.Data();
	return out;
}

struct tA  {
	int data[100];
};

struct tB {
	int data[100];
};

std::istream &operator>>(std::istream &in, tA &d)
{
	in >> d.data[0];
	return in;
}
std::istream &operator>>(std::istream &in, tB &d)
{
	in >> d.data[0];
	return in;
}

std::ostream &operator<<(std::ostream &out, const tA &d)
{
	out << d.data[0];
	return out;
}
std::ostream &operator<<(std::ostream &out, const tB &d)
{
	out << d.data[0];
	return out;
}


namespace CLAM {

//  	template<> Array<tB>& Array<tB>::operator = (const Array<tB>& src)
//  	{
//  		mStep = src.mStep;
//  		Resize(src.Size());
//  		SetSize(src.Size());
//  		memcpy(mpData,src.mpData,sizeof(B)*src.mSize);
//  		return *this;
//  	}

}


// Let's see how tough is this guy...
void hardcore_test()
{
	std::cout << "'Evil' Test on the CLAM::Array" << std::endl;

	time( NULL );  // Initializing the seeds for getting true random numbers

	Array< int >             mySimpleIntArray;
	Array< Complex > myComplexArray; //:)
	Array< B >               myArrayOfComponents;
	Array< Dyn >             myDynamicTypeArray;

	// arrays initialization

	unsigned int index;
	
	std::cout << "'Evil' AddElem Test starting:" << std::endl;

	for ( index = 0; index < 30; index++ )
	{
		Dyn myDyn;

		try
		{
			mySimpleIntArray.AddElem( index );
			myComplexArray.AddElem( Complex( float(index), float(index) ) );
			myArrayOfComponents.AddElem( B() );

			// The inicialization of the Dynamic Type Array will be quite special

			
			if ( ( float ( rand() ) / float( RAND_MAX ) ) > 0.5f ) 
			{
				myDyn.AddIntegerList();
				myDyn.UpdateData();
				myDyn.SetIntegerList( mySimpleIntArray );
				
			}
			if ( ( float ( rand() ) / float( RAND_MAX ) ) > 0.5f ) 
			{
				myDyn.AddMyA();
				myDyn.UpdateData();
				myDyn.SetMyA( A() );
				
			}
			if ( ( float ( rand() ) / float( RAND_MAX ) ) > 0.5f ) 
			{
				myDyn.AddMyBList();
				myDyn.UpdateData();
				myDyn.SetMyBList( myArrayOfComponents );
			}

			myDynamicTypeArray.AddElem( myDyn );

			std::cout << "." << std::endl;
		}
		catch ( CLAM::Err e )
		{
			std::cout << " F(" << index << ") : "
					  << e.what() << std::endl;
			result = 1;
		}
	}

	std::cout << "'Evil' AddElem test done" << std::endl;

	// 'Evil' DeleteElem test

	unsigned int chosen=0;

	std::cout << "'Evil' DeleteElem test starting" << std::endl;

	for ( index = 0; index < 15; index++ )
	{
		try
		{
			chosen = int( ( float ( rand() ) / float( RAND_MAX ) ) * float(mySimpleIntArray.Size()-1) );
			mySimpleIntArray.DeleteElem( chosen );
			std::cout << "I(" << chosen << ")S ";		
			chosen = int( ( float ( rand() ) / float( RAND_MAX ) ) * float(myComplexArray.Size()-1) );
			myComplexArray.DeleteElem( chosen );
			std::cout << "C(" << chosen << ")S ";		
			chosen = int( ( float ( rand() ) / float( RAND_MAX ) ) * float(myArrayOfComponents.Size()-1) );
			myArrayOfComponents.DeleteElem( chosen );
			std::cout << "Co(" << chosen << ")S ";		
			chosen = int( ( float ( rand() ) / float( RAND_MAX ) ) * float(myDynamicTypeArray.Size()-1) );
			myDynamicTypeArray.DeleteElem( chosen );
			std::cout << "DT(" << chosen << ")S ";		
		}
		catch( CLAM::Err e )
		{
			std::cout << "F(" << chosen << ") ";
		}
		std::cout << std::endl;
	}
	
	std::cout << "'Evil' DeleteElem test done" << std::endl;

}

int main()
{
	using namespace CLAM;
	std::cout << "Legend:" << std::endl
			  << "  \".\" : passed check" << std::endl
			  << "  \"X\" : failed check" << std::endl
			  << "  \"S\" : failed self reference consistency check" << std::endl;

	try {

		{ // We build all the local variables in a new code block,
		  // So they get destroyed inside the try/catch block.
			int i;
			// Array construction
			std::cout << "Testing construction" << std::endl;
			Array<int> intarray;
			Array<TData> doublearray;
			Array<ComplexTmpl<TData> > complexarray;
			Array<SelfRefering > selfarray;
	
			// Element addition:
			// We build 25 ... 49
			std::cout << "Testing element adition";
			for (i=25; i<50; i++) {
				intarray.AddElem(i);
				doublearray.AddElem(TData(i));
				complexarray.AddElem(ComplexTmpl<TData>(i,i));
				selfarray.AddElem(SelfRefering(i));
				std::cout << '.' << std::flush;
			}
			// Element Insertion:
			// We get 0...39 25 ... 49
			std::cout << std::endl << "Testing element insertion";
			for (i=0; i<40; i++) {
				intarray.InsertElem(i,i);
				doublearray.InsertElem(i,TData(i));
				complexarray.InsertElem(i,ComplexTmpl<TData>(i,i));
				selfarray.InsertElem(i,SelfRefering(i));
				std::cout << '.' << std::flush;
			}
			// Array resizing
			int oldsize;
			std::cout << std::endl << "Testing array resizing";
			oldsize = intarray.AllocatedSize();
			for (i=oldsize; i<oldsize+1024; i++)
				intarray.Resize(i);
			intarray.Resize(oldsize);
			std::cout<< '.' << std::flush;
			oldsize = doublearray.AllocatedSize();
			for (i=oldsize; i<oldsize+1024; i++)
				doublearray.Resize(i);
			doublearray.Resize(oldsize);
			std::cout<< '.' << std::flush;
			oldsize = complexarray.AllocatedSize();
			for (i=oldsize; i<oldsize+1024; i++)
				complexarray.Resize(i);
			complexarray.Resize(oldsize);
			std::cout<< '.' << std::flush;
			oldsize = selfarray.AllocatedSize();
			for (i=oldsize; i<oldsize+1024; i++)
				selfarray.Resize(i);
			selfarray.Resize(oldsize);
			std::cout<< '.' << std::flush;

			// Element Deletion:
			// We should get 0... 24 25 ... 50
			std::cout << std::endl << "Testing element deletion";
			for (i=0; i<15; i++) {
				intarray.DeleteElem(25);
				doublearray.DeleteElem(25);
				complexarray.DeleteElem(25);
				try {
					selfarray.DeleteElem(25);
					std::cout << '.' << std::flush;
				} 
				catch (CLAM::Err e) {
					std::cout << 'S';
				}
			}
			// Asignments and comparisions.
			std::cout << std::endl << "Testing element access";
			for (i=0; i<50; i++) {
				if ( intarray[i] != i ||
					 doublearray[i] != TData(i) ||
					 complexarray[i] != ComplexTmpl<TData>(i,i) ) {
					std::cout << intarray[i] << 'X';
					result = 1;
				}
				else
					std::cout << '.';
			}
			
			std::cout << std::endl << "Testing asignment and comparision";
			Array<int> newint(intarray);
			if (! (newint == intarray) ) {
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";
			newint = intarray;
			if (! (newint == intarray) ) {
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";
			Array<TData> newdouble(doublearray);
			if (! (newdouble == doublearray) ) {
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";
			newdouble = doublearray;
			if (! (newdouble == doublearray) ) {
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";
			Array<ComplexTmpl<TData> > newcomplex(complexarray);
			if (! (newcomplex == complexarray) ) {
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";
			newcomplex = complexarray;
			if (! (newcomplex == complexarray) ) {
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";
			Array<SelfRefering > newself(selfarray);
			if (! (newself == selfarray) ) {
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";
			newself = selfarray;
			if (! (newself == selfarray) ){
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";

		//Concatenation
			std::cout << std::endl << "Testing concatenation";
			oldsize=newint.Size();
			newint += intarray;
			for (i=0; i<oldsize; i++) newint.DeleteElem(0);
			if (! (newint == intarray) ){
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";
			oldsize=newdouble.Size();
			newdouble += doublearray;
			for (i=0; i<oldsize; i++) newdouble.DeleteElem(0);
			if (! (newdouble == doublearray) ){
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";
			oldsize=newcomplex.Size();
			newcomplex += complexarray;
			for (i=0; i<oldsize; i++) newcomplex.DeleteElem(0);
			if (! (newcomplex == complexarray) ){
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";
			oldsize=newself.Size();
			newself += selfarray;
			for (i=0; i<oldsize; i++) {
				try {
					newself.DeleteElem(0);
				}
				catch (CLAM::Err e) {
					std::cout << "S";
					result = 1;
				}
			}
			if (! (newself == selfarray) ){
				std::cout << "X";
				result = 1;
			}
			else
				std::cout << ".";

			std::cout << std::endl << "Testing speed" << std::endl;
			std::cout << "  Unoptimised copy ...";
			Array<A> obj1, obj2;
			long j;
			for (j=0; j<10000000; j++)
				obj1=obj2;
			std::cout << "  done" << std::endl;;
			
			std::cout << "  Optimised copy ...";
			Array<B> obj3, obj4;
			for (j=0; j<10000000; j++)
				obj3=obj4;
			std::cout << "  done" << std::endl;;
			
		} // Array destruction :)

		// hardcore test
		hardcore_test();

		if (result) {
			std::cout << "Failed." << std::endl;
			return result;
		}

		std::cout << "Passed." << std::endl;
		return 0;

	}
	catch(CLAM::Err e) {
		e.Print();
		return 1;
	}
}


