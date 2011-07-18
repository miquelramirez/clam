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

#include "ErrDynamicType.hxx"

// IMPORTANT: this next flag will slow all the dynamic types mechanisms
// So it can only be set (defined) when developing, testing or maintaining
// dynamic types.

// #define CLAM_EXTRA_CHECKS_ON_DT

// Anyway this flag should be defined in the project/makefile of the test.


// TODO: strcpy is nos safe for name and typeName
// TODO: shallow and deep copies has no sense as long as we don't have pointers
// TODO: Review in which cases shared prototype is useful
// TODO: Review in which cases shared data is useful
// TODO: Is there any need of precomputing fixed offsets in preallocated mode

namespace CLAM {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DynamicType::DynamicType(const int nAttr, TAttr * attributeTable)
	: _typeDescTable(attributeTable)
	, _numAttr(nAttr)
	, _data(0)
	, _dynamicTable(new TDynInfo[_numAttr + 1])
	, _maxAttrSize(_typeDescTable[_numAttr].offset)
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
	, _data(0)
	, _dynamicTable(0)
	, _maxAttrSize(prototype._maxAttrSize)
	, _allocatedDataSize(0)
	, _preallocateAllAttributes(prototype._preallocateAllAttributes)
	, _attributesNeedingUpdate(0)
{

	if (prototype._data)
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

	if (_data)
	{
		for (unsigned i=0; i<_numAttr; i++) 
		{
			if (not AttrHasData(i)) continue;
			t_destructor dest = _typeDescTable[i].destructObj;
			dest (_data+_dynamicTable[i].offs);
		}
	}
	if (_data) delete [] _data;
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
	for (unsigned i=0; i<=_numAttr; i++)
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

	for (unsigned i=0; i<_numAttr; i++)
	{
		TDynInfo & inf = _dynamicTable[i];
		int offs = _typeDescTable[i].offset; // Fixed offset
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
			t_destructor dest = _typeDescTable[i].destructObj;
			dest (olddata+inf.offs);
			inf.hasBeenRemoved = false;
			inf.offs = -1;
		}
		else 
		{
			t_new_copy   copy = _typeDescTable[i].newObjCopy;
			t_destructor dest = _typeDescTable[i].destructObj;

			copy(_data+offs, olddata+inf.offs);
			dest(olddata+inf.offs);
			inf.offs = offs;
		}
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
	for (unsigned int i=0; i<_numAttr; i++)
	{
		TDynInfo & inf = _dynamicTable[i];
		int offs = _typeDescTable[i].offset;
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
	_dynamicTable[_numAttr].offs = 1;       //at least the object that has created the table points to it.
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
	RemoveAllMem(); // deletes all mem in _data and calls de destructor of every object.

	_data=0;
	_dynamicTable = prototype._dynamicTable;
	_maxAttrSize = prototype._maxAttrSize;
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
		
	SelfCopyPrototype(prototype);

	_data = new char[_allocatedDataSize];

	// Copies (deepCopy) all the objects pointed by this dynamic type that derives from
	for (unsigned i=0; i<_numAttr; i++)
	{
		if (!ExistAttr(i)) continue;
		void* pos = GetPtrToData_(i);
		//now a nested object must be replaced. It maight be a pointer not registered as it.
		//the nested object will be copied from the nested object at "this"
		t_new_copy copy = _typeDescTable[i].newObjCopy;
		copy(pos, prototype.GetPtrToData_(i));
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
	if (!_typeDescTable) 
		throw ErrDynamicType("in FullfilsInvariant: there's no typeDescTable. "
							 "The most likely thing is that the object	is "
							 "created with a non-macro-expanded-constructor."
							 "In that case the constructor MUST call the"
							 "MandatoryInit() method (called from constructor). Check it !");

	if (!_dynamicTable)
		return;

	unsigned auxAllocatedSize=0;
	unsigned nAdded = 0;
	unsigned nRemoved = 0;
	int incData=0, decData=0;
	bool usedblock[_allocatedDataSize];
	
	for (unsigned j=0; j<_allocatedDataSize; j++) usedblock[j] = false;

	for (unsigned i=0; i<_numAttr; i++)
	{
		TDynInfo & dyninfo = _dynamicTable[i];
		
		// check state consistency.
		if (dyninfo.hasBeenAdded && dyninfo.hasBeenRemoved) 
			throw ErrDynamicType("in FullfilsInvariant: an attribute has both Added & Removed flags set. Class: ", GetClassName() );

		if (dyninfo.hasBeenAdded) nAdded++;
		if (dyninfo.hasBeenRemoved) nRemoved++;

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
			auxAllocatedSize += _typeDescTable[i].size;
			for (unsigned j=unsigned(dyninfo.offs); j<unsigned(dyninfo.offs+_typeDescTable[i].size); j++)
				if (usedblock[j]) throw ErrDynamicType("in FullfilsInvariant: overlaped area in data table");
				else usedblock[j]=true;
		}
		if (AttrHasData(i)) 
		{
			if (dyninfo.hasBeenRemoved) decData += _typeDescTable[i].size;
			if (!_data) throw ErrDynamicType("in FullfilsInvariant: An attr. has data but _data==0");
		}
		else 
			if (dyninfo.hasBeenAdded) incData += _typeDescTable[i].size;
		
		else if (dyninfo.offs != -1) 
			throw ErrDynamicType(" in FullfilsInvariant: attribute not informed with dynamic offset <> -1");
	}
	if (auxAllocatedSize > _allocatedDataSize) 
		throw ErrDynamicType("in FullfilsInvariant: allocatedDataSize attribute is not consistent. Class: ", GetClassName() );
	if ((nAdded + nRemoved) != _attributesNeedingUpdate)
		throw ErrDynamicType("in FullfilsInvariant: _attributesNeedingUpdate is not consistent with hasBeenAdded/Removed flags. Class: ", GetClassName() );
}


void DynamicType::Debug() const
{
	TAttr * attr = 0;
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

		attr = &_typeDescTable[i];
		std::cout
			<< " "
			<< (dyninf.hasBeenAdded? "A" : "-")
			<< (dyninf.hasBeenRemoved? "R" : "-" )
			<< " [" <<i<<"] "
			<< dyninf.offs << " ,"
			<< attr->offset << " ,"
			<< attr->id << ", "
			<< attr->type << ", {"
			<< attr->isComponent << ", "
			<< attr->isDynamicType << ", "
			<< ExistAttr(i) << ", "
			<< attr->size << ", "
			<< (ExistAttr(i)? GetPtrToData_(i):"X")
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
	int offset,
	t_new constructor,
	t_new_copy copyConstructor,
	t_destructor destructor
	)
{
	strcpy(attributeTable[index].id, name);
	strcpy(attributeTable[index].type, typeName);
	attributeTable[index].size = size;
	// default value. This field is used in UpdateData in Fixed offsets mode.
	attributeTable[index].offset = offset;
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

