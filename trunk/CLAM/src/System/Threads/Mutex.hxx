#ifndef __MUTEX__
#define __MUTEX__

#include <pthread.h>
#include "Lock.hxx"

namespace CLAM
{

struct xtime;

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
