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

#ifndef	_CircularBuffer_
#define _CircularBuffer_

#include "Array.hxx"
#include "DataTypes.hxx"
#include "Err.hxx"

namespace CLAM
{


	/**
	 * Generic container-like circular buffer class.
	 * It can be pre-allocated to a certain capacity and 
	 * resized at any time (not while doing concurrent 
	 * reading/writing). This circular buffer is not thread-safe.
	 * You can either read/write single elements at a time or 
	 * read/write in blocks. The size of these blocks is specified 
	 * the read size and write size respectively.
	 * The circular buffer must be initialized (by calling Init()) 
	 * prior to being used. An initial read offset can be specified 
	 * which indicates the index at which the buffer starts reading 
	 * after it's been initialized (default: 0).
	 */
	template <class T> class CircularBuffer
	{
	protected: 
		Array<T> mBuffer;
		TSize mLogicalSize;
		TSize mReadSize;
		TSize mWriteSize;
		TSize mWriteIndex; // XXX: might want these mutable and read-like methods declared const
		TSize mReadIndex; // XXX: might want these mutable and read-like methods declared const
		TSize mInitialReadOffset;
		TSize mInitialWriteOffset;
	
	public:
	 
		/**
		 * Default constructor.
		 * Read and write indices will be initialized to 0. Read and write 
		 * sizes will be initialized to 1. Initial read and write offsets will 
		 * be initialized to 0. The buffer size will be 0.
		 */
		CircularBuffer() : mBuffer()
		{
			mWriteIndex = 0;				
			mReadIndex = 0;
			mReadSize = mWriteSize = 1;
			mInitialReadOffset = 0;
			mInitialWriteOffset = 0;
			SetBufferSize(0);
			mLogicalSize = 0;
		}
		
		/**
		 * Constructor which initializes the buffer to a certain capacity.
		 * This capacity (or buffer size) can be arbitrary (not power-of-two, 
		 * etc), but must be greater than 0.
		 */
		CircularBuffer(TSize bufferSize) : mBuffer(bufferSize)
		{
			CLAM_ASSERT(bufferSize > 0, "CircularBuffer:CircularBuffer: BufferSize has to be larger than zero");
			SetBufferSize(bufferSize);
			mWriteIndex = 0;				
			mReadIndex = 0;
			mReadSize = mWriteSize = 1;
			mInitialReadOffset = 0;
			mInitialWriteOffset = 0;
			mLogicalSize = 0;
		}

		// Methods for setting up the buffer: -------------------------------------------

		/**
		 * Returns the circular buffer's buffer size.
		 */
		TSize GetBufferSize() const
		{
			return mBuffer.Size()-GetReadSize();
		}

		/**
		 * Resizes the buffer to a new capacity (or buffer size). 
		 * This capacity (or buffer size) can be arbitrary (not power-of-two, 
		 * etc). If size is set to 0 or less the buffer will be deallocated.
		 */
		void SetBufferSize(TSize size)
		{
			mBuffer.Resize(size+GetReadSize());
			mBuffer.SetSize(size+GetReadSize());
			InitPointers();
		}

		/**
		 * Returns the index of the element that the buffer 
		 * is about to write (ie. on the next write-like call).
		 */
		TSize GetWriteIndex() const
		{
			return mWriteIndex;
		}

		/**
		 * Returns the index of the element that the buffer 
		 * is about to read (ie. on the next read-like call).
		 */
		TSize GetReadIndex() const
		{
			return mReadIndex;
		}

		/**
		 * Returns the initial read offset. That is, the 
		 * index at which the buffer starts reading after it 
		 * has been initialized.
		 */
		TSize GetInitialReadOffset() const
		{
			return mInitialReadOffset;
		}

		/**
		 * Sets the initial read offset. That is, the
		 * index at which the buffer starts reading after it 
		 * has been initialized. Init() or InitPointers() should be 
		 * called after this method.
		 */
		void SetInitialReadOffset(TSize size)
		{
			CLAM_ASSERT((size >= 0)&&(size < GetBufferSize()-1),
				"CircularBuffer:SetInitialReadOffset: InitialReadOffset has to be larger than zero");
			mInitialReadOffset = size;
		}

		/**
		 * Returns the initial write offset. That is, the 
		 * index at which the buffer starts writeing after it 
		 * has been initialized.
		 */
		TSize GetInitialWriteOffset() const
		{
			return mInitialWriteOffset;
		}

		/**
		 * Sets the initial write offset. That is, the
		 * index at which the buffer starts writeing after it 
		 * has been initialized. Init() or InitPointers() should be 
		 * called after this method.
		 */
		void SetInitialWriteOffset(TSize size)
		{
			CLAM_ASSERT((size >= 0)&&(size < GetBufferSize()-1),
				"CircularBuffer:SetInitialWriteOffset: InitialWriteOffset has to be larger than zero");
			mInitialWriteOffset = size;
		}

		// Methods for data acces: ------------------------------------------------------
	
		/**
		 * Initializes the buffer by setting all elements to 0 and 
		 * making sure read/write indices are set correctly (taking 
		 * into account initial read/write offsets).
		 */
		void Init()
		{
			SetBufferToZero();
			InitPointers();
		}

		/**
		 * Sets all elements in circular buffer to zero.
		 */
		void SetBufferToZero()
		{
			T *pBuffer;
			pBuffer = mBuffer.GetPtr();
			memset(pBuffer, 0, GetBufferSize()*sizeof(T));
		}

		/**
		 * Initializes read/write indices (taking into account 
		 * initial read/write offsets).
		 */
		void InitPointers()
		{
			if(mInitialReadOffset < 0)
				mReadIndex = GetBufferSize() + mInitialReadOffset;
			else
				mReadIndex = mInitialReadOffset;
			if(mInitialWriteOffset < 0)
				mWriteIndex = GetBufferSize() + mInitialWriteOffset;
			else
				mWriteIndex = mInitialWriteOffset;
			mLogicalSize=0;
		}

		// Methods for reading and writing: ---------------------------------------------

		/**
		 * Reads a single element at the current read index 
		 * into element.
		 */
		void Read(T& element)
		{
			element = mBuffer[mReadIndex];
			IncreaseReadIndex();
		}

		
		void NonCopyRead(Array<T>& buffer)
		{
			TSize limit;
			if((limit=mReadIndex+mReadSize)>GetBufferSize())
			{
				//will have to use phantom zone	
				memcpy(mBuffer.GetPtr()+GetBufferSize(),mBuffer.GetPtr(),mReadSize*sizeof(T));
			}
			buffer.SetPtr(mBuffer.GetPtr()+mReadIndex,mReadSize);
			IncreaseReadIndex(mReadSize);


		}
		
		/**
		 * Reads read size number of elements starting at the 
		 * current read index into buffer. If the read size + 
		 * read index is out of bounds (ie. past the end of the buffer) 
		 * the reading will 'wrap-around'.
		 */
		void Read(T* buffer)
		{
			TSize limit;
			if((limit=mReadIndex+mReadSize)>GetBufferSize())
			{
				TSize secondHalf=limit%GetBufferSize();
				TSize firstHalf=mReadSize-secondHalf;
				memcpy(buffer,mBuffer.GetPtr()+mReadIndex,firstHalf*sizeof(T));
				memcpy(buffer+firstHalf,mBuffer.GetPtr(),secondHalf*sizeof(T));
			}
			else
			{
				memcpy(buffer,mBuffer.GetPtr()+mReadIndex,mReadSize*sizeof(T));
			}
			
			IncreaseReadIndex(mReadSize);
		}

		/**
		 * Reads read size number of elements starting at the 
		 * current read index into the array at the specified offset 
		 * (default: offset = 0). The destination array must be large 
		 * enough to hold read size number of elements at the specified 
		 * offset. If reading is out of bounds it will wrap-around.
		 * Note: define CLAM_HIGH_OPTIMIZATIONS for this function to be 
		 * efficient.
		 */
		void Read(Array<T>& in, TSize offset = 0) // XXX: maybe call in, out instead?
		{
			CLAM_ASSERT(GetReadSize() <= in.Size()+offset, "Error, input buffer is not large enough"); // XXX: maybe call the input buffer the output buffer?

#ifdef CLAM_HIGH_OPTIMIZATIONS
			Read(in.GetPtr()+offset);
#else
			for(int i=0;i<mReadSize;i++)
				Read(in[i+offset]);
#endif
		}

		/**
		 * Writes a single element at the current write index.
		 */
		void Write(const T& element)
		{
			mBuffer[mWriteIndex] = element;
			IncreaseWriteIndex();
		}

		/**
		 * Writes write size number of elements starting at the 
		 * current write index from buffer. If it must write out 
		 * of bounds, it will 'wrap-around'.
		 */
		void Write(const T* buffer)
		{
			TSize limit;
			if((limit=mWriteIndex+mWriteSize)>GetBufferSize())
			{
				TSize secondHalf=limit%GetBufferSize();
				TSize firstHalf=mWriteSize-secondHalf;
				memcpy(mBuffer.GetPtr()+mWriteIndex,buffer,firstHalf*sizeof(T));
				memcpy(mBuffer.GetPtr(),buffer+firstHalf,secondHalf*sizeof(T));
			}
			else
			{
				memcpy(mBuffer.GetPtr()+mWriteIndex,buffer,mWriteSize*sizeof(T));
			}
			
			IncreaseWriteIndex(mWriteSize);
		}

		/**
		 * Writes write size number of elements starting at the current 
		 * write index from the array starting at the specified offset 
		 * (default: offset = 0). The input array must have atleast write 
		 * size number of elements from the specified offset onwards.
		 * Note: define CLAM_HIGH_OPTIMIZATIONS for this method to be efficient.
		 */
		void Write(const Array<T>& in, TSize offset = 0) 
		{
			CLAM_ASSERT(GetWriteSize()<=in.Size()+offset,"Error, input buffer is not large enough");
#ifdef CLAM_HIGH_OPTIMIZATIONS
			Write(in.GetPtr()+offset);
#else
			for(int i=0;i<mWriteSize;i++)
				Write(in[i+offset]);
#endif
		}

		/**
		 * Does an additive write of a single element at the 
		 * current write index. It 'mixes' the old element's content 
		 * with the new one.
		 */
		void Add(const T& elem)
		{
			mBuffer[mWriteIndex] += elem;
			IncreaseWriteIndex();
		}

		/**
		 * Does an additive write of write size number of elements 
		 * at the current write index onwards from the specified 
		 * input array starting at the specified offset (default: 
		 * offset = 0) onwards.
		 * Note: not very efficient.
		 */
		void Add(const Array<T>& in, TSize offset = 0)
		{
			CLAM_ASSERT(GetWriteSize()<=in.Size()+offset,"Error, input buffer is not large enough");
			for(int i=0;i<mWriteSize;i++)
				Add(in[i+offset]);
			// XXX: might also want a CLAM_HIGH_OPTIMIZATIONS version of this method...
		}

		/**
		 * Increases the read index by the specified number of 
		 * elements (default: step = 1).
		 */
		void IncreaseReadIndex(TSize step = 1)
		{
			mReadIndex += step;
			mReadIndex=mReadIndex%GetBufferSize();
			mLogicalSize-=step;
			CLAM_ASSERT(mLogicalSize>=0,"Error:Read Index surpassed Write Index");
		}

		/**
		 * Increases the write index by the specified number of 
		 * elements (default: step = 1).
		 */
		void IncreaseWriteIndex(TSize step = 1)
		{
			// XXX: might want to assert that step > 0
			mWriteIndex += step;
			mWriteIndex =mWriteIndex%GetBufferSize();
			mLogicalSize+=step;
			CLAM_ASSERT(mLogicalSize<=GetBufferSize(),"Error:Write Index surpassed Read Index");
		}

		/**
		 * Decreases the read index by the specified number of 
		 * elements (default: step = 1).
		 */
		void DecreaseReadIndex(TSize step = 1)
		{
			// XXX: might want to assert that step > 0
			mReadIndex -= step;
			mReadIndex =mReadIndex%GetBufferSize();
			if(mReadIndex<0) mReadIndex=GetBufferSize()+mReadIndex;
			mLogicalSize+=step;
			CLAM_ASSERT(mLogicalSize<=GetBufferSize(),"Error:Write Index surpassed Read Index");
		}

		/**
		 * Decreases the write index by the specified number of 
		 * elements (default: step = 1).
		 */
		void DecreaseWriteIndex(TSize step = 1)
		{
			// XXX: might want to assert that step > 0
			mWriteIndex -= step;
			mWriteIndex =mWriteIndex%GetBufferSize();
			if(mWriteIndex<0) mWriteIndex =GetBufferSize()+mWriteIndex ;
			mLogicalSize-=step;
			CLAM_ASSERT(mLogicalSize>=0,"Error:Read Index surpassed Write Index");
		}

		/**
		 * Returns a pointer (well, reference) to the element 
		 * at the specified index. If the specified index is 
		 * out of bounds it will 'wrap-around'.
		 */
		T& GetPtrToElement(int absPos)
		{
			int index = absPos%GetBufferSize(); // XXX: unnecessary copy.
			
			return mBuffer[index];
		}
		
		// Getters and setters: ---------------------------------------------------------

		/**
		 * Returns the current read size. The read size 
		 * determines how many elements will read at once 
		 * when doing 'chunk' reads.
		 */
		TSize GetReadSize() const
		{
			return mReadSize;
		}

		/**
		 * Returns the current write size. The write size 
		 * determines how many elements will be written at once 
		 * when doing 'chunk' writes.
		 */
		TSize GetWriteSize() const
		{
			return mWriteSize;
		}

		/**
		 * Sets the read size. This read size must be greater than or 
		 * equal to 0 and less than or equal to the buffer size.
		 */
		void SetReadSize(TSize size)
		{
			CLAM_ASSERT(size>=0&&size<=GetBufferSize(),"AudioCircularBuffer:SetReadSize: ReadSize has to be larger than zero");
			TSize previousBufferSize=GetBufferSize();
			mReadSize = size;
			SetBufferSize(previousBufferSize);
		}

		/**
		 * Sets the write size. This write size must be greater than or 
		 * equal to 0 and less than or equal to the buffer size.
		 */
		void SetWriteSize(TSize size)
		{
			CLAM_ASSERT(size>=0&&size<=GetBufferSize(),"AudioCircularBuffer:SetWriteSize: WriteSize has to be larger than zero");
			mWriteSize = size;
		}
	};



}; //end of namespace

#endif //_CircularBuffer_

