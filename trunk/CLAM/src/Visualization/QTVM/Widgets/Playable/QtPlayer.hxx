#ifndef __QTPLAYER__
#define __QTPLAYER__

#include <qwidget.h>
#include "MediaTime.hxx"

class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class Player;
		
		class QtPlayer : public QWidget
		{
			Q_OBJECT
			
			public:
				QtPlayer(QWidget* parent=0);
				virtual ~QtPlayer();

				void SetPlaySegment(const MediaTime& time);
				bool IsPlaying();
				
			public slots:
				void play();
				void pause();
				void stop();

			protected:
				Player* _player;
				
				void SetPlayer(Player* player);
				
			private:
				QPushButton *_play,*_pause,*_stop;

				void InitWidgets();
				
		};
	}
}

#endif
