#ifndef __VMQT_AUDIO_PLAYER_H__
#define __VMQT_AUDIO_PLAYER_H__

#include <vector>
#include "vm_player.hxx"

namespace CLAM
{
	class Audio;

	namespace VM
	{
		class AudioPlayer : public Player
		{
			Q_OBJECT
		public:
			AudioPlayer(QObject* parent=0);
			~AudioPlayer();

			void set_data(std::vector<const Audio*> data);

		public slots:
			void muteChannel0(bool muted);
			void muteChannel1(bool muted);
			
		private:
			bool pl_chn0_muted;
			bool pl_chn1_muted;

			const Audio* pl_chn0;
			const Audio* pl_chn1;

			void run();
		};
	}
}

#endif
