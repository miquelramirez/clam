#include "Mutex.hxx"
#include "RecursiveMutex.hxx"
#include "Condition.hxx"
#include "Thread.hxx"
#include "xtime.hxx"
#include <iostream>
using std::cout;
using std::endl;
#include <list>
#include "Assert.hxx"
#include "Err.hxx"

template < typename M >
void test_lock( M* overload_provider = 0 )
{
	typedef M MutexType;
	typedef typename M::ScopedLock LockType;

	MutexType mutex;
	CLAM::Condition condition;

	// Test lock constructors
	{
		LockType lock( mutex, false );
		CLAM_ASSERT( !lock, "ScopedLock constructor failed" );
	}
	
	LockType lock( mutex );
	CLAM_ASSERT( lock, "ScopedLock constructor failed" );

	// Time out test. Since nobody notifies the condition variable
	// it should time out

	CLAM::xtime xt;
	CLAM_ASSERT( CLAM::xtime_get(&xt, CLAM::TIME_UTC) == CLAM::TIME_UTC, " Something went wrong with time initialization" );
	xt.nsec += 100000000; // one tenth of a sec

	CLAM_ASSERT( condition.TimedWait( lock, xt ) == false, "Condition didn't timeout" );
	CLAM_ASSERT( lock, "Lock was messed up" );
	
	// Test the lock and unlock

	lock.Unlock();
	CLAM_ASSERT( !lock, "Lock not unlocked!" );
	lock.Lock();
	CLAM_ASSERT( lock, "Unlocked lock not locked" );
}

template <typename M>
void test_trylock( M* overload_provider = 0 )
{
	typedef M MutexType;
	typedef typename M::ScopedTryLock TryLockType;

	MutexType mutex;
	CLAM::Condition condition;

	// Lock constructors test
	{
		TryLockType lock( mutex );
		CLAM_ASSERT( lock, "Constructor failed" );
	}
	{
		TryLockType lock( mutex, false);
		CLAM_ASSERT( !lock, "Constructor failed" );
	}
	TryLockType lock( mutex, true );
	CLAM_ASSERT( lock, "Constructor failed" );

	// Fast timeout
	CLAM::xtime xt;
	CLAM_ASSERT( CLAM::xtime_get( &xt, CLAM::TIME_UTC ) == CLAM::TIME_UTC, "Clock test failed" );
	xt.nsec += 100000000; // one tenth of a second

	// This should timeout

	CLAM_ASSERT( condition.TimedWait( lock ,xt ) == false, "Condition didn't time out" );
	CLAM_ASSERT( lock, "Lock was messed" );

	// now we try the lock, unlock and trylock
	lock.Unlock();
	CLAM_ASSERT( !lock, "Unlock() did not work" );
	lock.Lock(); 
	CLAM_ASSERT( lock, "Lock() did not work" );
	lock.Unlock();
	CLAM_ASSERT( !lock, "Unlock() did not work" );
	CLAM_ASSERT( lock.TryLock(), "Unable to acquire a lock onto a unlocked mutex"  );
	CLAM_ASSERT( lock, "mutex not locked" );
}

template <typename M>
void test_timedlock( M* overload_provider = 0)
{
    typedef M MutexType;
    typedef typename M::ScopedTimedLock TimedLockType;

    MutexType mutex;
    CLAM::Condition condition;

    // Test the lock's constructors.
    {
        // Construct and initialize an xtime for a fast time out.
        CLAM::xtime xt;
        CLAM_ASSERT(CLAM::xtime_get(&xt, CLAM::TIME_UTC) == CLAM::TIME_UTC, "Clock Test failed");
        xt.nsec += 100000000; // one tenth of a second

        TimedLockType lock(mutex, xt);
        CLAM_ASSERT(lock, "Constructor failed");
    }
    {
        TimedLockType lock(mutex, false);
        CLAM_ASSERT(!lock, "Constructor failed");
    }
    TimedLockType lock(mutex, true);
    CLAM_ASSERT(lock, "Constructor failed");

    // Construct and initialize an xtime for a fast time out.
    CLAM::xtime xt;
    CLAM_ASSERT(CLAM::xtime_get(&xt, CLAM::TIME_UTC) == CLAM::TIME_UTC, "Clock test failed");
    xt.nsec += 100000000;

    // Test the lock and the mutex with condition variables.
    // No one is going to notify this condition variable.  We expect to
    // time out.
    CLAM_ASSERT(condition.TimedWait(lock, xt) == false, "Condition did not timeout");
    CLAM_ASSERT(lock,"Test failed");

    // Test the lock, unlock and timedlock methods.
    lock.Unlock();
    CLAM_ASSERT(!lock, "Unlock() failed");
    lock.Lock();
    CLAM_ASSERT(lock, "Lock() failed");
    lock.Unlock();
    CLAM_ASSERT(!lock, "Unlock() failed");
    CLAM_ASSERT(CLAM::xtime_get(&xt, CLAM::TIME_UTC) == CLAM::TIME_UTC, "Test failed");
    xt.nsec += 100000000;
    CLAM_ASSERT(lock.TimedLock(xt), "TimedLock() did not work");
}

void test_mutex()
{
	typedef CLAM::Mutex mutex;
	test_lock<mutex>();

	cout << "Mutex test passed...." << endl;
}

void test_try_mutex()
{
	typedef CLAM::TryMutex mutex;
	test_lock<mutex>();
	test_trylock<mutex>();
	
	cout << "TryMutex test passed..." << endl;
}

void test_timed_mutex()
{
	typedef CLAM::TimedMutex mutex;
	test_lock<mutex>();
	test_trylock<mutex>();
	test_timedlock<mutex>();

	cout << "TimedMutex test passed..." << endl;
}

void test_recursive_mutex()
{
	typedef CLAM::RecursiveMutex mutex;

	test_lock<mutex>();
	mutex m;
	{
		mutex::ScopedLock lock1( m );
		mutex::ScopedLock lock2( m );
	}
	cout << "RecursiveMutex test passed..." << endl;
}

void test_recursive_try_mutex()
{
	typedef CLAM::RecursiveTryMutex mutex;
	
	test_lock<mutex>();
	test_trylock<mutex>();
	mutex m;
	{
		mutex::ScopedLock lock1(m);
		mutex::ScopedLock lock2(m);
	}
	cout << "RecursiveTryMutex test passed..." << endl;
}

void test_recursive_timed_mutex()
{
	typedef CLAM::RecursiveTimedMutex mutex;
	
	test_lock<mutex>();
	test_trylock<mutex>();
	test_timedlock<mutex>();
	mutex m;
	{
		mutex::ScopedLock lock1(m);
		mutex::ScopedLock lock2(m);
	}
	
	cout << "RecursiveTimedMutex test passed..." << endl;
}

class TestCondition
{
public:

	TestCondition()
		: notified(0), awoken(0)
	{
	}

	virtual void ThreadCode()
	{
		CLAM::Mutex::ScopedLock lock( mutex );
		CLAM_ASSERT( lock, "Lock did not worked" );
		
		while ( !(notified) )
			condition.Wait(lock);

		CLAM_ASSERT( lock, "Lock was messed" );
		awoken++;
	}

public:

	CLAM::Mutex     mutex;
	CLAM::Condition condition;
	int notified;
	int awoken;
};

void test_condition_notify_one()
{
	TestCondition test_cond;

	CLAM::Thread t;

	t.SetThreadCode( makeMemberFunctor0( test_cond, TestCondition, ThreadCode ) );
	
	t.Start();
	{
		CLAM::Mutex::ScopedLock lock( test_cond.mutex );
		CLAM_ASSERT( lock, "Unable to lock the mutex" );
		test_cond.notified++;
		test_cond.condition.NotifyOne();
	}

	t.Stop();
	CLAM_ASSERT(test_cond.awoken==1,"Test Condition::NotifyOne failed!" );

	cout << "Test Condition::NotifyOne passed!" << endl;
}

void test_condition_notify_all()
{
	const int nthreads = 5;
	
	typedef std::list<CLAM::Thread*> ThreadGroup;
	typedef std::list<CLAM::Thread*>::iterator TGIterator;
	
	ThreadGroup threads;
	TestCondition test_cond;

	for ( int i = 0; i < nthreads; i++ )
		threads.push_back( new CLAM::Thread() );

	TGIterator it;	

	for ( it = threads.begin(); it != threads.end(); it++)
		(*it)->SetThreadCode( makeMemberFunctor0( test_cond, TestCondition, ThreadCode ) );
	
	for ( it = threads.begin(); it != threads.end(); it++ )
		(*it)->Start();

	{
		CLAM::Mutex::ScopedLock lock( test_cond.mutex );
		CLAM_ASSERT( lock, "Not able to lock: Test Condition::NotifyAll failed" );
		test_cond.notified++;
		test_cond.condition.NotifyAll();
	}

	for ( it = threads.begin(); it != threads.end(); it++ )
		(*it)->Stop();
	
	for ( it = threads.begin(); it != threads.end(); it++ )
		delete (*it);



	CLAM_ASSERT( test_cond.awoken == nthreads, "Test Condition::NotifyAll failed" );


	cout << "Test Condition::NotifyAll passed" << endl;
}

struct ConditionPredicate
{
	ConditionPredicate( int& var, int val ) 
		: mVar(var), mVal(val)
	{
	}

	bool operator() ()
	{
		return mVar == mVal;
	}

	int& mVar;
	int  mVal;
};

class TestConditionWait : public TestCondition
{
public:

	virtual void ThreadCode()
	{
		CLAM::Mutex::ScopedLock lock( mutex );
		CLAM_ASSERT( lock, "Test Condition Wait failed: not able to lock the mutex"  );
		
		// Test wait
		while ( !notified )
			condition.Wait( lock );
		
		CLAM_ASSERT( lock, "Test Condition Wait failed: lock messed up during wait" );
		CLAM_ASSERT( notified == 1, "Test Condition Wait failed: guard var messed" );

		awoken++;
		condition.NotifyOne();

		// Test predicate wait
		condition.Wait( lock, ConditionPredicate( notified, 2 ) );
		CLAM_ASSERT( lock, "Test Condition Wait failed: lock messed during wait" );
		CLAM_ASSERT( notified == 2, "Test Condition Wait failed: Guard variable failed" );
		awoken++;
		condition.NotifyOne();

		// Test timed_wait.
		CLAM::xtime xt;
		CLAM_ASSERT( CLAM::xtime_get(&xt, CLAM::TIME_UTC) == CLAM::TIME_UTC, "Clock mismatch!" );
		xt.nsec += 100000000; // one tenth of a second
		while (notified != 3)
			condition.TimedWait(lock, xt);
		CLAM_ASSERT(lock, "Test Condition Wait failed: lock messed during wait" );
		CLAM_ASSERT(notified == 3, "Test Condition Wait failed: Guard variable messed");
		awoken++;
		condition.NotifyOne();

	   // Test predicate timed_wait.
		CLAM_ASSERT(CLAM::xtime_get(&xt, CLAM::TIME_UTC) == CLAM::TIME_UTC, "Clock mismatch!" );
		xt.sec += 2;
		CLAM_ASSERT( condition.TimedWait(lock, xt, ConditionPredicate(notified, 4)), "Test Condition Wait failed: TimedOut!");
		CLAM_ASSERT(lock, "Lock messed during wait");
		CLAM_ASSERT(notified == 4, "Test Condition Wait failed: Guard variable messed");
		awoken++;
	}
};

void test_condition_waits()
{
	TestConditionWait test_cond;
	TestCondition* pBase;
	CLAM::Thread t;
	
	pBase = static_cast<TestCondition*>( &test_cond ); // upcast
	t.SetThreadCode( makeMemberFunctor0( *pBase, TestCondition, ThreadCode  ) );
	t.Start();
	CLAM::xtime xt;

	{
		CLAM::Mutex::ScopedLock lock( test_cond.mutex );
		CLAM_ASSERT( lock, "Test condition wait failed: mutex not locked" );
		
		t.Sleep( 1000 );
        test_cond.notified++;
        test_cond.condition.NotifyOne();
        while (test_cond.awoken != 1)
            test_cond.condition.Wait(lock);
        CLAM_ASSERT(test_cond.awoken == 1, "Test failed" );

        CLAM_ASSERT(CLAM::xtime_get(&xt, CLAM::TIME_UTC) == CLAM::TIME_UTC, "test condition waits failed");
		t.Sleep( 1000 );
        test_cond.notified++;
        test_cond.condition.NotifyOne();
        while (test_cond.awoken != 2)
            test_cond.condition.Wait(lock);
        CLAM_ASSERT(test_cond.awoken == 2, "test condition waits failed");

        CLAM_ASSERT(CLAM::xtime_get(&xt, CLAM::TIME_UTC) == CLAM::TIME_UTC, "test condition waits failed");
        t.Sleep( 1000 );
        test_cond.notified++;
        test_cond.condition.NotifyOne();
        while (test_cond.awoken != 3)
            test_cond.condition.Wait(lock);
        CLAM_ASSERT(test_cond.awoken == 3, "test condition waits failed");
	}

	CLAM_ASSERT(CLAM::xtime_get(&xt, CLAM::TIME_UTC) == CLAM::TIME_UTC, "test condition waits failed");
    t.Sleep( 1000 );
    test_cond.notified++;
    test_cond.condition.NotifyOne();
    CLAM_ASSERT(CLAM::xtime_get(&xt, CLAM::TIME_UTC) == CLAM::TIME_UTC, "test condition waits failed");
    t.Sleep(1000);
    t.Stop();
    CLAM_ASSERT(test_cond.awoken == 4, "test condition waits failed");

	cout << "Test condition waits passed" << endl;
}

void test_condition()
{
	cout << "Testing Condition::NotifyOne() " << std::endl;
	test_condition_notify_one();
	cout << "Testing Condition::NotifyAll() " << std::endl;
	test_condition_notify_all();
	cout << "Testing Condition:: waits" << std::endl;
	test_condition_waits();
}


class Sleepy
{
public:

	void ThreadCode()
	{

		double b = 10.5;
		double c = 0.71;

		for ( int i = 0; i < 1000000L; i++)
		{
			double a = b / c;
		}
	}
};

void test_sleep_and_wake()
{
	Sleepy sl;

	CLAM::Thread t;

	t.SetThreadCode( makeMemberFunctor0( sl, Sleepy, ThreadCode ) );
	
	t.Start();

	t.Sleep(1000);
	cout << "*";

	for ( int i = 0; i < 10; i++ )
		cout << "+";

	cout << endl;

	t.WakeUp();

	t.Stop();

	cout << "Test sleep and wake passed!" << endl;
}

int main( int argc, char** argv )
{
	try
		{
			cout << "Testing Mutex class" << endl;
			test_mutex();
			cout << "Testing TryMutex class" << endl;
			test_try_mutex();
			cout << "Testing TimedMutex class" << endl;
			test_timed_mutex();
			cout << "Testing Recursive Mutex class" << endl;
			test_recursive_mutex();
			cout << "Testing Recursive TryMutex class" << endl;
			test_recursive_try_mutex();
			cout << "Testing Recursive TimedMutex class" << endl;
			test_recursive_timed_mutex();
			cout << "Testing Sleep and Wakeup" << endl;
//			test_sleep_and_wake();
			cout << "Testing Condition class" << endl;
			test_condition();

			cout << "All tests passed. Congratulations!" << endl;
		}
	catch( CLAM::Err& e )
		{
			e.Print();
			std::cerr << "Abnormal program termination" << std::endl;
		}

	return 0;
}

