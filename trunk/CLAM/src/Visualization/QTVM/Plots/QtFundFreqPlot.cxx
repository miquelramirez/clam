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
#include "SingleLabel.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "QtFundFreqPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		QtFundFreqPlot::QtFundFreqPlot(QWidget* parent, const char * name, WFlags f) 
			: QtPresentation(parent,name,f)
		{
			mSlotPlayingTimeReceived.Wrap(this,&QtFundFreqPlot::PlayingTime);
			mSlotStopPlayingReceived.Wrap(this,&QtFundFreqPlot::StopPlaying);

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
			lefthole->setFixedSize(50,30);
			panel->addWidget(lefthole);

			mPlayer = new QtFundPlayer(this);
			((QtFundPlayer*)mPlayer)->SetSlotPlayingTime(mSlotPlayingTimeReceived);
			((QtFundPlayer*)mPlayer)->SetSlotStopPlaying(mSlotStopPlayingReceived);
			mPlayer->setFixedSize(75,30);
			panel->addWidget(mPlayer);

			panel->addStretch(1);

			mLeftFreqLab = new SingleLabel(this," Hz ","Frequency");
			mRightFreqLab = new SingleLabel(this," Hz ","Frequency Right");

			panel->addWidget(mLeftFreqLab);
			panel->addWidget(mRightFreqLab);

			panel->addStretch(1);
			
			mLabelsGroup = new TimeSegmentLabelsGroup(this);
			mLabelsGroup->setMinimumSize(186,25);
			panel->addWidget(mLabelsGroup);

			righthole = new QFrame(this);
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

		void QtFundFreqPlot::SetDialColor(Color c)
		{
			((FundPlotController*)mController)->SetDialColor(c);
		}

		void QtFundFreqPlot::SetRegionColor(Color c)
		{
			((FundPlotController*)mController)->SetRegionColor(c);
		}

		void QtFundFreqPlot::keyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
					((FundPlotController*)mController)->SetKeyShiftPressed(true);
					break;

				case Qt::Key_Insert:
					((FundPlotController*)mController)->SetKeyInsertPressed(true); 
					break;
						
				case Qt::Key_Delete:
					((FundPlotController*)mController)->SetKeyDeletePressed(true); 
					break;

				default:
					break;
			}
		}

		void QtFundFreqPlot::keyReleaseEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
					((FundPlotController*)mController)->SetKeyShiftPressed(false);			
					break;

				case Qt::Key_Insert:
					((FundPlotController*)mController)->SetKeyInsertPressed(false); 
					break;
						
				case Qt::Key_Delete:
					((FundPlotController*)mController)->SetKeyDeletePressed(false); 
					break;

				default:
					break;
			}
		}

		void QtFundFreqPlot::updateRegion(MediaTime time)
		{
			mPlayer->stop();
			mPlayer->SetPlaySegment(time);
			mLabelsGroup->UpdateLabels(time);

			UpdateFreqLabels(time);
		}

		void QtFundFreqPlot::UpdateFreqLabels(MediaTime time)
		{
			if(time.HasDuration())
			{
				mLeftFreqLab->Update(((FundPlotController*)mController)->GetFreq(time.GetBegin()));
				mLeftFreqLab->SetToolTip("Frequency Left");
				mRightFreqLab->Update(((FundPlotController*)mController)->GetFreq(time.GetEnd()));
				mRightFreqLab->show();
			}
			else
			{
				mLeftFreqLab->Update(((FundPlotController*)mController)->GetFreq(time.GetBegin()));
				mLeftFreqLab->SetToolTip("Frequency");
				mRightFreqLab->clear();
				mRightFreqLab->hide();
			}
		}

		void QtFundFreqPlot::SetPlotController()
		{
			SetController(new FundPlotController());
		}

		void QtFundFreqPlot::Connect()
		{
			// Connections
			connect(((FundPlotController*)mController),SIGNAL(initialYRulerRange(double,double)),this,SLOT(initialYRulerRange(double,double)));
			connect(((FundPlotController*)mController),SIGNAL(selectedRegion(MediaTime)),this,SLOT(updateRegion(MediaTime)));
			connect(((FundPlotController*)mController),SIGNAL(currentPlayingTime(float)),this,SIGNAL(currentPlayingTime(float)));
			connect(((FundPlotController*)mController),SIGNAL(stopPlaying(float)),this,SIGNAL(stopPlaying(float)));
		}

		void QtFundFreqPlot::SetPData(const Segment& seg)
		{
			((QtFundPlayer*)mPlayer)->SetData(seg);
		}

		void QtFundFreqPlot::DisplayBackgroundBlack()
		{
			SetBackgroundColor(VMColor::Black());
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Orange());
		}

		void QtFundFreqPlot::DisplayBackgroundWhite()
		{
			SetBackgroundColor(VMColor::White());
			SetForegroundColor(VMColor::Blue());
			SetDialColor(VMColor::Black());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Red());
		}

		void QtFundFreqPlot::hideEvent(QHideEvent* e)
		{
			((QtFundPlayer*)mPlayer)->stop();
			QWidget::hideEvent(e);
		}

		void QtFundFreqPlot::closeEvent(QCloseEvent *e)
		{
			RemoveFromPlayList();
			e->accept();
		}

		void QtFundFreqPlot::SetMarks(std::vector<unsigned>& marks)
		{
			((FundPlotController*)mController)->SetMarks(marks);
		}

		std::vector<unsigned>& QtFundFreqPlot::GetMarks()
		{
			return ((FundPlotController*)mController)->GetMarks();
		}

		void QtFundFreqPlot::SetMarksColor(Color c)
		{
			((FundPlotController*)mController)->SetMarksColor(c);
		}

		void QtFundFreqPlot::initialYRulerRange(double min, double max)
		{
			QtPresentation::initialYRulerRange(min,max);
			lefthole->setFixedSize(YRulerWidth(),lefthole->height());
		}
	
		void QtFundFreqPlot::PlayingTime(TData time)
		{
			((FundPlotController*)mController)->UpdateTimePos(time);
		}

		void QtFundFreqPlot::StopPlaying(TData time)
		{
			((FundPlotController*)mController)->StopPlaying(time);
		}

		void QtFundFreqPlot::setCurrentPlayingTime(float t)
		{
			((FundPlotController*)mController)->UpdateTimePos(TData(t));
		}

		void QtFundFreqPlot::receivedStopPlaying(float t)
		{
			((FundPlotController*)mController)->StopPlaying(TData(t));
		}

		std::vector<QString> QtFundFreqPlot::GetSegmentationTags()
		{
			return ((FundPlotController*)mController)->GetTags();
		}
	     
    }	
}

// END

