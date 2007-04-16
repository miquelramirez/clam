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
#ifndef _DummyDynamicTypes_hxx_
#define _DummyDynamicTypes_hxx_

#include <iostream>
#include <vector>

#include "XMLStorage.hxx" // CLAM

#include "Component.hxx" // CLAM
#include "DynamicType.hxx" // CLAM
#include "Array.hxx" // CLAM

namespace CLAMTest {

//////////////////////////////////////////////////////////////////////

/**
 * A Dummy Component class with three members of different basic 
 * StoredAsLeaf types members are initiated to 8.
 */ 
class CompWithBasics8 : public CLAM::Component
{
private:
	static bool sTraceStatus;
public:
	int mA;
	float mB;
	double mC;
	void f() {};
	static void Trace(bool status) {
		sTraceStatus=status;
	}
	CompWithBasics8() : mA(8), mB(8), mC(8)
	{
		if (sTraceStatus)
			std::cout << "Constructor CompWithBasics8 at " << this << std::endl;
	}
	CompWithBasics8(const CompWithBasics8& arg) : mA(arg.mA), mB(arg.mB), mC(arg.mC)
	{
		if (sTraceStatus)
			std::cout << "Constructor CompWithBasics8 at "<< this 
			          << " copying from argument at : " << &arg << std::endl;
	}
	~CompWithBasics8()
	{
		if (sTraceStatus)
			std::cout << "Destructor CompWithBasics8 at "<< this << std::endl;
	}

	void StoreOn(CLAM::Storage & storage) const
	{
		CLAM::XMLAdapter<int> adaptera(mA, "CWB8a", true);
		storage.Store(adaptera);
		CLAM::XMLAdapter<float> adapterb(mB, "CWB8b", true);
		storage.Store(adapterb);
		CLAM::XMLAdapter<double> adapterc(mC, "CWB8c", true);
		storage.Store(adapterc);
	}
	void LoadFrom(CLAM::Storage & storage)
	{
		CLAM::XMLAdapter<int> adaptera(mA, "CWB8a", true);
		CLAM_ASSERT(storage.Load(adaptera),
			"Failed to load CompWithBasics8 item a");
		CLAM::XMLAdapter<float> adapterb(mB, "CWB8b", true);
		CLAM_ASSERT(storage.Load(adapterb),
			"Failed to load CompWithBasics8 item b");
		CLAM::XMLAdapter<double> adapterc(mC, "CWB8c", true);
		CLAM_ASSERT(storage.Load(adapterc),
			"Failed to load CompWithBasics8 item c");
	}

	const char* GetClassName() const { return "CompWithBasics8"; }
};

//////////////////////////////////////////////////////////////////////

/**
 * A CompWithBasics8 subclass that puts its members a value of
 * 9 instead 8.
 */ 
class CompWithBasics9 : public CompWithBasics8
{
private:
	static bool sTraceStatus;
public:
	static void Trace(bool status) {
		sTraceStatus=status;
	}
public:
	CompWithBasics9() 
	{
		mA=9; 
		mB=9;
		mC=9;
		if (sTraceStatus)
			std::cout << "Constructor CompWithBasics9 at " 
			          << this << std::endl;
	}
	CompWithBasics9(const CompWithBasics9 & arg)
	{
		mA=arg.mA;
		mB=arg.mB;
		mC=arg.mC;
		if (sTraceStatus)
			std::cout << "Constructor CompWithBasics9 at " << this 
			          << " from argument at " << &arg << std::endl;
	}
	~CompWithBasics9()
	{
		if (sTraceStatus)
			std::cout << "Destructor CompWithBasics9 at "
			          << this << std::endl;
	}

	void StoreOn(CLAM::Storage & storage) const
	{
		CLAM::XMLAdapter<int> adaptera(mA, "CWB9a", true);
		storage.Store(adaptera);
		CLAM::XMLAdapter<float> adapterb(mB, "CWB9b", true);
		storage.Store(adapterb);
		CLAM::XMLAdapter<double> adapterc(mC, "CWB9c", true);
		storage.Store(adapterc);
	}
	void LoadFrom(CLAM::Storage & storage)
	{
		CLAM::XMLAdapter<int> adaptera(mA, "CWB9a", true);
		CLAM_ASSERT(storage.Load(adaptera),
			"Failed to load CompWithBasics8 item a");
		CLAM::XMLAdapter<float> adapterb(mB, "CWB9b", true);
		CLAM_ASSERT(storage.Load(adapterb),
			"Failed to load CompWithBasics8 item b");
		CLAM::XMLAdapter<double> adapterc(mC, "CWB9c", true);
		CLAM_ASSERT(storage.Load(adapterc),
			"Failed to load CompWithBasics8 item c");
	}
};


// Dyn: A dynamicType contaning A and B as DynAttributes

class Dyn : public CLAM::DynamicType
{
private:
	static bool sTraceStatus;
public:
	static void Trace(bool status) {
		sTraceStatus=status;
	}
public:
	DYNAMIC_TYPE(Dyn, 5);
	DYN_ATTRIBUTE(0, public, int, Int);
	DYN_ATTRIBUTE(1, public, CompWithBasics8, MyA);
	DYN_ATTRIBUTE(2, public, CompWithBasics9, MyB);
	DYN_ATTRIBUTE(3, public, int, MoreInt);
	DYN_ATTRIBUTE(4, public, Dyn, SubDyn);
public:
	virtual ~Dyn() 
	{
		if (sTraceStatus)
			std::cout << "Deleting a Dyn at " << this << std::endl;
	} 

public:
	void Populate() {
		AddAll();
		UpdateData();
		GetInt()=0;
		GetMoreInt()=100;
	}
	void Modify(int changeValue) {
		GetInt()+=changeValue;
		GetMoreInt()+=changeValue;
	}
};



/**
 * This class is like the Dyn but it is stored in a diferent way.
 */
class DynAlt : public Dyn
{
private:
	static bool sTraceStatus;
public:
	static void Trace(bool status) {
		sTraceStatus=status;
	}
	virtual ~DynAlt() 
	{
		if (sTraceStatus)
			std::cout << "Deleting a DynAlt at " << this << std::endl;
	}
	
	void StoreOn(CLAM::Storage & storage) const
	{
		// This Attribute is changed in order
		StoreSubDyn(storage);
		// MyA is left
		StoreMyB(storage);
		// MyInt is stored as an attribute (the default is element 
		// and adifferent name.
		if (HasMoreInt()) {
				CLAM::XMLAdapter<int> adapter(GetMoreInt(), "Size", false);
				storage.Store(adapter);
		}
		// An extra item
		std::string added = "AddedContent";
		CLAM::XMLAdapter<std::string> adapter(added, "Added", false);
		storage.Store(adapter);
	}
	void LoadFrom(CLAM::Storage & storage)
	{
		AddAll();
		UpdateData();
		// This Attribute is changed in order
		LoadSubDyn( storage );
		// MyA is left
		LoadMyB( storage );
		// MyInt is stored as an attribute (the default is element 
		// and adifferent name.
		{
			CLAM::XMLAdapter<int> adapter(GetMoreInt(), "Size", false);
			if (!storage.Load(adapter)) {
				RemoveMoreInt();
			}
		}
		// An extra item
		{
			std::string dummy;
			CLAM::XMLAdapter<std::string> adapter(dummy, "Added", false);
			CLAM_ASSERT(storage.Load(adapter),
				"The additional parameter was not pressent");
		}
	}
};

/**
 * A dynamic type containing as dyn attributes components Array and a
 * basic type (float) Array.
 */
class DynWithArrays : public CLAM::DynamicType {
	public:
		DYNAMIC_TYPE(DynWithArrays, 3);
		DYN_ATTRIBUTE(0, public, float, MyFloat);
		DYN_ATTRIBUTE(1, public, CLAM::Array<float>, MyFloatArray);
		DYN_ATTRIBUTE(2, public, CLAM::Array<Dyn>, MyDynArray);
	private:
		void DefaultInit() {
			AddAll();
			UpdateData();
		}
	public:
		void Populate() {
			GetMyFloat()=0;
			AddAll();
			UpdateData();
			CLAM::Array<float> &vf = GetMyFloatArray();
			CLAM::Array<Dyn> &vd = GetMyDynArray();
			for (int i=9; i--;) {
				vf.AddElem( float(i) );
				Dyn d;
				d.Populate();
				d.Modify(i);
				vd.AddElem(d);
			}
		}
		void Modify(int changeValue) {
			AddAll();
			UpdateData();
			CLAM::Array<float> &vf = GetMyFloatArray();
			unsigned int i;
			for (i=vf.Size(); i--;) {
				vf[i]+=changeValue;
			}
			CLAM::Array<Dyn> &vd = GetMyDynArray();
			for (i=vd.Size(); i--; ) {
				vd[i].Modify(changeValue);
			}
			GetMyFloat()+=changeValue;
		}
};

/**
 * A dynamic type containing as dyn attributes components Array and a
 * basic type (float) Array.
 */
class DynWithIterables : public CLAM::DynamicType {
	public:
		DYNAMIC_TYPE(DynWithIterables, 3);
		DYN_ATTRIBUTE(0, public, float, MyFloat);
		DYN_CONTAINER_ATTRIBUTE(1, public, std::vector<float>, MyFloatVector, notused);
		DYN_CONTAINER_ATTRIBUTE(2, public, std::vector<Dyn>, MyDynVector, aDyn);
	private:
		void DefaultInit() {
			AddAll();
			UpdateData();
		}
	public:
		void Populate() {
			GetMyFloat()=0;
			AddAll();
			UpdateData();
			std::vector<float> &vf = GetMyFloatVector();
			std::vector<Dyn> &vd = GetMyDynVector();
			for (int i=9; i--;) {
				vf.push_back( float(i) );
				Dyn d;
				d.Populate();
				d.Modify(i);
				vd.push_back(d);
			}
		}
		void Modify(int changeValue) {
			AddAll();
			UpdateData();
			std::vector<float> &vf = GetMyFloatVector();
			std::vector<float>::iterator itf = vf.begin();
			for (; itf!= vf.end(); itf++) {
				*itf+=changeValue;
			}
			std::vector<Dyn> &vd = GetMyDynVector();
			std::vector<Dyn>::iterator itd = vd.begin();
			for (; itd != vd.end(); itd++) {
				itd->Modify(changeValue);
			}
			GetMyFloat()+=changeValue;
		}
};



}//namespace CLAMTest

#endif

