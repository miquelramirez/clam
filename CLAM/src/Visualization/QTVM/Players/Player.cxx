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

#include "Player.hxx"

namespace CLAM
{
	namespace VM
	{
		Player::Player()
		{
			HaveData(false);
			SetPlaying(false);
			SetPaused(false);
			SetBeginTime(TData(0.0));
		}
		
		Player::~Player()
		{
		    Stop();
		}
			
		void Player::Play()
		{
			CLAM_ASSERT(HaveData(),"The player has no data to play!");
			if(IsPaused()) SetPaused(false);
			if(IsPlaying()) Stop();
			_thread.Start();
			SetPlaying(true);
		}
		
		void Player::Stop()
		{
			if(IsPlaying())
			{
				SetPaused(false);
				SetPlaying(false);
				_thread.Stop();
			}
		}
		
		void Player::Pause()
		{
			SetPaused(true);
			SetPlaying(false);
		}
				
		void Player::SetBounds(const MediaTime& time)
		{
			_beginTime = time.GetBegin();
			_time.SetBegin(time.GetBegin());
			_time.SetEnd(time.GetEnd());
		}

		void Player::HaveData(bool d)
		{
			_haveData = d;
		}

		bool Player::HaveData()
		{
			return _haveData;
		}

		void Player::SetPlaying(bool playing)
		{
			_isPlaying = playing;
		}

		bool Player::IsPlaying()
		{
			return _isPlaying;
		}

		void Player::SetPaused(bool paused)
		{
			_isPaused = paused;
		}

		bool Player::IsPaused()
		{
			return _isPaused;
		}

		void Player::SetBeginTime(const TData& begin)
		{
			_beginTime = begin;
		}

		TData Player::GetBeginTime() const
		{
			return _beginTime;
		}
	}
}

// END

