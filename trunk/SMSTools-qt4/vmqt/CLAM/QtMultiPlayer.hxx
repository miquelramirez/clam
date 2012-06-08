#ifndef __QTMULTIPLAYER__
#define __QTMULTIPLAYER__

#include <vector>
#include <CLAM/QtPlayer.hxx>

namespace CLAM
{
    namespace VM
    {
		class Player;
		
		class QtMultiPlayer : public QtPlayer
		{
			Q_OBJECT

		public:
			QtMultiPlayer(QWidget* parent=0);
			virtual ~QtMultiPlayer();

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
			std::vector<Player*> mPlayers;
			int                  mCurrentPlayer;
			bool                 mAllPlayers;
	    	
			virtual void SetCurrentPlayer(int playerID)=0;

			void AddPlayer(Player* player);
			void AllPlayers(bool all);					
	   
		};
    }
}

#endif

