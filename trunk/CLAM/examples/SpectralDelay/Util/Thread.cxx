#include "Thread.hxx"
#include "Assert.hxx"
#ifndef WIN32
#include <cstdio>
#endif
#ifdef WIN32
#include <windows.h>
#undef GetClassName
#endif

typedef void *(*pthread_start_pfunc) (void *);

typedef void (*pthread_clean_pfunc) (void *);

using namespace CLAM;

Thread::Thread()
	: mHasCode( false ), mHasCleanup( false ), mIsCancelled(false)
{
}

Thread::~Thread()
{
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
		struct sched_param sched_param;
		int policy;

		if (pthread_getschedparam(pthread_self(), &policy, &sched_param) < 0) {
			printf("Scheduler getparam failed...\n");
		}
		sched_param.sched_priority = sched_get_priority_max(SCHED_RR)-1;
		if (!pthread_setschedparam(pthread_self(), SCHED_RR, &sched_param)) {
			printf("Scheduler set to Round Robin with priority %i...\n", sched_param.sched_priority);
		}
	#endif  
}

void Thread::Start()
{
	CLAM_ASSERT( mHasCode, "The thread has no code to execute!" );
	CLAM_ASSERT( mHasCleanup, "The thread has no cleanup routine!");

	pthread_create(&mThreadID, NULL, (pthread_start_pfunc)LaunchThread, this );
}

void Thread::Stop()
{
	pthread_cleanup_push((pthread_clean_pfunc)LaunchThreadCleanup, this);
	
	pthread_cancel(mThreadID);
	mIsCancelled = true;
	pthread_join(mThreadID,NULL);
	pthread_cleanup_pop(1);
	mIsCancelled = false;
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

	pSelf->SetupPriorityPolicy();
	pSelf->mThreadCode();

	return NULL;
}

void Thread::LaunchThreadCleanup( void* pvoid )
{
	Thread* pSelf = (Thread*)pvoid;

	pSelf->mCleanUpCode;
}
