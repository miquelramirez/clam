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

#ifndef	_CircularBuffer_
#define _CircularBuffer_

#include "Array.hxx"
#include "DataTypes.hxx"
#include "Err.hxx"

namespace CLAM {

	template <class T> class CircularBuffer
	{
	/*testing*/
	//protected:
	public:
		Array<T> mBuffer;
		TSize mLogicalSize;
		TSize mReadSize;
		TSize mWriteSize;
		TSize mWriteIndex;
		TSize mReadIndex;
		TSize mInitialReadOffset;
		TSize mInitialWriteOffset;
	
	public:
	 
		/** CONSTRUCTION 
		*/
		CircularBuffer():mBuffer()
		{
			mWriteIndex = 0;				
			mReadIndex = 0;
			mReadSize=mWriteSize=1;
			mInitialReadOffset=0;
			mInitialWriteOffset=0;
			SetBufferSize(0);
			mLogicalSize=0;
		}
		
		CircularBuffer(TSize bufferSize)
			:mBuffer(bufferSize)
		{
			CLAM_ASSERT(bufferSize > 0,"CircularBuffer:CircularBuffer: BufferSize has to be larger than zero");
			SetBufferSize(bufferSize);
			mWriteIndex = 0;				
			mReadIndex = 0;
			mReadSize=mWriteSize=1;
			mInitialReadOffset=0;
			mInitialWriteOffset=0;
			mLogicalSize=0;
		};


		/** GET AND SETTERS
		*/
			
		//get pointer to element
		T& GetPtrToElement(int absPos)
		{
			int index = absPos;
			if(index >= GetBufferSize())
				index -= GetBufferSize();
			if(index < 0)
				index += GetBufferSize();

			return mBuffer[index];
		};

		TSize GetBufferSize()
		{
			return mBuffer.Size();
		};

		void SetBufferSize(TSize size)
		{
			mBuffer.Resize(size);
			mBuffer.SetSize(size);
			InitPointers();
		};


		TSize GetWriteIndex()
		{
			return mWriteIndex;
		};

		TSize GetReadIndex()
		{
			return mReadIndex;
		};

		
		TSize GetInitialReadOffset()
		{
			return mInitialReadOffset;
		};

		void SetInitialReadOffset(TSize size)
		{
			CLAM_ASSERT((mInitialReadOffset >= 0)&&(mInitialReadOffset < GetBufferSize()-1),
				"CircularBuffer:SetInitialReadOffset: InitialReadOffset has to be larger than zero");
			mInitialReadOffsetSize = size;
		};

		
		/** PUBLIC METHODS FOR DATA ACCESS
		*/
		
		void Init()
		{
			SetBufferToZero();
			InitPointers();
			
		};

		void SetBufferToZero()
		{
			T *pBuffer;
			pBuffer = mBuffer.GetPtr();
			memset(pBuffer, 0, GetBufferSize()*sizeof(T));
			
		};

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
			
		};


		/**PUBLIC METHODS FOR BUFFER READ AND WRITE
		*/

		void Read(T& element)
		{
			element = mBuffer[mReadIndex];
			IncreaseReadIndex();
			
		};

		
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

		void Read(Array<T>& in,TSize offset=0) //const
		{
			CLAM_ASSERT(GetReadSize()<=in.Size()+offset,"Error, input buffer is not large enough");

#ifdef CLAM_HIGH_OPTIMIZATIONS
			
			Read(in.GetPtr()+offset);
		
#else
			for(int i=0;i<mReadSize;i++)
				Read(in[i+offset]);
#endif		
		};

		void Write(const T& element)
		{
			mBuffer[mWriteIndex] = element;
			IncreaseWriteIndex();
			
		};

		
		
		void Write (const T* buffer)
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
		
		void Write(const Array<T>& in,TSize offset=0) 
		{
			CLAM_ASSERT(GetWriteSize()<=in.Size()+offset,"Error, input buffer is not large enough");
#ifdef CLAM_HIGH_OPTIMIZATIONS
			
			Write(in.GetPtr()+offset);
		
#else
			for(int i=0;i<mWriteSize;i++)
				Write(in[i+offset]);
#endif
		};
		
		void Add(const T& elem)
		{
			mBuffer[mWriteIndex]+=elem;
			IncreaseWriteIndex();
		}

		void Add(const Array<T>& in,TSize offset=0)
		{
			CLAM_ASSERT(GetWriteSize()<=in.Size()+offset,"Error, input buffer is not large enough");
			for(int i=0;i<mWriteSize;i++)
				Add(in[i+offset]);
		}

		
		void IncreaseReadIndex(TSize step=1)
		{
			mReadIndex += step;
			mReadIndex=mReadIndex%GetBufferSize();
			mLogicalSize-=step;
			CLAM_ASSERT(mLogicalSize>=0,"Error:Read Index surpassed Write Index");
		};

		void IncreaseWriteIndex(TSize step=1)
		{
			mWriteIndex += step;
			mWriteIndex =mWriteIndex%GetBufferSize();
			mLogicalSize+=step;
			CLAM_ASSERT(mLogicalSize<=GetBufferSize(),"Error:Write Index surpassed Read Index");
		};

		void DecreaseReadIndex(TSize step=1)
		{
			mReadIndex -= step;
			mReadIndex =mReadIndex%GetBufferSize();
			if(mReadIndex<0) mReadIndex=GetBufferSize()+mReadIndex;
			mLogicalSize+=step;
			CLAM_ASSERT(mLogicalSize<=GetBufferSize(),"Error:Write Index surpassed Read Index");
			
		};

		void DecreaseWriteIndex(TSize step=1)
		{
			mWriteIndex -= step;
			mWriteIndex =mWriteIndex%GetBufferSize();
			if(mWriteIndex<0) mWriteIndex =GetBufferSize()+mWriteIndex ;
			mLogicalSize-=step;
			CLAM_ASSERT(mLogicalSize>=0,"Error:Read Index surpassed Write Index");
		};

		/** GET AND SETTERS
		*/
		TSize GetReadSize() const
		{
			return mReadSize;
		}

		TSize GetWriteSize() const
		{
			return mWriteSize;
		}

		void SetReadSize(TSize size)
		{
			CLAM_ASSERT(size>=0&&size<=GetBufferSize(),"AudioCircularBuffer:SetReadSize: ReadSize has to be larger than zero");
			mReadSize = size;
			
		}

		void SetWriteSize(TSize size)
		{
			CLAM_ASSERT(size>=0&&size<=GetBufferSize(),"AudioCircularBuffer:SetWriteSize: WriteSize has to be larger than zero");
			mWriteSize = size;
			
		}

		TSize GetBufferSize() const
		{
			return GetBufferSize;
		}



	};
}; //end of namespace

#endif //_CircularBuffer_
