#ifndef __QTSINGLEBPFPLAYER__
#define __QTSINGLEBPFPLAYER__

#include "Thread.hxx"
#include "BPF.hxx"
#include "QtSinglePlayer.hxx"

namespace CLAM
{
	class Audio;

	namespace VM
	{
		class QtSingleBPFPlayer : public QtSinglePlayer
		{
			Q_OBJECT
		public:
			QtSingleBPFPlayer(QWidget* parent=0);
			~QtSingleBPFPlayer();

			void SetData(const BPF& bpf);
			void SetAudioPtr(const Audio* audio, int chn);
			void SetDuration(TData duration);
			void SetSampleRate(TData sr);
			void Update(TIndex index, TData value);

		public slots:
			void play();

		private:
			double mYMin;
			double mYMax;
			bool   mMustDoMapping;
			bool   mWorking;
			bool   mCancelThread;

			BPF    mBPF;
			Thread mThread;
			
			void AdaptIncomingData();
			void thread_code();
			void StopThread();
			std::pair<double,double> GetMinMaxY(const BPF& bpf);
		};
	}
}

#endif

