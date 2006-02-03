#ifndef __VMQT_PLAYABLE_AUDIO_PLOT_H__
#define __VMQT_PLAYABLE_AUDIO_PLOT_H__

#include "vm_audio_plot.hxx"

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

			void set_data(const Audio& audio, bool update=false);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void setColorSchema(int);

		private:
			QComboBox* wp_combo_box;

			WPlayer*     wp_wplayer;
			AudioPlayer* wp_audio_player;

			void init_playable_audio_plot();
		};
	}
}

#endif
