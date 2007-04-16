#ifndef __QTSINGLEPLAYER__
#define __QTSINGLEPLAYER__

#include "QtPlayer.hxx"

namespace CLAM
{
    namespace VM
    {
		class Player;
		
		class QtSinglePlayer : public QtPlayer
		{	
		public:
			QtSinglePlayer(QWidget* parent=0);
			virtual ~QtSinglePlayer();

			void SetPlaySegment(const MediaTime& time);
			bool IsPlaying();
			bool IsRunning();

			void SetSlotPlayingTime(Slotv1<TData>& slot);
			void SetSlotStopPlaying(Slotv1<TData>& slot);
				
        public slots:
			virtual void play();
			virtual void pause();
			virtual void stop();

		protected:
			Player* mPlayer;
				
			void SetPlayer(Player* player);
							
		};
    }
}

#endif

