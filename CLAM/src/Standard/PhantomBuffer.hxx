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

#ifndef _PHANTOM_BUFFER_H
#define _PHANTOM_BUFFER_H

#include "Assert.hxx"
#include "Array.hxx"

namespace CLAM {

	template<class T>
	class PhantomBuffer {

		Array<T> mArray;

		unsigned int mLogicalSize;

		unsigned int mPhantomSize;

		void UpdatePhantom(unsigned int pos, unsigned int size);
		void UpdateBegining(unsigned int pos, unsigned int size);

	public:
		PhantomBuffer();
		T*   Read(unsigned int pos, unsigned int size);
		void Read(Array<T>& dest, unsigned int pos, unsigned int size);
		void Touch(unsigned int pos, unsigned int size);
		void Write(unsigned int pos, unsigned int size,const T* data);

		int  Size() {return mLogicalSize;}
		void Resize(unsigned int size,
		            unsigned int phantom,
		            unsigned int insert_pos);
		void FulfilsInvariant() const;
	};

	template<class T>
	PhantomBuffer<T>::PhantomBuffer()
		: mLogicalSize(0),
		  mPhantomSize(0)
	{}


	template<class T>
	void PhantomBuffer<T>::UpdatePhantom(unsigned int pos, unsigned int size)
	{
		CLAM_DEBUG_ASSERT(pos+size <= mPhantomSize,
		                 "PhantomBuffer::UpdatePhantom():"
		                 "Requested region falls outside phantom zone");
		unsigned int s,d;
		for (s=pos, d=mLogicalSize+pos; s<pos+size; s++, d++)
			mArray[d] = mArray[s];
	}

	template<class T>
	void PhantomBuffer<T>::UpdateBegining(unsigned int pos, unsigned int size)
	{
		CLAM_DEBUG_ASSERT(pos+size <= mPhantomSize,
		                 "PhantomBuffer::UpdateBegining():"
		                 "Requested region falls outside begining zone");
		unsigned int s,d;
		for (d=pos, s=mLogicalSize+pos; d<size; d++, s++)
			mArray[d] = mArray[s];
	}

	template<class T>
	void PhantomBuffer<T>::Resize(unsigned int new_logical_size, 
								  unsigned int new_phantom_size, 
								  unsigned int insertion_pos)
	{
		CLAM_ASSERT(new_logical_size >= mLogicalSize,
		           "PhantomBuffer::Resize(): Size can not be decreased.");
		CLAM_ASSERT(new_phantom_size >= mPhantomSize,
		           "PhantomBuffer::Resize(): PhantomSize can not be decreased.");
		CLAM_ASSERT(insertion_pos <= mLogicalSize,
		           "PhantomBuffer::Resize(): "
		           "Insertion position can not be greater than previous size.");
		CLAM_ASSERT(new_logical_size >= new_phantom_size,
		           "PhantomBuffer::Resize(): Phantom Size can not be greater than size.");

		unsigned int new_total_size = new_logical_size + new_phantom_size;
		unsigned int old_phantom_size = mPhantomSize;
		unsigned int old_logical_size = mLogicalSize;
		unsigned int phantom_increase = new_phantom_size-old_phantom_size;
		
		mPhantomSize = new_phantom_size;
		mLogicalSize = new_logical_size;

		bool needs_insertion = insertion_pos < old_logical_size;

		if (needs_insertion)
		{
			unsigned int elements_to_insert = new_logical_size - old_logical_size;

			mArray.Resize( new_total_size - elements_to_insert );
			mArray.SetSize( new_total_size - elements_to_insert );

			unsigned int i;
			for (i=0; i<elements_to_insert; i++)
				mArray.InsertElem(insertion_pos,T());

			if (insertion_pos < old_phantom_size)
				UpdatePhantom(insertion_pos, new_phantom_size-insertion_pos);
			else
				UpdatePhantom(old_phantom_size, phantom_increase);
		}
		else
		{
			mArray.Resize(new_total_size);
			mArray.SetSize(new_total_size);
			UpdatePhantom(old_phantom_size, phantom_increase);
		}
	}

	template<class T>
	T* PhantomBuffer<T>::Read(unsigned int pos, unsigned int size)
	{
		CLAM_DEBUG_ASSERT( (pos+size-1) <= (mLogicalSize+mPhantomSize),
		                  "PhantomBuffer::Read(): Position outside bounds");
		return &mArray[pos];
	}

	template<class T>
	void PhantomBuffer<T>::Read(Array<T> &dest, unsigned int pos, unsigned int size)
	{
		CLAM_DEBUG_ASSERT( (pos+size-1) <= (mLogicalSize+mPhantomSize),
		                  "PhantomBuffer::Read(): Position outside bounds");
		mArray.GiveChunk(pos,size,dest);
	}

	template<class T>
	void PhantomBuffer<T>::Touch(unsigned int pos, unsigned int size)
	{
		CLAM_DEBUG_ASSERT( (pos+size) <= (mLogicalSize+mPhantomSize),
		                  "PhantomBuffer::Write(): Position outside bounds");
		unsigned int writen_area_end = pos+size;

		if (writen_area_end > mLogicalSize)
		{
			unsigned int update_start;
			if (mLogicalSize > pos)
				update_start = 0;
			else
				update_start = pos-mLogicalSize;
			UpdateBegining(update_start, writen_area_end - mLogicalSize - update_start);
		}
		else if ( pos < mPhantomSize )
		{
			int update_end;
			if (mPhantomSize < writen_area_end )
				update_end = mPhantomSize;
			else
				update_end = writen_area_end;
			UpdatePhantom(pos,update_end-pos);
		}
	}

	template<class T>
	void PhantomBuffer<T>::Write(unsigned int pos, unsigned int size,const T* data)
	{
		CLAM_DEBUG_ASSERT( (pos+size) <= (mLogicalSize+mPhantomSize),
		                  "PhantomBuffer::Write(): Position outside bounds");
		unsigned int s,d;

		for (s=0, d=pos; s<size; s++, d++)
			mArray[d] = data[s];
		Touch(pos,size);
	}

	template<class T>
	void PhantomBuffer<T>::FulfilsInvariant() const
	{
		CLAM_ASSERT(mArray.Size() == mLogicalSize + mPhantomSize,
		           "PhantomBuffer<T>::FulfilsInvariant(): "
		           "internal vector size unconsistency");
		CLAM_ASSERT(mPhantomSize <= mLogicalSize,
		           "PhantomBuffer<T>::FulfilsInvariant(): "
		           "Phantom size biger than logical size");
		unsigned int i;
		for (i=0; i<mPhantomSize; i++)
			CLAM_ASSERT(mArray[i] == mArray[i+mLogicalSize],
			            "PhantomBuffer<T>::FulfilsInvariant(): "
			            "Phantom data is unconsistent!");
				   
	}

};

#endif
