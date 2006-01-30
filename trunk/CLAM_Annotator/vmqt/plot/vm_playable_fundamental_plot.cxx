#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QComboBox>
#include <QPixmap>
#include "vm_plot2d.hxx"
#include "vm_locator_renderer.hxx"
#include "vm_wplayer.hxx"
#include "vm_fund_freq_player.hxx"
#include "vm_icon_data.hxx"
#include "vm_playable_fundamental_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableFundamentalPlot::PlayableFundamentalPlot(QWidget* parent)
			: FundamentalPlot(parent)
		{
			init_playable_fundamental_plot();
		}
			
		PlayableFundamentalPlot::~PlayableFundamentalPlot()
		{
		}

		void PlayableFundamentalPlot::set_data(const CLAM::Segment& segment, bool update)
		{
			FundamentalPlot::set_data(segment,update);
			if(!update)
			{
				wp_fund_player->set_duration(double(segment.GetEndTime())-double(segment.GetBeginTime()));
			}
			(segment.HasAudio()) ? wp_play_mode_combo_box->show() : wp_play_mode_combo_box->hide();
			wp_play_mode_combo_box->setCurrentIndex(1);
			wp_fund_player->set_playing_flags(CLAM::VM::eUseOscillator);
			wp_fund_player->set_data(segment);
		}

		void PlayableFundamentalPlot::backgroundWhite()
		{
			FundamentalPlot::backgroundWhite();
			wp_schema_combo_box->setPalette(Qt::white);
			wp_play_mode_combo_box->setPalette(Qt::white);
			wp_wplayer->setPalette(Qt::white);
			wp_wplayer->set_pixmaps(QPixmap((const char**)icon_play_black),
									QPixmap((const char**)icon_pause_black),
									QPixmap((const char**)icon_stop_black));
		}

		void PlayableFundamentalPlot::backgroundBlack()
		{
			FundamentalPlot::backgroundBlack();
			wp_schema_combo_box->setPalette(Qt::darkGreen);
			wp_play_mode_combo_box->setPalette(Qt::darkGreen);
			wp_wplayer->setPalette(Qt::darkGreen);
			wp_wplayer->set_pixmaps(QPixmap((const char**)icon_play_white),
									QPixmap((const char**)icon_pause_white),
									QPixmap((const char**)icon_stop_white));
		}

		void PlayableFundamentalPlot::setColorSchema(int index)
		{
			(index == 0) ? backgroundWhite() : backgroundBlack();
		}

		void PlayableFundamentalPlot::setPlayingFlags(int index)
		{
			switch(index)
			{
				case 0:
					wp_fund_player->set_playing_flags(CLAM::VM::eAudio);
					break;
				case 1:
					wp_fund_player->set_playing_flags(CLAM::VM::eUseOscillator);
					break;
				case 2:
					wp_fund_player->set_playing_flags(CLAM::VM::eAudio | CLAM::VM::eUseOscillator);
					break;
				default:
					break;
			}
		}

		void PlayableFundamentalPlot::init_playable_fundamental_plot()
		{
			wp_fund_player = new CLAM::VM::FundFreqPlayer;
			wp_wplayer = new CLAM::VM::WPlayer(this);
			wp_wplayer->set_player(wp_fund_player);

			wp_play_mode_combo_box = new QComboBox(this);
			wp_play_mode_combo_box->addItem("Audio");
			wp_play_mode_combo_box->addItem("Fundamental");
			wp_play_mode_combo_box->addItem("Audio+Fundamental");
			wp_play_mode_combo_box->setToolTip("Choose Playing Mode");
			wp_play_mode_combo_box->setCurrentIndex(1);
			wp_play_mode_combo_box->hide();

			wp_schema_combo_box = new QComboBox(this);
			wp_schema_combo_box->addItem("Background White");
			wp_schema_combo_box->addItem("Background Black");
			wp_schema_combo_box->setToolTip("Choose Color Schema");

			QFrame* play_panel_hole = new QFrame(this);
			play_panel_hole->setFixedSize(10,20);

			QHBoxLayout* play_panel = new QHBoxLayout;
			play_panel->setMargin(0);
			play_panel->setSpacing(0);

			play_panel->addWidget(wp_wplayer);
			play_panel->addStretch(1);
			play_panel->addWidget(wp_play_mode_combo_box);
			play_panel->addWidget(play_panel_hole);
			play_panel->addWidget(wp_schema_combo_box);

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
			
			connect(static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator")),
					SIGNAL(selectedRegion(double,double)),wp_fund_player,SLOT(timeBounds(double,double)));
			connect(wp_fund_player,SIGNAL(playingTime(double)),
					static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator")),SLOT(updateLocator(double)));
			connect(wp_fund_player,SIGNAL(stopTime(double,bool)),
					static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator")),SLOT(updateLocator(double,bool)));
			connect(wp_schema_combo_box,SIGNAL(activated(int)),this,SLOT(setColorSchema(int)));
			connect(wp_play_mode_combo_box,SIGNAL(activated(int)),this,SLOT(setPlayingFlags(int)));

			backgroundWhite();
		}
	}
}

// END
