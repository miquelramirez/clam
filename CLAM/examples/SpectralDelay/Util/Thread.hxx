#ifndef __THREAD__
#define __THREAD__

#include <pthread.h>
#include "CBL.hxx"

namespace CLAM
{

class Thread
{
public:

	Thread();

	virtual ~Thread();

	void Start();

	void Stop();

	virtual void SetupPriorityPolicy();

	void SetThreadCode( const CBL::Functor0& thread_code );

	void SetCleanupCode( const CBL::Functor0& cleanup_code );

	inline bool IsCancelled() const
	{
		return mIsCancelled;
	}

private:
	bool               mHasCode;
	bool               mHasCleanup;
	pthread_t          mThreadID;
	bool               mIsCancelled;
	CBL::Functor0      mThreadCode;
	CBL::Functor0      mCleanUpCode;

private:

	// Pthreads needed static functions for launching and cleaning

	static void* LaunchThread( void* pSelf ); 

	static void  LaunchThreadCleanup( void* pSelf );
};

}

#endif // Thread.hxx
