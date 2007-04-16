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

#ifndef __RECURSIVEMUTEX__
#define __RECURSIVEMUTEX__

#include <pthread.h>
#include "Lock.hxx"

namespace CLAM
{

	struct xtime;

	class RecursiveMutex
	{
	public:
		
		friend class Detail::LockOps<RecursiveMutex>;

		typedef Detail::LockOps<RecursiveMutex> LockOps;
		typedef Detail::ScopedLock<RecursiveMutex> ScopedLock;

		RecursiveMutex();

		~RecursiveMutex();

	public:
		struct ConditionVar
		{
			long count;
			pthread_mutex_t* pmutex;
		};
	private:
		void DoLock();
		void DoUnlock();
		void DoLock( ConditionVar& state );
		void DoUnlock( ConditionVar& state );

		pthread_mutex_t mMutex;
		unsigned mCount;
		pthread_cond_t mUnlocked;
		pthread_t      mThreadID;
		bool           mValidID;
	};

	class RecursiveTryMutex
	{
	public:
		friend class Detail::LockOps<RecursiveTryMutex>;

		typedef Detail::ScopedLock<RecursiveTryMutex> ScopedLock;
		typedef Detail::ScopedTryLock<RecursiveTryMutex> ScopedTryLock;

		RecursiveTryMutex();

		~RecursiveTryMutex();

	private:
		struct ConditionVar
		{
			long count;
			pthread_mutex_t* pmutex;
		};

		void DoLock();
		bool DoTryLock();
		void DoUnlock();
		void DoLock( ConditionVar& state );
		void DoUnlock( ConditionVar& state );

		pthread_mutex_t mMutex;
		unsigned mCount;
		pthread_cond_t mUnlocked;
		pthread_t      mThreadID;
		bool           mValidID;
	};

	class RecursiveTimedMutex
	{
	public:
		friend class Detail::LockOps<RecursiveTimedMutex>;

		typedef Detail::ScopedLock<RecursiveTimedMutex> ScopedLock;
		typedef Detail::ScopedTryLock<RecursiveTimedMutex> ScopedTryLock;
		typedef Detail::ScopedTimedLock<RecursiveTimedMutex> ScopedTimedLock;

		RecursiveTimedMutex();

		~RecursiveTimedMutex();

	private:
		struct ConditionVar
		{
			long count;
			pthread_mutex_t* pmutex;
		};

		void DoLock();
		bool DoTryLock();
		bool DoTimedLock( const xtime& xt );
		void DoUnlock();
		void DoLock( ConditionVar& state );
		void DoUnlock( ConditionVar& state );

		pthread_mutex_t mMutex;
		unsigned mCount;
		pthread_cond_t mUnlocked;
		pthread_t      mThreadID;
		bool           mValidID;
	};

} // namespace CLAM


#endif // RecursiveMutex.hxx

