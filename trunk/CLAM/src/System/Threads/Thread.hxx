#ifndef __THREAD__
#define __THREAD__

#include <pthread.h>
#include "CBL.hxx"
#include "Condition.hxx"


namespace CLAM
{
	class Condition;

class Thread
{
public:

	Thread();

	virtual ~Thread();

	void Start();

	void Stop();

	void Sleep();

	void Sleep( unsigned int milliseconds );

	void WakeUp();

	virtual void SetupPriorityPolicy();

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

private:
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
