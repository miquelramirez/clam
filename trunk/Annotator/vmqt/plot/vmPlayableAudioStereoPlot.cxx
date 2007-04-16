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
#include "vmLocatorRenderer.hxx"
#include "vmWPlayer.hxx"
#include "vmAudioPlayer.hxx"
#include "vmIconData.hxx"
#include "vmPlayableAudioStereoPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		PlayableAudioStereoPlot::PlayableAudioStereoPlot(QWidget* parent)
			: AudioStereoPlot(parent)
		{
			InitPlayableAudioStereoPlot();
		}
			
		PlayableAudioStereoPlot::~PlayableAudioStereoPlot()
		{
		}

		void PlayableAudioStereoPlot::SetData(const Audio& chn0, const Audio& chn1, bool update)
		{
			AudioStereoPlot::SetData(chn0,chn1,update);
			if(!update)
			{
				mAudioPlayer->SetDuration(double(chn0.GetDuration()/1000.0));
			}
			std::vector<const Audio*> data(2);
			data[0]=&chn0;
			data[1]=&chn1;
			mAudioPlayer->SetData(data);
		}

		void PlayableAudioStereoPlot::backgroundWhite()
		{
			AudioStereoPlot::backgroundWhite();
			mMuteChn0->setPalette(Qt::white);
			mMuteChn1->setPalette(Qt::white);
			mComboBox->setPalette(Qt::white);
			mWPlayer->setPalette(Qt::white);
			mWPlayer->SetPixmaps(QPixmap((const char**)icon_play_black),
								 QPixmap((const char**)icon_pause_black),
								 QPixmap((const char**)icon_stop_black));
		}

		void PlayableAudioStereoPlot::backgroundBlack()
		{
			AudioStereoPlot::backgroundBlack();
			QPalette mute_palette;
			mute_palette.setBrush(QPalette::Background,Qt::white); // QTPORT  should be QPalette::Window
			mute_palette.setBrush(QPalette::Foreground,Qt::white); // QTPORT  should be QPalette::WindowText
			mute_palette.setBrush(QPalette::Button,Qt::white);
			mute_palette.setBrush(QPalette::ButtonText,Qt::white);
			mMuteChn0->setPalette(mute_palette);
			mMuteChn1->setPalette(mute_palette);
			mComboBox->setPalette(Qt::darkGreen);
			mWPlayer->setPalette(Qt::darkGreen);
			mWPlayer->SetPixmaps(QPixmap((const char**)icon_play_white),
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
					mAudioPlayer->muteChannel0(true);
					break;
				case Qt::Unchecked:
					mAudioPlayer->muteChannel0(false);
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
					mAudioPlayer->muteChannel1(true);
					break;
				case Qt::Unchecked:
					mAudioPlayer->muteChannel1(false);
					break;
				default:
					break;
			}
		}

		void PlayableAudioStereoPlot::InitPlayableAudioStereoPlot()
		{
			mAudioPlayer = new AudioPlayer;
			mWPlayer = new WPlayer(this);
			mWPlayer->SetPlayer(mAudioPlayer);

			mComboBox = new QComboBox(this);
			mComboBox->addItem("Background White");
			mComboBox->addItem("Background Black");
			mComboBox->setToolTip("Choose Color Schema");

			mMuteChn0 = new QCheckBox("mute left channel",this);
			mMuteChn1 = new QCheckBox("mute right channel",this);
			mMuteChn0->setCheckState(Qt::Unchecked);
			mMuteChn1->setCheckState(Qt::Unchecked);
			
			QFontMetrics fm(mMuteChn0->font());
			int cheight = fm.height();
			mMuteChn0->setFixedHeight(cheight);
			mMuteChn1->setFixedHeight(cheight);

			QVBoxLayout* mute_panel = new QVBoxLayout;
			mute_panel->setMargin(0);
			mute_panel->setSpacing(0);
			mute_panel->addWidget(mMuteChn0);
			mute_panel->addWidget(mMuteChn1);

			QFrame* panel_hole = new QFrame(this);
			panel_hole->setFixedSize(10,20);

			QHBoxLayout* play_panel = new QHBoxLayout;
			play_panel->setMargin(0);
			play_panel->setSpacing(0);

			play_panel->addWidget(mWPlayer);
			play_panel->addStretch(1);
			play_panel->addLayout(mute_panel);
			play_panel->addWidget(panel_hole);
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

			mLayout->addLayout(layout,4,1);
			
			connect(static_cast<Locator*>(mDisplay[MASTER]->GetRenderer("locator0")),
					SIGNAL(selectedRegion(double,double)),mAudioPlayer,SLOT(timeBounds(double,double)));
			connect(static_cast<Locator*>(mDisplay[SLAVE]->GetRenderer("locator1")),
					SIGNAL(selectedRegion(double,double)),mAudioPlayer,SLOT(timeBounds(double,double)));
			connect(mAudioPlayer,SIGNAL(playingTime(double)),
					static_cast<Locator*>(mDisplay[MASTER]->GetRenderer("locator0")),
					SLOT(updateLocator(double)));
			connect(mAudioPlayer,SIGNAL(playingTime(double)),
					static_cast<Locator*>(mDisplay[SLAVE]->GetRenderer("locator1")),
					SLOT(updateLocator(double)));
			connect(mAudioPlayer,SIGNAL(stopTime(double,bool)),
					static_cast<Locator*>(mDisplay[MASTER]->GetRenderer("locator0")),
					SLOT(updateLocator(double,bool)));
			connect(mAudioPlayer,SIGNAL(stopTime(double,bool)),
					static_cast<Locator*>(mDisplay[SLAVE]->GetRenderer("locator1")),
					SLOT(updateLocator(double,bool)));
			connect(mComboBox,SIGNAL(activated(int)),this,SLOT(setColorSchema(int)));
			connect(mMuteChn0,SIGNAL(stateChanged(int)),this,SLOT(muteChannel0(int)));
			connect(mMuteChn1,SIGNAL(stateChanged(int)),this,SLOT(muteChannel1(int)));

			backgroundWhite();
		}
	}
}

// END

