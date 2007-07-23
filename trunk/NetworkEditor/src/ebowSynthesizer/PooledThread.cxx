#include "PooledThread.hxx"
#include <pthread.h>

namespace CLAM
{
typedef void *(*pthread_start_pfunc) (void *);

PooledThread::PooledThread(ThreadPool* argThreadPoolPtr, bool argRealtime)
: Thread(argRealtime), mThreadPoolPtr(argThreadPoolPtr)
{
}

PooledThread::~PooledThread()
{
}

void PooledThread::Start()
{
	//CLAM_ASSERT( mHasCode, "The thread has no code to execute!" );

	mRunning = true;
	pthread_create(&mThreadID,
					NULL,
					(pthread_start_pfunc) LaunchPooledThread,
					this );
}


void PooledThread::ReturnToPool()
{
	mThreadPoolPtr->ReturnThreadToPool(this);
}

void* PooledThread::LaunchPooledThread( void* pvoid )
{
	PooledThread* pSelf = (PooledThread*)pvoid;

	pSelf->mRunning=true;
	pSelf->SetupPriorityPolicy();
	pSelf->mThreadCode();
	pSelf->mRunning = false;

	return NULL;
}


} // end namespace CLAM
