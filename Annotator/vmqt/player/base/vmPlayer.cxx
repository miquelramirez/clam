/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "vmPlayList.hxx"
#include "vmPlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		Player::Player(QObject* parent)
			: QThread(parent)
			, mPlayStatus(Stoped)
			, mSamplingRate(44100.0)
			, mBeginTime(0.0)
			, mTimeBounds(0.0,1.0)
		{
			PlayList::Add(this);
		}
		
		Player::~Player()
		{
			PlayList::Remove(this);
			if (mPlayStatus == Playing)
			{
				mPlayStatus = Stoped;
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
			return mPlayStatus == Playing;
		}

		void Player::play()
		{
			PlayList::Stop();
			mPlayStatus = Playing;
			start();
		}

		void Player::pause()
		{
			mPlayStatus = Paused;
		}

		void Player::stop()
		{
			mPlayStatus = Stoped;
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

