#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QComboBox>
#include <QPixmap>
#include "vm_plot2d.hxx"
#include "vm_locator_renderer.hxx"
#include "vm_wplayer.hxx"
#include "vm_audio_player.hxx"
#include "vm_icon_data.hxx"
#include "vm_playable_audio_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableAudioPlot::PlayableAudioPlot(QWidget* parent)
			: AudioPlot(parent)
		{
			init_playable_audio_plot();
		}
			
		PlayableAudioPlot::~PlayableAudioPlot()
		{
		}

		void PlayableAudioPlot::set_data(const Audio& audio, bool update)
		{
			AudioPlot::set_data(audio,update);
			if(!update)
			{
				wp_audio_player->set_duration(double(audio.GetDuration()/1000.0));
			}
			std::vector<const Audio*> data(1);
			data[0]=&audio;
			wp_audio_player->set_data(data);
		}

		void PlayableAudioPlot::backgroundWhite()
		{
			AudioPlot::backgroundWhite();
			wp_combo_box->setPalette(Qt::white);
			wp_wplayer->setPalette(Qt::white);
			wp_wplayer->set_pixmaps(QPixmap((const char**)icon_play_black),
									QPixmap((const char**)icon_pause_black),
									QPixmap((const char**)icon_stop_black));
		}

		void PlayableAudioPlot::backgroundBlack()
		{
			AudioPlot::backgroundBlack();
			wp_combo_box->setPalette(Qt::darkGreen);
			wp_wplayer->setPalette(Qt::darkGreen);
			wp_wplayer->set_pixmaps(QPixmap((const char**)icon_play_white),
									QPixmap((const char**)icon_pause_white),
									QPixmap((const char**)icon_stop_white));
		}

		void PlayableAudioPlot::setColorSchema(int index)
		{
			(index == 0) ? backgroundWhite() : backgroundBlack();
		}

		void PlayableAudioPlot::init_playable_audio_plot()
		{
			wp_audio_player = new AudioPlayer;
			wp_wplayer = new WPlayer(this);
			wp_wplayer->set_player(wp_audio_player);

			wp_combo_box = new QComboBox(this);
			wp_combo_box->addItem("Background White");
			wp_combo_box->addItem("Background Black");
			wp_combo_box->setToolTip("Choose Color Schema");

			QHBoxLayout* play_panel = new QHBoxLayout;
			play_panel->setMargin(0);
			play_panel->setSpacing(0);

			play_panel->addWidget(wp_wplayer);
			play_panel->addStretch(1);
			play_panel->addWidget(wp_combo_box);

			QFrame* top_hole = new QFrame(this);
			QFrame* bottom_hole = new QFrame(this);
			top_hole->setFixedHeight(5);
			bottom_hole->setFixedHeight(5);

			QVBoxLayout* layout = new QVBoxLayout;
			layout->setMargin(0);
			layout->setSpacing(0);
			layout->addWidget(top_hole);
			layout->addLayout(play_panel);
			layout->addWidget(bottom_hole);

			wp_layout->addLayout(layout,3,1);
			
			connect(static_cast<Locator*>(wp_plot->get_renderer("locator")),
					SIGNAL(selectedRegion(double,double)),wp_audio_player,SLOT(timeBounds(double,double)));
			connect(wp_audio_player,SIGNAL(playingTime(double)),
					static_cast<Locator*>(wp_plot->get_renderer("locator")),SLOT(updateLocator(double)));
			connect(wp_audio_player,SIGNAL(stopTime(double,bool)),
					static_cast<Locator*>(wp_plot->get_renderer("locator")),SLOT(updateLocator(double,bool)));
			connect(wp_combo_box,SIGNAL(activated(int)),this,SLOT(setColorSchema(int)));

			backgroundWhite();
		}
	}
}

// END
