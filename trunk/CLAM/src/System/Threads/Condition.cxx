#include "Condition.hxx"

#include "Thread.hxx"
#include "ErrSystem.hxx"
#include "Assert.hxx"
#include "xtime.hxx"

namespace CLAM
{

	Condition::Condition()
	{
		int res  = 0;

		res = pthread_cond_init( &mCondition, 0 );
		
		CLAM_ASSERT( res == 0, "pthread_cond_init call failed" );
	}

	Condition::~Condition()
	{
		int res = 0;

		res = pthread_cond_destroy( &mCondition );

		CLAM_ASSERT( res==0, "pthread_cond_destroy call failed" );
	}

	void Condition::NotifyAll()
	{
		int res = 0;
		
		res = pthread_cond_broadcast( &mCondition );

		CLAM_ASSERT( res==0, "pthread_cond_broadcast call failed!" );
	}

	void Condition::NotifyOne()
	{
		int res = 0;

		res = pthread_cond_signal( &mCondition );

		CLAM_ASSERT( res == 0, "pthread_cond_signal call failed" );
	}

	void Condition::DoWait( pthread_mutex_t* pmutex )
	{
		int res = 0;

		res = pthread_cond_wait( &mCondition, pmutex );
		
		CLAM_ASSERT( res == 0, "pthread_cond_wait call failed" );
	}

	bool Condition::DoTimedWait( const xtime& xt, pthread_mutex_t* pmutex )
	{
		timespec ts;

		to_timespec( xt, ts );

		int res = 0;

		res = pthread_cond_timedwait( &mCondition, pmutex, &ts );

		CLAM_ASSERT( res == 0 || res == ETIMEDOUT ,"SOmething strange has happened");

		return res != ETIMEDOUT;
	}

} // end of namespace CLAM
