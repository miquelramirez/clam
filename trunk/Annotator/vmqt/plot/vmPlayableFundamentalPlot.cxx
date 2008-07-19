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
#include <QtGui/QPixmap>
#include "vmPlot2D.hxx"
#include "vmLocatorRenderer.hxx"
#include "vmWPlayer.hxx"
#include "vmFundFreqPlayer.hxx"
#include "vmIconData.hxx"
#include "vmPlayableFundamentalPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableFundamentalPlot::PlayableFundamentalPlot(QWidget* parent)
			: FundamentalPlot(parent)
		{
			InitPlayableFundamentalPlot();
		}
			
		PlayableFundamentalPlot::~PlayableFundamentalPlot()
		{
		}

		void PlayableFundamentalPlot::SetData(const Segment& segment, bool update)
		{
			FundamentalPlot::SetData(segment,update);
			if(!update)
			{
				mFundPlayer->SetDuration(double(segment.GetEndTime())-double(segment.GetBeginTime()));
			}
			(segment.HasAudio()) ? mPlayModeComboBox->show() : mPlayModeComboBox->hide();
			mPlayModeComboBox->setCurrentIndex(1);
			mFundPlayer->SetPlayingFlags(CLAM::VM::eUseOscillator);
			mFundPlayer->SetData(segment);
		}

		void PlayableFundamentalPlot::backgroundWhite()
		{
			FundamentalPlot::backgroundWhite();
			mSchemaComboBox->setPalette(Qt::white);
			mPlayModeComboBox->setPalette(Qt::white);
			mWPlayer->setPalette(Qt::white);
			mWPlayer->SetPixmaps(QPixmap((const char**)icon_play_black),
								 QPixmap((const char**)icon_pause_black),
								 QPixmap((const char**)icon_stop_black));
		}

		void PlayableFundamentalPlot::backgroundBlack()
		{
			FundamentalPlot::backgroundBlack();
			mSchemaComboBox->setPalette(Qt::darkGreen);
			mPlayModeComboBox->setPalette(Qt::darkGreen);
			mWPlayer->setPalette(Qt::darkGreen);
			mWPlayer->SetPixmaps(QPixmap((const char**)icon_play_white),
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
					mFundPlayer->SetPlayingFlags(CLAM::VM::eAudio);
					break;
				case 1:
					mFundPlayer->SetPlayingFlags(CLAM::VM::eUseOscillator);
					break;
				case 2:
					mFundPlayer->SetPlayingFlags(CLAM::VM::eAudio | CLAM::VM::eUseOscillator);
					break;
				default:
					break;
			}
		}

		void PlayableFundamentalPlot::InitPlayableFundamentalPlot()
		{
			mFundPlayer = new FundFreqPlayer;
			mWPlayer = new WPlayer(this);
			mWPlayer->SetPlayer(mFundPlayer);

			mPlayModeComboBox = new QComboBox(this);
			mPlayModeComboBox->addItem("Audio");
			mPlayModeComboBox->addItem("Fundamental");
			mPlayModeComboBox->addItem("Audio+Fundamental");
			mPlayModeComboBox->setToolTip("Choose Playing Mode");
			mPlayModeComboBox->setCurrentIndex(1);
			mPlayModeComboBox->hide();

			mSchemaComboBox = new QComboBox(this);
			mSchemaComboBox->addItem("Background White");
			mSchemaComboBox->addItem("Background Black");
			mSchemaComboBox->setToolTip("Choose Color Schema");

			QFrame* play_panel_hole = new QFrame(this);
			play_panel_hole->setFixedSize(10,20);

			QHBoxLayout* play_panel = new QHBoxLayout;
			play_panel->setMargin(0);
			play_panel->setSpacing(0);

			play_panel->addWidget(mWPlayer);
			play_panel->addStretch(1);
			play_panel->addWidget(mPlayModeComboBox);
			play_panel->addWidget(play_panel_hole);
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
			
			connect(_locator,
					SIGNAL(selectedRegion(double,double)),mFundPlayer,SLOT(timeBounds(double,double)));
			connect(mFundPlayer,SIGNAL(playingTime(double)),
					_locator,SLOT(updateLocator(double)));
			connect(mFundPlayer,SIGNAL(stopTime(double,bool)),
					_locator,SLOT(updateLocator(double,bool)));
			connect(mSchemaComboBox,SIGNAL(activated(int)),this,SLOT(setColorSchema(int)));
			connect(mPlayModeComboBox,SIGNAL(activated(int)),this,SLOT(setPlayingFlags(int)));

			backgroundWhite();
		}
	}
}

// END

