#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QComboBox>
#include <QCheckBox>
#include <QPixmap>
#include "vm_plot2d.hxx"
#include "vm_grid.hxx"
#include "vm_locator_renderer.hxx"
#include "vm_wplayer.hxx"
#include "vm_bpf_player.hxx"
#include "vm_icon_data.hxx"
#include "vm_playable_bpf_plot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableBPFPlot::PlayableBPFPlot(QWidget* parent)
			: BPFPlot(parent)
		{
			init_playable_bpf_plot();
		}
			
		PlayableBPFPlot::~PlayableBPFPlot()
		{
		}

		void PlayableBPFPlot::set_data(CLAM::BPF* bpf)
		{
			BPFPlot::set_data(bpf);
			wp_bpf_player->set_data(*bpf);
		}

		void PlayableBPFPlot::set_xrange(double xmin, double xmax, CLAM::VM::EScale scale)
		{
			BPFPlot::set_xrange(xmin,xmax,scale);
			wp_bpf_player->set_duration(xmax);
		}

		void PlayableBPFPlot::set_yrange(double ymin, double ymax, CLAM::VM::EScale scale)
		{
			BPFPlot::set_yrange(ymin,ymax,scale);
			wp_bpf_player->set_pitch_bounds(ymin,ymax);
		}

		void PlayableBPFPlot::backgroundWhite()
		{
			BPFPlot::backgroundWhite();
			wp_show_grid->setPalette(Qt::white);
			wp_snap_grid->setPalette(Qt::white);
			wp_combo_box->setPalette(Qt::white);
			wp_wplayer->setPalette(Qt::white);
			wp_wplayer->set_pixmaps(QPixmap((const char**)icon_play_black),
									QPixmap((const char**)icon_pause_black),
									QPixmap((const char**)icon_stop_black));
		}

		void PlayableBPFPlot::backgroundBlack()
		{
			BPFPlot::backgroundBlack();
			QPalette cb_palette;
			cb_palette.setBrush(QPalette::Window,Qt::white);
			cb_palette.setBrush(QPalette::WindowText,Qt::white);
			cb_palette.setBrush(QPalette::Button,Qt::white);
			cb_palette.setBrush(QPalette::ButtonText,Qt::white);
			wp_show_grid->setPalette(cb_palette);
			wp_snap_grid->setPalette(cb_palette);
			wp_combo_box->setPalette(Qt::darkGreen);
			wp_wplayer->setPalette(Qt::darkGreen);
			wp_wplayer->set_pixmaps(QPixmap((const char**)icon_play_white),
									QPixmap((const char**)icon_pause_white),
									QPixmap((const char**)icon_stop_white));
		}

		void PlayableBPFPlot::readOnly()
		{
			BPFPlot::readOnly();
			wp_snap_grid->hide();
		}

		void PlayableBPFPlot::set_color_schema(int index)
		{
			(index == 0) ? backgroundWhite() : backgroundBlack();
		}
		
		void PlayableBPFPlot::show_grid(int state)
		{
			switch(state)
			{
				case Qt::Checked:
					showGrid(true);
					wp_snap_grid->setEnabled(true);
					break;
				case Qt::Unchecked:
					showGrid(false);
					snapToGrid(Qt::Unchecked);
					wp_snap_grid->setCheckState(Qt::Unchecked);
					wp_snap_grid->setEnabled(false);
					break;
				default:
					break;
			}
		}

		void PlayableBPFPlot::snap_to_grid(int state)
		{
			switch(state)
			{
				case Qt::Checked:
					snapToGrid(true);
					break;
				case Qt::Unchecked:
					snapToGrid(false);
					break;
				default:
					break;
			}
		}

		void PlayableBPFPlot::init_playable_bpf_plot()
		{
			wp_bpf_player = new CLAM::VM::BPFPlayer;
			wp_wplayer = new CLAM::VM::WPlayer(this);
			wp_wplayer->set_player(wp_bpf_player);

			wp_combo_box = new QComboBox(this);
			wp_combo_box->addItem("Background White");
			wp_combo_box->addItem("Background Black");
			wp_combo_box->setToolTip("Choose Color Schema");

			wp_show_grid = new QCheckBox("show grid",this);
			wp_snap_grid = new QCheckBox("snap to grid",this);
			wp_show_grid->setCheckState(Qt::Unchecked);
			wp_snap_grid->setCheckState(Qt::Unchecked);
			wp_snap_grid->setEnabled(false);
			
			QFontMetrics fm(wp_show_grid->font());
			int cheight = fm.height();
			wp_show_grid->setFixedHeight(cheight);
			wp_snap_grid->setFixedHeight(cheight);

			QVBoxLayout* grid_settings_panel = new QVBoxLayout;
			grid_settings_panel->setMargin(0);
			grid_settings_panel->setSpacing(0);
			grid_settings_panel->addWidget(wp_show_grid);
			grid_settings_panel->addWidget(wp_snap_grid);

			QFrame* panel_hole = new QFrame(this);
			panel_hole->setFixedSize(10,20);

			QHBoxLayout* play_panel = new QHBoxLayout;
			play_panel->setMargin(0);
			play_panel->setSpacing(0);

			play_panel->addWidget(wp_wplayer);
			play_panel->addStretch(1);
			play_panel->addLayout(grid_settings_panel);
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

			wp_layout->addLayout(layout,3,1);
			
			connect(static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator")),
					SIGNAL(selectedRegion(double,double)),wp_bpf_player,SLOT(timeBounds(double,double)));
			connect(wp_bpf_player,SIGNAL(playingTime(double)),
					static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator")),SLOT(updateLocator(double)));
			connect(wp_bpf_player,SIGNAL(stopTime(double,bool)),
					static_cast<CLAM::VM::Locator*>(wp_plot->get_renderer("locator")),SLOT(updateLocator(double,bool)));
			connect(wp_combo_box,SIGNAL(activated(int)),this,SLOT(set_color_schema(int)));
			connect(wp_show_grid,SIGNAL(stateChanged(int)),this,SLOT(show_grid(int)));
			connect(wp_snap_grid,SIGNAL(stateChanged(int)),this,SLOT(snap_to_grid(int)));

			backgroundWhite();
		}
	}
}

// END
