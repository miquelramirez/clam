#ifndef __VMQT_PLAYABLE_AUDIO_PLOT_H__
#define __VMQT_PLAYABLE_AUDIO_PLOT_H__

#include "vm_audio_plot.hxx"

class QComboBox;

namespace CLAM
{
	namespace VM
	{
		class CLAM::VM::WPlayer;
		class CLAM::VM::AudioPlayer;

		class PlayableAudioPlot : public CLAM::VM::AudioPlot
		{
			Q_OBJECT
		public:
			PlayableAudioPlot(QWidget* parent=0);
			~PlayableAudioPlot();

			void set_data(const CLAM::Audio& audio, bool update=false);

		public slots:
			void backgroundWhite();
			void backgroundBlack();

		private slots:
			void setColorSchema(int);

		private:
			QComboBox* wp_combo_box;

			CLAM::VM::WPlayer*     wp_wplayer;
			CLAM::VM::AudioPlayer* wp_audio_player;

			void init_playable_audio_plot();
		};
	}
}

#endif
