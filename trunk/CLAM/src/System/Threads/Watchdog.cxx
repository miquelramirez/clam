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

#include "Watchdog.hxx"

#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>

namespace CLAM {


	sig_atomic_t Watchdog::mBeat = 0;
	unsigned int Watchdog::mQuantumTime = 5;
	bool         Watchdog::mStarted = false;
	bool         Watchdog::mVerbose = false;

	void *Watchdog::BeatThread(void*)
	{
		setuid(getuid());

		while(1)
		{
			sleep(mQuantumTime);
			if (mBeat != 1)
			{
				mBeat = 1;
			}
			if (mVerbose)
			{
				putchar('-');
				fflush(stdout);
			}
		}
	}


	void *Watchdog::WatchdogThread(void*)
	{
		setuid(getuid());

		if (mVerbose) puts("WatchdogThread: Started.");

		while(1)
		{
			sleep(2*mQuantumTime);

			if (mBeat == 1)
			{
				mBeat = 0;
			}
			else
			{
				puts("WatchdogThread: CPU exhaustion detected. Exiting.");
				exit(1);
			}
		}
		return 0;
	}


	bool Watchdog::Start(unsigned int quantum, bool verbose)
	{
		if (mStarted)
			return true;

		if (quantum < 1)
		{
			puts("Watchdog::Start(): Time quantum must be at least 1 second.");
			return false;
		}

		mVerbose = verbose;

		pthread_t          beat_thread;
		pthread_attr_t     beat_attrs;

		pthread_attr_init(&beat_attrs);

		if (pthread_create(&beat_thread,
						   &beat_attrs,
						   &BeatThread, 0) != 0)
		{
			puts("Watchdog::Start(): Failed to create beat thread (you need to be root).");
			perror("pthread_create()");
			return false;
		}

		pthread_t          watchdog_thread;
		pthread_attr_t     watchdog_attrs;
		struct sched_param watchdog_param;

		watchdog_param.sched_priority = 99;

		pthread_attr_init(&watchdog_attrs);
		pthread_attr_setschedpolicy(&watchdog_attrs,SCHED_FIFO);
		pthread_attr_setschedparam(&watchdog_attrs,&watchdog_param);

		if (pthread_create(&watchdog_thread,
						   &watchdog_attrs,
						   &WatchdogThread, 0) != 0)
		{
			puts("Watchdog::Start(): Failed to create watchdog thread (you need to be root).");
			perror("pthread_create()");
			return false;
		}

		mStarted = true;

		return true;
	}

}

