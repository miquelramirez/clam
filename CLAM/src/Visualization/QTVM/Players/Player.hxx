#ifndef __PLAYER__
#define __PLAYER__

#include "MediaTime.hxx"
#include "Thread.hxx"

namespace CLAM
{
	namespace VM
	{
		class Player 
		{
			public:
				Player();
				virtual ~Player();
				
				void Play();
				void Pause();
				void Stop();

				bool IsPlaying();

				void SetBounds(const MediaTime& time);
	
			protected:
				Thread _thread;
				MediaTime _time;

				virtual void thread_code()=0;
				
				void HaveData(bool d);
				void SetPlaying(bool playing);
				void SetPaused(bool paused);

				bool IsPaused();
					
				bool HaveData();
								
				void SetBeginTime(const TData& begin);
				TData GetBeginTime() const;
				
			private:
				TData _beginTime;
				bool _haveData;
				bool _isPlaying;
				bool _isPaused;
				
		};
	}
}

#endif

