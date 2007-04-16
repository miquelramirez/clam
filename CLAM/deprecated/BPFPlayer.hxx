#ifndef __BPFPLAYER__
#define __BPFPLAYER__

#include "BPF.hxx"
#include "Player.hxx"

namespace CLAM
{
	class Audio;

	namespace VM
	{
		enum { LEFT_CHANNEL=0, RIGHT_CHANNEL=1, BOTH_CHANNELS=2 };

		class BPFPlayer : public Player
		{
		public:
			BPFPlayer();
			~BPFPlayer();

			void SetData(const BPF& bpf);
			void SetAudioPtr(const Audio* audio, int chn);
			void SetDuration(TData duration);
			void SetSampleRate(TData sr);
			void Update(TIndex index, TData yvalue);

		protected:
			void thread_code();

		private:
			BPF          mBPFData;
			const Audio* mAudioPtrL;
			const Audio* mAudioPtrR;
			TData        mSampleRate;

			TIndex GetFirstIndex();
		};
	}
}

#endif

