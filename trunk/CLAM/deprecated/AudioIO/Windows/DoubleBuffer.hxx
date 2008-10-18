/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

