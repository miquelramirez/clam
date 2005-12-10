#ifndef __BPFPLAYER__
#define __BPFPLAYER__

#include "BPF.hxx"
#include "Player.hxx"

namespace CLAM
{
	class Audio;

	namespace VM
	{
		class BPFPlayer : public Player
		{
		public:
			BPFPlayer();
			~BPFPlayer();

			void SetData(const BPF& bpf);
			void SetAudioPtr(const Audio* audio);
			void SetDuration(TData duration);

			void Update(TIndex index, TData yvalue);

		protected:
			void thread_code();

		private:
			BPF          mBPFData;
			const Audio* mAudioPtr;

			TIndex GetFirstIndex();
		};
	}
}

#endif
