#ifndef __MUTEX__
#define __MUTEX__

#include <pthread.h>
#include "Lock.hxx"
#include <errno.h>

//! http://www.boost.org/libs/thread/doc/mutex_concept.html

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

	friend class Detail::LockOps<Mutex>;
	friend class Detail::ScopedLock<Mutex>;

	typedef Detail::ScopedLock<Mutex> ScopedLock;

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
	friend class Detail::LockOps<TryMutex>;

	typedef Detail::ScopedLock<TryMutex> ScopedLock;
	typedef Detail::ScopedTryLock<TryMutex> ScopedTryLock;

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
	friend class Detail::LockOps<TimedMutex>;

	typedef Detail::ScopedLock<TimedMutex> ScopedLock;
	typedef Detail::ScopedTryLock<TimedMutex> ScopedTryLock;
	typedef Detail::ScopedTimedLock<TimedMutex> ScopedTimedLock;

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
