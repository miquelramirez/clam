#ifndef __VMQT_PLAYABLE_AUDIO_PLOT_H__
#define __VMQT_PLAYABLE_AUDIO_PLOT_H__

#include "vmAudioPlot.hxx"

class QComboBox;

namespace CLAM
{
	namespace VM
	{
		class WPlayer;
		class AudioPlayer;

		class PlayableAudioPlot : public AudioPlot
		{
			Q_OBJECT
		public:
			PlayableAudioPlot(QWidget* parent=0);
			~PlayableAudioPlot();

			void SetData(const Audio& audio, bool update=false);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void setColorSchema(int);

		private:
			QComboBox* mComboBox;

			WPlayer*     mWPlayer;
			AudioPlayer* mAudioPlayer;

			void InitPlayableAudioPlot();
		};
	}
}

#endif
