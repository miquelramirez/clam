/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QFrame>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <QtGui/QPixmap>
#include "vmPlot2D.hxx"
#include "vmGrid.hxx"
#include "vmBPFEditor.hxx"
#include "vmLocatorRenderer.hxx"
#include "vmWPlayer.hxx"
#include "vmBPFPlayer.hxx"
#include "vmIconData.hxx"
#include "vmPlayableMultiBPFPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableMultiBPFPlot::PlayableMultiBPFPlot(QWidget* parent)
			: MultiBPFPlot(parent)
		{
			InitPlayableMultiBPFPlot();
		}
			
		PlayableMultiBPFPlot::~PlayableMultiBPFPlot()
		{
		}

		void PlayableMultiBPFPlot::AddBPF(const QString& key, BPF* bpf)
		{
			MultiBPFPlot::AddBPF(key,bpf);
			if(mCurrentBPFComboBox->findText(key) == -1)
			{
				mCurrentBPFComboBox->addItem(key);
				if(mCurrentBPFComboBox->count() > 1) mCurrentBPFComboBox->show();
			}
			setCurrentBPF(mCurrentBPFComboBox->currentText());
		}

		void PlayableMultiBPFPlot::SetXRange(double xmin, double xmax, ERulerScale scale)
		{
			MultiBPFPlot::SetXRange(xmin,xmax,scale);
			mBPFPlayer->SetDuration(xmax);
		}

		void PlayableMultiBPFPlot::SetYRange(double ymin, double ymax, ERulerScale scale)
		{
			MultiBPFPlot::SetYRange(ymin,ymax,scale);
			mBPFPlayer->SetPitchBounds(ymin,ymax);
		}

		void PlayableMultiBPFPlot::backgroundWhite()
		{
			MultiBPFPlot::backgroundWhite();
			mShowGrid->setPalette(Qt::white);
			mSnapGrid->setPalette(Qt::white);
			mSchemaComboBox->setPalette(Qt::white);
			mCurrentBPFComboBox->setPalette(Qt::white);
			mWPlayer->setPalette(Qt::white);
			mWPlayer->SetPixmaps(QPixmap((const char**)icon_play_black),
								 QPixmap((const char**)icon_pause_black),
								 QPixmap((const char**)icon_stop_black));
		}

		void PlayableMultiBPFPlot::backgroundBlack()
		{
			MultiBPFPlot::backgroundBlack();
			QPalette cb_palette;
			cb_palette.setBrush(QPalette::Background,Qt::white);  //QTPORT QPalette::Window
			cb_palette.setBrush(QPalette::Foreground,Qt::white); //QTPORT QPalette::WindowText
			cb_palette.setBrush(QPalette::Button,Qt::white);
			cb_palette.setBrush(QPalette::ButtonText,Qt::white);
			mShowGrid->setPalette(cb_palette);
			mSnapGrid->setPalette(cb_palette);
			mSchemaComboBox->setPalette(Qt::darkGreen);
			mCurrentBPFComboBox->setPalette(Qt::darkGreen);
			mWPlayer->setPalette(Qt::darkGreen);
			mWPlayer->SetPixmaps(QPixmap((const char**)icon_play_white),
								 QPixmap((const char**)icon_pause_white),
								 QPixmap((const char**)icon_stop_white));
		}

		void PlayableMultiBPFPlot::readOnly()
		{
			MultiBPFPlot::readOnly();
			mSnapGrid->hide();
		}

		void PlayableMultiBPFPlot::setCurrentBPF(QString key)
		{
			MultiBPFPlot::setCurrentBPF(key);
			mBPFPlayer->SetData(static_cast<BPFEditor*>(mPlot->GetRenderer(key))->GetData());
		}

		void PlayableMultiBPFPlot::set_color_schema(int index)
		{
			(index == 0) ? backgroundWhite() : backgroundBlack();
		}
	
		void PlayableMultiBPFPlot::show_grid(int state)
		{
			switch(state)
			{
				case Qt::Checked:
					showGrid(true);
					mSnapGrid->setEnabled(true);
					break;
				case Qt::Unchecked:
					showGrid(false);
					snapToGrid(Qt::Unchecked);
					mSnapGrid->setCheckState(Qt::Unchecked);
					mSnapGrid->setEnabled(false);
					break;
				default:
					break;
			}
		}

		void PlayableMultiBPFPlot::snap_to_grid(int state)
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

		void PlayableMultiBPFPlot::InitPlayableMultiBPFPlot()
		{
			mBPFPlayer = new BPFPlayer;
			mWPlayer = new WPlayer(this);
			mWPlayer->SetPlayer(mBPFPlayer);

			mCurrentBPFComboBox = new QComboBox(this);
			mCurrentBPFComboBox->setToolTip("Choose Current BPF");
			mCurrentBPFComboBox->hide();

			mSchemaComboBox = new QComboBox(this);
			mSchemaComboBox->addItem("Background White");
			mSchemaComboBox->addItem("Background Black");
			mSchemaComboBox->setToolTip("Choose Color Schema");

			mShowGrid = new QCheckBox("show grid",this);
			mSnapGrid = new QCheckBox("snap to grid",this);
			mShowGrid->setCheckState(Qt::Unchecked);
			mSnapGrid->setCheckState(Qt::Unchecked);
			mSnapGrid->setEnabled(false);
			
			QFontMetrics fm(mShowGrid->font());
			int cheight = fm.height();
			mShowGrid->setFixedHeight(cheight);
			mSnapGrid->setFixedHeight(cheight);

			QVBoxLayout* grid_settings_panel = new QVBoxLayout;
			grid_settings_panel->setMargin(0);
			grid_settings_panel->setSpacing(0);
			grid_settings_panel->addWidget(mShowGrid);
			grid_settings_panel->addWidget(mSnapGrid);

			QFrame* panel_hole0 = new QFrame(this);
			QFrame* panel_hole1 = new QFrame(this);
			panel_hole0->setFixedSize(10,20);
			panel_hole1->setFixedSize(10,20);

			QHBoxLayout* play_panel = new QHBoxLayout;
			play_panel->setMargin(0);
			play_panel->setSpacing(0);

			play_panel->addWidget(mWPlayer);
			play_panel->addStretch(1);
			play_panel->addLayout(grid_settings_panel);
			play_panel->addWidget(panel_hole0);
			play_panel->addWidget(mCurrentBPFComboBox);
			play_panel->addWidget(panel_hole1);
			play_panel->addWidget(mSchemaComboBox);

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

			mLayout->addLayout(layout,3,1);
			
			connect(static_cast<Locator*>(mPlot->GetRenderer("locator")),
					SIGNAL(selectedRegion(double,double)),mBPFPlayer,SLOT(timeBounds(double,double)));
			connect(mBPFPlayer,SIGNAL(playingTime(double)),
					static_cast<Locator*>(mPlot->GetRenderer("locator")),SLOT(updateLocator(double)));
			connect(mBPFPlayer,SIGNAL(stopTime(double,bool)),
					static_cast<Locator*>(mPlot->GetRenderer("locator")),SLOT(updateLocator(double,bool)));
			connect(mSchemaComboBox,SIGNAL(activated(int)),this,SLOT(set_color_schema(int)));
			connect(mCurrentBPFComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(setCurrentBPF(QString)));
			connect(mShowGrid,SIGNAL(stateChanged(int)),this,SLOT(show_grid(int)));
			connect(mSnapGrid,SIGNAL(stateChanged(int)),this,SLOT(snap_to_grid(int)));

			backgroundWhite();
		}
	}
}

// END

