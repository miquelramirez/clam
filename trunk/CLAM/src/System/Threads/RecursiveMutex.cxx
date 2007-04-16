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

#include "RecursiveMutex.hxx"
#include "xtime.hxx"
#include "Assert.hxx"
#include <pthread.h>
#include <errno.h>

namespace CLAM
{


	RecursiveMutex::RecursiveMutex()
		: mCount(0), mValidID( false )
	{
		pthread_mutexattr_t attr;

		int res = 0;

		res = pthread_mutexattr_init( &attr );
		CLAM_ASSERT( res == 0, "pthread_mutexattr_init failed" );

		res = pthread_mutex_init( &mMutex, &attr );
		CLAM_ASSERT( res == 0, "pthread_mutex_init failed");

		res = pthread_cond_init( &mUnlocked, 0 );

		if ( res!= 0 )
			{
				pthread_mutex_destroy( &mMutex );
				throw ThreadResourceError("pthread_cond_init failed");
			}	
	}

	RecursiveMutex::~RecursiveMutex()
	{
		int res = 0;
		res = pthread_mutex_destroy( &mMutex );

		CLAM_ASSERT( res == 0, "pthread_mutex_destroy failed" );

		res = pthread_cond_destroy( &mUnlocked );

		CLAM_ASSERT( res == 0, "pthread_cond_destroy failed" );
	}


	void RecursiveMutex::DoLock()
	{
		int res = 0;

		res = pthread_mutex_lock( &mMutex );

		pthread_t tid = pthread_self();

		if ( mValidID && pthread_equal( mThreadID, tid ) )
			++ mCount;
		else
			{
				while ( mValidID )
					{
						res = pthread_cond_wait( &mUnlocked, &mMutex );
						CLAM_ASSERT( res == 0, "pthread_cond_wait failed" );
					}

				mThreadID = tid;
				mValidID = true;
				mCount = 1;
			}
		
		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
	}

	void RecursiveMutex::DoUnlock( )
	{
		int res = 0;
		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_lock failed" );

		pthread_t tid = pthread_self();

		if ( mValidID && !pthread_equal( mThreadID, tid ) )
			{
				res = pthread_mutex_unlock( &mMutex );
				CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
			}
		
		if ( --mCount == 0 )
			{
				CLAM_ASSERT( mValidID, "Spurious value change!" );
				mValidID = false;
				
				res = pthread_cond_signal( &mUnlocked );
				CLAM_ASSERT( res==0, "pthread_cond_signal failed" );
			}

		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
	}

	void RecursiveMutex::DoLock( ConditionVar& state )
	{
		int res = 0;
		
		while ( mValidID )
			{
				res = pthread_cond_wait( &mUnlocked, &mMutex );
				CLAM_ASSERT( res == 0, "pthread_cond_wait failed!" );
			}

		mThreadID = pthread_self();
		mValidID = true;
		mCount = state.count;

		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
	}

	void RecursiveMutex::DoUnlock( ConditionVar& state )
	{
		int res = 0;

		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_lock failed" );

		CLAM_ASSERT( mValidID, "Spurious value change!" );
		mValidID = false;

		res = pthread_cond_signal( &mUnlocked );
		CLAM_ASSERT( res == 0, "pthread_cond_signal failed!" );

		state.pmutex = &mMutex;
		state.count = mCount;
	}

	RecursiveTryMutex::RecursiveTryMutex()
		: mCount(0), mValidID( false )
	{
		pthread_mutexattr_t attr;

		int res = 0;

		res = pthread_mutexattr_init( &attr );
		CLAM_ASSERT( res == 0, "pthread_mutexattr_init failed" );

		res = pthread_mutex_init( &mMutex, &attr );
		CLAM_ASSERT( res == 0, "pthread_mutex_init failed");

		res = pthread_cond_init( &mUnlocked, 0 );

		if ( res!= 0 )
			{
				pthread_mutex_destroy( &mMutex );
				throw ThreadResourceError("pthread_cond_init failed");
			}	
	}

	RecursiveTryMutex::~RecursiveTryMutex()
	{
		int res = 0;
		res = pthread_mutex_destroy( &mMutex );

		CLAM_ASSERT( res == 0, "pthread_mutex_destroy failed" );

		res = pthread_cond_destroy( &mUnlocked );

		CLAM_ASSERT( res == 0, "pthread_cond_destroy failed" );
	}


	void RecursiveTryMutex::DoLock()
	{
		int res = 0;

		res = pthread_mutex_lock( &mMutex );

		pthread_t tid = pthread_self();

		if ( mValidID && pthread_equal( mThreadID, tid ) )
			++ mCount;
		else
			{
				while ( mValidID )
					{
						res = pthread_cond_wait( &mUnlocked, &mMutex );
						CLAM_ASSERT( res == 0, "pthread_cond_wait failed" );
					}

				mThreadID = tid;
				mValidID = true;
				mCount = 1;
			}
		
		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
	}

	bool RecursiveTryMutex::DoTryLock()
	{
		int res = 0;
		
		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0, " pthread_mutex_lock failed " );

		bool ret = false;

		pthread_t tid = pthread_self();
		
		if (mValidID && pthread_equal(mThreadID, tid ) )
			{
				++mCount;
				ret = true;
			}
		else if ( !mValidID )
			{
				mThreadID = tid;
				mValidID = true;
				mCount = 1;
				ret = true;
			}

		res = pthread_mutex_unlock(&mMutex );
		CLAM_ASSERT( res==0, "pthread_mutex_unlock failed" );
		
		return ret;
	}

	void RecursiveTryMutex::DoUnlock( )
	{
		int res = 0;
		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_lock failed" );

		pthread_t tid = pthread_self();

		if ( mValidID && !pthread_equal( mThreadID, tid ) )
			{
				res = pthread_mutex_unlock( &mMutex );
				CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
			}
		
		if ( --mCount == 0 )
			{
				CLAM_ASSERT( mValidID, "Spurious value change!" );
				mValidID = false;
				
				res = pthread_cond_signal( &mUnlocked );
				CLAM_ASSERT( res==0, "pthread_cond_signal failed" );
			}

		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
	}

	void RecursiveTryMutex::DoLock( ConditionVar& state )
	{
		int res = 0;
		
		while ( mValidID )
			{
				res = pthread_cond_wait( &mUnlocked, &mMutex );
				CLAM_ASSERT( res == 0, "pthread_cond_wait failed!" );
			}

		mThreadID = pthread_self();
		mValidID = true;
		mCount = state.count;

		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
	}

	void RecursiveTryMutex::DoUnlock( ConditionVar& state )
	{
		int res = 0;

		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_lock failed" );

		CLAM_ASSERT( mValidID, "Spurious value change!" );
		mValidID = false;

		res = pthread_cond_signal( &mUnlocked );
		CLAM_ASSERT( res == 0, "pthread_cond_signal failed!" );

		state.pmutex = &mMutex;
		state.count = mCount;
	}


	RecursiveTimedMutex::RecursiveTimedMutex()
		: mCount(0), mValidID( false )
	{
		pthread_mutexattr_t attr;

		int res = 0;

		res = pthread_mutexattr_init( &attr );
		CLAM_ASSERT( res == 0, "pthread_mutexattr_init failed" );


		res = pthread_mutex_init( &mMutex, &attr );
		CLAM_ASSERT( res == 0, "pthread_mutex_init failed");

		res = pthread_cond_init( &mUnlocked, 0 );

		if ( res!= 0 )
			{
				pthread_mutex_destroy( &mMutex );
				throw ThreadResourceError("pthread_cond_init failed");
			}	
	}

	RecursiveTimedMutex::~RecursiveTimedMutex()
	{
		int res = 0;
		res = pthread_mutex_destroy( &mMutex );

		CLAM_ASSERT( res == 0, "pthread_mutex_destroy failed" );

		res = pthread_cond_destroy( &mUnlocked );

		CLAM_ASSERT( res == 0, "pthread_cond_destroy failed" );
	}


	void RecursiveTimedMutex::DoLock()
	{
		int res = 0;

		res = pthread_mutex_lock( &mMutex );

		pthread_t tid = pthread_self();

		if ( mValidID && pthread_equal( mThreadID, tid ) )
			++ mCount;
		else
			{
				while ( mValidID )
					{
						res = pthread_cond_wait( &mUnlocked, &mMutex );
						CLAM_ASSERT( res == 0, "pthread_cond_wait failed" );
					}

				mThreadID = tid;
				mValidID = true;
				mCount = 1;
			}
		
		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
	}

	bool RecursiveTimedMutex::DoTryLock()
	{
		int res = 0;
		
		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0, " pthread_mutex_lock failed " );

		bool ret = false;

		pthread_t tid = pthread_self();
		
		if (mValidID && pthread_equal(mThreadID, tid ) )
			{
				++mCount;
				ret = true;
			}
		else if ( !mValidID )
			{
				mThreadID = tid;
				mValidID = true;
				mCount = 1;
				ret = true;
			}

		res = pthread_mutex_unlock(&mMutex );
		CLAM_ASSERT( res==0, "pthread_mutex_unlock failed" );
		
		return ret;
	}

	bool RecursiveTimedMutex::DoTimedLock( const xtime& xt )
	{
		int res = 0;

		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0 , "pthread_mutex_lock failed" );

		bool ret = false;

		pthread_t tid = pthread_self();

		if ( mValidID && pthread_equal( mThreadID, tid ) )
			{
				++mCount;
				ret = true;
			}
		else
			{
				timespec ts;
				
				to_timespec( xt, ts );

				while( mValidID )
					{
						res = pthread_cond_timedwait( &mUnlocked, &mMutex, &ts );
						if ( res == ETIMEDOUT )
							break;
						CLAM_ASSERT( res==0, "pthread_cond_timedwait failed" );
					}

				if ( !mValidID )
					{
						mThreadID = tid;
						mValidID = true;
						mCount = 1;
						ret = true;
					}
			}

		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res==0, "pthread_mutex_unlock failed" );

		return ret;
	}

	void RecursiveTimedMutex::DoUnlock( )
	{
		int res = 0;
		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_lock failed" );

		pthread_t tid = pthread_self();

		if ( mValidID && !pthread_equal( mThreadID, tid ) )
			{
				res = pthread_mutex_unlock( &mMutex );
				CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
			}
		
		if ( --mCount == 0 )
			{
				CLAM_ASSERT( mValidID, "Spurious value change!" );
				mValidID = false;
				
				res = pthread_cond_signal( &mUnlocked );
				CLAM_ASSERT( res == 0, "pthread_cond_signal failed" );
			}

		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
	}

	void RecursiveTimedMutex::DoLock( ConditionVar& state )
	{
		int res = 0;
		
		while ( mValidID )
			{
				res = pthread_cond_wait( &mUnlocked, &mMutex );
				CLAM_ASSERT( res == 0, "pthread_cond_wait failed!" );
			}

		mThreadID = pthread_self();
		mValidID = true;
		mCount = state.count;

		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_unlock failed" );
	}

	void RecursiveTimedMutex::DoUnlock( ConditionVar& state )
	{
		int res = 0;

		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0, "pthread_mutex_lock failed" );

		CLAM_ASSERT( mValidID, "Spurious value change!" );
		mValidID = false;

		res = pthread_cond_signal( &mUnlocked );
		CLAM_ASSERT( res == 0, "pthread_cond_signal failed!" );

		state.pmutex = &mMutex;
		state.count = mCount;
	}


} // end of namespace CLAM

