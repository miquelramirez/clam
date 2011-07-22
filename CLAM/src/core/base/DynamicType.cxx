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
#include <vector> // needed in UpdateData 
#include <algorithm> // needed in UpdateData 

#include "XMLStorage.hxx"

// IMPORTANT: this next flag will slow all the dynamic types mechanisms
// So it can only be set (defined) when developing, testing or maintaining
// dynamic types.

#define CLAM_EXTRA_CHECKS_ON_DT

// Anyway this flag should be defined in the project/makefile of the test.


// TODO: strcpy is nos safe for name and typeName
// TODO: Review in which cases shared prototype is useful

namespace CLAM {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DynamicType::DynamicType(const int nAttr, TAttr * attributeTable)
	: _typeDescTable(attributeTable)
	, _numAttr(nAttr)
	, _maxAttrSize(_typeDescTable[_numAttr].size)
	, _dynamicTable(new TDynInfo[_numAttr + 1])
	, _data(0)
	, _allocatedDataSize(0)
	, _preallocateAllAttributes(false)
	, _attributesNeedingUpdate(0)
{
	for ( unsigned i=0; i < _numAttr; i++)
	{
		_dynamicTable[i].offs = -1;
		_dynamicTable[i].hasBeenAdded = false;
		_dynamicTable[i].hasBeenRemoved = false;
	}
	InitDynTableRefCounter();
}

DynamicType::DynamicType(const DynamicType& prototype)
	: _typeDescTable(prototype._typeDescTable)
	, _numAttr(prototype._numAttr)
	, _maxAttrSize(prototype._maxAttrSize)
	, _dynamicTable(0)
	, _data(0)
	, _allocatedDataSize(0)
	, _preallocateAllAttributes(prototype._preallocateAllAttributes)
	, _attributesNeedingUpdate(0)
{

	SelfDeepCopy(prototype);

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

	if (_data)
	{
		for (unsigned i=0; i<_numAttr; i++) 
		{
			if (not AttrHasData(i)) continue;
			t_destructor dest = _typeDescTable[i].destructObj;
			dest (_data+_dynamicTable[i].offs);
		}
		delete [] _data;
	}
	if (_dynamicTable)
	{
		DecrementDynTableRefCounter();
		if (DynTableRefCounter() == 0)
			delete [] _dynamicTable;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////
// Main memory management methods: AddAttribute, RemoveAttribute and UpdateData


void DynamicType::AddAttribute (const unsigned i)
{
	TDynInfo &inf = _dynamicTable[i];

	if (inf.hasBeenAdded) return; // No need to add again

	if (inf.hasBeenRemoved)
	{
		// Undoing a previous pending removal
		inf.hasBeenRemoved = false;
		_attributesNeedingUpdate--;

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

		return;
	}
	if (AttrHasData(i)) return; // Already instantiated, no change

	// Need to modify the dynamic table, copy it if it is shared
	CopyOnWriteDynamicTable();

	_dynamicTable[i].hasBeenAdded = true;
	_attributesNeedingUpdate++;

	// At this point a UpdateData call is needed

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

}

//////////////////////////////////////////////////////////////////////////////////////////////////
void DynamicType::RemoveAttribute(const unsigned i)
{
	TDynInfo &inf = _dynamicTable[i];

	if (inf.hasBeenRemoved) return; // No need to remove again

	if (inf.hasBeenAdded) 
	{
		// Undoing a previous pending addition
		inf.hasBeenAdded=false;
		_attributesNeedingUpdate--;

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

		return;
	}
	if (!AttrHasData(i)) return; // Already deinstantiated, no change
	if (!_data) return; // No attribute instantiated at all

	// Need to modify the dynamic table, copy it if it is shared
	CopyOnWriteDynamicTable();

	_dynamicTable[i].hasBeenRemoved = true;
	_attributesNeedingUpdate++;

	// At this point a UpdateData call is needed

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

}

void DynamicType::CopyOnWriteDynamicTable()
{
	if (DynTableRefCounter() > 1) // then this object is different from the prototye that gave its shape
	{  // so create a new _dynamicTable
		DecrementDynTableRefCounter();
		TDynInfo *oldTable = _dynamicTable;
		_dynamicTable = new TDynInfo[_numAttr + 1];
		memcpy(_dynamicTable, oldTable, sizeof(TDynInfo)*(_numAttr+1));
		InitDynTableRefCounter();
		// dont delete the oldTable: it's still used by at least its prototype
	}
}

void DynamicType::AddAll()
{
	unsigned nAttributes = GetNDynamicAttributes();
	for (unsigned i=0; i<nAttributes; i++)
		AddAttribute(i);
}

void DynamicType::RemoveAll()
{
	unsigned nAttributes = GetNDynamicAttributes();
	for (unsigned i=0; i<nAttributes; i++)
		RemoveAttribute(i);
}


unsigned DynamicType::RequestedSize() const
{
	if (_preallocateAllAttributes) return _maxAttrSize;
	unsigned requestedSize = 0;
	for (unsigned i=0; i<_numAttr; i++)
	{
		if (_dynamicTable[i].hasBeenRemoved) continue;
		if (not AttrHasData(i) and not _dynamicTable[i].hasBeenAdded)
			continue;
		requestedSize+=_typeDescTable[i].size;
	}
	return requestedSize;
}

//////////////////////////////////////////////////////////////////////////////////////////////////7
// return whether some update has been made.
bool DynamicType::UpdateData()
{

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

	// Skip if no AddX or RemoveX methods are pending
	if (!_attributesNeedingUpdate) return false; 

	unsigned requestedSize = RequestedSize();

	if (requestedSize==_maxAttrSize) // Preallocated mode
	{
		// Different depending if we are just getting in to it or we already got
		if (_allocatedDataSize<_maxAttrSize)
			UpdateDataGoingToPreAllocatedMode();
		else
			UpdateDataInPreAllocatedMode();
	}
	else
	{
		// Just reuse existing memory if we don't need more memory
		// and the shrinking is small enough (shrinkThreshold constant)
		if (
			requestedSize <= _allocatedDataSize and 
			requestedSize + shrinkThreshold < _allocatedDataSize
		)
			UpdateDataByShrinking();
		else
			UpdateDataByReallocating(requestedSize);
	}

#	ifdef CLAM_EXTRA_CHECKS_ON_DT
		FullfilsInvariant();
#	endif //CLAM_EXTRA_CHECKS_ON_DT

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

/** SHRINK MODE: now we'll reuse the allocated data table.
 * two traversals: the first one is for moving the existing attributes:
 */
void DynamicType::UpdateDataByShrinking()
{
	std::vector< std::pair<int,unsigned> > sortedByOffset(_numAttr);  
	for (unsigned i=0; i<_numAttr; i++)
		sortedByOffset[i]=std::make_pair(_dynamicTable[i].offs, i);
	std::sort(sortedByOffset.begin(), sortedByOffset.end());

	unsigned offs=0;
	for (unsigned i=0; i<_numAttr; i++)
	{
		unsigned j = sortedByOffset[i].second;
		// First loop, just consider previously existing attributes
		if (not AttrHasData(j)) continue;
		TDynInfo & attrib = _dynamicTable[j];

		// Remove if marked as so
		if (attrib.hasBeenRemoved)
		{
			t_destructor dest = _typeDescTable[j].destructObj;
			dest (_data+attrib.offs);
			
			attrib.offs = -1;
			attrib.hasBeenRemoved = false;
			continue;
		}

		// If don't reallocate it, just if offset changed
		int size = _typeDescTable[j].size;
		if (unsigned(attrib.offs) != offs)
		{
			// New location, so move it
			t_new_copy   copy = _typeDescTable[j].newObjCopy;
			t_destructor dest = _typeDescTable[j].destructObj;
			char * newLocation = _data+offs;
			char * oldLocation =_data+attrib.offs;
			if (newLocation + size <= oldLocation)
			{
				// No overlap, no temporary needed
				copy(newLocation, oldLocation);
				dest(oldLocation);
			}
			else
			{
				// Overlap, need a temporary
				char tmpData[size];
				copy(tmpData, oldLocation);
				dest(oldLocation);
				copy(newLocation, tmpData);
				dest(tmpData);
			}
			attrib.offs = offs;
		}
		offs += size;
	} 
	// now it's time for the new (added) attributes
	for (unsigned i=0; i<_numAttr; i++)
	{
		TDynInfo & attrib = _dynamicTable[i];
		if (attrib.hasBeenAdded)
		{
			t_new fnew=_typeDescTable[i].newObj;
			fnew(_data+offs);
			attrib.hasBeenAdded = false;
			attrib.offs = offs;
			offs += _typeDescTable[i].size;
		}
	}
	_attributesNeedingUpdate=0;
}

//  STANDARD MODE (reallocate and compact memory)
void DynamicType::UpdateDataByReallocating (unsigned newSize)
{
	char* olddata = _data;
	_data = new char[newSize];

	unsigned offs=0;
	for (unsigned int i=0; i<_numAttr; i++)
	{
		TDynInfo & inf = _dynamicTable[i];
		if (not AttrHasData(i))
		{
			if (not inf.hasBeenAdded) continue; // Not there and still not, skip

			// Not there but has been added, add it
			t_new fnew=_typeDescTable[i].newObj;
			fnew(_data+offs);
			inf.hasBeenAdded = false;
			inf.offs = offs;
			offs += _typeDescTable[i].size;
		}
		else if (_dynamicTable[i].hasBeenRemoved) 
		{
			// There but has been removed, remove it
			t_destructor dest = _typeDescTable[i].destructObj;
			dest (olddata+inf.offs);
			inf.hasBeenRemoved = false;
			inf.offs = -1;
		}
		else
		{
			// There and still there, copy it
			t_new_copy   copy = _typeDescTable[i].newObjCopy;
			t_destructor dest = _typeDescTable[i].destructObj;
			copy(_data+offs, olddata+inf.offs);
			dest(olddata+inf.offs);
			inf.offs = offs;
			offs += _typeDescTable[i].size;
		}
	}
	delete [] olddata;
	_allocatedDataSize = newSize;
	_attributesNeedingUpdate=0;
}

void DynamicType::UpdateDataGoingToPreAllocatedMode()
{
	// the last reallocation:
	char* olddata = _data;
	_data = new char[_maxAttrSize];
	int offs = 0;
	for (unsigned i=0; i<_numAttr; i++)
	{
		TDynInfo & inf = _dynamicTable[i];
		if (not AttrHasData(i))
		{
			if (not inf.hasBeenAdded) continue; // Not there and still not, skip

			// Not there but has been added, add it
			t_new fnew=_typeDescTable[i].newObj;
			fnew(_data+offs);
			inf.hasBeenAdded = false;
			inf.offs = offs;
		}
		else if (_dynamicTable[i].hasBeenRemoved) 
		{
			// There, but has to be removed
			t_destructor dest = _typeDescTable[i].destructObj;
			dest (olddata+inf.offs);
			inf.hasBeenRemoved = false;
			inf.offs = -1;
		}
		else 
		{
			// There, has to be copied
			t_new_copy   copy = _typeDescTable[i].newObjCopy;
			t_destructor dest = _typeDescTable[i].destructObj;

			copy(_data+offs, olddata+inf.offs);
			dest(olddata+inf.offs);
			inf.offs = offs;
		}
		// Preallocated, so 
		offs += _typeDescTable[i].size;
	}
	delete [] olddata;
	_allocatedDataSize = _maxAttrSize;
	_attributesNeedingUpdate=0;
}

void DynamicType::UpdateDataInPreAllocatedMode()
{
	// now, no reallocation.
	// we'll use these pre-fixed offsets. 
	// we need no attributes-copies. only creations and destructions:
	int offs = 0;
	for (unsigned int i=0; i<_numAttr; i++)
	{
		TDynInfo & inf = _dynamicTable[i];
		if (AttrHasData(i)) 
		{
			if (_dynamicTable[i].hasBeenRemoved) 
			{
				t_destructor dest = _typeDescTable[i].destructObj;
				dest (_data+inf.offs);
				inf.hasBeenRemoved = false;
				inf.offs = -1;
			}
			// else leave the attribute in peace.
		}
		else  // !AttrHasData(i)
		{
			if (inf.hasBeenAdded)
			{
				t_new fnew=_typeDescTable[i].newObj;
				fnew(_data+offs);
				inf.hasBeenAdded = false;
				inf.offs = offs;
			}
		}
		offs += _typeDescTable[i].size;
	}

	_attributesNeedingUpdate=0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// these methods are important to be inline. TODO: check if the compiler does it automatically
// or should be better move them to the .hxx

int DynamicType::DynTableRefCounter()
{
	return _dynamicTable[_numAttr].offs;
}
void DynamicType::InitDynTableRefCounter()
{
	//at least the object that has created the table points to it.
	_dynamicTable[_numAttr].offs = 1;
}

int DynamicType::DecrementDynTableRefCounter()
{
	return --_dynamicTable[_numAttr].offs;
}
int DynamicType::IncrementDynTableRefCounter()
{
	return ++_dynamicTable[_numAttr].offs;
}


//////////////////////////////////////////////////////////////////////
// Component interface implementation
//////////////////////////////////////////////////////////////////////

Component* DynamicType::DeepCopy() const
{	
	DynamicType* selfCopy = &GetDynamicTypeCopy();
	return selfCopy;
};


void DynamicType::SelfCopyPrototype(const DynamicType &prototype)
{
	_data=0;
	_dynamicTable = prototype._dynamicTable;
	_allocatedDataSize = prototype._allocatedDataSize;
	_preallocateAllAttributes = prototype._preallocateAllAttributes;
	IncrementDynTableRefCounter();
}

void DynamicType::SelfDeepCopy(const DynamicType &prototype)
{
	CLAM_ASSERT( not prototype._attributesNeedingUpdate,
		"making a copy of a non-updated DT is not allowed since the copy share the same dynamic-info"
	);
	if (this==&prototype) return;

	RemoveAllMem(); // deletes all mem in _data and calls de destructor of every object.
	SelfCopyPrototype(prototype);

	if (not prototype._data) return;

	_data = new char[_allocatedDataSize];

	// Copies (deepCopy) all the objects pointed by this dynamic type that derives from
	for (unsigned i=0; i<_numAttr; i++)
	{
		if (!HasAttribute(i)) continue;
		//now a nested object must be replaced. It maight be a pointer not registered as it.
		//the nested object will be copied from the nested object at "this"
		t_new_copy copy = _typeDescTable[i].newObjCopy;
		copy(GetPtrToData_(i), prototype.GetPtrToData_(i));
	}
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
	CLAM_ASSERT( _typeDescTable,
		"in FullfilsInvariant: there's no typeDescTable. "
		"The most likely thing is that the object is "
		"created with a non-macro-expanded-constructor. "
		"Should pass the result of a call to the static "
		"method TypeDescriptionTable() generated by DT macros.");

	if (!_dynamicTable)
		return;

	unsigned auxAllocatedSize=0;
	unsigned nAdded = 0;
	unsigned nRemoved = 0;

	bool usedblock[_allocatedDataSize];
	for (unsigned j=0; j<_allocatedDataSize; j++) usedblock[j] = false;

	for (unsigned i=0; i<_numAttr; i++)
	{
		TDynInfo & dyninfo = _dynamicTable[i];

		CLAM_ASSERT( dyninfo.offs >= -1,
			"Offset should be -1 or a natural number");

		CLAM_ASSERT(not dyninfo.hasBeenAdded or not dyninfo.hasBeenRemoved,
			"An attribute has both Added and Removed flags set.");

		CLAM_ASSERT( AttrHasData(i) or not dyninfo.hasBeenRemoved,  
			"An attribute hasBeenRemoved but has no data (offs==-1)");

		CLAM_ASSERT( not AttrHasData(i) or not dyninfo.hasBeenAdded,  
			"An attribute hasBeenAdded but already had data (offs>0)");

		if (dyninfo.hasBeenAdded) nAdded++;
		if (dyninfo.hasBeenRemoved) nRemoved++;

		// data blocks usage
		if (dyninfo.offs >= 0) 
		{
			auxAllocatedSize += _typeDescTable[i].size;
			for (unsigned j=unsigned(dyninfo.offs); j<unsigned(dyninfo.offs+_typeDescTable[i].size); j++)
			{
				CLAM_ASSERT( not usedblock[j],
					"overlaped area in data table");
				usedblock[j]=true;
			}
		}
		if (AttrHasData(i))
			CLAM_ASSERT( _data,
				"An attribute is instanciated but no _data");
	}
	CLAM_ASSERT( auxAllocatedSize <= _allocatedDataSize,
			"allocatedDataSize attribute is not consistent.");
	CLAM_ASSERT( (nAdded + nRemoved) == _attributesNeedingUpdate,
			"_attributesNeedingUpdate is not consistent "
			"with hasBeenAdded/Removed flags.");
}


void DynamicType::Debug() const
{
	std::cout <<std::endl
		<< "Class Name: "<< GetClassName() 
		<< " at: " << this
		<< std::endl
		<< "{ allocatedSize, maxAttrsSize, pendingUpdates } = { "
		<< _allocatedDataSize << ", "
		<< _maxAttrSize << ", "
		<< _attributesNeedingUpdate << " }\n"
		<< std::endl
		<< "[#attr.], dyn_offs,statc_offs,name,type,{comp,dynType,ptr},exist,size,Ptr" << std::endl
		<< "--------------------------------------------------------------------------------" << std::endl;
	for (unsigned i=0; i<_numAttr; i++)
	{
		TDynInfo & dyninf = _dynamicTable[i];

		const TAttr * attr = &_typeDescTable[i];
		std::cout
			<< " "
			<< (dyninf.hasBeenAdded? "A" : "-")
			<< (dyninf.hasBeenRemoved? "R" : "-" )
			<< " [" <<i<<"] "
			<< dyninf.offs << " ,"
			<< attr->id << ", "
			<< attr->type << ", {"
			<< attr->isComponent << ", "
			<< attr->isDynamicType << ", "
			<< HasAttribute(i) << ", "
			<< attr->size << ", "
			<< (HasAttribute(i)? GetPtrToData_(i):"X")
			<< std::endl;
	}
	std::cout<<std::endl;

	// Only dump when CLAM_USE_XML defined
	#ifdef CLAM_USE_XML
	XMLStorage::Dump(*this, GetClassName(), "Debug.xml");
	#endif// CLAM_USE_XML
}

void DynamicType::AttributeTableSetFields_(
	TAttr * attributeTable, unsigned index,
	const char* name,
	const char* typeName,
	unsigned size,
	t_new constructor,
	t_new_copy copyConstructor,
	t_destructor destructor
	)
{
	strcpy(attributeTable[index].id, name);
	strcpy(attributeTable[index].type, typeName);
	attributeTable[index].size = size;
	// references to creation/destruction fuctions of the type/class
	attributeTable[index].newObj = constructor;
	attributeTable[index].newObjCopy = copyConstructor;
	attributeTable[index].destructObj = destructor;
}
void DynamicType::AttributeTableSetTypeFlags_(
	TAttr * attributeTable, unsigned index,
	const void * typedNullPointer)
{
	attributeTable[index].isComponent = false;
	attributeTable[index].isDynamicType = false;
}

void DynamicType::AttributeTableSetTypeFlags_(
	TAttr * attributeTable, unsigned index,
	const Component * typedNullPointer)
{
	attributeTable[index].isComponent = true;
	attributeTable[index].isDynamicType = false;
}

void DynamicType::AttributeTableSetTypeFlags_(
	TAttr * attributeTable, unsigned index,
	const DynamicType * typedNullPointer)
{
	attributeTable[index].isComponent = true;
	attributeTable[index].isDynamicType = true;
}


}; //namespace CLAM

