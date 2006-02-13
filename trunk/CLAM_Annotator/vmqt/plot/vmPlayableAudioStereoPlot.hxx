#ifndef __VMQT_PLAYABLE_AUDIO_STEREO_PLOT_H__
#define __VMQT_PLAYABLE_AUDIO_STEREO_PLOT_H__

#include "vmAudioStereoPlot.hxx"

class QComboBox;
class QCheckBox;

namespace CLAM
{
	namespace VM
	{
		class WPlayer;
		class AudioPlayer;

		class PlayableAudioStereoPlot : public AudioStereoPlot
		{
			Q_OBJECT
		public:
			PlayableAudioStereoPlot(QWidget* parent=0);
			~PlayableAudioStereoPlot();

			void SetData(const Audio& chn0, const Audio& chn1, bool update=false);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void setColorSchema(int);
			void muteChannel0(int);
			void muteChannel1(int);

		private:
			QComboBox* mComboBox;
			QCheckBox* mMuteChn0;
			QCheckBox* mMuteChn1;

			WPlayer*     mWPlayer;
			AudioPlayer* mAudioPlayer;

			void InitPlayableAudioStereoPlot();
		};
	}
}

#endif
