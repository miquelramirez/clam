#include "vm_play_list.hxx"
#include "vm_player.hxx"

namespace CLAM
{
	namespace VM
	{
		Player::Player(QObject* parent)
			: QThread(parent)
			, pl_is_playing(false)
			, pl_is_paused(false)
			, pl_sampling_rate(44100.0)
			, pl_begin_time(0.0)
			, pl_time_bounds(0.0,1.0)
		{
			PlayList::add(this);
		}
		
		Player::~Player()
		{
			PlayList::remove(this);
			if(pl_is_playing)
			{
				pl_is_playing = false;
				terminate();
				wait();
			}
		}

		void Player::set_duration(double dur)
		{
			pl_time_bounds.min = 0.0;
			pl_time_bounds.max = dur;
			pl_begin_time = 0.0;
		}

		void Player::set_sambling_rate(double sr)
		{
			pl_sampling_rate = sr;
		}

		void Player::set_playing_flags(int flags)
		{
			pl_playing_flags = flags;
		}

		bool Player::is_playing() const
		{
			return pl_is_playing;
		}

		void Player::play()
		{
			PlayList::stop();
			pl_is_playing = true;
			pl_is_paused = false;
			start();
		}

		void Player::pause()
		{
			pl_is_paused = true;
			pl_is_playing = false;
		}

		void Player::stop()
		{
			pl_is_playing = false;
			pl_is_paused = false;
			wait();
		}

		void Player::timeBounds(double min, double max)
		{
			if(min >= max) return;
			pl_time_bounds.min = min;
			pl_time_bounds.max = max;
			pl_begin_time = pl_time_bounds.min;
		}
	}
}

// END
