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

#ifndef __MUTEX__
#define __MUTEX__

#if USE_PTHREADS != 1
#error USE_PTHREADS was not set to 1 in your settings.cfg file, but you are including files that require this. Please fix your settings.cfg
#endif

#include <pthread.h>
#include "Lock.hxx"
#include <errno.h>

// http://www.boost.org/libs/thread/doc/mutex_concept.html

namespace CLAM
{

struct xtime;
/** From the www.boost.org docs:
 * A mutex (short for mutual-exclusion) object is used to serializes access to a resource 
 * shared between multiple threads. The Mutex concept, with TryMutex and TimedMutex  refinements, 
 * formalize the requirements. A model that implements Mutex and its refinements has two states: 
 * locked and unlocked. Before using a shared resource, a thread locks a Boost.Threads mutex object 
 * (an object whose type is a model of Mutex or one of it's refinements), insuring thread-safe  
 * access to the shared resource. When use of the shared resource is complete, the thread unlocks 
 * the mutex object, allowing another thread to acquire the lock and use the shared resource.
 */
class Mutex
{
public:

	friend class Hidden::LockOps<Mutex>;
	friend class Hidden::ScopedLock<Mutex>;

	typedef Hidden::ScopedLock<Mutex> ScopedLock;

	Mutex();

	~Mutex();

private:
	
	struct ConditionVar
	{
		pthread_mutex_t* pmutex;
	};

	void DoLock();
	
	void DoUnlock();
	
	void DoLock( ConditionVar& state );

	void DoUnlock( ConditionVar& state );

	pthread_mutex_t mMutex;

};

class TryMutex
{
public:
	friend class Hidden::LockOps<TryMutex>;

	typedef Hidden::ScopedLock<TryMutex> ScopedLock;
	typedef Hidden::ScopedTryLock<TryMutex> ScopedTryLock;

	TryMutex();
	~TryMutex();

private:	
	struct ConditionVar
	{
		pthread_mutex_t* pmutex;
	};

	void DoLock();

	bool DoTryLock();

	void DoUnlock();
	
	void DoLock( ConditionVar& state );

	void DoUnlock( ConditionVar& state );

	pthread_mutex_t mMutex;

};

class TimedMutex
{
public:
	friend class Hidden::LockOps<TimedMutex>;

	typedef Hidden::ScopedLock<TimedMutex> ScopedLock;
	typedef Hidden::ScopedTryLock<TimedMutex> ScopedTryLock;
	typedef Hidden::ScopedTimedLock<TimedMutex> ScopedTimedLock;

	TimedMutex();
	~TimedMutex();

private:
	struct ConditionVar
	{
		pthread_mutex_t* pmutex;
	};

	void DoLock();

	bool DoTryLock();

	bool DoTimedLock( const xtime& xt );

	void DoUnlock();
	
	void DoLock( ConditionVar& state );

	void DoUnlock( ConditionVar& state );

	pthread_mutex_t mMutex;
	
	pthread_cond_t  mCondition;

	bool mLocked;

};

} // end of namespace CLAM

#endif // Mutex.hxx

