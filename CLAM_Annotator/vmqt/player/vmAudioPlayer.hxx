#ifndef __VMQT_AUDIO_PLAYER_H__
#define __VMQT_AUDIO_PLAYER_H__

#include <vector>
#include "vmPlayer.hxx"

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

			void SetData(std::vector<const Audio*> data);

		public slots:
			void muteChannel0(bool muted);
			void muteChannel1(bool muted);
			
		private:
			bool mChn0Muted;
			bool mChn1Muted;

			const Audio* mChn0;
			const Audio* mChn1;

			void run();
		};
	}
}

#endif
