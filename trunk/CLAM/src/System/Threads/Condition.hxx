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

#ifndef __CONDITION__
#define __CONDITION__

#include "ErrSystem.hxx"
#include "Lock.hxx"
#include <pthread.h>

namespace CLAM
{

	struct xtime;

	class Condition
	{
	public:
		Condition();
		~Condition();

		void NotifyOne();
		void NotifyAll();
		
		template <typename L>
		void Wait( L& lock )
		{
			if ( !lock )
				throw LockError("Invalid lock");
			
			DoWait( lock.mMutex );
		}

		template <typename L, typename Pr >
		void Wait( L& lock, Pr predicate )
		{
			if ( !lock )
				throw LockError("Invalid lock");

			while ( !predicate() )
				DoWait( lock.mMutex );
		}

		template <typename L>
		bool TimedWait( L& lock, const xtime& xt )
		{
			if (!lock )
				throw LockError();

			return DoTimedWait( lock.mMutex, xt );
		}

		template <typename L, typename Pr>
		bool TimedWait( L& lock, const xtime& xt, Pr predicate )
		{
			if ( !lock )
				throw LockError();

			while( !predicate() )
				{
					if (!DoTimedWait( lock.mMutex, xt ) )
						return false;
				}

			return true;
		}

	private:
		
		template <typename M>
		void DoWait( M& mutex )
		{
			typedef typename Detail::LockOps<M> LockOps;
			typename LockOps::LockState state;

			LockOps::Unlock( mutex, state );
			DoWait( state.pmutex );
			LockOps::Lock( mutex, state );
		}

		template <typename M>
		bool DoTimedWait( M& mutex, const xtime& xt )
		{
			typedef typename Detail::LockOps<M> LockOps;
			typename LockOps::LockState state;
			
			LockOps::Unlock( mutex, state );

			bool ret = false;

			ret = DoTimedWait( xt, state.pmutex );

			LockOps::Lock( mutex, state );

			return ret;
		}

		void DoWait( pthread_mutex_t* pMutex );

		bool DoTimedWait( const xtime& xt, pthread_mutex_t* pMutex);

		pthread_cond_t mCondition;
	};

} // end of namespace CLAM

#endif // Condition.hxx

