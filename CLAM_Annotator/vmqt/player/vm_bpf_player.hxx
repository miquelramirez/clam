#ifndef __VMQT_BPF_PLAYER_H__
#define __VMQT_BPF_PLAYER_H__

#include "BPF.hxx"
#include "vm_player.hxx"

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

			void set_data(const BPF& bpf);
			void set_audio_ptr(const Audio* audio);
			void set_pitch_bounds(double min, double max);

		private:
			const BPF*   pl_bpf;
			const Audio* pl_audio;
			Range        pl_pitch_bounds;

			void run();

			unsigned first_index();
			double get_pitch(unsigned index);
		};
	}
}

#endif
