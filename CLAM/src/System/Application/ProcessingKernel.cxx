#include "ProcessingKernel.hxx"
#include "CBL.hxx"
#include "Thread.hxx"
#include <pthread.h>

using namespace CLAM;

ProcessingKernel::ProcessingKernel()
	: mShouldRun( true )
{
}

bool ProcessingKernel::UserCondition()
{
	return true;
}

void ProcessingKernel::Cancel()
{
	mShouldRun = false;
}

bool ProcessingKernel::LoopCondition()
{
	return mShouldRun&&UserCondition();
}

void ProcessingKernel::AttachToThread( Thread& att_thread )
{
	att_thread.SetThreadCode( makeMemberFunctor0( *this, ProcessingKernel, ProcessingLoop ) );
	att_thread.SetCleanupCode( makeMemberFunctor0( *this, ProcessingKernel, Cancel ) );
}


