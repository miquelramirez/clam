#ifndef __DOUBLEBUFFER__
#define __DOUBLEBUFFER__

#include "ErrOutOfMemory.hxx"
#include "CLAM_windows.h"
#undef GetClassName

namespace CLAM
{

struct DoubleBuffer
{
	short*   mFrontBuffer;
	short*   mBackBuffer;
	short*   mBuffA;
	short*   mBuffB;
	unsigned mLen;
	//bool     mBackBufferReady;
	HANDLE   mBackBufferReady;

	DoubleBuffer()
		: mBuffA( NULL ), mBuffB( NULL ), mLen(0), mBackBufferReady( false )
	{
		mBackBufferReady = CreateEvent( NULL, TRUE, FALSE, NULL );
	}

	~DoubleBuffer()
	{
		if ( mBuffA )
			delete [] mBuffA;
		if ( mBuffB )
			delete [] mBuffB;
	}

	void Allocate( unsigned nelems ) throw (ErrOutOfMemory);

	inline void DeAllocate()
	{
		if ( mBuffA )
			delete [] mBuffA;
		if ( mBuffB )
			delete [] mBuffB;

		mBuffA = NULL;
		mBuffB = NULL;
		mFrontBuffer = NULL;
		mBackBuffer = NULL;
	}

	void SwapBuffers()
	{
		short* temp;
		temp = mFrontBuffer;
		mFrontBuffer = mBackBuffer;
		mBackBuffer = temp;
		SetEvent( mBackBufferReady );
	}

	inline unsigned GetSize() const
	{
		return mLen;
	}

};


struct FullDuplexDoubleBuffer
{
	DoubleBuffer* mInputDblBuff;
	DoubleBuffer* mOutputDblBuff;

	inline unsigned GetSize()
	{
		return mInputDblBuff->mLen;
	}
};

}

#endif // DoubleBuffer.hxx
