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

#include "Mutex.hxx"
#include "Assert.hxx"
#include "xtime.hxx"
#include "ErrSystem.hxx"

namespace CLAM
{

	Mutex::Mutex()
	{
		int res = 0;

		res = pthread_mutex_init( &mMutex, 0 );

		CLAM_ASSERT( res == 0, "Not able to initialize mutex" );
	}

	Mutex::~Mutex()
	{
		int res = 0;

		res = pthread_mutex_destroy( &mMutex );

		CLAM_ASSERT( res == 0, "Not able to destroy mutex" );
	}

	void Mutex::DoLock()
	{
		int res = 0;
		
		res = pthread_mutex_lock( &mMutex );

		if ( res == EDEADLK )
			throw LockError(); // What is EDEADLK????

		CLAM_ASSERT( res == 0, "Not able to lock the mutex" );
	}
	
	void Mutex::DoUnlock()
	{
		int res = 0;
		
		res = pthread_mutex_unlock( &mMutex );

		if ( res == EPERM )
			throw LockError(); // What is EPERM?????

		CLAM_ASSERT( res == 0, "Not able to Unlock the mutex" );
	}

	void Mutex::DoLock( ConditionVar& state )
	{
		// Does nothing
	}

	void Mutex::DoUnlock( ConditionVar& state )
	{
		state.pmutex = &mMutex;
	}
	
	TryMutex::TryMutex()
	{
		int res = 0;

		res = pthread_mutex_init( &mMutex, 0 );

		CLAM_ASSERT( res == 0, "Not able to initialize mutex" );
		
	}

	TryMutex::~TryMutex()
	{
		int res = 0;

		res = pthread_mutex_destroy( &mMutex );

		CLAM_ASSERT( res == 0, "Not able to destroy mutex" );

	}

	void TryMutex::DoLock()
	{
		int res = 0;
		res = pthread_mutex_lock( &mMutex );
		if ( res == EDEADLK )
			throw LockError();

		CLAM_ASSERT( res == 0, "Not able to lock mutex" );
	}

	bool TryMutex::DoTryLock()
	{
		int res = 0;

		res = pthread_mutex_trylock(&mMutex );

		if ( res == EDEADLK )
			throw LockError();

		CLAM_ASSERT ( (res == 0) || (res == EBUSY), "Not able to try-lock the mutex" );

		return res == 0;
	}

	void TryMutex::DoUnlock()
	{
		int res = 0;
		
		res = pthread_mutex_unlock( &mMutex );

		if ( res == EPERM )
			throw LockError(); // What is EPERM?????

		CLAM_ASSERT( res == 0, "Not able to Unlock the mutex" );
	}

	void TryMutex::DoLock( ConditionVar& state )
	{
		// Does nothing
	}

	void TryMutex::DoUnlock( ConditionVar& state )
	{
		state.pmutex = &mMutex;
	}

	TimedMutex::TimedMutex()
		: mLocked( false )
	{
		int res = 0;

		res = pthread_mutex_init( &mMutex, 0 );

		CLAM_ASSERT( res==0, "Not able to initilize the mutex" );

		res = pthread_cond_init( &mCondition, 0 );

		if ( res!= 0 )
			{
				pthread_mutex_destroy( &mMutex );
				throw ThreadResourceError("Unable to initialize Condition " );
			}
	}

	TimedMutex::~TimedMutex()
	{
		CLAM_ASSERT( !mLocked, "Mutex was locked while attempting to destroy it!" );
		
		int res = 0;

		res = pthread_mutex_destroy(&mMutex );

		CLAM_ASSERT( res == 0, "Unable to destroy the mutex!" );

		res = pthread_cond_destroy( &mCondition );

		CLAM_ASSERT( res == 0, "Unable to destroy the Condition variable" );
	}
	
	void TimedMutex::DoLock( )
	{
		int res = 0;

		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0, "Unable to lock mutex (already locked) " );

		while( mLocked )
			{
				res = pthread_cond_wait( &mCondition, &mMutex );
				CLAM_ASSERT( res == 0, "Wait failed"  );
			}

		CLAM_ASSERT( !mLocked, "Spurious value for loop condition" );
		mLocked = true;

		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res ==0, "Unable to unlock the mutex" );
	} 
	
	bool TimedMutex::DoTryLock()
	{
		int res = 0;

		res = pthread_mutex_lock( &mMutex );

		CLAM_ASSERT( res == 0, "Unable to lock the mutex" );

		bool ret = false;

		if ( !mLocked )
			{
				mLocked = true;
				ret = true;
			}

		res = pthread_mutex_unlock( &mMutex );
		CLAM_ASSERT( res==0, "Unable to unlock the mutex" );

		return ret;
	}

	bool TimedMutex::DoTimedLock( const xtime& xt )
	{
		int res = 0;
		res = pthread_mutex_lock( &mMutex );
		CLAM_ASSERT( res == 0, "Unable to lock the mutex" );

		timespec ts;
		to_timespec(xt, ts);

		while (mLocked)
			{
				res = pthread_cond_timedwait(&mCondition, &mMutex, &ts);
				CLAM_ASSERT(res == 0 || res == ETIMEDOUT, "Low level error");

				if (res == ETIMEDOUT)
					break;
			}

		bool ret = false;
		if (!mLocked)
			{
				mLocked = true;
				ret = true;
			}

		res = pthread_mutex_unlock(&mMutex);
		CLAM_ASSERT(res == 0, "Something low level failed!");
		return ret;
	}

	void TimedMutex::DoUnlock()
	{
		int res = 0;
		res = pthread_mutex_lock(&mMutex);
		CLAM_ASSERT(res == 0, "Unable to lock the mutex");

		CLAM_ASSERT(mLocked, "No condition spurious value change");
		mLocked = false;

		res = pthread_cond_signal(&mCondition);
		CLAM_ASSERT(res == 0, "Not able to change the condition var value");

		res = pthread_mutex_unlock(&mMutex);
		CLAM_ASSERT(res == 0, "Unable to unlock the mutex");
	}

	void TimedMutex::DoLock(ConditionVar& v)
	{
		int res = 0;
		while (mLocked)
		{
			res = pthread_cond_wait(&mCondition, &mMutex);
			CLAM_ASSERT(res == 0, "pthread_cond_wait call failed!");
		}

		CLAM_ASSERT(!mLocked, "Spurious value change");
		mLocked = true;

		res = pthread_mutex_unlock(&mMutex);
		CLAM_ASSERT(res == 0, "pthread_mutex_unlock call failed!");
	}

	void TimedMutex::DoUnlock(ConditionVar& state)
	{
		int res = 0;
		res = pthread_mutex_lock(&mMutex);
		CLAM_ASSERT(res == 0, "pthread_mutex_lock call failed");

		CLAM_ASSERT(mLocked, "Spurious value change!");
		mLocked = false;

		res = pthread_cond_signal(&mCondition);
		CLAM_ASSERT(res == 0, "pthread_cond_signal call failed!");

		state.pmutex = &mMutex;
	}

} // end of namespace CLAM

