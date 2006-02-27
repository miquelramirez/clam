#ifndef __VMQT_BPF_PLAYER_H__
#define __VMQT_BPF_PLAYER_H__

#include "BPF.hxx"
#include "vmPlayer.hxx"

namespace CLAM
{
	class Audio;

	namespace VM
	{
		class BPFPlayer : public Player
		{
			Q_OBJECT
		public:
			BPFPlayer(QObject* parent=0);
			~BPFPlayer();

			void SetData(const BPF& bpf);
			void SetAudioPtr(const Audio* audio, unsigned channelMask=1|2); // TODO: Make this be enums
			void SetPitchBounds(double min, double max);

		private:
			const BPF*   mBPF;
			const Audio* mAudio;
			Range        mPitchBounds;

			void run();

			unsigned FirstIndex();
			double GetPitch(unsigned index);
		};
	}
}

#endif
