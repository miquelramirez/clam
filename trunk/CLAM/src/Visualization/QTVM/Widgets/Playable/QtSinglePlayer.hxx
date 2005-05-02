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
	    ~QtSinglePlayer();

	    void SetPlaySegment(const MediaTime& time);
	    bool IsPlaying();

	    void SetSlotPlayingTime(Slotv1<TData>& slot);
	    void SetSlotStopPlaying(Slotv1<TData>& slot);
				
        public slots:
	    void play();
	    void pause();
	    void stop();

	protected:
	    Player* _player;
				
	    void SetPlayer(Player* player);
							
	};
    }
}

#endif
