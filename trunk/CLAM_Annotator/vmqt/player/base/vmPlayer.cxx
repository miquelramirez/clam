#include "vmPlayList.hxx"
#include "vmPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		Player::Player(QObject* parent)
			: QThread(parent)
			, mIsPlaying(false)
			, mIsPaused(false)
			, mSamplingRate(44100.0)
			, mBeginTime(0.0)
			, mTimeBounds(0.0,1.0)
		{
			PlayList::Add(this);
		}
		
		Player::~Player()
		{
			PlayList::Remove(this);
			if(mIsPlaying)
			{
				mIsPlaying = false;
				terminate();
				wait();
			}
		}

		void Player::SetDuration(double dur)
		{
			if(dur <= 0) return;
			mTimeBounds.min = 0.0;
			mTimeBounds.max = dur;
			mBeginTime = 0.0;
		}

		void Player::SetSamplingRate(double sr)
		{
			mSamplingRate = sr;
		}

		void Player::SetPlayingFlags(int flags)
		{
			mPlayingFlags = flags;
		}

		bool Player::IsPlaying() const
		{
			return mIsPlaying;
		}

		void Player::play()
		{
			PlayList::Stop();
			mIsPlaying = true;
			mIsPaused = false;
			start();
		}

		void Player::pause()
		{
			mIsPaused = true;
			mIsPlaying = false;
		}

		void Player::stop()
		{
			mIsPlaying = false;
			mIsPaused = false;
			wait();
		}

		void Player::timeBounds(double min, double max)
		{
			if(min < 0) return;
			if(min >= max) return;
			mTimeBounds.min = min;
			mTimeBounds.max = max;
			mBeginTime = mTimeBounds.min;
		}
	}
}

// END
