#include "ProcessingKernel.hxx"
#include "CBL.hxx"
#include "Thread.hxx"
#include <pthread.h>

using namespace CLAM;

bool ProcessingKernel::UserCondition()
{
	return true;
}

bool ProcessingKernel::LoopCondition()
{
	pthread_testcancel();

	return UserCondition();
}

void ProcessingKernel::AttachToThread( Thread& att_thread )
{
	att_thread.SetThreadCode( makeMemberFunctor0( *this, ProcessingKernel, ProcessingLoop ) );
	att_thread.SetCleanupCode( makeMemberFunctor0( *this, ProcessingKernel, ProcessingCleanup ) );
}
