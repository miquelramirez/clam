#include "DoubleBuffer.hxx"

namespace CLAM
{

	void DoubleBuffer::Allocate( unsigned nelems )
	{
		mBuffA = new short[nelems];
		mBuffB = new short[nelems];

		if ( mBuffA == NULL || mBuffB == NULL )
			throw (ErrOutOfMemory(nelems*2));

		mFrontBuffer = mBuffA;
		mBackBuffer = mBuffB;
		mLen = nelems;
		mBackBufferReady = false;
	}
}
