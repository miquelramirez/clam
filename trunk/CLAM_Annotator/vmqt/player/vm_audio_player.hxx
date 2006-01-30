#ifndef __VMQT_AUDIO_PLAYER_H__
#define __VMQT_AUDIO_PLAYER_H__

#include <vector>
#include "vm_player.hxx"

namespace CLAM
{
	class CLAM::Audio;

	namespace VM
	{
		class AudioPlayer : public CLAM::VM::Player
		{
			Q_OBJECT
		public:
			AudioPlayer(QObject* parent=0);
			~AudioPlayer();

			void set_data(std::vector<const CLAM::Audio*> data);

		public slots:
			void muteChannel0(bool muted);
			void muteChannel1(bool muted);
			
		private:
			bool pl_chn0_muted;
			bool pl_chn1_muted;

			const CLAM::Audio* pl_chn0;
			const CLAM::Audio* pl_chn1;

			void run();
		};
	}
}

#endif
