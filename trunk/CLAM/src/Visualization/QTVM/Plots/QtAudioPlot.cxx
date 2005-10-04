/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <vector>
#include <qlayout.h>
#include <qframe.h>
#include "AudioPlotController.hxx"
#include "QtAudioPlayer.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "QtAudioPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		QtAudioPlot::QtAudioPlot(QWidget* parent, const char * name, WFlags f) 
			: SingleDisplayPlot(parent,name,f)
		{
			mSlotPlayingTimeReceived.Wrap(this,&QtAudioPlot::PlayingTime);
			mSlotStopPlayingReceived.Wrap(this,&QtAudioPlot::StopPlaying);

			mPlayBounds.SetBegin(TData(-1.0));
			mPlayBounds.SetEnd(TData(-1.0));
			SetPlotController();
			InitAudioPlot();	
			Connect();
		}

		QtAudioPlot::~QtAudioPlot()
		{
		}

		void QtAudioPlot::InitAudioPlot()
		{
			QBoxLayout* panel = new QHBoxLayout;
			
			QFrame* lefthole = new QFrame(this);
			lefthole->setFixedSize(YRulerWidth(),30);
			
			mPlayer = new QtAudioPlayer(this);
			mPlayer->setFixedSize(75,30);
			((QtAudioPlayer*)mPlayer)->SetSlotPlayingTime(mSlotPlayingTimeReceived);
			((QtAudioPlayer*)mPlayer)->SetSlotStopPlaying(mSlotStopPlayingReceived);
			AddToPlayList();
			
			mLabelsGroup = new TimeSegmentLabelsGroup(this);
			mLabelsGroup->setMinimumSize(186,25);
			
			QFrame* righthole = new QFrame(this);
			righthole->setFixedSize(20,30);
			
			panel->addWidget(lefthole);
			panel->addWidget(mPlayer);
			panel->addStretch(1);
			panel->addWidget(mLabelsGroup);
			panel->addWidget(righthole);

			AddToMainLayout(panel);	
		}

		void QtAudioPlot::SetData(const Audio& audio, bool to_controller)
		{
			if(to_controller)
			{
				((AudioPlotController*)mController)->SetData(audio);
				SetPData(audio,true);
			}
			else
			{
				SetPData(audio,false);
			}
		}

		void QtAudioPlot::UpdateData(const Audio& audio)
		{
			((AudioPlotController*)mController)->UpdateData(audio);
		}

		void QtAudioPlot::SetForegroundColor(Color c)
		{
			((AudioPlotController*)mController)->SetDataColor(c);
		}

		void QtAudioPlot::SetRegionColor(Color c)
		{
			((AudioPlotController*)mController)->SetRegionColor(c);
		}

		void QtAudioPlot::updateRegion(MediaTime time)
		{
			if(	time.GetBegin()==mPlayBounds.GetBegin() &&
				time.GetEnd()==mPlayBounds.GetEnd()) return;
	
			mPlayBounds=time;
			mPlayer->stop();
			mPlayer->SetPlaySegment(time);
			mLabelsGroup->UpdateLabels(time);

			emit regionTime(time);
			emit regionTime(float(time.GetBegin()), float(time.GetEnd()));
		}

		void QtAudioPlot::SetPlotController()
		{
			SetController(new AudioPlotController());
		}

		void QtAudioPlot::Connect()
		{
			// Connections
			connect(((AudioPlotController*)mController),SIGNAL(selectedRegion(MediaTime)),this,SLOT(updateRegion(MediaTime)));
			connect(((AudioPlotController*)mController),SIGNAL(currentPlayingTime(float)),this,SIGNAL(currentPlayingTime(float)));
			connect(((AudioPlotController*)mController),SIGNAL(stopPlayingTime(float)),this,SIGNAL(stopPlayingTime(float)));
		}

		void QtAudioPlot::SetPData(const Audio& audio, bool setTime)
		{
			std::vector<const Audio*> data;
			data.resize(1);
			data[0]=&audio;
			((QtAudioPlayer*)mPlayer)->SetData(data, setTime);
		}

		void QtAudioPlot::DisplayBackgroundBlack()
		{
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Orange());
			SetBackgroundColor(VMColor::Black());
		}

		void QtAudioPlot::DisplayBackgroundWhite()
		{
			SetForegroundColor(VMColor::Blue());
			SetDialColor(VMColor::Black());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Red());
			SetBackgroundColor(VMColor::White());
		}

		void QtAudioPlot::hideEvent(QHideEvent* e)
		{
			((QtAudioPlayer*)mPlayer)->stop();
			SingleDisplayPlot::hideEvent(e);
		}

		void QtAudioPlot::closeEvent(QCloseEvent *e)
		{
			RemoveFromPlayList();
			e->accept();
		}
	
		void QtAudioPlot::PlayingTime(TData time)
		{
			((AudioPlotController*)mController)->updateTimePos(float(time));
		}

		void QtAudioPlot::StopPlaying(TData time)
		{
			((AudioPlotController*)mController)->stopPlaying(float(time));
		}

		void QtAudioPlot::setCurrentPlayingTime(float t)
		{
			((AudioPlotController*)mController)->updateTimePos(t);
		}

		void QtAudioPlot::receivedStopPlaying(float t)
		{
			((AudioPlotController*)mController)->stopPlaying(t);
		}

    }	
}

// END

