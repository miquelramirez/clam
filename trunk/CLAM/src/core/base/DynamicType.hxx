/* DynamicType.hxx: interface for the DynamicType class.
 * written by Pau Arumí - May 2001
 * new version (that stores every type) : 21-July-2001
 *
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

#ifndef _DynamicType_
#define _DynamicType_

#include "XMLAdapter.hxx"
#include "XMLIterableAdapter.hxx"
#include "XMLComponentAdapter.hxx"

#include "DynamicTypeMacros.hxx"  //this file is not included anywhere but here.

#include "Component.hxx"
#include "DataTypes.hxx"

#include <new>

namespace CLAM {

/////////////////////////////////////////////////////////////////////////////
// Class DynamicType declaration :
//
/////////////////////////////////////////////////////////////////////////////


/**
	This class implements a type that is dynamic. That is, it allows to add &
	remove fields or attributes at run time, optimizing this way the memory used.
	All the dynamic attributes are nevertheless perceived typed. So the compiler
	can garant the type consistency in every access to the dynamic attributes.
	It also allows herarchic structures and implements de Component interface
	so it can be stored all the tree (to XML format, for example)
	and can be copied (swallow or deep copy). (see the methods: SwallowCopy, 
	DeepCopy and StoreOn )
	<p>
	In this class there is implemented all the memory management, but is an abstract
	class: to work with dynamic types, is necessary to define a concrete dynamic 
	type (derives from this). A concrete dynamic type must be defined following 
	a very specific set of rules; basically the attributes are registered using a
	macros mechanism that expand a known interface for accessing attributes.
	<p>
	As these methods are expanded by macros, they can not be documented inside the
	concrete dynamic type. Hence they will be documented here:
	
	@see Component
 */
class DynamicType : public Component
{
protected:
	struct TAttr;
public:
	/**
		Constructs a DynamicType object that can hold @param nAttr attributes.
		<B>This constructor must be only used from the concrete dyn. type constructor.</B>
		This constructor creates a dynamic type that is a new prototype. That means
		that has its own dynamic information (which attrs. are instanciated, etc.)
		Furthermore, the new object is set owner of its memory.
	*/
	DynamicType(const int nAttr, TAttr * attributeTable);

	/**
	* Copy constructor of a dynamic Type.
	* <B>This constructor must be only used from the concrete dyn. type constructor.</B> 
	* The created object will use the dynamic type description of anotyer dynamic Type.
	* @param prototype Another dynamic type from which the dynamic shape is taken.
	* @param shareData Tells whether the new object will share the 
	* same data of the prototype, or not.
	*/

	DynamicType(const DynamicType& prototype, const bool shareData, const bool deepCopy);
	DynamicType(const DynamicType& prototype);
	virtual ~DynamicType();
	
	virtual const char* GetClassName() const =0;

	/**
	* Method used to resize the data space of the dynamic type, necessary when some
	* AddXXX() / RemoveXXX() (where XXX is an attribute name) has been done.
	* This operation does not check it if has been some attributes changes.
	* In the case that the object is "not owner" of its memory this flag is changed
	* to "owner", and a new data table is created.
	*
	* @return whether some modification has ocurred or not.
	*/
	bool UpdateData();
	
	/// Returns the number of dynamic attributes.
	unsigned GetNDynamicAttributes() const { return _numAttr; }

	/// Instantiates attribute at position i. Requires UpdateData() to be effective.
	void AddAttribute (const unsigned i);

	/// Deinstantiates attribute at position i. Requires UpdateData() to be effective.
	void RemoveAttribute (const unsigned i);

	/// Instantiates all attributes. Requires UpdateData() to be effective.
	void AddAll();

	/// Deinstantiates all attributes. Requires UpdateData() to be effective.
	void RemoveAll();

	/// Returns the name of the attribute at position i.
	const char * GetDynamicAttributeName(unsigned i) const { return _typeDescTable[i].id; }

	/// Returns the type_info of attribute at position i.
	virtual const std::type_info & GetTypeId(unsigned i) const=0;

	/// Returns true if the attribute at position i is a component.
	bool AttributeIsComponent(unsigned i) const {return _typeDescTable[i].isComponent; }

	/// Returns true if the attribute at position i is a dynamic type as well.
	bool AttributeIsDynamictype(unsigned i) const {return _typeDescTable[i].isDynamicType; }

	/// Returns true if the attribute at position i is instantiated.
	bool IsAttributeInstantiated(unsigned i) const {return _dynamicTable[i].offs!=-1; }

	/// Returns a void pointer to the data of the attribute at position 0.
	/// @pre the attribute must be instantiated (undefined behaviour if not)
	const void * GetAttributeAsVoidPtr(unsigned i) const {
		return GetPtrToData_(i);
	}

	/// Convenience accessor for GetAttributeAsVoidPtr that cast it to a component if it is so.
	/// @pre the attribute must be instantiated (undefined behaviour if not)
	const Component * GetAttributeAsComponent(unsigned i) const {
		if (!_typeDescTable[i].isComponent) return 0;
		return static_cast<const Component *> (GetPtrToData_(i));
	}

	/// Convenience accessor for GetAttributeAsVoidPtr that cast it to a component if it is so.
	/// @pre the attribute must be instantiated (undefined behaviour if not)
	Component * GetAttributeAsComponent(unsigned i) {
		if (! (_typeDescTable[i].isComponent)) return 0;
		return static_cast<Component *> (GetPtrToData_(i));
	}

	void FullfilsInvariant() const;

	/// Returns a deep copied object of the same class.
	/// Receiver has to handle memory.
	virtual Component * DeepCopy() const;

	/// Returns a default constructed object of the same class.
	/// Receiver has to handle memory.
	virtual Component * Species() const=0;

public:
	enum {idLength = 120, typeLength = 120}; //TODO: rise exception if the type is too long

protected:
	/**
		Redefine this method if you need to override the default
		behaviour when initializing a default constructed object.
		It is called at the end of the default constructor for 
		the concrete type, expanded by the DYNAMIC_TYPE macros.
		By default it does nothing.
		@see CopyInit()
	*/
	void DefaultInit() {}

	/**
		Redefine this method if you need to override the default
		behaviour when initializing a copy constructed object.
		It is called at the end of the copy constructor for 
		the concrete type, expanded by the DYNAMIC_TYPE macros.
		By default it does nothing.
		@see DefaultInit
	*/
	void CopyInit(const DynamicType & dt) {}

	/**
		You should NOT call or overwrite this method directly.
		It is called by UpdateData() and reimplemented by
		DYNAMIC_TYPE macros. It should create the table with
		the dynamic attributes information.
		@see UpdateData()
	*/
	virtual void InformAll()
	{
		// lets calculates the offsets of the "Pre allocated mode"
		CLAM_DEBUG_ASSERT(_typeDescTable,"static table don't exist. in DT::InformAll()");
		int adder=0;
		for (unsigned int i=0; i<_numAttr; i++)
		{
			_typeDescTable[i].offset = adder;
			adder += _typeDescTable[i].size;
		}
		maxAttrSize = adder;
	};

protected:
	// Types of the constructors and destructors that all registerd type must have.
	// A pointer to these functions is stored into the typeDescTable. (an array of TAttr) 
	// The definition of TAttr is following:
	typedef void* (*t_new)(void* pos);
	typedef void* (*t_new_copy)(void* pos,void* orig);
	typedef void (*t_destructor)(void* pos);

private:
	void static InformAttr_ (
		TAttr* attributeTable, unsigned id, const char* name, unsigned size, const char* type,
		const bool isPtr, const t_new, const t_new_copy, const t_destructor);

protected:
	inline static void InformTypedAttr_(
		TAttr* attributeTable, unsigned id, const char* name, unsigned size, const char *type,
		const bool isPtr, const t_new, const t_new_copy, const t_destructor,
		const Component* typedNullPointer);

	inline static void InformTypedAttr_(
		TAttr* attributeTable, unsigned id, const char* name, unsigned size, const char *type,
		const bool isPtr, const t_new, const t_new_copy, const t_destructor,
		const DynamicType* typedNullPointer);

	inline static void InformTypedAttr_(
		TAttr* attributeTable, unsigned id, const char* name, unsigned size, const char *type,
		const bool isPtr, const t_new, const t_new_copy, const t_destructor,
		const void* typedNullPointer);
	
protected:
	enum {shrinkThreshold = 80}; // Bytes.  That constant means that when updating data, if the
	                             // used data disminish an amount superior that this threshold,
	                             // data will be reallocated (shrunk)

	/// item of the typeDescTable, that is static created only once in the concrete class constructor
	struct TAttr
	{
		char id[idLength];                   
		char type[typeLength];
		int size;
		int offset;
		t_new newObj;
		t_new_copy newObjCopy;
		t_destructor destructObj;

		bool isComponent : 1; ///< Whether the attribute is a Component
		bool isStorable : 1; ///< Whether the attribute is basic storable (streamable)
		bool isDynamicType : 1; ///< Whether the attribute is a DynamicType or not
		bool isPointer : 1; ///< TODO: review this, seems to be always false
	};

	// item of the _dynamicTable, that holds the dynamic information of the dynamic type
	struct TDynInfo
	{
		int offs;  // attribute offset of the data table. Has a -1 value when
		           // the attr is not instantiated (have no entry at the data table).
		bool hasBeenAdded : 1;
		bool hasBeenRemoved : 1;
	};
	virtual DynamicType& GetDynamicTypeCopy(const bool shareData = false, const bool deep = false) const =0;
	virtual Component* ShallowCopy() const;
	DynamicType& operator= (const DynamicType& source);


	// Public Accesors to protected data. Necesary in the implementation of Branches (aggregates)
	inline unsigned    GetNumAttr() const { return _numAttr; };
	inline unsigned    GetNumActiveAttr() const { return numActiveAttr; }
	inline char*       GetData() const { return data; }
	inline void        SetData(char* srcData) { data = srcData;}
	inline TDynInfo*   GetDynamicTable() const { return _dynamicTable; }
	inline TAttr*      GetTypeDescTable() const { return _typeDescTable; }
	inline unsigned    GetDataSize() const { return _dataSize; }
	inline bool        IsInstanciate() const { return (data != 0); }
	inline bool        OwnsItsMemory() const { return bOwnsItsMemory; }
	inline void        SetOwnsItsMemory(const bool owns) { bOwnsItsMemory = owns; }
	inline bool        ExistAttr(unsigned id) const;
	inline void        SetPreAllocateAllAttributes() { bPreAllocateAllAttributes=true; }


public:
	// Developing tools:
	void Debug() const;

private:
	unsigned        numActiveAttr;
protected:
	char            *data;
private:
	TDynInfo *      _dynamicTable;
	TAttr *         _typeDescTable;
	unsigned        _dataSize;
	bool            bOwnsItsMemory;
	unsigned		_numAttr;    // the total number of dyn. attrs.
	unsigned		maxAttrSize;	// the total dyn. attrs. size
	unsigned        allocatedDataSize;

	inline int      DynTableRefCounter();
	inline void     InitDynTableRefCounter();
	inline int      DecrementDynTableRefCounter();
	inline int      IncrementDynTableRefCounter();

private:
	inline bool   AttrHasData(unsigned i) const { return (_dynamicTable[i].offs > -1); };
	inline void   RemoveAllMem();
	inline void*  GetPtrToData_(const unsigned id) const;
	inline void*  GetDataAsPtr_(const unsigned id) const;
	inline void   SetDataAsPtr_(const unsigned id, void* p);
	
	/** support method for UpdateData() @see UpdateData() */
	void BeMemoryOwner();
	/** support method for UpdateData(). @see UpdateData() 
	 *  SHRINK MODE: now we'll reuse the allocated data table deleting the gaps.
	 *  two traversals: the first one is for moving the existing attributes:
	 *  the second one for allocating the new attributes
	 */
	void UpdateDataByShrinking();

	/** support method for UpdateData(). @see UpdateData() 
	 *  STANDARD MODE: a new reallocation of data table is done.
	 *  and all existing attributes copies (copy constructor)
	 */
	void UpdateDataByStandardMode();
	
	/** support method for UpdateData(). @see UpdateData() 
	 *  Going to Pre Allocated Mode: the last reallocation is done, and the fixed offs are used.
	 */
	void UpdateDataGoingToPreAllocatedMode();
	
	/** support method for UpdateData(). @see UpdateData() Fixed offs (taken from
	 * typeDescTable are used.
	 */
	void UpdateDataInPreAllocatedMode();

	void SelfCopyPrototype(const DynamicType &orig);
	void SelfSharedCopy(const DynamicType &orig);
	void SelfShallowCopy(const DynamicType &orig);
	void SelfDeepCopy(const DynamicType &orig);
	bool bPreAllocateAllAttributes;

public:
	virtual void StoreOn(CLAM::Storage & storage) const {
		this->StoreDynAttributes(storage);
	}
	virtual void LoadFrom(CLAM::Storage & storage) {
		this->LoadDynAttributes(storage);
	}

protected:
	template <unsigned int NAttrib> 
	class AttributePositionBase { 
	public:
		static const int value;
	};
	
protected:
	virtual void StoreDynAttributes(CLAM::Storage & s) const=0;
	virtual void LoadDynAttributes(CLAM::Storage & s)=0;
	template <typename AttribType>
	void StoreAttribute(StaticTrue* asLeave, CLAM::Storage &s ,AttribType & object, const char* name) const 
	{
		CLAM::XMLAdapter<AttribType> adapter(object, name, true);
		s.Store (adapter);
	}
	template <typename AttribType>
	void StoreAttribute(StaticFalse* asLeave, CLAM::Storage &s ,AttribType & object, const char* name) const
	{
		CLAM::XMLComponentAdapter adapter(object, name, true);
		s.Store (adapter);
	} 
	template <typename AttribType>
	void StoreIterableAttribute(CLAM::Storage &s ,AttribType & object, const char* name, const char* elemName) const
	{
		CLAM::XMLIterableAdapter<AttribType> adapter(object, elemName, name, true);
		s.Store (adapter);
	} 

	template <typename AttribType>
	bool LoadAttribute(StaticTrue* asLeave, CLAM::Storage &s ,AttribType & object, const char* name) {
		CLAM::XMLAdapter<AttribType> adapter(object, name, true);
		return s.Load (adapter);	
	}
	template <typename AttribType>
	bool LoadAttribute(StaticFalse* asLeave, CLAM::Storage &s ,AttribType & object, const char* name) {
		CLAM::XMLComponentAdapter adapter(object, name, true);
		return s.Load (adapter);	
	} 
	template <typename AttribType>
	bool LoadIterableAttribute(CLAM::Storage &s ,AttribType & object, const char* name, const char* elemName) {
		CLAM::XMLIterableAdapter<AttribType> adapter(object, elemName, name, true);
		return s.Load (adapter);
	} 
};


//////////////////////////////////////////////////////////////////
// STATIC MEMBERS DEFINITION

template <unsigned int NAttrib> const int DynamicType::AttributePositionBase<NAttrib>::value = NAttrib;

//////////////////////////////////////////////////////////////////
// IMPLEMENTATION OF INLINE FUNCTIONS

inline bool DynamicType::ExistAttr(unsigned id) const 
{ 

	if (!data) return false;

	TDynInfo &inf = _dynamicTable[id];
	return (inf.offs != -1 && !inf.hasBeenAdded && !inf.hasBeenRemoved); 
}

inline void* DynamicType::GetDataAsPtr_(const unsigned id) const
{
	return *(void**)&data[_dynamicTable[id].offs];
}

inline void* DynamicType::GetPtrToData_(const unsigned id) const
{
	return (void*)&data[_dynamicTable[id].offs];
}

inline void DynamicType::SetDataAsPtr_(const unsigned id, void* p)
{
	*(void**)&data[_dynamicTable[id].offs] = p;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void DynamicType::InformTypedAttr_(
	TAttr * attributeTable,
	unsigned index,
	const char * name,
	unsigned size,
	const char * type,
	const bool isPtr,
	const t_new fnew,
	const t_new_copy fcopy,
	const t_destructor fdestr,
	const void * typedNullPointer)
{
	InformAttr_(attributeTable, index, name, size, type, isPtr, fnew, fcopy, fdestr);
	attributeTable[index].isComponent = false;
	attributeTable[index].isDynamicType = false;
	attributeTable[index].isStorable = false;
}

inline void DynamicType::InformTypedAttr_(
	TAttr * attributeTable,
	unsigned index,
	const char * name,
	unsigned size,
	const char * type,
	const bool isPtr,
	const t_new fnew,
	const t_new_copy fcopy,
	const t_destructor fdestr,
	const Component * typedNullPointer)
{
	InformAttr_(attributeTable, index, name, size, type, isPtr, fnew, fcopy, fdestr);
	attributeTable[index].isComponent = true;
	attributeTable[index].isDynamicType = false;
	attributeTable[index].isStorable = false;
}

inline void DynamicType::InformTypedAttr_(
	TAttr * attributeTable,
	unsigned index,
	const char * name,
	unsigned size,
	const char * type,
	const bool isPtr,
	const t_new fnew,
	const t_new_copy fcopy,
	const t_destructor fdestr,
	const DynamicType * typedNullPointer)
{
	InformAttr_(attributeTable, index, name, size, type, isPtr, fnew, fcopy, fdestr);
	attributeTable[index].isComponent = true;
	attributeTable[index].isDynamicType = true;
	attributeTable[index].isStorable = false;
}


} //namespace CLAM

#endif // !defined _DynamicType_

