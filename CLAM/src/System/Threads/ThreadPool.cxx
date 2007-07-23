#include "ThreadPool.hxx"
#include "Thread.hxx"
#include "PooledThread.hxx"
#include <algorithm>

namespace CLAM
{

ThreadPool::ThreadPool(int argInitialNumberOfThreads, bool argIsRealtime)
	: mIsRealtime(argIsRealtime)
{
	for (int counter = 0; counter < argInitialNumberOfThreads; counter++)
	{
		idleThreads.push_back( new CLAM::PooledThread(this, mIsRealtime) );
	}
}
ThreadPool::~ThreadPool()
{
	EmptyPool();
}

void ThreadPool::EmptyPool()
{
	Mutex::ScopedLock lock( dequeMutex );

	// First we stop and delete all the busy threads
	for (int counter = 0; counter < busyThreads.size(); counter++)
	{
		PooledThread* threadPtr = busyThreads.at( counter );
		if (threadPtr != NULL)
		{
			if (threadPtr->IsRunning())
				threadPtr->Stop();
			delete threadPtr;
		}
	}
	busyThreads.clear();

	// Next we delete all the idle threads
	for (int counter = 0; counter < idleThreads.size(); counter++)
	{

		PooledThread* threadPtr = idleThreads.at( counter );
		if (threadPtr != NULL)
		{
			delete threadPtr;
		}
	}
	idleThreads.clear();
}

PooledThread* ThreadPool::GetThreadFromPool()
{
	Mutex::ScopedLock lock( dequeMutex );

	// are there any more idle threads?
	if (idleThreads.size() == 0)
	{
		// check to see if any of the threads in the busy queue are actually idle
		bool foundIdleThread = false;
		std::deque<PooledThread*>::iterator iter;
		for (iter = busyThreads.begin(); iter < busyThreads.end(); iter++)
		{
			PooledThread* threadPtr = *iter;
			if ( !threadPtr->IsRunning() )
			{
				busyThreads.erase(iter);
				idleThreads.push_back(threadPtr);
				foundIdleThread = true;
			}
		}

		if ( !foundIdleThread )
		{
			idleThreads.push_back( new CLAM::PooledThread(this, mIsRealtime) );
		}
	}

	PooledThread* threadPtr = idleThreads.at(0);
	idleThreads.pop_front();
	busyThreads.push_back(threadPtr);

	return threadPtr;
}

void ThreadPool::ReturnThreadToPool(PooledThread* argThreadPtr)
{
	Mutex::ScopedLock lock( dequeMutex );

	std::deque<PooledThread*>::iterator iter = find ( busyThreads.begin(), busyThreads.end(), argThreadPtr );
	busyThreads.erase( iter);

	idleThreads.push_back( argThreadPtr );
}

} // end namespace CLAM
