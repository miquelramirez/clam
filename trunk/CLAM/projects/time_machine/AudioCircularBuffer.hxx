#ifndef	_AudioCircularBuffer_
#define _AudioCircularBuffer_

#include "Array.hxx"
#include "DataTypes.hxx"
#include "Defines.hxx"
#include "Err.hxx"

namespace MTG {

	template <class T> class AudioCircularBuffer
	{
	protected:
		MTG::Array<T> mCircBuffer;
		TInt32 mFrameSize;				
		TInt32 mCircBufferSize;	
		TInt32 mWriteIndex;
		TInt32 mReadIndex;
  
	public:
   
    /** CONSTRUCTION 
    */
		AudioCircularBuffer(TInt32 frameSize, TInt32 frameBufferSize)
			:mCircBuffer(frameBufferSize)
		{
		#ifdef PARANOID
			if(frameSize > 0)
		#endif
				mFrameSize = frameSize;
		#ifdef PARANOID
			else throw Err("LFCircularFrame:LFCircularFrame: FrameSize has to be larger than zero");
		#endif
		#ifdef PARANOID
			if(frameBufferSize > 0)
		#endif
				mCircBufferSize = frameBufferSize;
		#ifdef PARANOID
			else throw Err("LFCircularFrameBuffer:LFCircularFrameBuffer: FrameBufferSize has to be larger than zero");
		#endif
			mCircBuffer.Resize(mCircBufferSize);
			mCircBuffer.SetSize(mCircBufferSize);
			mWriteIndex = 0;
			mReadIndex = 0;
		};


		/**PUBLIC METHODS FOR BUFFER READ AND WRITE
		*/
		bool ReadFromBuffer(MTG::Array<T>& bufferData)
		{
			TIndex previousIndex=mReadIndex;
			bool splitBuffer = IncreaseReadIndex(mFrameSize); 
			
			//get pointers to data
			T* ptr1=mCircBuffer.GetPtr();
			T* ptr2=bufferData.GetPtr();
			int n = bufferData.Size();

			// First we must check that index can be increased
			if(previousIndex+n> mCircBufferSize) //End of circular buffer has not been reached
			{
				int toEnd = mCircBuffer.AllocatedSize()-previousIndex;
				memcpy(ptr2, ptr1+previousIndex, sizeof(T)*toEnd);
				memcpy(ptr2+toEnd, ptr1, sizeof(T)*(n-toEnd));
			}
			else
			{
				memcpy(ptr2,ptr1+previousIndex,	sizeof(T)*n);
			}		
			return true;
		};

		bool ReadFromBuffer(MTG::Audio& in)
		{
			return ReadFromBuffer(in.GetFloatBuffer());
		};
		

		bool OverwriteBuffer(T *pData, int size)
		{
			TIndex previousIndex=mWriteIndex;
			bool splitBuffer = IncreaseWriteIndex(size);
			//get pointers to data
			T* pmBuffer=mCircBuffer.GetPtr();

			//first me must check if the index can be increaased
			if(splitBuffer) //End of circular buffer has not been reached
			{
				int toEnd = mCircBuffer.AllocatedSize()-previousIndex;
				memcpy(pmBuffer+previousIndex, pData, sizeof(T)*toEnd);
				memcpy(pmBuffer, pData+toEnd, sizeof(T)*(size-toEnd)); 
			}
			else
			{
				memcpy(pmBuffer+previousIndex,pData,size*sizeof(T));
			}
			return true;
		};

		bool OverwriteBuffer(MTG::Array<T>& in)
		{
			return OverwriteBuffer(in.GetPtr(), in.AllocatedSize());
		};

		bool OverwriteBuffer(MTG::Audio& in)
		{
			return OverwriteBuffer(in.GetFloatBuffer());
		};



		bool AddToBuffer(int nElements, TData *pbuffer, int bufferSize)
		{
			TIndex previousIndex=mWriteIndex;
			bool splitBuffer = IncreaseWriteIndex(bufferSize);

			//get pointers to data
			T* pmCircBuffer=mCircBuffer.GetPtr();

			//first me must check if the index can be increaased
			if(splitBuffer)//End of circular buffer has not been reached
			{
				int toEnd = mCircBuffer.AllocatedSize()-previousIndex;
				
				for(int i=0, j=previousIndex; i<toEnd; i++, j++)
					pmCircBuffer[j] += pbuffer[i];

				for(i=0, j=toEnd; i<bufferSize-toEnd; i++, j++)
					pmCircBuffer[i] += pbuffer[j];
			}
			else
			{
				for(int i=0, j=previousIndex; i<bufferSize; i++, j++)
					pmCircBuffer[j] += pbuffer[i];
			}
			return true;
		};

		bool AddToBuffer(int nElements, MTG::Array<T>& bufferData)
		{
			return AddToBuffer(nElements, bufferData.GetPtr(), bufferData.AllocatedSize());
		};



    /** GET AND SETTERS
    */
		TInt32 GetFrameSize(){
			return mFrameSize;}

		bool SetFrameSize(TInt32 size){
		#ifdef PARANOID
			if(mFrameSize > 0){
		#endif
				mFrameSize = size;
				return true;
		#ifdef PARANOID
			}
			else{
				throw Err("AudioCircularBuffer:SetFrameSize: FrameSize has to be larger than zero");
				return false;}
		#endif
		};

		TInt32 GetBufferSize(){
			return mCircBufferSize;}

		bool SetBufferSize(TInt32 size){
		#ifdef PARANOID
			if(mCircBufferSize > 0){
		#endif
				mCircBufferSize = size;
				return true;
		#ifdef PARANOID
			}
			else{
				throw Err("AudioCircularBuffer:SetFrameBufferSize: FrameBufferSize has to be larger than zero");
				return false;}
		#endif
		};


		TInt32 GetWriteIndex(){
			return mWriteIndex;}

		bool SetWriteIndex(TInt32 size){
		#ifdef PARANOID
			if((mWriteIndex >= 0)&&(mWriteIndex < mCircBufferSize-1)){
		#endif
				mWriteIndexSize = size;
				return true;
		#ifdef PARANOID
			}
			else{
				throw Err("AudioCircularBuffer:SetFrameWriteIndex: FrameWriteIndex has to be larger than zero");
				return false;}
		#endif
		};

		TInt32 GetReadIndex(){
			return mReadIndex;}


		bool SetReadIndex(TInt32 size){
		#ifdef PARANOID
			if((mReadIndex >= 0)&&(mReadIndex < mCircBufferSize-1)){
		#endif
				mReadIndexSize = size;
				return true;
		#ifdef PARANOID
			}
			else{
				throw Err("AudioCircularBuffer:SetFrameReadIndex: FrameReadIndex has to be larger than zero");
				return false;}
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
			pBuffer = mCircBuffer.GetPtr();
			memset(pBuffer, 0, mCircBufferSize*sizeof(T));
			return true;
		};

		bool InitPointers()
		{
			mReadIndex = 0;
			mWriteIndex = 0;
			return true;
		};

		/**PUBLIC METHODS FOR INDEX HANDLING
		*/
		bool IncreaseReadIndex(TInt32 step = 1)
		{
			mReadIndex += step;
			if(mReadIndex >= mCircBufferSize){
				mReadIndex = mReadIndex - mCircBufferSize;
				return true;}
			return false;
		};


		bool IncreaseWriteIndex(TInt32 step = 1)
		{
			mWriteIndex += step;
			if(mWriteIndex >= mCircBufferSize){
				mWriteIndex = mWriteIndex - mCircBufferSize;
				return true;}
			return false;
		};


		bool DecreaseReadIndex(TInt32 step = 1)
		{
			mReadIndex -= step;
			if(mReadIndex < 0){
				mReadIndex = mReadIndex + mCircBufferSize;
				return true;}
			return false;
		};


		bool DecreaseWriteIndex(TInt32 step = 1)
		{
			mWriteIndex -= step;
			if(mWriteIndex < 0){
				mWriteIndex = mWriteIndex + mCircBufferSize;						
				return true;}
			return false;
		};
	};
}; //end of namespace

#endif _AudioCircularBuffer_