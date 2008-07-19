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
#include "vmAudioPlayer.hxx"
#include "vmIconData.hxx"
#include "vmPlayableAudioPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableAudioPlot::PlayableAudioPlot(QWidget* parent)
			: AudioPlot(parent)
		{
			InitPlayableAudioPlot();
		}
			
		PlayableAudioPlot::~PlayableAudioPlot()
		{
		}

		void PlayableAudioPlot::SetData(const Audio& audio, bool update)
		{
			AudioPlot::SetData(audio,update);
			if(!update)
			{
				mAudioPlayer->SetDuration(double(audio.GetDuration()/1000.0));
			}
			std::vector<const Audio*> data(1);
			data[0]=&audio;
			mAudioPlayer->SetData(data);
		}

		void PlayableAudioPlot::backgroundWhite()
		{
			AudioPlot::backgroundWhite();
			mComboBox->setPalette(Qt::white);
			mWPlayer->setPalette(Qt::white);
			mWPlayer->SetPixmaps(QPixmap((const char**)icon_play_black),
								 QPixmap((const char**)icon_pause_black),
								 QPixmap((const char**)icon_stop_black));
		}

		void PlayableAudioPlot::backgroundBlack()
		{
			AudioPlot::backgroundBlack();
			mComboBox->setPalette(Qt::darkGreen);
			mWPlayer->setPalette(Qt::darkGreen);
			mWPlayer->SetPixmaps(QPixmap((const char**)icon_play_white),
								 QPixmap((const char**)icon_pause_white),
								 QPixmap((const char**)icon_stop_white));
		}

		void PlayableAudioPlot::setColorSchema(int index)
		{
			(index == 0) ? backgroundWhite() : backgroundBlack();
		}

		void PlayableAudioPlot::InitPlayableAudioPlot()
		{
			mAudioPlayer = new AudioPlayer;
			mWPlayer = new WPlayer(this);
			mWPlayer->SetPlayer(mAudioPlayer);

			mComboBox = new QComboBox(this);
			mComboBox->addItem("Background White");
			mComboBox->addItem("Background Black");
			mComboBox->setToolTip("Choose Color Schema");

			QHBoxLayout* play_panel = new QHBoxLayout;
			play_panel->setMargin(0);
			play_panel->setSpacing(0);

			play_panel->addWidget(mWPlayer);
			play_panel->addStretch(1);
			play_panel->addWidget(mComboBox);

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
					SIGNAL(selectedRegion(double,double)),mAudioPlayer,SLOT(timeBounds(double,double)));
			connect(mAudioPlayer,SIGNAL(playingTime(double)),
					_locator,SLOT(updateLocator(double)));
			connect(mAudioPlayer,SIGNAL(stopTime(double,bool)),
					_locator,SLOT(updateLocator(double,bool)));
			connect(mComboBox,SIGNAL(activated(int)),this,SLOT(setColorSchema(int)));

			backgroundWhite();
		}
	}
}

// END

