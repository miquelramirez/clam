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

#ifndef _List_
#define _List_

#include "DataTypes.hxx"
#include "Err.hxx"
#include "Assert.hxx"
#include "Component.hxx"
#include "TypeInfo.hxx"

#include "XMLAdapter.hxx"
#include "XMLComponentAdapter.hxx"

namespace CLAM {

template <class T2> void StoreMemberOn(T2 &item, Storage & storage);

template <class T> class List:public Component
{

	class Node
	{
		friend class List<T>;
	private:
		T mValue;
		Node *mpNext,*mpPrevious;
	public:
		Node(const T& value)
			{
				mpNext=mpPrevious=0;
				mValue=value;
			}
		const T& Value(void) const{ return mValue; }
		T& Value(void) { return mValue; }
		Node *Next(void) { return mpNext; }
		Node *Previous(void) { return mpPrevious; }
	};



public:
	const char * GetClassName() const {return 0;}

	List()
	{
		mpFirst = mpLast = mpCurrent = 0;
		mCurrentIndex = 0;
		mSize = 0;

		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	}

	List & operator = (const List& src)
	{
		int i;
		if (mSize>0)
		{
			DoLast();
			while(mpCurrent)
			{
				DeleteElem(mSize-1);
			}

		}
		for(i=0;i<src.Size();i++)
		{
			AddElem(src[i]);
		}
		return *this;
	}
	
	List(const List& src)
	{
		mpFirst = mpLast = mpCurrent = 0;
		mCurrentIndex = 0;
		mSize = 0;
		*this=src;
	}
	
	~List()
	{
		mpCurrent = mpFirst;
		while (mpCurrent)
		{
			Node* next = mpCurrent->mpNext;
			delete mpCurrent;
			mpCurrent = next;
		}
	}
	
	void AddElem(const T& value);
	void InsertElem(const T& value,TIndex i);
	void InsertElem(const T& value);
	void DeleteElem(TIndex i);
	void DeleteElem();
	void Extract(T& value,TIndex i);
	void Extract(T& value);
	TSize Size() const
	{
		return mSize;
	}
	bool IsEmpty() const
	{
		return mSize==0;
	} 
	const T& operator [] (TIndex i) const;
	T& operator [] (TIndex i);
	const T& Current() const
	{
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");
		CLAM_ASSERT(mpCurrent,"Trying to access non-exixting current pointer");
		return mpCurrent->Value();
	}
	const T& First() const
	{
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");
		CLAM_ASSERT(mSize>=0,"Trying to access empty list");
		return mpFirst->Value();
	}
	const T& Last() const
	{
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");
		CLAM_ASSERT(mSize>=0,"Trying to access empty list");
		return mpLast->Value();
	}
	T& Current()
	{
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");
		CLAM_ASSERT(mpCurrent,"Trying to access non-exixting current pointer");
		return mpCurrent->Value();
	}
	T& First()
	{
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");
		CLAM_ASSERT(mSize>=0,"Trying to access empty list");
		return mpFirst->Value();
	}
	T& Last()
	{
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");
		CLAM_ASSERT(mSize>=0,"Trying to access empty list");
		return mpLast->Value();
	}
	void DoFirst()
	{
		mpCurrent=mpFirst;
		mCurrentIndex=0;
	}
	void DoLast()
	{
		mpCurrent=mpLast;
		mCurrentIndex=mSize-1;
	}
	void DoNext()
	{
		CLAM_ASSERT(mCurrentIndex<mSize-1,"Current element is already last one");
		mpCurrent=mpCurrent->mpNext;
		mCurrentIndex++;
	}
	void DoPrevious()
	{
		CLAM_ASSERT(mCurrentIndex>0,"Current element is already first one");
		mpCurrent=mpCurrent->mpPrevious;
		mCurrentIndex--;
	}
	
	bool IsLast()
	{
		return (mpCurrent==mpLast);
	}

	bool Done(void)
	{
		return mCurrentIndex==mSize;
	}

	bool IsFirst()
	{
		return (mpCurrent==mpFirst);
	}

	int CurrentIndex() const{return mCurrentIndex;}
	bool FulfillsInvariant (void) const;
	
private:
	
	Node* GetNodeAt(TIndex i);
	void LinkNode(Node* pA,Node* pB);
	void AddNode(Node* pA);
	void InsertNode(Node* pA);
	void InsertNode(Node* pWhere, Node* pWhat);
	void DeleteNode();
	void DeleteNode(Node* pNode);

	Node *mpFirst,*mpLast;
	mutable Node* mpCurrent;
	mutable TIndex mCurrentIndex;
	TSize mSize;


public:

	void StoreOn(Storage & storage) const
	{

		if(mSize<=0) return;
		// TODO: think if it's the best way to check if there is data.
		typedef typename TypeInfo<T>::StorableAsLeaf IsStorableAsLeaf;
		for (int i=0; i<mSize; i++) 
		{
			StoreMemberOn(
				(IsStorableAsLeaf*)0, 
				&(*this)[i], 
				storage
			);
		}
	}

	void LoadFrom(Storage & storage)
	{
		typedef typename TypeInfo<T>::StorableAsLeaf IsStorableAsLeaf;
		do AddElem(T());
		while (LoadMemberFrom( (IsStorableAsLeaf *)0, &(Last()), storage));
		DoLast();
		DeleteNode();
	}
private:
	void StoreMemberOn(StaticTrue* asLeave, const void * item, Storage & storage) const {
		XMLAdapter<T> adapter(*(T*)item);
		storage.Store(adapter);
	}
	void StoreMemberOn(StaticFalse* asLeave, const Component * item, Storage & storage) const {
		const char* className = item->GetClassName();
		const char* label = className? className : "Element";
		XMLComponentAdapter adapter(*item, label, true);
		storage.Store(adapter);
	}
	bool StoreMemberOn(StaticFalse* asLeave, const void * item, Storage & storage) const {
		CLAM_ASSERT(false, "Trying to Store an object that is not neither a streamable nor a Component");
		return false;
	}
	bool LoadMemberFrom(StaticTrue* asLeave, void * item, Storage & storage) {
		XMLAdapter<T> adapter(*(T*)item);
		return storage.Load(adapter);
	}
	bool LoadMemberFrom(StaticFalse* asLeave, Component * item, Storage & storage) {
		const char* className = item->GetClassName();
		const char* label = className? className : "Element";
		XMLComponentAdapter adapter(*item, label, true);
		return storage.Load(adapter);
	}
	bool LoadMemberFrom(StaticFalse* asLeave, void * item, Storage & storage) {
		CLAM_ASSERT(false, "Trying to Load an object that is not neither a streamable nor a Component");
		return false;
	}
};




template <class T> inline void List<T>::AddElem(const T& value)
{
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	AddNode(new Node(value));
	mCurrentIndex=mSize-1;
	mpCurrent=mpLast;
	
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

}

template <class T> inline void List<T>::DeleteElem(TIndex i)
{
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");
	CLAM_ASSERT(i<mSize,"Trying to access non-existing element");
	mCurrentIndex=i;
	mpCurrent=GetNodeAt(i);
	DeleteNode(mpCurrent);

	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

}

template <class T> inline void List<T>::DeleteNode()
{
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	DeleteNode(mpCurrent);

	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

}

template <class T> inline void List<T>::DeleteNode(Node* pNode)
{
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	if(pNode!=mpFirst)
	{
		mpCurrent=pNode->Previous();
		mCurrentIndex--;
	}
	else
	{
		mpCurrent=pNode->Next();
		mCurrentIndex=0;
	}
	if(pNode!=mpLast&&pNode!=mpFirst)
	{
		pNode->mpPrevious->mpNext=pNode->Next();
		pNode->mpNext->mpPrevious=pNode->Previous();
	}
	else
	{
		if(pNode==mpFirst)
		{
			mpFirst=pNode->mpNext;
			if(mpFirst)
				mpFirst->mpPrevious=0;;
		}
		if(pNode==mpLast)
		{
			mpLast=pNode->mpPrevious;
			if(mpLast)
				mpLast->mpNext=0;
		}
	}
	delete pNode;
	pNode=0;
	mSize--;
	if(mSize==0) mCurrentIndex=-1;

	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

}

template <class T> inline void List<T>::InsertElem(const T& value,TIndex i)
{
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");
	CLAM_ASSERT(i<=mSize,"Trying to insert out of bounds");
	
	InsertNode(GetNodeAt(i), new Node(value));
	
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

}


template <class T> inline void List<T>::InsertNode(Node* pNewNode)
{
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	InsertNode(mpCurrent,pNewNode);

	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

}

template <class T> inline void List<T>::InsertNode(Node* pWhere,Node* pNewNode)
{
	CLAM_ASSERT(pNewNode,"Not a valid Nodeent to insert");
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	if(pWhere!=mpFirst) pWhere->mpPrevious->mpNext=pNewNode;
	pNewNode->mpPrevious=pWhere->mpPrevious;
	pWhere->mpPrevious=pNewNode;
	pNewNode->mpNext=pWhere;
	mpCurrent=pNewNode;
	if(pWhere==mpFirst) mpFirst=pNewNode;
	mSize++;

	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

}

template <class T> inline void List<T>::Extract(T& value,TIndex i)
{
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");
	CLAM_ASSERT(i<mSize,"Trying to access non-existing element");
	
	value=(*this)[i];
	DeleteElem(i);

	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

}

template <class T> inline void List<T>::Extract(T& value)
{
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	Extract(value,mCurrentIndex);

	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

}

template <class T> inline void List<T>::LinkNode(Node* pA,Node* pB)
{
	pA->mpNext = pB;
	pB->mpPrevious = pA;
}

template <class T> inline void List<T>::AddNode(Node* pNode)
{
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	if (mpLast) LinkNode(mpLast,pNode);
	else mpFirst = mpCurrent = pNode;
	mpLast = pNode;
	mpCurrent=mpLast;
	mSize++;
	mCurrentIndex=mSize-1;

	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

}




template <class T> inline const T& List<T>::operator [] (TIndex i) const{
	/* this function is optimized, by starting searching from the current 
	index, or from the beginning or the end, when that's closer.
	*/
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	if (i<=0)
	{
		mCurrentIndex=0;
		mpCurrent=mpFirst;
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

		return ((Node*)mpFirst)->mValue;
	}
	if (i>=mSize-1)
	{
		mCurrentIndex=mSize-1;
		mpCurrent=mpLast;
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

		return ((Node*)mpLast)->mValue;
	}
	if (mCurrentIndex<i) {
		if (i<((mCurrentIndex+mSize)>>1)) {
			do {
				mCurrentIndex++;
				mpCurrent = mpCurrent->Next();			
			}	while (mCurrentIndex<i);				
		} else {
			mCurrentIndex = mSize-1;
			mpCurrent = mpLast;
			while (mCurrentIndex>i) {
				mCurrentIndex--;
				mpCurrent = mpCurrent->Previous();				
			}
		}
	}else if (mCurrentIndex>i)
	{
		if (i>(mCurrentIndex>>1)) {
			do {
				mCurrentIndex--;
				mpCurrent = mpCurrent->Previous();
			}	while (mCurrentIndex>i);
		} else {
			mCurrentIndex=0;
			mpCurrent = mpFirst;
			while (mCurrentIndex<i) {
				mCurrentIndex++;
				mpCurrent = mpCurrent->Next();							
			}
		}
	}
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	return ((Node*)mpCurrent)->mValue;
}

template <class T> inline T& List<T>::operator [] (TIndex i) {
	/* this function is optimized, by starting searching from the current 
	index, or from the beginning or the end, when that's closer.
	*/
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	if (i<=0)
	{
		mCurrentIndex=0;
		mpCurrent=mpFirst;
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

		return ((Node*)mpFirst)->mValue;
	}
	if (i>=mSize-1)
	{
		mCurrentIndex=mSize-1;
		mpCurrent=mpLast;
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

		return ((Node*)mpLast)->mValue;
	}
	if (mCurrentIndex<i) {
		if (i<((mCurrentIndex+mSize)>>1)) {
			do {
				mCurrentIndex++;
				mpCurrent = mpCurrent->Next();			
			}	while (mCurrentIndex<i);				
		} else {
			mCurrentIndex = mSize-1;
			mpCurrent = mpLast;
			while (mCurrentIndex>i) {
				mCurrentIndex--;
				mpCurrent = mpCurrent->Previous();				
			}
		}
	}else if (mCurrentIndex>i)
	{
		if (i>(mCurrentIndex>>1)) {
			do {
				mCurrentIndex--;
				mpCurrent = mpCurrent->Previous();
			}	while (mCurrentIndex>i);
		} else {
			mCurrentIndex=0;
			mpCurrent = mpFirst;
			while (mCurrentIndex<i) {
				mCurrentIndex++;
				mpCurrent = mpCurrent->Next();							
			}
		}
	}
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	return ((Node*)mpCurrent)->mValue;
}

template <class T> inline typename List<T>::Node* List<T>::GetNodeAt(TIndex i){
	/* this function is optimized, by starting searching from the current 
	index, or from the beginning or the end, when that's closer.
	*/
	if (i<=0)
	{
		mpCurrent=mpFirst;
		mCurrentIndex=0;
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

		return mpFirst;
	}
	if (i>=mSize-1)
	{
		mpCurrent=mpLast;
		mCurrentIndex=mSize-1;
		CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

		return mpLast;
	}
	if (mCurrentIndex<i) {
		if (i<((mCurrentIndex+mSize)>>1)) {
			do {
				mCurrentIndex++;
				mpCurrent = mpCurrent->Next();			
			}	while (mCurrentIndex<i);				
		} else {
			mCurrentIndex = mSize-1;
			mpCurrent = mpLast;
			while (mCurrentIndex>i) {
				mCurrentIndex--;
				mpCurrent = mpCurrent->Previous();				
			}
		}
	}else if (mCurrentIndex>i)
	{
		if (i>(mCurrentIndex>>1)) {
			do {
				mCurrentIndex--;
				mpCurrent = mpCurrent->Previous();
			}	while (mCurrentIndex>i);
		} else {
			mCurrentIndex=0;
			mpCurrent = mpFirst;
			while (mCurrentIndex<i) {
				mCurrentIndex++;
				mpCurrent = mpCurrent->Next();							
			}
		}
	}
	CLAM_DEBUG_ASSERT(FulfillsInvariant(),"List does not fulfill invariant");

	return mpCurrent;
}

template <class T> inline bool List<T>::FulfillsInvariant(void) const
{
	int i;
	if(mSize>0)
	{
		if (mpFirst->mpPrevious || mpLast->mpNext || 
			mSize<0 || (mCurrentIndex<0) || (mpCurrent==0)
			)
		{
			return false;
		}
		Node* pTmp=mpCurrent; 
		for(i=mCurrentIndex;i>=0;i--)
		{
			CLAM_ASSERT(pTmp->mpPrevious || i==0, "Current pointer not consistent");
			pTmp=pTmp->mpPrevious;
		}
	}
	return true;
}

};

#endif

