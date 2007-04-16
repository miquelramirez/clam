/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Condition.hxx"
#include "Thread.hxx"
#include "ErrSystem.hxx"
#include "Assert.hxx"
#include "xtime.hxx"
#include <errno.h>

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

