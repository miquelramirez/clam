#ifndef __LOCK__
#define __LOCK__

#include "ErrSystem.hxx"
#include "xtime.hxx"

namespace CLAM
{
	class Condition;
	
	namespace Detail
	{
		
		using CLAM::xtime;
				
		template <typename MutexType>
		class LockOps 
		{
		private:
			LockOps() { }
			
		public:
			typedef typename MutexType::ConditionVar LockState;
			
			static void Lock(MutexType& m)
			{
				m.DoLock();
			}
			static bool TryLock(MutexType& m)
			{
				return m.DoTryLock();
			}
			static bool TimedLock(MutexType& m, const xtime& xt)
			{
				return m.DoTimedLock(xt);
			}
			static void Unlock(MutexType& m)
			{
				m.DoUnlock();
			}
			static void Lock(MutexType& m, LockState& state)
			{
				m.DoLock(state);
			}
			static void Unlock(MutexType& m, LockState& state)
			{
				m.DoUnlock(state);
			}
		};
		
		template <typename MutexType>
		class ScopedLock
		{
		public:
			typedef MutexType mutex_type;
			
			explicit ScopedLock(MutexType& mx, bool initially_locked=true)
				: mMutex(mx), mLocked(false)
			{
				if (initially_locked) 
					Lock();
			}
			~ScopedLock()
			{
				if (mLocked) 
					Unlock();
			}
			
			void Lock()
			{
				if (mLocked) 
					throw LockError( "Trying to Lock already locked Mutex" );
				
				Detail::LockOps<mutex_type>::Lock(mMutex);
				mLocked = true;
			}
			
			void Unlock()
			{
				if (!mLocked) 
					throw LockError("Trying to Unlock already a not yet locked Mutex");
			
				LockOps<mutex_type>::Unlock(mMutex);

				mLocked = false;
			}

			bool Locked() const 
			{ 
				return mLocked; 
			}

			operator const void*() const 
			{ 
				return mLocked ? this : 0; 
			}

		private:
			friend class CLAM::Condition;

			MutexType& mMutex;
			bool mLocked;
		};

		template <typename TryMutexType>
		class ScopedTryLock 
		{
		public:
			typedef TryMutexType mutex_type;

			explicit ScopedTryLock(TryMutexType& mx)
				: mMutex(mx), mLocked(false)
			{
				TryLock();
			}

			ScopedTryLock(TryMutexType& mx, bool initially_locked)
				: mMutex(mx), mLocked(false)
			{
				if (initially_locked) 
					Lock();
			}
		
			~ScopedTryLock()
			{
				if (mLocked) 
					Unlock();
			}

			void Lock()
			{
				if (mLocked) 
					throw LockError("Trying to Lock already locked Mutex");
			
				LockOps<TryMutexType>::Lock(mMutex);

				mLocked = true;
			}

			bool TryLock()
			{
				if (mLocked) 
					throw LockError("Trying to Lock already locked Mutex");

				return (mLocked = LockOps<TryMutexType>::TryLock(mMutex));
			}
		
			void Unlock()
			{
				if (!mLocked) 
					throw LockError("Trying to Unlock already a not yet locked Mutex");
			
				LockOps<TryMutexType>::Unlock(mMutex);

				mLocked = false;
			}

			bool Locked() const 
			{ 
				return mLocked; 
			}
		
			operator const void*() const 
			{ 
				return mLocked ? this : 0; 
			}

		private:
		
			friend class CLAM::Condition;

			TryMutexType& mMutex;
			bool mLocked;
		};

		template <typename TimedMutexType>
		class ScopedTimedLock 
		{
		public:
			typedef TimedMutexType mutex_type;

			ScopedTimedLock(TimedMutexType& mx, const xtime& xt )
				: mMutex(mx), mLocked(false)
			{
			
				TimedLock(xt);
			}
		
			ScopedTimedLock(TimedMutexType& mx, bool initially_locked)
				: mMutex(mx), mLocked(false)
			{
				if (initially_locked) 
					Lock();
			}
		
			~ScopedTimedLock()
			{
				if (mLocked) 
					Unlock();
			}

			void Lock()
			{
				if (mLocked) 
					throw LockError("Trying to Lock already locked Mutex");
			
				LockOps<TimedMutexType>::Lock(mMutex);

				mLocked = true;
			}

			bool TimedLock(const xtime& xt)
			{
				if (mLocked) 
					throw LockError("Trying to Lock already locked Mutex");

				return (mLocked = LockOps<TimedMutexType>::TimedLock(mMutex, xt));
			}
			void Unlock()
			{
				if (!mLocked) 
					throw LockError("Trying to Unlock already a not yet locked Mutex");

				LockOps<TimedMutexType>::Unlock(mMutex);
			
				mLocked = false;
			}

			bool Locked() const 
			{ 
				return mLocked; 
			}
		
			operator const void*() const 
			{ 
				return mLocked ? this : 0; 
			}

		private:

			friend class CLAM::Condition;

			TimedMutexType& mMutex;
			bool mLocked;
		};
	
	} // End of namespace detail

} // end of namespace CLAM

#endif // Lock.hxx
