#ifndef __VMQT_WPLAYER_H__
#define __VMQT_WPLAYER_H__

#include <QWidget>
#include "vm_data_types.hxx"

class QPushButton;

namespace CLAM
{
	namespace VM
	{
		class CLAM::VM::Player;
		
		class WPlayer : public QWidget
		{
			Q_OBJECT
		public:
			WPlayer(QWidget* parent=0);
			~WPlayer();

			void set_player(CLAM::VM::Player* player);
			void set_pixmaps(const QPixmap& play, const QPixmap& pause, const QPixmap& stop);
			
		private slots:
			void play();
			void pause();
			void stop();

		private:
			QPushButton* wp_play;
			QPushButton* wp_pause;
			QPushButton* wp_stop;

			CLAM::VM::Player* wp_player;

			void build_wplayer();
		};
	}
}

#endif
