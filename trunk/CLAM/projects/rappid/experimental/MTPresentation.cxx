

#include "MTPresentation.hxx"
#include "Assert.hxx"
#include <errno.h>
#include <cstdlib>

using namespace CLAMGUI;


MTPresentation::MTPresentation(EPolicy policy)
	: mPolicy(policy)
{
	pthread_mutexattr_t attributes;
	pthread_mutexattr_init(&attributes);

#	ifdef __USE_GNU
#		if defined(CLAM_DISABLE_CHECKS)
			pthread_mutexattr_settype(&attributes,PTHREAD_MUTEX_ADAPTIVE_NP);
#		else
			pthread_mutexattr_settype(&attributes,PTHREAD_MUTEX_ERRORCHECK_NP);
#		endif
#	endif

	pthread_mutex_init(&mMutex,    &attributes);
	pthread_mutex_init(&mStepMutex,&attributes);

	pthread_condattr_t cond_attr;
	pthread_condattr_init(&cond_attr);

	pthread_cond_init(&mStepRequest,&cond_attr);
}

MTPresentation::~MTPresentation()
{
	int res = pthread_mutex_destroy(&mMutex);
	if (res == EBUSY) {
		exit(1);
	}
}

void MTPresentation::Step()
{
	int result = pthread_mutex_lock(&mStepMutex);

	CLAM_ASSERT( result != EDEADLK,
				 "MTPresentation::Step():"
				 " Deadlock detected.");

	pthread_cond_signal(&mStepRequest);
	pthread_mutex_unlock(&mStepMutex);
	
}

void MTPresentation::TogleFreeze()
{
	mFrozen = !mFrozen;
}

bool MTPresentation::WaitStepAndTryLock()
{
	int result = 0;
	switch (mPolicy) {
	case eBlocking:
		result = pthread_mutex_lock(&mMutex);
		break;
	case eRealTime:
		result = pthread_mutex_trylock(&mMutex);
		if (result == EBUSY)
			return false;
		break;
	case eStepByStep:
		if (mFirstStep) {
			mFirstStep = false;
			break;
		}
		result = pthread_mutex_lock(&mStepMutex);
		CLAM_ASSERT( result != EDEADLK,
					 "MTPresentation::TryLock():"
					 " Step deadlock detected.");
		pthread_cond_wait(&mStepRequest,&mStepMutex);
		pthread_mutex_unlock(&mStepMutex);

		result = pthread_mutex_lock(&mMutex);
		break;
		
	}
	CLAM_ASSERT( result != EDEADLK,
				 "MTPresentation::TryLock():: Deadlock detected.");
	return true;
}


void MTPresentation::Lock()
{
	int result = pthread_mutex_lock(&mMutex);

	CLAM_ASSERT(result != EDEADLK,
				"MTPresentation::Unock(): "
				"Not locked or locked in different thread.");
}

void MTPresentation::Unlock()
{
	int result = pthread_mutex_unlock(&mMutex);

	CLAM_ASSERT(result != EPERM,
				"MTPresentation::Unock(): "
				"Not locked or locked in different thread.");
}
