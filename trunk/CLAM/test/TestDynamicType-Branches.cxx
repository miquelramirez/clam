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

//////////////////////////////////////////////////////////////////
// Test for DynamicTyes
//
// written by Pau Arumí - May 2001
////////////////////////////////////////////////////////////////////


#include "TestDynamicType-Branches.hxx"
#include "Array.hxx"
#include "BranchCompactMem.hxx"

#ifdef CLAM_USE_XML
#include <fstream>
#include "XMLStorage.hxx"
#include "XMLStaticAdapter.hxx"
#include "XMLComponentAdapter.hxx"
#endif//CLAM_USE_XML

int test1()
{
	int i;
	float f;


	DynamicNote myNote;

	myNote.AddPitch();
	myNote.AddNSines();
	myNote.UpdateData();
	myNote.AddpADSR();

	BranchCompactMem<DynamicNote> b(myNote);
	b.CreateElems(4);            	//  4 elems

	b[0].SetPitch(0.0f);
	b[1].SetPitch(11.1f);
	b[2].SetPitch(22.2f);
	b[3].SetPitch(33.3f);
	b[4].SetPitch(44.4f);


	DynamicNote n1(myNote);
	n1.SetPitch(3.14f);
	n1.SetNSines(8);
	b.Push(n1).Push(n1).Push(n1).Push(n1).Push(n1);               // 9 elems
	int n = b.GetNumElems();

	f = b[0].GetPitch();
	f = b[1].GetPitch();
	f = b[2].GetPitch();
	f = b[3].GetPitch();
	f = b[4].GetPitch();
	i = b[0].GetNSines();
	i = b[8].GetNSines();

	b.Delete(2);            		// 8 elems
	f = b[2].GetPitch();

	DynamicNote n3(n1);
	n3.SetPitch(123.123f);

	b.Insert(3, n3);
	f = b[3].GetPitch();
	f = b[4].GetPitch();
	b.Insert(0, n3);
	f = b[1].GetPitch();

	n = b.GetNumElems();              // 10 elems

	return 0;
	/*
	 
		myNote.SetPitch(4.14f);
	 
		myNote.AddB();
		myNote.UpdateData();
		
		DynamicNote n1(myNote);
		
		f=myNote.GetPitch();
	 
		myNote.SetNSines(10);
		n1.SetNSines(8888);
	 
		i=myNote.GetNSines();
		j=n1.GetNSines();
	 
		k=myNote.GetA();
		
		n1.AddA();
		n1.AddB();
		n1.UpdateData();
		n1.SetA(3);
	//	n1.SetB(4);
	 
		k=myNote.GetA();
		i=n1.GetA();
	 
		
		f=n1.GetPitch();
		
		i = 9898;
		myNote.AddpIint();
		myNote.SetpIint(&i);
		j  = *myNote.GetpInt()+102;
	 
		return 0;
	*/
}

int testStructure()
{

	float f;

	DynamicNote myNote;

	myNote.AddPitch();
	myNote.AddNSines();
	myNote.AddpADSR();
	myNote.UpdateData();


	DynamicADSR adsr;
	adsr.AddAttackLevel();
	adsr.AddDecayTime();
	adsr.AddSustainLevel();              // all attribures added
	adsr.UpdateData();
	adsr.SetAttackLevel(10.0f);
	adsr.SetDecayTime(0.23f);
	adsr.SetSustainLevel(7.77f);
	myNote.SetpADSR(&adsr);


	BranchCompactMem<DynamicNote> b(myNote);
	b.CreateElems(4);            	//  4 elems

	//	b[0].SetPitch(0.0f); b[1].SetPitch(11.1f); b[2].SetPitch(22.2f); b[3].SetPitch(33.3f); b[4].SetPitch(44.4f);

	void* ptr;
	ptr = b[0].GetpADSR();
	f = b[0].GetPitch();
	ptr = b[3].GetpADSR();
	f = b[3].GetPitch();



	DynamicNote n1(myNote);              // a new note using myNote as a prototype. As myNote have data, it will be copied
	n1.SetPitch(3.14f);
	n1.SetNSines(8);

	b.Push(n1).Push(n1).Push(n1).Push(n1).Push(n1);               // 9 elems	int n = b.GetNumElems();

	f = adsr.GetAttackLevel();
	f = myNote.GetpADSR()->GetAttackLevel();
	f = n1.GetpADSR()->GetAttackLevel();


	f = b[7].GetpADSR()->GetAttackLevel();

	return 0;


}

int testXML()
{

	DynamicNote myNote;

	myNote.AddPitch();
	myNote.AddNSines();
	myNote.AddpADSR();
	myNote.AddpCTest();
	myNote.AddpInt();
	myNote.AddWave();

	myNote.UpdateData();
	myNote.SetPitch(22.22f);
	myNote.SetNSines(3131);

	int i = 3;
	myNote.SetpInt(&i);
	
		Test testclass;
		myNote.SetpCTest(&testclass);
	 
		DynamicADSR adsr;
		adsr.AddAttackLevel(); adsr.AddDecayTime(); adsr.AddSustainLevel();             // all attributes added
		adsr.UpdateData();
		adsr.SetAttackLevel(10.0f); adsr.SetDecayTime(0.23f); adsr.SetSustainLevel(7.77f);
	 
		myNote.SetpADSR(&adsr);
	
	BranchCompactMem<DynamicNote> b(myNote);
	b.CreateElems(4);            	//  4 elems

	b[0].SetPitch(0.0f);
	b[1].SetPitch(11.1f);
	b[2].SetPitch(22.2f);
	b[3].SetPitch(33.3f);
	//	b[4].SetPitch(44.4f);
	DataArray array(6);
	array.AddElem(0.0f);
	array.AddElem(1.1f);
	array.AddElem(2.2f);
	array.AddElem(3.3f);
	array.AddElem(4.4f);
	array.AddElem(5.5f);
	array.AddElem(6.6f);
	array.AddElem(7.7f);
	myNote.SetWave(&array);

	DynamicNote n1(myNote);             // a new note using myNote as a prototype. As myNote have data, it will be copied
	n1.SetPitch(3.14f);
	n1.SetNSines(8);
	b.Push(n1).Push(n1).Push(n1).Push(n1).Push(n1);              // 9 elems



#ifdef CLAM_USE_XML
	
	// Storing result to XML document and check if loading matches
	CLAM_ASSERT(XMLInputOutputMatches(b, "TestDynamicType-Branches.xml"), 
		"Original resulting DTBranch and XML reloaded one don't match");

#endif//CLAM_USE_XML

	DynamicNote n(myNote);
	n.UpdateData();

	return 0;

}

int testDelete()
{

	DynamicNote myNote;

	myNote.AddPitch();
	myNote.AddNSines();
	myNote.AddpADSR();
	myNote.UpdateData();
	
	
	

	BranchCompactMem<DynamicNote> b(myNote);
	b.CreateElems(4);            	//  4 elems

	void *p = b[0].GetpADSR();


	DynamicADSR adsr;
	adsr.AddAttackLevel();
	adsr.AddDecayTime();
	adsr.AddSustainLevel();              // all attributes added
	adsr.UpdateData();
	adsr.SetAttackLevel(10.0f);
	adsr.SetDecayTime(0.23f);
	adsr.SetSustainLevel(7.77f);

	myNote.SetpADSR(&adsr);

	DynamicADSR voidAdsr;
	myNote.SetpADSR(&voidAdsr);


	DynamicNote n1(myNote);
	DynamicNote n2(n1);
	DynamicNote n3(n1);

	//delete myNote;

	return 0;


}

int testDeepCopy()
{

	DynamicNote myNote;

	myNote.AddPitch();
	myNote.AddNSines();
	myNote.AddpADSR();
	myNote.UpdateData();

	myNote.SetPitch(1.1f);
	myNote.SetNSines(10);

	DynamicADSR adsr;
	adsr.AddAttackLevel();
	adsr.AddDecayTime();
	adsr.AddSustainLevel();              // all attribures added
	adsr.UpdateData();
	adsr.SetAttackLevel(10.0f);
	adsr.SetDecayTime(0.23f);
	adsr.SetSustainLevel(7.77f);
	myNote.SetpADSR(&adsr);

	Component* noteCopy = myNote.DeepCopy();

	myNote.SetPitch(9999.9f);
	myNote.GetpADSR()->SetAttackLevel(9999.9f);

	// now myCopy and noteCopy are different.
	// you can take a look at the output file DeepCopy.xml


	BranchCompactMem<DynamicNote> b(myNote);
	b.CreateElems(4);            	//  4 elems

	Component* branchCopy = b.ShallowCopy();

#ifdef CLAM_USE_XML
	XMLable* adapter = NULL;
	XMLStorage storage ("XML-Document");
	adapter = new XMLComponentAdapter(*noteCopy, "DeepCopy_of_a_note", true);
	storage.Store(adapter);
	delete adapter;

	adapter = new XMLComponentAdapter(myNote, "original_myNote", true);
	storage.Store(adapter);
	delete adapter;

	adapter = new XMLComponentAdapter(*branchCopy, "SwallowCopy_of_a_branch", true);
	storage.Store(adapter);
	delete adapter;


	fstream fileout("testCopy.xml", ios::out);
	storage.dumpOn(fileout);
#endif//CLAM_USE_XML


	return 0;


}
/*
#include "CCLAMTmplArray.hxx"

class CCLAMPort
{
public:
	DynamicType *mpData;

	enum EType{
	    eUnknown,
	    eInput,
	    eOutput
	};

	EType mType;

	CCLAMPort(DynamicType* pType = 0, EType t = eUnknown)
	{
		mpData = pType;
		mType = t;
	}
};

enum EDataFormat DataFormat(const CCLAMPort &)
{
	return eFmtDefault;
}

typedef CCLAMTmplArray<CMTGPort> CMTGPortArray;

class CCLAMProcObj
{
protected:
public:
	CCLAMPortArray mPorts;
	void AddPort(DynamicType* pType, CCLAMPort::EType portType)
	{
		mPorts.AddElem(*(new CCLAMPort(pType, portType)));
	}
	virtual void Do(void) = 0;
};

#include "DynamicType.hxx"

class ConcreteTypeA: public DynamicType
{
public:
	REG_NUM_ATTR (ConcreteTypeA, 2);

	REGISTER (0, I1, int);
	REGISTER (1, I2, int);
};

class ConcreteTypeB: public DynamicType
{
public:
	REG_NUM_ATTR (ConcreteTypeB, 2);

	REGISTER (0, F1, float);
	REGISTER (1, F2, float);
};

int table[4] = {4, 4, 4, 4};

class CCLAMConcreteProcObj: public CMTGProcObj
{
public:
	ConcreteTypeB mInput;
	ConcreteTypeA mOutput;

	void InitTypes(void)
	{
		mOutput.AddI1();
		mInput.AddF1();
		mInput.AddF2();

		mInput.UpdateData();
		mOutput.UpdateData();
	}

	CCLAMConcreteProcObj()
	{
		InitTypes();

		AddPort(&mInput, CCLAMPort::eInput);
		AddPort(&mOutput, CCLAMPort::eOutput);
	}
	void Do(ConcreteTypeB* in, ConcreteTypeA* ret)
	{
		ret->SetI1(in->GetF1()*32000. + 2);
	}
	void Do(float* f, int* i)
	{
		*i = *f * 32000. + 2;
	}
	void Do(char* f, int id1, char* i, int id2)
	{
		//		*(int*)&i[table[id2]] = *(float*)f[table[id1]]*32000.+2;
		*(int*)&i[table[id1]] = *(float*) & f[table[id2]] * 32000. + 2;
	}
	void Do(void)
	{
		Do(&mInput, &mOutput);
	}
};

class CCLAMConcreteProcObj2: public CMTGProcObj
{
public:
	ConcreteTypeA mInput;
	ConcreteTypeB mOutput;

	void InitTypes(void)
	{
		mInput.AddI1();
		mOutput.AddF1();
		mOutput.AddF2();

		mInput.UpdateData();
		mOutput.UpdateData();
	}

	CCLAMConcreteProcObj2()
	{
		InitTypes();

		AddPort(&mInput, CCLAMPort::eInput);
		AddPort(&mOutput, CCLAMPort::eOutput);
	}
	void Do(ConcreteTypeA* in, ConcreteTypeB* ret)
	{
		ret->SetF1(float(in->GetI1()) / 32000.);
	}
	void Do(int* i, float* f)
	{
		*f = float(*i) / 32000.;
	}
	void Do(char* i, int id1, char* f, int id2)
	{
		//		*(float*)&f[table[id1]] = float(*(int*)&i[table[id2]])/32000.;
		*(float*)&f[table[id1]] = float(*(int*) & i[table[id2]]) / 32000.;
	}
	void Do(void)
	{
		Do(&mInput, &mOutput);
	}
};
//#include <sys/time.h>
//#include <unistd.h>

#define N 10000000

/*
void testProcObj(void)
{
	CCLAMConcreteProcObj foo1;
	CCLAMConcreteProcObj2 foo2;
 
	CCLAMProcObj* p1 = &foo1;
	CCLAMProcObj* p2 = &foo2;
 
	ConcreteTypeB b(foo1.mInput);
	ConcreteTypeA a(foo1.mOutput);
 
	float vb;
	int va;
 
	char vbArr[10];
	char vaArr[10];
 
	long dt;
 
	long t1, t2;
 
//------------------------------
 
	int id1 = 2;
	int id2 = 1;
 
	*(float*)&vbArr[4] = 0.5;
 
	t1 = clock();
 
	for (int i = 0;i < N;i++)
	{
		foo1.Do(vbArr, id1, vaArr, id2);
		foo2.Do(vaArr, id1, vbArr, id2);
	}
 
	t2 = clock();
 
	dt = t2 - t1;
 
	printf("Direct Cast check = %f\n", *(float*)&vbArr[4]);
	printf("Duration: %d\n", dt);
 
//------------------------------
 
	b.SetF1(0.5);
 
	t1 = clock();
 
	for (int i = 0;i < N;i++)
	{
		foo1.Do(&b, &a);
		foo2.Do(&a, &b);
	}
 
	t2 = clock();
 
	dt = t2 - t1;
 
	printf("DynamicType check = %f\n", b.GetF1());
	printf("Duration: %d\n", dt);
 
//------------------------------
 
	vb = 0.5;
 
	t1 = clock();
 
	for (int i = 0;i < N;i++)
	{
		foo1.Do(&vb, &va);
		foo2.Do(&va, &vb);
	}
 
	t2 = clock();
 
	dt = t2 - t1;
 
	printf("Primitive check = %f\n", vb);
	printf("Duration: %d\n", dt);
 
//------------------------------
 
	foo1.mInput.SetpData(foo2.mOutput.GetpData());
	foo2.mInput.SetpData(foo1.mOutput.GetpData());
 
	foo1.mInput.SetF1(0.5);
 
	t1 = clock();
 
	for (int i = 0;i < N;i++)
	{
		p1->Do();
		p2->Do();
	}
 
	t2 = clock();
 
	dt = t2 - t1;
 
	printf("Runtime check = %f\n", foo2.mOutput.GetF1());
	printf("Duration: %d\n", dt);
 
 
 
 
}
*/


int main()
{

	
	using namespace CLAM;

	TempTest<int> test;
	test.AddA(); test.UpdateData();
	test.SetA(8);
	int a = test.GetA();


	int ret = 0;
	ret = testDelete();
	// ret = testXML();
	//	ret = testStructure();
	// ret = testDeepCopy();

	// testProcObj();

	return ret;
}
