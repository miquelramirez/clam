#ifndef __PLAYABLEPLOT__
#define __PLAYABLEPLOT__

namespace CLAM
{
	namespace VM
	{
		class QtPlayer;
		
		class PlayablePlot 
		{
			public:
				PlayablePlot();
				virtual ~PlayablePlot();
				
				void Play();
				void Pause();
				void Stop();
				
				bool IsPlaying();
			
			protected:
				QtPlayer* _player;

				void RemoveFromPlayList();

		};
	}
}

#endif

