#ifndef __VMQT_WPLAYER_H__
#define __VMQT_WPLAYER_H__

#include <QtGui/QWidget>
#include "vmDataTypes.hxx"

class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class Player;
		
		class WPlayer : public QWidget
		{
			Q_OBJECT
		public:
			WPlayer(QWidget* parent=0);
			~WPlayer();

			void SetPlayer(Player* player);
			void SetPixmaps(const QPixmap& play, const QPixmap& pause, const QPixmap& stop);
			
		private slots:
			void play();
			void pause();
			void stop();

		private:
			QPushButton* mPlay;
			QPushButton* mPause;
			QPushButton* mStop;

			Player* mPlayer;

			void BuildWPlayer();
		};
	}
}

#endif
