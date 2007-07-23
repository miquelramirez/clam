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

#ifndef __THREAD__
#define __THREAD__

#include <pthread.h>
#include "CBL.hxx"
#include "Condition.hxx"

// there is a Yield method defined in a Windows API which is included by windows.h
// this undefines the Windows Yield method
#ifdef Yield
#undef Yield
#endif

namespace CLAM
{
	class Condition;

class Thread
{
public:

	Thread(bool realtime = false);

	virtual ~Thread();

	virtual void Start();

	virtual void Stop();

	void Sleep();

	void Sleep( unsigned int milliseconds );

	void WakeUp();

	void Yield();

	void SetThreadCode( const CBL::Functor0& thread_code );

	void SetCleanupCode( const CBL::Functor0& cleanup_code );

	bool operator==( const Thread& other ) const;

	inline bool IsCancelled() const
	{
		return mIsCancelled;
	}

	inline bool IsRunning() const
	{
		return mRunning;
	}

protected:
	virtual void SetupPriorityPolicy();

	bool               mRealtime;
	bool               mHasCode;
	bool               mHasCleanup;
	pthread_t          mThreadID;
	bool               mIsCancelled;
	bool               mRunning;
	CBL::Functor0      mThreadCode;
	CBL::Functor0      mCleanUpCode;
	Condition          mSleepCondition;

private:

	void SleepDetail( unsigned int millisecs );

	// Pthreads needed static functions for launching and cleaning

	static void* LaunchThread( void* pSelf );

	static void  LaunchThreadCleanup( void* pSelf );
};

}

#endif // Thread.hxx

