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

#ifndef	_TemplateCircularBuffer_
#define _TemplateCircularBuffer_

#include "Array.hxx"
#include "DataTypes.hxx"
#include "Defines.hxx"
#include "Err.hxx"

namespace CLAM {

	template <class T> class TemplateCircularBuffer
	{
	protected:
		CLAM::Array<T> mBuffer;
		TInt32 mBufferSize;				
		TInt32 mFrameSize;				
		TInt32 mWriteIndex;
		TInt32 mReadIndex;
		TInt32 mInitialReadOffset;
		TInt32 mInitialWriteOffset;
	
	public:
	 
		/** CONSTRUCTION 
		*/

		TemplateCircularBuffer(TInt32 bufferSize)
			:mBuffer(bufferSize)
		{
			if(bufferSize > 0)
			{
				mBufferSize = bufferSize;
				mBuffer.Resize(mBufferSize);
				mBuffer.SetSize(mBufferSize);
				mWriteIndex = 0;				
				mReadIndex = 0;
			}
			else throw Err("TemplateCircularBuffer:TemplateCircularBuffer: BufferSize has to be larger than zero");
		};


		/** GET AND SETTERS
		*/
			
		//get pointer to element
		T& GetPtrToElement(int absPos)
		{
			int index = absPos;
			if(index >= mBufferSize)
				index -= mBufferSize;
			if(index < 0)
				index += mBufferSize;

			return mBuffer[index];
		};

		TInt32 GetBufferSize()
		{
			return mBufferSize;
		};

		bool SetBufferSize(TInt32 size)
		{
		#ifdef PARANOID
			if(mBufferSize > 0)
			{
				mBufferSize = size;
				return true;
			}
			else
			{
				throw Err("TemplateCircularBuffer:SetBufferSize: BufferSize has to be larger than zero");
				return false;
			}
		#else
			mBufferSize = size;
			return true;
		#endif
		};


		TInt32 GetWriteIndex()
		{
			return mWriteIndex;
		};

		bool SetWriteIndex(TInt32 size)
		{
		#ifdef PARANOID
			if((mWriteIndex >= 0)&&(mWriteIndex < mBufferSize-1)){
				mWriteIndexSize = size;
				return true;}
			else{
				throw Err("TemplateCircularBuffer:SetWriteIndex: FrameWriteIndex has to be larger than zero");
				return false;}
		#else
			mWriteIndexSize = size;
			return true;
		#endif
		};

		TInt32 GetReadIndex()
		{
			return mReadIndex;
		};

		bool SetReadIndex(TInt32 size)
		{
		#ifdef PARANOID
			if((mReadIndex >= 0)&&(mReadIndex < mBufferSize-1)){
				mReadIndexSize = size;
				return true;}
			else{
				throw Err("TemplateCircularBuffer:SetReadIndex: FrameReadIndex has to be larger than zero");
				return false;}
		#else
			mReadIndexSize = size;
			return true;
		#endif
		};


		TInt32 GetInitialReadOffset()
		{
			return mInitialReadOffset;
		};

		bool SetInitialReadOffset(TInt32 size)
		{
		#ifdef PARANOID
			if((mInitialReadOffset >= 0)&&(mInitialReadOffset < mBufferSize-1)){
				mInitialReadOffsetSize = size;
				return true;}
			else{
				throw Err("TemplateCircularBuffer:SetInitialReadOffset: InitialReadOffset has to be larger than zero");
				return false;}
		#else
			mInitialReadOffsetSize = size;
			return true;
		#endif
		};

		
		/** PUBLIC METHODS FOR DATA ACCESS
		*/
		
		bool InitBuffer()
		{
			SetBufferToZero();
			InitPointers();
			return true;
		};

		bool SetBufferToZero()
		{
			T *pBuffer;
			pBuffer = mBuffer.GetPtr();
			memset(pBuffer, 0, mBufferSize*sizeof(T));
			return true;
		};

		bool InitPointers()
		{
			if(mInitialReadOffset < 0)
				mReadIndex = mBufferSize + mInitialReadOffset;
			else
				mReadIndex = mInitialReadOffset;
			if(mInitialWriteOffset < 0)
				mWriteIndex = mBufferSize + mInitialWriteOffset;
			else
				mWriteIndex = mInitialWriteOffset;
			return true;
		};

		bool ReadFromBuffer(T& element)
		{
			element = mBuffer[mReadIndex];
			IncreaseReadIndex(1);
			return true;
		};

		bool OverwriteBuffer(T& element)
		{
			mBuffer[mWriteIndex] = element;
			IncreaseWriteIndex(1);
			return true;
		};


		bool IncreaseReadIndex(TInt32 step)
		{
			mReadIndex += step;
			if(mReadIndex >= mBufferSize){
				mReadIndex = mReadIndex - mBufferSize;
				return true;}
			return false;
		};

		bool IncreaseWriteIndex(TInt32 step)
		{
			mWriteIndex += step;
			if(mWriteIndex >= mBufferSize){
				mWriteIndex = mWriteIndex - mBufferSize;
				return true;}
			return false;
		};

		bool DecreaseReadIndex(TInt32 step)
		{
			mReadIndex -= step;
			if(mReadIndex < 0){
				mReadIndex = mReadIndex + mBufferSize;
				return true;}
			return false;
		};

		bool DecreaseWriteIndex(TInt32 step)
		{
			mWriteIndex -= step;
			if(mWriteIndex < 0){
				mWriteIndex = mWriteIndex + mBufferSize;						
				return true;}
			return false;
		};
	};
}; //end of namespace

#endif _TemplateCircularBuffer_
