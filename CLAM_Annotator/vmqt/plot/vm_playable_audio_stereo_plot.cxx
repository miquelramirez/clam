#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QComboBox>
#include <QCheckBox>
#include <QPixmap>
#include "vm_plot2d.hxx"
#include "vm_locator_renderer.hxx"
#include "vm_wplayer.hxx"
#include "vm_audio_player.hxx"
#include "vm_icon_data.hxx"
#include "vm_playable_audio_stereo_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableAudioStereoPlot::PlayableAudioStereoPlot(QWidget* parent)
			: AudioStereoPlot(parent)
		{
			init_playable_audio_stereo_plot();
		}
			
		PlayableAudioStereoPlot::~PlayableAudioStereoPlot()
		{
		}

		void PlayableAudioStereoPlot::set_data(const CLAM::Audio& chn0, const CLAM::Audio& chn1, bool update)
		{
			AudioStereoPlot::set_data(chn0,chn1,update);
			if(!update)
			{
				wp_audio_player->set_duration(double(chn0.GetDuration()/1000.0));
			}
			std::vector<const Audio*> data(2);
			data[0]=&chn0;
			data[1]=&chn1;
			wp_audio_player->set_data(data);
		}

		void PlayableAudioStereoPlot::backgroundWhite()
		{
			AudioStereoPlot::backgroundWhite();
			wp_mute_chn0->setPalette(Qt::white);
			wp_mute_chn1->setPalette(Qt::white);
			wp_combo_box->setPalette(Qt::white);
			wp_wplayer->setPalette(Qt::white);
			wp_wplayer->set_pixmaps(QPixmap((const char**)icon_play_black),
									QPixmap((const char**)icon_pause_black),
									QPixmap((const char**)icon_stop_black));
		}

		void PlayableAudioStereoPlot::backgroundBlack()
		{
			AudioStereoPlot::backgroundBlack();
			QPalette mute_palette;
			mute_palette.setBrush(QPalette::Window,Qt::white);
			mute_palette.setBrush(QPalette::WindowText,Qt::white);
			mute_palette.setBrush(QPalette::Button,Qt::white);
			mute_palette.setBrush(QPalette::ButtonText,Qt::white);
			wp_mute_chn0->setPalette(mute_palette);
			wp_mute_chn1->setPalette(mute_palette);
			wp_combo_box->setPalette(Qt::darkGreen);
			wp_wplayer->setPalette(Qt::darkGreen);
			wp_wplayer->set_pixmaps(QPixmap((const char**)icon_play_white),
									QPixmap((const char**)icon_pause_white),
									QPixmap((const char**)icon_stop_white));
		}

		void PlayableAudioStereoPlot::setColorSchema(int index)
		{
			(index == 0) ? backgroundWhite() : backgroundBlack();
		}

		void PlayableAudioStereoPlot::muteChannel0(int state)
		{
			switch(state)
			{
				case Qt::Checked:
					wp_audio_player->muteChannel0(true);
					break;
				case Qt::Unchecked:
					wp_audio_player->muteChannel0(false);
					break;
				default:
					break;
			}
		}
		
		void PlayableAudioStereoPlot::muteChannel1(int state)
		{
			switch(state)
			{
				case Qt::Checked:
					wp_audio_player->muteChannel1(true);
					break;
				case Qt::Unchecked:
					wp_audio_player->muteChannel1(false);
					break;
				default:
					break;
			}
		}

		void PlayableAudioStereoPlot::init_playable_audio_stereo_plot()
		{
			wp_audio_player = new CLAM::VM::AudioPlayer;
			wp_wplayer = new CLAM::VM::WPlayer(this);
			wp_wplayer->set_player(wp_audio_player);

			wp_combo_box = new QComboBox(this);
			wp_combo_box->addItem("Background White");
			wp_combo_box->addItem("Background Black");
			wp_combo_box->setToolTip("Choose Color Schema");

			wp_mute_chn0 = new QCheckBox("mute left channel",this);
			wp_mute_chn1 = new QCheckBox("mute right channel",this);
			wp_mute_chn0->setCheckState(Qt::Unchecked);
			wp_mute_chn1->setCheckState(Qt::Unchecked);
			
			QFontMetrics fm(wp_mute_chn0->font());
			int cheight = fm.height();
			wp_mute_chn0->setFixedHeight(cheight);
			wp_mute_chn1->setFixedHeight(cheight);

			QVBoxLayout* mute_panel = new QVBoxLayout;
			mute_panel->setMargin(0);
			mute_panel->setSpacing(0);
			mute_panel->addWidget(wp_mute_chn0);
			mute_panel->addWidget(wp_mute_chn1);

			QFrame* panel_hole = new QFrame(this);
			panel_hole->setFixedSize(10,20);

			QHBoxLayout* play_panel = new QHBoxLayout;
			play_panel->setMargin(0);
			play_panel->setSpacing(0);

			play_panel->addWidget(wp_wplayer);
			play_panel->addStretch(1);
			play_panel->addLayout(mute_panel);
			play_panel->addWidget(panel_hole);
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

			wp_layout->addLayout(layout,4,1);
			
			connect(static_cast<CLAM::VM::Locator*>(wp_display[MASTER]->get_renderer("locator0")),
					SIGNAL(selectedRegion(double,double)),wp_audio_player,SLOT(timeBounds(double,double)));
			connect(static_cast<CLAM::VM::Locator*>(wp_display[SLAVE]->get_renderer("locator1")),
					SIGNAL(selectedRegion(double,double)),wp_audio_player,SLOT(timeBounds(double,double)));
			connect(wp_audio_player,SIGNAL(playingTime(double)),
					static_cast<CLAM::VM::Locator*>(wp_display[MASTER]->get_renderer("locator0")),
					SLOT(updateLocator(double)));
			connect(wp_audio_player,SIGNAL(playingTime(double)),
					static_cast<CLAM::VM::Locator*>(wp_display[SLAVE]->get_renderer("locator1")),
					SLOT(updateLocator(double)));
			connect(wp_audio_player,SIGNAL(stopTime(double,bool)),
					static_cast<CLAM::VM::Locator*>(wp_display[MASTER]->get_renderer("locator0")),
					SLOT(updateLocator(double,bool)));
			connect(wp_audio_player,SIGNAL(stopTime(double,bool)),
					static_cast<CLAM::VM::Locator*>(wp_display[SLAVE]->get_renderer("locator1")),
					SLOT(updateLocator(double,bool)));
			connect(wp_combo_box,SIGNAL(activated(int)),this,SLOT(setColorSchema(int)));
			connect(wp_mute_chn0,SIGNAL(stateChanged(int)),this,SLOT(muteChannel0(int)));
			connect(wp_mute_chn1,SIGNAL(stateChanged(int)),this,SLOT(muteChannel1(int)));

			backgroundWhite();
		}
	}
}

// END
