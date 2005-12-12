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
			void SetSampleRate(TData sr);
			void Update(TIndex index, TData yvalue);

		protected:
			void thread_code();

		private:
			BPF          mBPFData;
			const Audio* mAudioPtr;
			TData        mSampleRate;

			TIndex GetFirstIndex();
		};
	}
}

#endif
