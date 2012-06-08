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

#include <CLAM/Player.hxx>

namespace CLAM
{
    namespace VM
    {
		Player::Player()
			: mBeginTime(TData(0.0))
			, mHaveData(false)
			, mIsPlaying(false)
			, mIsPaused(false)
			, mIsStopped(false)
		{
		}
		
		Player::~Player()
		{
			Stop();
		}
			
		void Player::Play()
		{
			if(!HaveData()) return;
			if(IsPaused()) SetPaused(false);
			if(IsStopped()) SetStopped(false);
			if(IsPlaying()) Stop();
			SetPlaying(true);
			mThread.Start();
		}
		
		void Player::Stop()
		{
			if(IsPlaying())
			{
				SetPaused(false);
				SetPlaying(false);
				SetStopped(true);
				mThread.Stop();
			}
		}
		
		void Player::Pause()
		{
			SetPaused(true);
			SetPlaying(false);
			SetStopped(false);
		}
				
		void Player::SetBounds(const MediaTime& time)
		{
			mBeginTime = time.GetBegin();
			mTime.SetBegin(time.GetBegin());
			mTime.SetEnd(time.GetEnd());
		}

		void Player::SetSlotPlayingTime(Slotv1<TData>& slot)
		{
			mSigPlayingTime.Connect(slot);
		}

		void Player::SetSlotStopPlaying(Slotv1<TData>& slot)
		{
			mSigStop.Connect(slot);
		}

		void Player::HaveData(bool d)
		{
			mHaveData = d;
		}

		const bool& Player::HaveData() const
		{
			return mHaveData;
		}

		void Player::SetPlaying(bool playing)
		{
			mIsPlaying = playing;
		}

		const bool& Player::IsPlaying() const
		{
			return mIsPlaying;
		}

		void Player::SetPaused(bool paused)
		{
			mIsPaused = paused;
		}

		const bool& Player::IsPaused() const
		{
			return mIsPaused;
		}

		void Player::SetBeginTime(const TData& begin)
		{
			mBeginTime = begin;
		}

		const TData& Player::GetBeginTime() const
		{
			return mBeginTime;
		}

		void Player::SetStopped(bool stopped)
		{
			mIsStopped = stopped;
		}

		const bool& Player::IsStopped() const
		{
			return mIsStopped;
		}

		bool Player::IsRunning() const
		{
			return mThread.IsRunning();
		}
    }
}

// END

