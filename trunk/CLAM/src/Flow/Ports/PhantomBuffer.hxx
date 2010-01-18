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

#ifndef _PHANTOM_BUFFER_H
#define _PHANTOM_BUFFER_H


#include <vector>
#include <iostream>
#include "Assert.hxx"

namespace CLAM {

	template<class T>
	class PhantomBuffer 
	{
	public:
		PhantomBuffer();
		T*   Read(unsigned int pos, unsigned int size);

		/**
			Updates the beginning or phantom zone corresponding with 'symetric'
			of the specifyed window
		 	/see UpdatePhantom
	 	 */
		void Touch(unsigned int pos, unsigned int size);

		/**
			Copies an array into the buffer and updates (touches) the
			'repeated' data (beginning or phantom)
			/see Touch
		**/
		void Write(unsigned int pos, unsigned int size,const T* data);

		unsigned LogicalSize() const {return mLogicalSize;}
		
		unsigned int PhantomSize() const { return mBuffer.size() - mLogicalSize; }

		void Resize(unsigned int size,
		            unsigned int phantom,
		            unsigned int insert_pos);
	
		/**
			For debugging purposes. Checks that:
			logical / phantom sizes are consistents and
			'repeated' data is consistent
			/see Touch
		**/
		void FulfilsInvariant() const;

	private:
		/**
			Updates the phantom zone corresponding with 'symetric' of the specifyed window
			/see UpdateBeginning
	 	*/
		void UpdatePhantom(unsigned int pos, unsigned int size);
		/**
			Updates the beginning zone corresponding with 'symetric' of the specifyed window
			/see UpdatePhantom
	 	c*/
		void UpdateBeginning(unsigned int pos, unsigned int size);

		unsigned int AllocatedSize() const
		{ 
			return mBuffer.size(); 
		}

	
		// attributes
		std::vector<T> mBuffer;
		unsigned mLogicalSize;
	};

	
	template<class T>
	PhantomBuffer<T>::PhantomBuffer()
		: mLogicalSize(0) 
	{}


	template<class T>
	void PhantomBuffer<T>::UpdatePhantom(unsigned int pos, unsigned int size)
	{
		CLAM_DEBUG_ASSERT(pos+size <= PhantomSize(),
		                 "PhantomBuffer::UpdatePhantom():"
		                 "Requested region falls outside phantom zone");
		unsigned int s,d;
		for (s=pos, d=mLogicalSize+pos; s<pos+size; s++, d++)
			mBuffer[d] = mBuffer[s];
	}

	template<class T>
	void PhantomBuffer<T>::UpdateBeginning(unsigned int pos, unsigned int size)
	{
		CLAM_DEBUG_ASSERT(pos+size <= PhantomSize(),
		                 "PhantomBuffer::UpdateBeginning():"
		                 "Requested region falls outside beginning zone");
		unsigned int s,d;
		for (d=pos, s=mLogicalSize+pos; d<size; d++, s++)
			mBuffer[d] = mBuffer[s];
	}


	template<class T>
	void PhantomBuffer<T>::Resize(unsigned int newLogicalSize, unsigned int newPhantomSize, unsigned int insertionPos)
	{
		CLAM_ASSERT(insertionPos <= LogicalSize(),
		           "PhantomBuffer::Resize(): "
		           "Insertion position can not be greater than previous size.");
		CLAM_ASSERT(newPhantomSize <= newLogicalSize,
		           "PhantomBuffer::Resize(): Phantom Size can not be greater than size.");

		if ( newLogicalSize <= LogicalSize() )
			return;

		CLAM_ASSERT(newPhantomSize >= PhantomSize(),
		           "PhantomBuffer::Resize(): PhantomSize can not be decreased.");

		unsigned int logicalElementsToInsert = newLogicalSize - LogicalSize();
		unsigned int phantomElementsToInsert = newPhantomSize - PhantomSize();

		typename std::vector<T>::iterator it = mBuffer.begin();
		it += insertionPos;
		mBuffer.insert(it, logicalElementsToInsert, T());

		mBuffer.insert( mBuffer.end(), phantomElementsToInsert,  T());
		//TODO update phantom

		mLogicalSize = newLogicalSize;
	}


	template<class T>
	T* PhantomBuffer<T>::Read(unsigned int pos, unsigned int size)
	{
		CLAM_DEBUG_ASSERT( (pos+size-1) <= ( AllocatedSize() ),
		                  "PhantomBuffer::Read(): Position outside bounds");
		return &mBuffer[pos];
	}


	template<class T>
	void PhantomBuffer<T>::Touch(unsigned int pos, unsigned int size)
	{
		CLAM_DEBUG_ASSERT( (pos+size) <= ( AllocatedSize() ),
		                  "PhantomBuffer::Write(): Position outside bounds");
		unsigned int writen_area_end = pos+size;

		if (writen_area_end > mLogicalSize)
		{
			unsigned int update_start;
			if (mLogicalSize > pos)
				update_start = 0;
			else
				update_start = pos-mLogicalSize;
			UpdateBeginning(update_start, writen_area_end - mLogicalSize - update_start);
		}
		else if ( pos < PhantomSize() )
		{
			int update_end;
			if (PhantomSize() < writen_area_end )
				update_end = PhantomSize();
			else
				update_end = writen_area_end;
			UpdatePhantom(pos,update_end-pos);
		}
	}


	template<class T>
	void PhantomBuffer<T>::Write(unsigned int pos, unsigned int size,const T* data)
	{
		CLAM_DEBUG_ASSERT( (pos+size) <= ( AllocatedSize() ),
		                  "PhantomBuffer::Write(): Position outside bounds");
		unsigned int s,d;

		for (s=0, d=pos; s<size; s++, d++)
			mBuffer[d] = data[s];
		Touch(pos,size);
	}


	template<class T>
	void PhantomBuffer<T>::FulfilsInvariant() const
	{
		CLAM_ASSERT(mBuffer.size() == AllocatedSize(),
		           "PhantomBuffer<T>::FulfilsInvariant(): "
		           "internal vector size unconsistency");
		CLAM_ASSERT(PhantomSize() <= mLogicalSize,
		           "PhantomBuffer<T>::FulfilsInvariant(): "
		           "Phantom size biger than logical size");
		unsigned int i;
		for (i=0; i<PhantomSize(); i++)
			CLAM_ASSERT(mBuffer[i] == mBuffer[i+mLogicalSize],
			            "PhantomBuffer<T>::FulfilsInvariant(): "
			            "Phantom data is unconsistent!");

	}

} // namespace CLAM

#endif

