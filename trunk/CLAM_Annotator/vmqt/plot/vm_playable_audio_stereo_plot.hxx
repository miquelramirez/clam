#ifndef __VMQT_PLAYABLE_AUDIO_STEREO_PLOT_H__
#define __VMQT_PLAYABLE_AUDIO_STEREO_PLOT_H__

#include "vm_audio_stereo_plot.hxx"

class QComboBox;
class QCheckBox;

namespace CLAM
{
	namespace VM
	{
		class WPlayer;
		class AudioPlayer;

		class PlayableAudioStereoPlot : public CLAM::VM::AudioStereoPlot
		{
			Q_OBJECT
		public:
			PlayableAudioStereoPlot(QWidget* parent=0);
			~PlayableAudioStereoPlot();

			void set_data(const Audio& chn0, const Audio& chn1, bool update=false);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void setColorSchema(int);
			void muteChannel0(int);
			void muteChannel1(int);

		private:
			QComboBox* wp_combo_box;
			QCheckBox* wp_mute_chn0;
			QCheckBox* wp_mute_chn1;

			WPlayer*     wp_wplayer;
			AudioPlayer* wp_audio_player;

			void init_playable_audio_stereo_plot();
		};
	}
}

#endif
