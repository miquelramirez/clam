#ifndef __QTSINGLEBPFPLAYEREXT__
#define __QTSINGLEBPFPLAYEREXT__

#include <qwidget.h>
#include "BPF.hxx"
#include "BPFPlayer.hxx"
#include "PlayablePlot.hxx"
#include "Slotv1.hxx"

using SigSlot::Slotv1;

namespace CLAM
{
	class Audio;

	namespace VM
	{
		class QtSingleBPFPlayerExt : public QWidget, public PlayablePlot
		{
			Q_OBJECT
		public:
			QtSingleBPFPlayerExt(QWidget* parent=0);
			~QtSingleBPFPlayerExt();

			void SetData(const BPF& bpf);
			void SetAudioPtr(const Audio* audio, int chn);
			void SetDuration(TData duration);
			void SetSampleRate(TData sr);

		signals:
			void playingTime(float);
			void stopPlaying(float);

		public slots:
			void setRegionTime(float, float);
			void updateYValue(int, float);

		protected:
			void closeEvent(QCloseEvent* e);

		private:
			Slotv1<TData> mSlotPlayingTime;
			Slotv1<TData> mSlotStopPlaying;

			void InitWidget();
			void PlayingTime(TData time);
			void StopPlaying(TData time);
		};
	}
}

#endif

