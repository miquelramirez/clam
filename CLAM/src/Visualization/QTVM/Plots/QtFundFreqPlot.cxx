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

#include <qlayout.h>
#include <qframe.h>
#include "FundPlotController.hxx"
#include "QtFundPlayer.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "QtFundFreqPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		QtFundFreqPlot::QtFundFreqPlot(QWidget* parent, const char * name, WFlags f) 
			: SingleDisplayPlot(parent,name,f)
		{
			mSlotPlayingTimeReceived.Wrap(this,&QtFundFreqPlot::PlayingTime);
			mSlotStopPlayingReceived.Wrap(this,&QtFundFreqPlot::StopPlaying);

			mPlayBounds.SetBegin(TData(-1.0));
			mPlayBounds.SetEnd(TData(-1.0));

			SetPlotController();
			InitFundFreqPlot();
			Connect();
		}

		QtFundFreqPlot::~QtFundFreqPlot()
		{
		}

		void QtFundFreqPlot::InitFundFreqPlot()
		{
			QHBoxLayout* panel = new QHBoxLayout;
			
			lefthole = new QFrame(this);
			lefthole->setFixedSize(YRulerWidth(),30);
			panel->addWidget(lefthole);

			mPlayer = new QtFundPlayer(this);
			((QtFundPlayer*)mPlayer)->SetSlotPlayingTime(mSlotPlayingTimeReceived);
			((QtFundPlayer*)mPlayer)->SetSlotStopPlaying(mSlotStopPlayingReceived);
			AddToPlayList();
			mPlayer->setFixedSize(75,30);
			panel->addWidget(mPlayer);

			panel->addStretch(1);

			mLabelsGroup = new TimeSegmentLabelsGroup(this);
			mLabelsGroup->setMinimumSize(186,25);
			panel->addWidget(mLabelsGroup);

			QFrame* righthole = new QFrame(this);
			righthole->setFixedSize(20,30);
			panel->addWidget(righthole);

			AddToMainLayout(panel);
		}

		void QtFundFreqPlot::SetData(const Segment& segment)
		{
			((FundPlotController*)mController)->SetData(segment);
			SetPData(segment);
		}

		void QtFundFreqPlot::SetForegroundColor(Color c)
		{
			((FundPlotController*)mController)->SetDataColor(c);
		}

		void QtFundFreqPlot::SetRegionColor(Color c)
		{
			((FundPlotController*)mController)->SetRegionColor(c);
		}

		void QtFundFreqPlot::updateRegion(MediaTime time)
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

		void QtFundFreqPlot::SetPlotController()
		{
			SetController(new FundPlotController());
		}

		void QtFundFreqPlot::Connect()
		{
			// Connections
			connect(((FundPlotController*)mController),SIGNAL(selectedRegion(MediaTime)),this,SLOT(updateRegion(MediaTime)));
			connect(((FundPlotController*)mController),SIGNAL(currentPlayingTime(float)),this,SIGNAL(currentPlayingTime(float)));
			connect(((FundPlotController*)mController),SIGNAL(stopPlayingTime(float)),this,SIGNAL(stopPlayingTime(float)));
		}

		void QtFundFreqPlot::SetPData(const Segment& seg)
		{
			((QtFundPlayer*)mPlayer)->SetData(seg);
		}

		void QtFundFreqPlot::DisplayBackgroundBlack()
		{
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Orange());
			SetBackgroundColor(VMColor::Black());
		}

		void QtFundFreqPlot::DisplayBackgroundWhite()
		{
			SetForegroundColor(VMColor::Blue());
			SetDialColor(VMColor::Black());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Red());
			SetBackgroundColor(VMColor::White());
		}

		void QtFundFreqPlot::hideEvent(QHideEvent* e)
		{
			((QtFundPlayer*)mPlayer)->stop();
			SingleDisplayPlot::hideEvent(e);
		}

		void QtFundFreqPlot::closeEvent(QCloseEvent *e)
		{
			RemoveFromPlayList();
			e->accept();
		}

		void QtFundFreqPlot::initialYRulerRange(double min, double max)
		{
			SingleDisplayPlot::initialYRulerRange(min,max);
			lefthole->setFixedSize(YRulerWidth(),lefthole->height());
		}
	
		void QtFundFreqPlot::PlayingTime(TData time)
		{
			((FundPlotController*)mController)->updateTimePos(float(time));
		}

		void QtFundFreqPlot::StopPlaying(TData time)
		{
			((FundPlotController*)mController)->stopPlaying(float(time));
		}

		void QtFundFreqPlot::setCurrentPlayingTime(float t)
		{
			((FundPlotController*)mController)->updateTimePos(t);
		}

		void QtFundFreqPlot::receivedStopPlaying(float t)
		{
			((FundPlotController*)mController)->stopPlaying(TData(t));
		}
 
    }	
}

// END

