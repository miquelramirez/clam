/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "DynamicType.hxx"
#include <cstdio>
#include <cstring>
#include <string>

#include <iostream>  // needed for Debug() method
#include <fstream>  // idem
#include <list>	   // needed in UpdateData 

#include "XMLStorage.hxx"

#include "ErrDynamicType.hxx"

// IMPORTANT: this next flag will slow all the dynamic types mechanisms
// So it can only be set (defined) when developing, testing or maintaining
// dynamic types.

// #define CLAM_EXTRA_CHECKS_ON_DT

// Anyway this flag should be defined in the project/makefile of the test.

namespace CLAM {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DynamicType::DynamicType(const int nAttr)
{
	// the typeDescTable is initialized into the concrete dynamic type. 
	// because we want that table to be static.(one per concrete class)
	numAttr = nAttr;
	dynamicTable = new TDynInfo[numAttr + 1];
	dynamicTable[numAttr].hasBeenAdded = dynamicTable[numAttr].hasBeenRemoved = false; // global modification flags.
	for ( unsigned i=0; i < numAttr; i++)
	{
		dynamicTable[i].offs = -1;
		dynamicTable[i].hasBeenAdded = false;
		dynamicTable[i].hasBeenRemoved = false;
	}
	numActiveAttr = 0;
	data = 0;
	dataSize = 0;
	allocatedDataSize = 0;
	maxAttrSize = 0;		// initialized in method InformAll()
	bOwnsItsMemory = true;
	bPreAllocateAllAttributes = false;
	InitDynTableRefCounter();
}

DynamicType::DynamicType(const DynamicType& prototype, const bool shareData, const bool deepCopy=true)
// no need of checking the concret class of the prototype, because always is called the the copy-constructor of
// the concrete class. So if you try to pass a prototype of a different concrete class the compiler will complain!
{
	typeDescTable = prototype.typeDescTable;

	numActiveAttr = 0;
	data = 0;
	dynamicTable=0;
	dataSize = 0;
	allocatedDataSize = 0;
	bPreAllocateAllAttributes = false;

	if (prototype.IsInstanciate())
	{
		if (!shareData)
			if (deepCopy)
				SelfDeepCopy(prototype);
			else
				SelfShallowCopy(prototype);
		else
			SelfSharedCopy(prototype);
	}
	else //  !prototype.Instanciate()
		SelfCopyPrototype(prototype);
}

DynamicType::DynamicType(const DynamicType& prototype)
{
	typeDescTable = prototype.typeDescTable;

	numActiveAttr = 0;
	data = 0;
	dynamicTable=0;
	dataSize = 0;
	allocatedDataSize = 0;
	bPreAllocateAllAttributes = prototype.bPreAllocateAllAttributes;


	if (prototype.IsInstanciate())
		SelfDeepCopy(prototype);
	else
		SelfCopyPrototype(prototype);

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT
}


DynamicType::~DynamicType()
{
	RemoveAllMem();
}

void DynamicType::RemoveAllMem()
{
#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

	if (data && bOwnsItsMemory)
	{
		for (unsigned i=0; i<numAttr; i++) 
			if (AttrHasData(i))
			{
				t_destructor dest = typeDescTable[i].destructObj;
				dest (data+dynamicTable[i].offs);
			}
	}
	if (data) 
	{
		delete [] data;
	}
	if (dynamicTable) {
		DecrementDynTableRefCounter();
		if (DynTableRefCounter() == 0)
			delete [] dynamicTable;
	}
}


void DynamicType::InformAttr_(unsigned val, const char* name, unsigned size, const char* type, const bool isPtr,
                            const t_new fnew, const t_new_copy fcopy, const t_destructor fdestr)
{
	CLAM_ASSERT(val<numAttr, 
		"There are more registered Attributes than the "
		"number defined in DYN_CLASS_TABLE macro.");
	CLAM_ASSERT(fnew, "in DT: a dynamic attribute don't have default-constructor !");
	CLAM_ASSERT(fcopy, "in DT: a dynamic attribute don't have copy constructor !");

	strcpy(typeDescTable[val].id, name);
	strcpy(typeDescTable[val].type, type);
	typeDescTable[val].isPointer = isPtr;
	typeDescTable[val].size = size;
	// default value. This field is used in UpdateData in Fixed offsets mode.
	typeDescTable[val].offset = -1;  
	// references to creation/destruction fuctions of the type/class
	typeDescTable[val].newObj = fnew;
	typeDescTable[val].newObjCopy = fcopy;
	typeDescTable[val].destructObj = fdestr;
	// informative flags:
	// flags that will be set at the AddTypedAttr_ 
	// (the overloaded function that calls this one)
	typeDescTable[val].isComponent = false;
	typeDescTable[val].isStorable = false;
	typeDescTable[val].isDynamicType = false;

}
/////////////////////////////////////////////////////////////////////////////////////////////
// Main memory management methods: AddAttr_, RemoveAttr_ and UpdateData


void DynamicType::AddAttr_ (const unsigned val, const unsigned size)
{
	// first we check if there is need to adding the attribute
	TDynInfo &inf = dynamicTable[val];

	if (inf.hasBeenAdded) 
		return;

	if (inf.hasBeenRemoved)
	{
		inf.hasBeenRemoved = false;
		++numActiveAttr;
		dataSize += size;

		// check if we can unset the global some-removed flag.
		dynamicTable[numAttr].hasBeenRemoved = false;
		for (unsigned int j=0; j<numAttr; j++) {
			if (dynamicTable[j].hasBeenRemoved) {
				dynamicTable[numAttr].hasBeenRemoved = true;
				break;
			}
		}
#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

		return;
	}
	if (AttrHasData(val)) return;
	
	// At this point, the actual attribute-adding is necessary

	if (DynTableRefCounter() > 1) // then this object is different from the prototye that gave its shape
	{  // so create a new dynamicTable
		DecrementDynTableRefCounter();
		TDynInfo *oldTable = dynamicTable;
		dynamicTable = new TDynInfo[numAttr + 1];
		memcpy(dynamicTable, oldTable, sizeof(TDynInfo)*(numAttr+1));
		InitDynTableRefCounter();
		// dont delete the oldTable: it's still used by at least its prototype
	}

	++numActiveAttr;
	dataSize += size;
	dynamicTable[val].hasBeenAdded = true;
	dynamicTable[numAttr].hasBeenAdded = true; //this is a global (for all attribute) flag that means that Update is necessary
	// at this point the data and dynamicTable may contain gaps, 
	// but they will be compacted at Update() time.

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

}

//////////////////////////////////////////////////////////////////////////////////////////////////7
void DynamicType::RemoveAttr_(const unsigned i)
{
	TDynInfo &inf = dynamicTable[i];

	if (inf.hasBeenRemoved) return;

	if (inf.hasBeenAdded) 
	{
		inf.hasBeenAdded=false;
		--numActiveAttr;
		dataSize -= typeDescTable[i].size;
		
		// check if we can unset the global some-added flag.
		dynamicTable[numAttr].hasBeenAdded = false;
		for (unsigned int j=0; j<numAttr; j++) {
			if (dynamicTable[j].hasBeenAdded) {
				dynamicTable[numAttr].hasBeenAdded = true;
				break;
			}
		}
#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

		return;
	}
	if (!AttrHasData(i) || !data) return;

	// at this point the actual attribute-deletion has to be done.
	// but the actual deletion will take place at UpdateData() time.
	
	if (DynTableRefCounter() > 1) // then this object is different from the prototye that gave its shape
	{  // so create a new dynamicTable
		DecrementDynTableRefCounter();
		TDynInfo *oldTable = dynamicTable;
		dynamicTable = new TDynInfo[numAttr + 1];
		memcpy(dynamicTable, oldTable, sizeof(TDynInfo)*(numAttr+1));
		InitDynTableRefCounter();
		// dont delete the oldTable: it's still used by at least its prototype
	}

	--numActiveAttr;
	dataSize -= typeDescTable[i].size;
	dynamicTable[i].hasBeenRemoved = 1;
	dynamicTable[numAttr].hasBeenRemoved = 1; // global flag that means Update necessary;

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

}


//////////////////////////////////////////////////////////////////////////////////////////////////7
// return whether some update has been made.
bool DynamicType::UpdateData()
{

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

	if (!bOwnsItsMemory ) // if !bOwnsItsMemory then it will own it from that point.
	{
		BeMemoryOwner();
		return true;
	}

	// if no AddXXX or RemoveXXX has been done then the update is not necessary
	if (!dynamicTable[numAttr].hasBeenAdded && !dynamicTable[numAttr].hasBeenRemoved) 
		return false; 

	// at this point. some Add / Remove has been done. 
	
	if (bPreAllocateAllAttributes) dataSize = maxAttrSize;
	
	if (dataSize <= allocatedDataSize && int(allocatedDataSize-dataSize) > shrinkThreshold)  
		// this "shrinkThreshold" constant  decides when to 
		// reallocate (and shrink or _compact_) memory
	{
		UpdateDataByShrinking();
		return true;
	} else if (dataSize==maxAttrSize && allocatedDataSize<maxAttrSize) 
		// it's the first that dataSize reach the maximum. (probably by the use of bPreAllocatedAllAttr flag.
		// now the offsets will be taken from the static table 
	{
		UpdateDataGoingToPreAllocatedMode();
		return true;

	} else if (dataSize==maxAttrSize && dataSize<=allocatedDataSize) 
	{
		// in this PreAllocatedMode the attr. offsets are fixed by the static table.
		UpdateDataInPreAllocatedMode();
		return true;
	}
	
	// else: memory has increasead or the amount decreased is bigger than the threshold
	// so do it in the STANDARD MODE (reallocate and compact memory)
	UpdateDataByStandardMode();

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
/** Updata support function */
void DynamicType::BeMemoryOwner()
{
		bOwnsItsMemory = true;
		TDynInfo *originalTable = dynamicTable;
		char* originalData = data;
		data = new char[dataSize];
		if (DynTableRefCounter() > 1) // then this object is different from the prototye that gave its shape
		{  // so create a new dynamicTable
			DecrementDynTableRefCounter();
			dynamicTable = new TDynInfo[numAttr + 1];
			memcpy(dynamicTable, originalTable, sizeof(TDynInfo)*(numAttr+1));
			InitDynTableRefCounter();
			// dont delete the oldTable: it's still used by at least its prototype
		}

		unsigned offs=0;
		for(unsigned i=0; i<numAttr; i++)
			if ((AttrHasData(i) && !dynamicTable[i].hasBeenRemoved) || dynamicTable[i].hasBeenAdded) // owhterwise doesn't need allocation
			{

				t_new_copy fnewcp=typeDescTable[i].newObjCopy;
				fnewcp(data+offs, originalData+originalTable[i].offs);
				dynamicTable[i].offs = offs;
				dynamicTable[i].hasBeenAdded = dynamicTable[i].hasBeenRemoved = false;
				offs += typeDescTable[i].size;
			}
			else 
			{	
				dynamicTable[i].hasBeenRemoved = false;
				dynamicTable[i].offs = -1;
			}
	
		allocatedDataSize = dataSize;
		dynamicTable[numAttr].hasBeenAdded = dynamicTable[numAttr].hasBeenRemoved = false;
	
	
	}

/** SHRINK MODE: now we'll reuse the allocated data table.
 * two traversals: the first one is for moving the existing attributes:
 */
void DynamicType::UpdateDataByShrinking()
{
	
		std::list< std::pair<int,int> > attrList(numAttr);  
		std::list< std::pair<int,int> >::iterator it;

		unsigned int i=0;
		for (it=attrList.begin(); it!=attrList.end(); it++)
		{
			(*it).first = dynamicTable[i].offs;
			(*it).second = i++;
		}

		attrList.sort();

		unsigned offs=0;
		unsigned j; // ordered attribute indes
		for (it=attrList.begin(); it!=attrList.end(); it++)
		{
			j = (*it).second;
			if (AttrHasData(j) && !dynamicTable[j].hasBeenRemoved)
			{
				if (unsigned(dynamicTable[j].offs) != offs) // only move data if necessary
				{
					t_new_copy   newc  = typeDescTable[j].newObjCopy;
					t_destructor dest  = typeDescTable[j].destructObj;
					/** @todo: optimize for the case in which the intermediate
					 *Copy is not needed. */
					char* aux = new char[typeDescTable[j].size];
					newc(aux,data+dynamicTable[j].offs);
					dest(data+dynamicTable[j].offs);
					newc(data+offs,aux);
					dest(aux);
					delete [] aux;
					dynamicTable[j].offs = offs;
				}
				offs += typeDescTable[j].size;
			}
			else if (AttrHasData(j) && dynamicTable[j].hasBeenRemoved)
			{
				t_destructor dest = typeDescTable[j].destructObj;
				dest (data+dynamicTable[j].offs);
				
				dynamicTable[j].offs = -1;
				dynamicTable[j].hasBeenRemoved = false;
			}
		} 
		// now it's time for the new (added) attributes
		for (i=0; i<numAttr; i++)
		{
			if (dynamicTable[i].hasBeenAdded)
			{
				t_new fnew=typeDescTable[i].newObj;
				fnew(data+offs);
				dynamicTable[i].offs = offs;
				offs += typeDescTable[i].size;
				dynamicTable[i].hasBeenAdded = false;
			}
		}

		dynamicTable[numAttr].hasBeenRemoved = false;
		dynamicTable[numAttr].hasBeenAdded = false;
}

//  STANDARD MODE (reallocate and compact memory)
void DynamicType::UpdateDataByStandardMode ()
{
	char* olddata = data;
	data = new char[dataSize];

	unsigned offs=0;
	for (unsigned int i=0; i<numAttr; i++)
	{
		TDynInfo & inf = dynamicTable[i];
		if (AttrHasData(i)) 
		{
			if (dynamicTable[i].hasBeenRemoved) 
			{
				t_destructor dest = typeDescTable[i].destructObj;
				dest (olddata+inf.offs);
				inf.hasBeenRemoved = false;
				inf.offs = -1;
			}
			else 
			{
				t_new_copy   newc = typeDescTable[i].newObjCopy;
				t_destructor dest = typeDescTable[i].destructObj;
				newc(data+offs,olddata+inf.offs);
				dest(olddata+inf.offs);
				inf.offs = offs;
				offs += typeDescTable[i].size;
			}
		}
		else  // !AttrHasData(i)
		{
			if (inf.hasBeenAdded)
			{
				t_new fnew=typeDescTable[i].newObj;
				fnew(data+offs);
				inf.hasBeenAdded = false;
				inf.offs = offs;
				offs += typeDescTable[i].size;
			}

		}
				
	} // for

		
	delete [] olddata;
	allocatedDataSize = dataSize;
	dynamicTable[numAttr].hasBeenAdded = dynamicTable[numAttr].hasBeenRemoved = false;
}

void DynamicType::UpdateDataGoingToPreAllocatedMode()
{
	// the last reallocation:
	char* olddata = data;
	data = new char[maxAttrSize];
	unsigned int i;
	// from now one we'll use these pre-fixed offsets.
	// the copy of attributes:
	for (i=0; i<numAttr; i++)
	{
		TDynInfo & inf = dynamicTable[i];
		int offs = typeDescTable[i].offset;
		if (AttrHasData(i)) 
		{
			if (dynamicTable[i].hasBeenRemoved) 
			{
				t_destructor dest = typeDescTable[i].destructObj;
				dest (olddata+inf.offs);
				inf.hasBeenRemoved = false;
				inf.offs = -1;
			}
			else 
			{
				t_new_copy   newc = typeDescTable[i].newObjCopy;
				t_destructor dest = typeDescTable[i].destructObj;
				
				newc(data+offs,olddata+inf.offs);
				dest(olddata+inf.offs);
				inf.offs = offs;
			}
		}
		else  // !AttrHasData(i)
		{
			if (inf.hasBeenAdded)
			{
				t_new fnew=typeDescTable[i].newObj;
				fnew(data+offs);
				inf.hasBeenAdded = false;
				inf.offs = offs;
			}
		}
				
	} // for each attribute.

	delete [] olddata;
	allocatedDataSize = dataSize;
	dynamicTable[numAttr].hasBeenAdded = dynamicTable[numAttr].hasBeenRemoved = false;
	
}

void DynamicType::UpdateDataInPreAllocatedMode()
{
	// now, no reallocation.
	// we'll use these pre-fixed offsets. 
	// we need no attributes-copies. only creations and destructions:
	for (unsigned int i=0; i<numAttr; i++)
	{
		TDynInfo & inf = dynamicTable[i];
		int offs = typeDescTable[i].offset;
		if (AttrHasData(i)) 
		{
			if (dynamicTable[i].hasBeenRemoved) 
			{
				t_destructor dest = typeDescTable[i].destructObj;
				dest (data+inf.offs);
				inf.hasBeenRemoved = false;
				inf.offs = -1;
			}
			// else leave the attribute in peace.
		}
		else  // !AttrHasData(i)
		{
			if (inf.hasBeenAdded)
			{
				t_new fnew=typeDescTable[i].newObj;
				fnew(data+offs);
				inf.hasBeenAdded = false;
				inf.offs = offs;
			}
		}
				
	} // for each attribute.

	dynamicTable[numAttr].hasBeenAdded = dynamicTable[numAttr].hasBeenRemoved = false;
	
}

/////////////////////////////////////////////////////////////////////////////////////////////
// these methods are important to be inline. TODO: check if the compiler does it automatically
// or should be better move them to the .hxx

int DynamicType::DynTableRefCounter()
{
	return dynamicTable[numAttr].offs;
}
void DynamicType::InitDynTableRefCounter()
{
	dynamicTable[numAttr].offs = 1;       //at least the object that has created the table points to it.
}





int DynamicType::DecrementDynTableRefCounter()
{
	return --dynamicTable[numAttr].offs;
}
int DynamicType::IncrementDynTableRefCounter()
{
	return ++dynamicTable[numAttr].offs;
}


//////////////////////////////////////////////////////////////////////
// Component interface implementation
//////////////////////////////////////////////////////////////////////

Component* DynamicType::ShallowCopy() const
{
	DynamicType* selfCopy = &(GetDynamicTypeCopy(false,false));

	return selfCopy;
}


Component* DynamicType::DeepCopy() const
{	
	DynamicType* selfCopy = &(GetDynamicTypeCopy(false,true));

	return selfCopy;
};


void DynamicType::SelfCopyPrototype(const DynamicType &prototype)
{
	RemoveAllMem(); // deletes all mem in data and calls de destructor of every object.

	numAttr = prototype.numAttr;
	numActiveAttr = prototype.numActiveAttr;
	dataSize = prototype.dataSize;
	allocatedDataSize = prototype.allocatedDataSize;
	maxAttrSize = prototype.maxAttrSize;
	dynamicTable = prototype.dynamicTable;
	data=0;
	bOwnsItsMemory=true;
	bPreAllocateAllAttributes = prototype.bPreAllocateAllAttributes;
	IncrementDynTableRefCounter();
}

void DynamicType::SelfSharedCopy(const DynamicType &prototype)
{
	SelfCopyPrototype(prototype);

	data = prototype.data;

	bOwnsItsMemory = false;
}

void DynamicType::SelfShallowCopy(const DynamicType &prototype)
{
	CLAM_ASSERT( 
		!prototype.dynamicTable[prototype.numAttr].hasBeenAdded &&	
		!prototype.dynamicTable[prototype.numAttr].hasBeenRemoved,
		"making a copy of a non-updated DT is not allowed since the copy share the same dynamic-info"
	);
	if (this==&prototype) return;

	SelfCopyPrototype(prototype);

	data = new char[allocatedDataSize];

	unsigned int i;
	for (i = 0; i < numAttr; i++)
	{
		if (!ExistAttr(i)) continue;
		void* pos = GetPtrToData_(i);
		t_new_copy fcopy = typeDescTable[i].newObjCopy;
		fcopy(pos, prototype.GetPtrToData_(i));
	}
}

void DynamicType::SelfDeepCopy(const DynamicType &prototype)
{
	CLAM_ASSERT( 
		!prototype.dynamicTable[prototype.numAttr].hasBeenAdded &&	
		!prototype.dynamicTable[prototype.numAttr].hasBeenRemoved,
		"making a copy of a non-updated DT is not allowed since the copy share the same dynamic-info"
	);
	if (this==&prototype) return;
		
	SelfCopyPrototype(prototype);

	data = new char[allocatedDataSize];

	// Copies (deepCopy) all the objects pointed by this dynamic type that derives from
	// Component. Copies this object and then link the copy of this, with the
	// children copies.
	Component** copyChildren = new Component * [prototype.numAttr];   // could be done without a table if space efficency is needed
	unsigned int i;
	for (i = 0; i < numAttr; i++)
	{
		if (prototype.ExistAttr(i) && typeDescTable[i].isComponent && typeDescTable[i].isPointer)
			copyChildren[i] = static_cast<Component*>(prototype.GetDataAsPtr_(i))->DeepCopy();
		else
			copyChildren[i] = 0;
	}

	for (i = 0; i < numAttr; i++)
	{
		if (!ExistAttr(i)) continue;
		void* pos = GetPtrToData_(i);
		if(copyChildren[i])
			SetDataAsPtr_(i, copyChildren[i]);
		else
		{
			//now a nested object must be replaced. It maight be a pointer not registered as it.
			//the nested object will be copied from the nested object at "this"
			t_new_copy fcopy = typeDescTable[i].newObjCopy;
			fcopy(pos, prototype.GetPtrToData_(i));
		}
	}
	delete [] copyChildren;
}

/////////////////////////////////////////////////////////////////////////////////////////////

DynamicType& DynamicType::operator=(const DynamicType& source)
{
	SelfDeepCopy(source);

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

	return *this;
}



/////////////////////////////////////////////////////////////////////////////////////77
// Developing aids methods: FullfilsInvariant and Debug

void DynamicType::FullfilsInvariant() const
{
	if (!typeDescTable) 
		throw ErrDynamicType("in FullfilsInvariant: there's no typeDescTable. "
							 "The most likely thing is that the object	is "
							 "created with a non-macro-expanded-constructor."
							 "In that case the constructor MUST call the"
							 "MandatoryInit() method (called from constructor). Check it !");

	if (!dynamicTable)
		return;

	unsigned auxAllocatedSize=0;
	bool someAdded = false, someRemoved = false;
	int incData=0, decData=0;
	bool *usedblock = new bool[allocatedDataSize];
	
	for (unsigned j=0; j<allocatedDataSize; j++) usedblock[j] = false;

	for (unsigned i=0; i<numAttr; i++)
	{
		TDynInfo & dyninfo = dynamicTable[i];
		
		// check state consistency.
		if (dyninfo.hasBeenAdded && dyninfo.hasBeenRemoved) 
			throw ErrDynamicType("in FullfilsInvariant: an attribute has both Added & Removed flags set. Class: ", GetClassName() );

		if (dyninfo.hasBeenAdded) someAdded = true;
		if (dyninfo.hasBeenRemoved) someRemoved = true;

		if (dyninfo.offs < -1) 
			throw ErrDynamicType("in FullfilsInvariant: a dynamic offset < -1");
		if( !AttrHasData(i) && dyninfo.hasBeenRemoved)  
			throw ErrDynamicType(" in FullfilsInvariant: an attribute has\
				no data (offs==-1) but do has the hasBeenRemoved flag set. Class: ", GetClassName() );
		
		if( AttrHasData(i) && dyninfo.hasBeenAdded)  
			throw ErrDynamicType(" in FullfilsInvariant: an attribute has\
				data (offs>0) but do has the hasBeenAdded flag set. Class: ", GetClassName() );
		// data size calculation
		if (dyninfo.offs >= 0) 
		{
			auxAllocatedSize += typeDescTable[i].size;
			for (unsigned j=unsigned(dyninfo.offs); j<unsigned(dyninfo.offs+typeDescTable[i].size); j++)
				if (usedblock[j]) throw ErrDynamicType("in FullfilsInvariant: overlaped area in data table");
				else usedblock[j]=true;
		}
		if (AttrHasData(i)) 
		{
			if (dyninfo.hasBeenRemoved) decData += typeDescTable[i].size;
			if (!data) throw ErrDynamicType("in FullfilsInvariant: An attr. has data but data==0");
		}
		else 
			if (dyninfo.hasBeenAdded) incData += typeDescTable[i].size;
		
		else if (dyninfo.offs != -1) 
			throw ErrDynamicType(" in FullfilsInvariant: attribute not informed with dynamic offset <> -1");

	}
	if (!bPreAllocateAllAttributes) {
		if (auxAllocatedSize+incData-decData != dataSize) 
			throw ErrDynamicType("in FullfilsInvariant: dataSize attribute is not consistent. Class: ", GetClassName() );
		if (auxAllocatedSize + incData - decData != dataSize)
			throw ErrDynamicType("in FullfilsInvariant: the dataSize is not well calculated. Class: ", GetClassName() );
	}
	if (auxAllocatedSize > allocatedDataSize) 
		throw ErrDynamicType("in FullfilsInvariant: allocatedDataSize attribute is not consistent. Class: ", GetClassName() );
	if (dynamicTable[numAttr].hasBeenAdded != someAdded) 
		throw ErrDynamicType("in FullfilsInvariant: global 'hasBeenAdded' flag inconsistent. Class: ", GetClassName() );
	if (dynamicTable[numAttr].hasBeenRemoved != someRemoved) 
		throw ErrDynamicType("in FullfilsInvariant: global 'hasBeenRemoved' flag inconsistent. Class: ", GetClassName() );
	
	delete[] usedblock;
}


void DynamicType::Debug() const
{
	TAttr * attr = 0;
	std::cout <<std::endl<<"Class Name: "<< GetClassName() << " at: " << this <<std::endl << "[#attr.], dyn_offs,statc_offs,name,type,{comp,dynType,ptr,strble},exist,size,Ptr"\
		<< std::endl << "------------------------------------------------------------------------------"<<std::endl;
	std::cout << "{ size, allocatedSize, maxAttrsSize } = { " << dataSize << " , " << allocatedDataSize << " , "
			<< maxAttrSize << " }\n";
	for (unsigned i=0; i<numAttr; i++)
	{
		TDynInfo & dyninf = dynamicTable[i];

		attr = &typeDescTable[i];
		std::cout << std::endl;
		if (dyninf.hasBeenAdded) std::cout << " A";
		else std::cout << " -";
		if (dyninf.hasBeenRemoved) std::cout << "R";
		else std::cout << "-";
		

		std::cout << " [" <<i<<"] ";

		std::cout << dyninf.offs << " , "<<attr->offset<<" , "<<attr->id<<" , "<<attr->type<<" , {"\
			<<attr->isComponent<<","<<attr->isDynamicType<<","<<attr->isPointer<<","\
			<<attr->isStorable<<"} , "<<ExistAttr(i)<<" , "<<attr->size\
			<<" , ";
		if(ExistAttr(i)) 
			std::cout << GetPtrToData_(i);
		
		if(attr->isPointer && ExistAttr(i)) std::cout << " points -> " << GetDataAsPtr_(i);
	}
	std::cout<<std::endl;

	// Only dump when CLAM_USE_XML defined
	#ifdef CLAM_USE_XML
	XMLStorage::Dump(*this, GetClassName(), "Debug.xml");
	#endif// CLAM_USE_XML
}

}; //namespace CLAM

