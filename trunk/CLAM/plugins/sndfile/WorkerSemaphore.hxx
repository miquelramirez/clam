/*
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
#ifndef WorkerSemaphore_hxx
#define WorkerSemaphore_hxx

#include <CLAM/Processing.hxx>
#include <CLAM/AudioOutPort.hxx>
#include <CLAM/InControl.hxx>
#include <CLAM/OutControl.hxx>
#include <CLAM/ProcessingConfig.hxx>
#include <CLAM/AudioInFilename.hxx> 
#include <CLAM/Audio.hxx>
#include <sndfile.hh>
#include <pthread.h>
#undef GetClassName // pthreads in windows includes windows.h with GetClassName
#include "clam_ringbuffer.h"

namespace CLAM
{
	class WorkerSemaphore
	{
		pthread_mutex_t _diskThreadLock;
		pthread_cond_t  _dataReadyCondition;
	public:
		WorkerSemaphore()
		{
			static pthread_cond_t sPthreadCondInitializer = PTHREAD_COND_INITIALIZER;
			_dataReadyCondition = sPthreadCondInitializer;

			static pthread_mutex_t sPthreadMutexInitializer = PTHREAD_MUTEX_INITIALIZER;
			_diskThreadLock = sPthreadMutexInitializer;
		}
		/**
		Called by the blockable worker thread whenever the work is
		exhausted. The worker thread will sleep until the real-time
		thread provides more work and calls signalWorkToDo.
		*/
		void waitMoreWork()
		{
			pthread_cond_wait(&_dataReadyCondition, &_diskThreadLock);
		}
		/**
		Called by the real-time thread when it adds more things to do
		for the blockable worker thread.
		If the worker is already working, this has no effect.
		If the worker was sleeping, it will wake up.
		*/
		void signalWorkToDo()
		{
			if (pthread_mutex_trylock(&_diskThreadLock) == 0) 
			{
				pthread_cond_signal (&_dataReadyCondition);
				pthread_mutex_unlock (&_diskThreadLock);
			}
		}
		/**
		If instanciated in the main scope of the worker thread,
		it will avoid the realtime thread to nag it with signals.
		*/
		class ScopedLock
		{
			pthread_mutex_t & _diskThreadLock;
		public:
			ScopedLock(WorkerSemaphore & semaphore)
				: _diskThreadLock(semaphore._diskThreadLock)
			{
				pthread_mutex_lock(&_diskThreadLock);
			}
			~ScopedLock()
			{
				pthread_mutex_unlock(&_diskThreadLock);
			}
		};

	};


}// namespace CLAM
#endif // WorkerSemaphore_hxx

