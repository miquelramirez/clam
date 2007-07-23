/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __ThreadPool__
#define __ThreadPool__

#include "PooledThread.hxx"
#include "Mutex.hxx"
#include <deque>

namespace CLAM {

class PooledThread;

/**
* This is a simple variety of thread pool. This class maintains a collection
* of threads. When an additional thread is needed and no threads are available
* in its idle pool, it first checks the status of the threads it has already
* given out and returns one of those to the client and the rest to the idle
* pool. If all the threads it has given out are still busy, it creates a
* new thread.
*
* This class should only be used if a relatively small number of threads is
* needed, because as the number of threads grows, the scheduling overhead of
* switching between threads can become a significant drag on your program's
* performance. If a relatively large number of tasks need to be run on threads,
* it would be better to use a work queue which is processed by a fixed number
* of threads.
*
* \author Greg Kellum [gkellum@iua.upf.edu] 7/20/2007
* \since  CLAM v1.1.
*/
class ThreadPool
{
public:
	/**
	* The constructor.
	*
	* \arg initialNumberOfThreads - self-explanatory
	* \arg isRealtime - if the thread is declared as realtime, it will get
	*      the highest level of scheduling priority possible which might
	*      cause threads of this pool to run before any other threads.
	*      if the thread is declared as not realtime, it will get half the
	*      scheduling priority of a realtime thread.
	*/
	ThreadPool(int initialNumberOfThreads = 10, bool isRealtime = false);

	~ThreadPool();

	/**
	* This gets a thread from the pool. When an additional thread is needed
	* and no threads are available in its idle pool, it first checks the status
	* of the threads it has already given out and returns one of those to the
	* client and the rest to the idle pool. If all the threads it has given out
	* are still busy, it creates a new thread.
	*
	*/
	PooledThread* GetThreadFromPool();

	void ReturnThreadToPool(PooledThread* aThread);

	void EmptyPool();

private:
	bool mIsRealtime;
	CLAM::Mutex dequeMutex;
	std::deque<PooledThread*> idleThreads;
	std::deque<PooledThread*> busyThreads;
};

} // end namespace CLAM

#endif
