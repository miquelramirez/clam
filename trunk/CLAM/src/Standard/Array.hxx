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

#ifndef _Array_
#define _Array_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#include "DataTypes.hxx"
#include "Err.hxx"
#include "Assert.hxx"
#include "ErrOutOfMemory.hxx"
#include "EDataFormat.hxx"
#include "Storage.hxx"
#include "Component.hxx"
#include "TypeInfo.hxx"

// @todo Remove this include. See Bug#111
#include "DynamicType.hxx"


#include "XMLAdapter.hxx"
#include "XMLArrayAdapter.hxx"
#include "XMLComponentAdapter.hxx"

namespace CLAM {

#ifndef CLAM_USE_STL_ARRAY

template <class T> class Array:public Component
{
private:
	T *mpData;
	TSize mAllocSize;
	TSize mSize;
	int mStep;
public:
	Array(TSize size = 0,TSize step = 1)
	{
		mSize = 0;
		mAllocSize = 0;
		mStep = step;
		mpData = NULL;
		Resize(size);
	}

	void Init(){
		Resize(0);
		SetSize(0);}

	Array(T* ptr,int size = 0)
	{
		mSize = mAllocSize = size;
		mStep = -1;
		mpData = ptr;
	}
	
	Array(const Array<T> &originalArray)
	{
		mpData = NULL;
		mSize = mAllocSize = mStep = 0;
		*this = originalArray;
	}

	~Array()
	{
		DestroyDataBuffer();
		mAllocSize=mSize=mStep=0;
	}
	
	const char * GetClassName() const {return NULL;}

	bool OwnsMemory() const {return mStep>=0; }

	TSize Size(void) const { return mSize; }
	TSize SizeInBytes(void) const { return mSize*sizeof(T); }
	TSize AllocatedSize(void) const { return mAllocSize; }
	TSize AllocatedSizeInBytes(void) const { return mAllocSize*sizeof(T); }

	void SetSize(TSize size)
	{
		CLAM_ASSERT(size <= mAllocSize || !OwnsMemory(), msgSetSizeOutOfRange);
		if (OwnsMemory())
		{
			if (size > mSize)
				InitializeDataBlock(mSize,size);
			if (size < mSize)
				UninitializeDataBlock(size,mSize);
		}
		mSize = size;
	}

	void SetStep(TSize step) { mStep = step;}

	TSize GetStep() const {return mStep;}

	void Resize(TSize newAllocSize)
	{
		CLAM_ASSERT(OwnsMemory(),"Array::Resize(): The array does not own its memory.");

		/* calculate the amount of bytes to allocate */
		/* effectively resize the array by allocating more memory */
		if(newAllocSize>0)
			ResizeDataBuffer(newAllocSize);
		else
		{
			if(mpData)
			{
				DestroyDataBuffer();
				mpData=NULL;
			}
		}

		mAllocSize = newAllocSize;

		if (mAllocSize<mSize)
			mSize = mAllocSize;
		
		/* if the pointer to the end of the array is over then you're out of memory */
		/* and an error message will be sent to the console */
		if (mAllocSize && !mpData)
			throw ErrOutOfMemory(mAllocSize*sizeof(T));
	}

	const T* GetPtr(void) const { return mpData; }
	T* GetPtr(void) { return mpData; }
	
	void SetPtr(T* ptr, int size = 0)
	{
		CLAM_ASSERT( !OwnsMemory() || mAllocSize==0, 
					 "Array: SetPtr: the array is not empty. (mAllocSize>0)");

		mSize = mAllocSize = size;
		mpData = ptr;

		if (ptr == 0 && size == 0)
			mStep = 1;  // Sets the array to empty state.
		else
			mStep = -1; // the array gets not owner of the new data.
	}

	inline void GiveChunk(int pos, int size, Array<T>&) const;

	inline void CopyChunk(int pos, int size, Array<T>&) const;

	EDataFormat Format() { return eFmtDefault; }

	const T& operator [](const int& i) const
	{
		CLAM_DEBUG_ASSERT(i>=0,msgIndexOutOfRange);
		CLAM_DEBUG_ASSERT(i<mSize,msgIndexOutOfRange);
		return mpData[i];
	}

	T& operator [](const int& i)
	{
		CLAM_DEBUG_ASSERT(i>=0,msgIndexOutOfRange);
		CLAM_DEBUG_ASSERT(i<mSize,msgIndexOutOfRange);
		return mpData[i];
	}
	
	void	AddElem(const T& elem)
	{
		CLAM_ASSERT(OwnsMemory(),"Array::AddElem(): Resize requiered,"
		                        " but this array does not own its memory!");
		if (mSize>=mAllocSize)
			Resize(mAllocSize+mStep);
		new(&mpData[mSize]) T(elem);
		mSize++;
	}
	void	InsertElem(int where,const T& elem)
	{
		CLAM_ASSERT(OwnsMemory(),"Array::InsertElem(): Resize requiered,"
		                        " but this array does not own its memory!");
		CLAM_ASSERT( (where>=0) && (where<mSize) ,msgInsertOutOfRange);
		if (mSize>=mAllocSize)
			Resize(mAllocSize+mStep);
		InsertElemInDataBuffer(where);
		new(&mpData[where]) T(elem);
		mSize++;
	}
	void	DeleteElem(int where)
	{
		CLAM_ASSERT(OwnsMemory(),"Array::DeleteElem(): Resize requiered,"
		                        " but this array does not own its memory!");
		CLAM_ASSERT(where>-1 ,msgDeleteOutOfRange);
		CLAM_ASSERT(where<mSize,msgDeleteOutOfRange);
		DeleteElemInDataBuffer(where);
		mSize--;
		if (mSize<mAllocSize-mStep) 
			Resize(mSize);
	}

	Array<T>& operator = (const Array<T>& src)
	{
		int tocopy;
		if (OwnsMemory())
		{
			if (Size() != src.Size())
				Resize(src.Size());
			mStep = src.mStep;
		} else {
			CLAM_ASSERT(src.Size()<=mAllocSize,
					"Cannot copy a larger array to an array that does not own it's memory!");
			// important to leave mStep untouched: it indicates that the array !OwnsMemory
		}
		tocopy = (src.Size()<Size())?src.Size():Size();
		CopyDataBlock(0,tocopy,src.mpData);
		InitializeCopyDataBlock(tocopy,src.Size(),src.mpData);
		mSize=src.Size();
		return *this;
	}

	Array<T>& operator += (const Array<T>& src)
	{
		int start = Size();
		Resize(Size()+src.Size());
		mSize+=src.Size();
		int end = Size();
		InitializeCopyDataBlock(start,end,0,src.mpData);
		return *this;
	}

	void Apply( T (*f)(T) )
	{
		int i;
		for (i=0; i<mSize; i++)
			(*this)[i] = f( (*this)[i] );
	}

	void Apply( T (*f)(T,int),int parameter )
	{
		int i;
		for (i=0; i<mSize; i++)
			(*this)[i] = f( (*this)[i], parameter );
	}

	void StoreOn(Storage & storage) const
	{
		StoreBufferOn((typename TypeInfo<T>::StorableAsLeaf *)NULL, mpData, storage);
	}
	void LoadFrom(Storage & storage)
	{
		LoadBufferFrom((typename TypeInfo<T>::StorableAsLeaf *)NULL, mpData, storage);
	}

	// Error messages, to ease tests a little while we decide
	// about error codes.
	static const char *msgSetSizeOutOfRange;
	static const char *msgIndexOutOfRange;
	static const char *msgInsertOutOfRange;
	static const char *msgDeleteOutOfRange;

private:
	inline void ResizeDataBuffer(int new_size);
	inline void DestroyDataBuffer(void);
	inline void InsertElemInDataBuffer(int position);
	inline void DeleteElemInDataBuffer(int position);
	inline void InitializeElement(int position);
	inline void InitializeDataBlock(int first, int last);
	inline void UninitializeDataBlock(int first, int last);
	inline void CopyDataBlock(int first, int last, const T* src);
	inline void InitializeCopyDataBlock(int first, int last, const T* src);
	inline void InitializeCopyDataBlock(int first, int last, int src_first, const T* src);

	void StoreBufferOn(StaticFalse* asLeave, const Component * polymorphicSelector, Storage & storage) const
	{
		if (mSize<=0) return;
		const char* className = mpData[1].GetClassName();
		const char* label = className? className : "Element";
		for (int i=0; i<mSize; i++)
		{
			XMLComponentAdapter adapter(mpData[i], label, true);
			storage.Store(adapter);
		}
	}
	void StoreBufferOn(StaticTrue* asLeave, const void * polymorphicSelector, Storage & storage) const 
	{
		XMLAdapter<unsigned> sizeAdapter(Size(),"size");
		storage.Store(sizeAdapter);
		XMLArrayAdapter<T> adapter(mpData,mSize);
		storage.Store(adapter);
	}
	void StoreBufferOn(StaticFalse* asLeave, const void * polymorphicSelector, Storage & storage) const 
	{
		CLAM_ASSERT(false, 
			"Trying to Store an object that is not neither a streamable nor a Component");
	}
	void LoadBufferFrom(StaticFalse* asLeave, Component * polymorphicSelector, Storage & storage)
	{
		const char* label = 0;
		while (true)
		{
			T elem;
			if (!label)
			{
				label = elem.GetClassName();
				if (!label)
					label = "Element";
			}
			XMLComponentAdapter adapter(elem, label, true);
			if (!storage.Load(adapter)) return;
			AddElem(elem);
		}
	}
	void LoadBufferFrom(StaticTrue* asLeave, void * polymorphicSelector, Storage & storage)
	{
		unsigned size;
		XMLAdapter<unsigned> sizeAdapter(size,"size");
		if (storage.Load(sizeAdapter))
		{
			Resize(size);
			SetSize(size);
			XMLArrayAdapter<T> adapter(mpData,mSize);
			storage.Load(adapter);
			// TODO: if false, then insert an error on the storage
			return;
		}

		while (true) {
			T elem;
			XMLAdapter<T> adapter(elem);
			if ( ! storage.Load(adapter)) return;
			AddElem(elem);
		}
	}
	void LoadBufferFrom(StaticFalse* asLeave, void * polymorphicSelector, Storage & storage)
	{
		CLAM_ASSERT(false, 
			"Trying to Store an object that is not neither a streamable nor a Component");
	}
/*
	void StoreMemberOn(StaticTrue* asLeave, void * item, Storage & storage) const {
		XMLAdapter<T> adapter(*(T*)item);
		storage.Store(adapter);
	}
	void StoreMemberOn(StaticFalse* asLeave, Component * item, Storage & storage) const {
		const char* className = item->GetClassName();
		const char* label = className? className : "Element";
		XMLComponentAdapter adapter(*item, label, true);
		storage.Store(adapter);
	}
	bool StoreMemberOn(StaticFalse* asLeave, void * item, Storage & storage) {
		CLAM_ASSERT(false, "Trying to Store an object that is not neither a streamable nor a Component");
		return false;
	}
*/
	bool LoadMemberFrom(StaticTrue* asLeave, void * item, Storage & storage) {
		XMLAdapter<T> adapter(*(T*)item);
		return storage.Load(adapter);
	}
	bool LoadMemberFrom(StaticFalse* asLeave, Component * item, Storage & storage) {
		const char* className = (item->GetClassName());
		const char* label = className? className : "Element";
		XMLComponentAdapter adapter(*item, label, true);
		return storage.Load(adapter);
	}
	bool LoadMemberFrom(StaticFalse* asLeave, void * item, Storage & storage) {
		CLAM_ASSERT(false, "Trying to Load an object that is not neither a streamable nor a Component");
		return false;
	}

};

// Method implementations


template<class T>
void Array<T>::GiveChunk(int pos, int size, Array<T>& a) const
{
	CLAM_ASSERT(pos + size <= mSize,
	            "Array::GiveChunk(): Chunk out of bounds.");
	a.SetPtr(&mpData[pos],size);
}

template<class T>
void Array<T>::CopyChunk(int pos, int size, Array<T>& a) const
{
	int last=pos+size;
	CLAM_ASSERT(last <= mSize,
	            "Array::CopyChunk(): Chunk out of bounds.");
	CLAM_ASSERT(pos <= a.mSize,
	            "Array::GiveChunk(): destination array does not have enough memory");
	int i;
	for (i=pos;i<last;i++)
		a.mpData[i-pos]=mpData[i];
}

template<class T>
void Array<T>::InitializeElement(int i)
{
	new (&mpData[i]) T();
}

template<class T>
void Array<T>::InitializeDataBlock(int first, int last)
{
	int i;
	for (i = first; i < last; i++)
		InitializeElement(i);
}

template<class T>
void Array<T>::UninitializeDataBlock(int first, int last)
{
	int i;
	for (i = first; i < last; i++)
		(&mpData[i])->~T();
}

template<class T>
void Array<T>::CopyDataBlock(int first, int last, const T* src)
{
	int i;
	for (i=first; i<last ;i++)
		mpData[i]=src[i];
}

template<class T>
void Array<T>::InitializeCopyDataBlock(int first, int last, const T* src)
{
	int i;
	for (i=first; i<last; i++)
		new(&mpData[i]) T(src[i]);
}

template<class T>
void Array<T>::InitializeCopyDataBlock(int first, int last, int src_first, const T* src)
{
	int i, j = src_first;
	for (i=first; i<last; i++)
		new (&mpData[i]) T(src[j++]);
}

#define CLAM_NUMERIC_ARRAY_INITIALIZATION(Type)          \
template<>                                              \
inline void Array<Type>::InitializeElement(int i)              \
{                                                       \
    mpData[i]=0;                                        \
}                                                       \


CLAM_NUMERIC_ARRAY_INITIALIZATION(unsigned long)
CLAM_NUMERIC_ARRAY_INITIALIZATION(unsigned int)
CLAM_NUMERIC_ARRAY_INITIALIZATION(unsigned short)
CLAM_NUMERIC_ARRAY_INITIALIZATION(unsigned char)
CLAM_NUMERIC_ARRAY_INITIALIZATION(signed long)
CLAM_NUMERIC_ARRAY_INITIALIZATION(signed int)
CLAM_NUMERIC_ARRAY_INITIALIZATION(signed short)
CLAM_NUMERIC_ARRAY_INITIALIZATION(signed char)
CLAM_NUMERIC_ARRAY_INITIALIZATION(double)
CLAM_NUMERIC_ARRAY_INITIALIZATION(float)

template<class T>
void Array<T>::DestroyDataBuffer()
{
	if (mStep!=-1)
	{
		UninitializeDataBlock(0,mSize);
		free(mpData);
	}
	mpData=NULL;
}

/** Unoptimised and safe default resizing code */
template<class T>
void Array<T>::ResizeDataBuffer(int new_size)
{
	if (new_size == mAllocSize)
		return;
	int i;
	T* old_data = mpData;
	mpData = (T*) malloc(new_size*sizeof(T));
	if (!old_data) return;
	int elems = new_size;
	if (mSize < elems)
		elems = mSize;
	InitializeCopyDataBlock(0,elems,old_data);
	for (i=0; i<mSize; i++)
		(&old_data[i])->~T();
	free(old_data);
}

/** Unoptimised and safe default element insertion code.
 *  <p>
 *  Warning: this does only the "memmove" operation.
 *  <p>
 *  mSize must hold the size BEFORE adding the element.
 *  The allocated size must be enough to hold a new element.
 */
template<class T>
void Array<T>::InsertElemInDataBuffer(int position)
{
	if (mSize>0)
		new(&mpData[mSize]) T(mpData[mSize-1]);
	for (int i=mSize-1; i>position; i--)
		mpData[i] = mpData[i-1];
	(&mpData[position])->~T();
}

/** Unoptimised and safe default element removal code.
 *  <p>
 *  Warning: this does only the "memmove" operation.
 *  <p>
 *  mSize must hold the size BEFORE removing the element.
 */
template<class T>
void Array<T>::DeleteElemInDataBuffer(int position)
{
	for (int i=position; i<mSize-1; i++)
		mpData[i] = mpData[i+1];
	(&mpData[mSize-1])->~T();
}

/** And fast spetialization for basic types */

#define CLAM_FAST_ARRAY_SPECIALIZATIONS(TYPE)                           \
template<>                                                             \
inline void Array<TYPE >::CopyDataBlock(int first, int last,                  \
                                 const TYPE *src)                      \
{                                                                      \
    if (last>first)                                                    \
        memcpy(&mpData[first],&src[first],                             \
               sizeof(TYPE)*(last-first));                             \
}                                                                      \
template<>                                                             \
inline void Array<TYPE >::InitializeCopyDataBlock(int first, int last,        \
                                           const TYPE *src)            \
{                                                                      \
    if (last>first)                                                    \
        memcpy(&mpData[first],&src[first],                             \
               sizeof(TYPE)*(last-first));                             \
}                                                                      \
template<>                                                             \
inline void Array<TYPE >::InitializeCopyDataBlock(int first, int last,        \
                                           int src_first,              \
                                           const TYPE *src)            \
{                                                                      \
    if (last>first)                                                    \
        memcpy(&mpData[first],&src[src_first],                         \
               sizeof(TYPE)*(last-first));                             \
}                                                                      \
template<>                                                             \
inline void Array<TYPE >::ResizeDataBuffer(int new_size)                      \
{                                                                      \
    mpData = (TYPE*) realloc(mpData,new_size*sizeof(TYPE));            \
}                                                                      \
template<>                                                             \
inline void Array<TYPE >::InsertElemInDataBuffer(int where)                   \
{                                                                      \
    memmove(&mpData[where+1],&mpData[where],                           \
            (mSize-where)*sizeof(TYPE));                               \
}                                                                      \
template<>                                                             \
inline void Array<TYPE >::DeleteElemInDataBuffer(int where)                   \
{                                                                      \
    memmove(&mpData[where],&mpData[where+1],                           \
            (mSize-where-1)*sizeof(TYPE));                             \
}                                                                      \

CLAM_FAST_ARRAY_SPECIALIZATIONS(unsigned long)
CLAM_FAST_ARRAY_SPECIALIZATIONS(unsigned int)
CLAM_FAST_ARRAY_SPECIALIZATIONS(unsigned short)
CLAM_FAST_ARRAY_SPECIALIZATIONS(unsigned char)
CLAM_FAST_ARRAY_SPECIALIZATIONS(signed long)
CLAM_FAST_ARRAY_SPECIALIZATIONS(signed int)
CLAM_FAST_ARRAY_SPECIALIZATIONS(signed short)
CLAM_FAST_ARRAY_SPECIALIZATIONS(signed char)
CLAM_FAST_ARRAY_SPECIALIZATIONS(double)
CLAM_FAST_ARRAY_SPECIALIZATIONS(float)


template <class T> inline Array<T> operator + (
	const Array<T>& a,const Array<T>& b)
{
	Array<T> ret = a;
	ret += b;
	return ret;
}

template <class T> inline bool operator == (
	const Array<T>& a,const Array<T>& b)
{
	if (a.Size()!=b.Size()) return false;
	for (int i=0;i<a.Size();i++)
	{
		if (a[i]!=b[i]) return false;		
	}
	return true;
}

// Format specializations. Based on EDataFormat.hxx
// Warning: This should be architecture dependent code.
template<> inline EDataFormat Array<int>::Format() { return eFmtS32L; }
template<> inline EDataFormat Array<short>::Format() { return eFmtS16L; }
template<> inline EDataFormat Array<float>::Format() { return eFmtF32B; }
template<> inline EDataFormat Array<double>::Format() { return eFmtF64B; }



#else // CLAM_USE_STL_ARRAY

	#include <vector>


	template<class T>
	class Array : public Component, public std::vector<T> {
	public:
		typedef std::vector<T>::iterator iterator;
		typedef std::vector<T>::const_iterator const_iterator;
		Array(TSize size = 0);
		Array(T* ptr,int size = 0);
		inline void  Init();

		inline bool  OwnsMemory() const;
		inline TSize Size() const;
		inline TSize SizeInBytes() const;
		inline TSize AllocatedSize() const;
		inline TSize AllocatedSizeInBytes() const;

		inline void  SetSize(TSize size);
		inline void  Resize(TSize size);

		inline const T& operator [](const int& i) const;
		inline T& operator [](const int& i);

		inline const T* GetPtr(void) const;
		inline T* GetPtr(void);
		inline void SetPtr(T* ptr);

		inline TSize GetStep();
		inline void SetStep(TSize step);

		inline void AddElem(const T& elem);
		inline void InsertElem(int pos, const T& elem);
		inline void DeleteElem(int where);
		inline Array<T>& operator += (const Array<T>& src);

		inline void Apply( T (*f)(T,int),int parameter );

		void StoreOn(Storage & storage) const;
		void LoadFrom(Storage & storage);

		// Error messages, to ease tests a little while we decide
		// about error codes.
		static const char *msgSetSizeOutOfRange;
		static const char *msgIndexOutOfRange;
		static const char *msgInsertOutOfRange;
		static const char *msgDeleteOutOfRange;

	private:

	void StoreMemberOn(void * item, Storage & storage) const;
	void StoreMemberOn(Component * item, Storage & storage) const;
	bool LoadMemberFrom(void * item, Storage & storage);
	bool LoadMemberFrom(Component * item, Storage & storage);

	};

template<class T>
Array<T>::Array(TSize size)
{ 
	if (size)
		reserve(size); 
}

template<class T>
void Array<T>::Init()
{
	reserve(0);
	resize(0);
}

template<class T>
Array<T>::Array(T* ptr,int size) 
{
	CLAM_ASSERT(false,"Array::Array(T*): Not implemented");
}

template<class T>
bool Array<T>::OwnsMemory() const 
{
	return true; 
}

template<class T>
TSize Array<T>::Size() const 
{
	return size(); 
}

template<class T>
TSize Array<T>::SizeInBytes() const 
{
	return size() * sizeof(T); 
}

template<class T>
TSize Array<T>::AllocatedSize() const 
{
	return capacity(); 
}

template<class T>
TSize Array<T>::AllocatedSizeInBytes() const 
{
	return capacity() * sizeof(T); 
}

template<class T>
void Array<T>::SetSize(TSize size) 
{
	CLAM_ASSERT(size <= ((signed)capacity()) ,msgSetSizeOutOfRange);
	resize(size); 
}

template<class T>
void Array<T>::Resize(TSize size) 
{
//	CLAM_ASSERT(mStep != -1,"Array::Resize(): Array does not own its memory");
	reserve(size); 
}

template<class T>
const T& Array<T>::operator [](const int& i) const
{	
	CLAM_DEBUG_ASSERT(i>=0 && i<(signed)Size(),msgIndexOutOfRange);
	return std::vector<T>::operator[](i); 
}

template<class T>
T& Array<T>::operator [](const int& i) 
{	
	CLAM_DEBUG_ASSERT(i>=0 && i<(signed)size(),msgIndexOutOfRange);
	return std::vector<T>::operator[](i); 
}

template<class T>
const T* Array<T>::GetPtr(void) const 
{
	return &(*this)[0];
}

template<class T>
T* Array<T>::GetPtr(void) 
{
	return &(*this)[0];
}

template<class T>
void Array<T>::SetPtr(T* ptr) 
{
	CLAM_ASSERT(false,"Array::SetPtr(T*): Not implemented");
}

template<class T>
TSize Array<T>::GetStep() 
{
	CLAM_ASSERT(false,"Array::GetStep(): Not implemented");
}

template<class T>
void Array<T>::SetStep(TSize step) 
{
	CLAM_ASSERT(false,"Array::SetStep(int): Not implemented");
}

template<class T>
void Array<T>::AddElem(const T& elem) 
{
//	CLAM_ASSERT(mStep != -1,"Array::AddElem(): Array does not own its memory");
	push_back(elem);
}

template<class T>
void Array<T>::InsertElem(int pos, const T& elem) 
{
	CLAM_ASSERT( (pos>=0) && (pos<(signed)size()) ,msgInsertOutOfRange);
//	CLAM_ASSERT(mStep != -1,"Array::InsertElem(): Array does not own its memory");
	insert(iterator(&(operator[](pos))),elem);
}

template<class T>
void Array<T>::DeleteElem(int where) 
{
	CLAM_ASSERT( (where>=0) && (where < ((signed)size())) ,msgDeleteOutOfRange);
	erase(iterator(&(operator[](where))));
}

template<class T>
Array<T>& Array<T>::operator += (const Array<T>& src) 
{
//	CLAM_ASSERT(mStep != -1,"Array::+=(): Array does not own its memory");
	insert(end(),src.begin(),src.end());
	return *this;
}

template<class T>
void Array<T>::Apply( T (*f)(T,int),int parameter )
{
	int i; 
	int s=size();
	for (i=0; i<s; i++)
		(*this)[i] = f( (*this)[i], parameter );
}

template<class T>
void Array<T>::StoreOn(Storage & storage) const
{
//	CLAM_ASSERT(mpData,"Array contains no buffer")
	int mSize = Size();
	T* mpData = GetPtr();
	for (int i=0; i<mSize; i++) {
		StoreMemberOn(&(mpData[i]), storage);
	}
}

template <class T>
void Array<T>::LoadFrom(Storage & storage)
{
//	CLAM_ASSERT(mpData,"Array contains no buffer")
	while (true) {
		T elem;
		if (!LoadMemberFrom(&(elem), storage)) return;
		AddElem(elem);
	}
}


template<class T>
void Array<T>::StoreMemberOn(void * item, Storage & storage) const
{
	XMLAdapter<T> adapter(*(T*)item);
	storage.Store(adapter);
}

template<class T>
void Array<T>::StoreMemberOn(Component * item, Storage & storage) const
{
	const char* className = item->GetClassName();
	const char* label = className? className : "Element";
	XMLComponentAdapter adapter(*item, label, true);
	storage.Store(adapter);
}

template<class T>
bool Array<T>::LoadMemberFrom(void * item, Storage & storage) 
{
	XMLAdapter<T> adapter(*(T*)item);
	return storage.Load(adapter);
}

template<class T>
bool Array<T>::LoadMemberFrom(Component * item, Storage & storage) 
{
	const char* className = item->GetClassName();
	const char* label = className? className : "Element";
	XMLComponentAdapter adapter(*item, label, true);
	return storage.Load(adapter);
}


#endif // CLAM_USE_STL_ARRAY

typedef Array<TData> DataArray;

template<class T>
const char* Array<T>::msgSetSizeOutOfRange =
"Array::SetSize(): Argument larger than allocated size\n"
"You can probably fix this calling Resize() befor SetSize().";

template<class T>
const char* Array<T>::msgIndexOutOfRange =
"Array::operator[]: Index out of range\n"
"This may happen if you forgot to call SetSize(...) in your code.\n"
"This is now needed. Just calling Resize() is not enough any more.";

template<class T>
const char* Array<T>::msgInsertOutOfRange = 
"Array::InsertElem: Index out of range";

template<class T>
const char* Array<T>::msgDeleteOutOfRange = 
"Array::DeleteElem: Index out of range";

}

#endif//_Array_
