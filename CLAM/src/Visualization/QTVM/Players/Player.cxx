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
			if(IsPlaying()) 
			{
				Stop();
				SetPlaying(false);
			}
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

