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

#include "BaseAudioApplication.hxx"
#include "AudioManager.hxx"
#include <pthread.h>
#include <cstdio>
#include <iostream>
#ifdef WIN32
#include "CLAM_windows.h"
#undef GetClassName
#else
#include <unistd.h>
#endif
#include "AudioManager.hxx"

namespace CLAM {

	typedef void *(*startfn) (void *);
	typedef void (*cleanfn) (void *);

	BaseAudioApplication::BaseAudioApplication()
	:Application()
	{
		cancel = false;
	}

	void BaseAudioApplication::Start(void)
	{
		cancel = false;
		pthread_create(&thread,NULL,(startfn)SAudioThread,this);
	}
	
	void BaseAudioApplication::Stop(void)
	{
		pthread_cleanup_push((cleanfn)SAudioThreadCleanup,this);

		cancel = true;
		pthread_join(thread,NULL);
				
		pthread_cleanup_pop(1);
		cancel = false;
	}

	void BaseAudioApplication::Run(int argc,char** argv)
	{
		Start();
		UserMain();
		Stop();
	}

	void BaseAudioApplication::UserMain(void)
	{
		printf("Press enter to terminate\n");
		getchar();
	}

	void* BaseAudioApplication::SAudioThread(BaseAudioApplication *pThis)
	{
	#ifdef WIN32
		BOOL res;
		DWORD err;
		
		// Maximum priority for the process: this has the upper hand over
		// everything else, including most essential OS services
		//res = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		// Toned down priority: this enables cache flush (at least)
		//res = SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS );

		// With the new AudioIO approach (POLL instead of NOTIFY), it seems that
		// we should NORMAL priority. If not, the system gets very slow.
		// 
		// the other solution (now used) is to have a Sleep in the polling loop
		res = SetPriorityClass(GetCurrentProcess(),HIGH_PRIORITY_CLASS );

		err = GetLastError();
		
		// Maximum priority for the thread under Win32
		//res = SetThreadPriority(GetCurrentThread(),THREAD_PRIORITY_TIME_CRITICAL);
		//Toned down thread priority
		//res = SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );
		// With the new AudioIO approach (POLL instead of NOTIFY), it seems that
		// we should should NORMAL priority. If not, the system gets very slow
		res = SetThreadPriority( GetCurrentThread(), THREAD_PRIORITY_HIGHEST );
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
	
		pThis->AudioMain();
	
		return NULL;
	}

	void BaseAudioApplication::SAudioThreadCleanup(BaseAudioApplication *pThis)
	{
	}

} // namespace CLAM

