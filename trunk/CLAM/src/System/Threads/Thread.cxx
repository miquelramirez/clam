#include "Thread.hxx"
#include "Mutex.hxx"
#include "xtime.hxx"
#include "Assert.hxx"
#ifndef WIN32
#include <cstdio>
#endif
#ifdef WIN32
#include "CLAM_windows.h"
#undef GetClassName
#endif

typedef void *(*pthread_start_pfunc) (void *);

typedef void (*pthread_clean_pfunc) (void *);

namespace CLAM
{

Thread::Thread(bool realtime):
	mHasCode( false ), 
  mHasCleanup( false ),
	mIsCancelled(false),
	mRunning(false),
	mRealtime(realtime)
{
}

Thread::~Thread()
{
	if ( mRunning )
		Stop();
}

void Thread::SetupPriorityPolicy()
{
	#ifdef WIN32
		BOOL res;
		DWORD err;
		
		res = SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS );
		err = GetLastError();
		res = SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_NORMAL );
		err = GetLastError();
	#else
	if (mRealtime)
	{
		struct sched_param sched_param;
		int policy;

		if (pthread_getschedparam(pthread_self(), &policy, &sched_param) < 0) {
			printf("Scheduler getparam failed...\n");
		}
		sched_param.sched_priority = sched_get_priority_max(SCHED_RR)-1;
		if (!pthread_setschedparam(pthread_self(), SCHED_RR, &sched_param)) {
			printf("Scheduler set to Round Robin with priority %i...\n", sched_param.sched_priority);
		}
	}else{
		struct sched_param sched_param;
		int policy;

		if (pthread_getschedparam(pthread_self(), &policy, &sched_param) < 0) {
			printf("Scheduler getparam failed...\n");
		}
		sched_param.sched_priority = sched_get_priority_max(policy)/2;
		pthread_setschedparam(pthread_self(), policy, &sched_param);
	}
	#endif  
}

void Thread::Start()
{
	CLAM_ASSERT( mHasCode, "The thread has no code to execute!" );

	mRunning = true;
	pthread_create(&mThreadID, NULL, (pthread_start_pfunc)LaunchThread, this );
}

void Thread::Stop()
{
	LaunchThreadCleanup(this);
	mIsCancelled = true;
	mIsCancelled = false;
	mRunning = false;
}


void Thread::SetThreadCode( const CBL::Functor0& thread_code )
{
	mThreadCode = thread_code;
	mHasCode = true;
}

void Thread::SetCleanupCode( const CBL::Functor0& cleanup_code )
{
	mCleanUpCode = cleanup_code;
	mHasCleanup = true;
}

void* Thread::LaunchThread( void* pvoid )
{
	Thread* pSelf = (Thread*)pvoid;

	pSelf->mRunning=true;
	pSelf->SetupPriorityPolicy();
	pSelf->mThreadCode();
	pSelf->mRunning = false;
	pthread_exit(NULL);

	return NULL;
}

void Thread::LaunchThreadCleanup( void* pvoid )
{
	Thread* pSelf = (Thread*)pvoid;
	if ( pSelf->mHasCleanup )
		pSelf->mCleanUpCode();

	pthread_join( pSelf->mThreadID, NULL );
}

void Thread::Sleep( unsigned int milliseconds )
{
	SleepDetail( milliseconds );
}

void Thread::Sleep()
{
	SleepDetail( 31536000 ); // 1 year
}

void Thread::SleepDetail( unsigned int milliseconds)
{

	xtime xt;

	to_time( milliseconds, xt );

	Mutex aMutex;

	Mutex::ScopedLock lock( aMutex );

	mSleepCondition.TimedWait( lock, xt );

}

void Thread::WakeUp()
{
	Mutex localMutex;

	Mutex::ScopedLock lock( localMutex );

	mSleepCondition.NotifyOne();
}

}
