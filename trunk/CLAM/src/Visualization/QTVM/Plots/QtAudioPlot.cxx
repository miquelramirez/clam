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
#include "SingleLabel.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "QtAudioPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		QtAudioPlot::QtAudioPlot(QWidget* parent, const char * name, WFlags f) 
			: QtPresentation(parent,name,f)
			, mShowRightAmp(false)
		{
			mSlotPlayingTimeReceived.Wrap(this,&QtAudioPlot::PlayingTime);
			mSlotStopPlayingReceived.Wrap(this,&QtAudioPlot::StopPlaying);

			mPlayBounds.SetBegin(-1.0);
			mPlayBounds.SetEnd(-1.0);
			SetPlotController();
			InitAudioPlot();	
			Connect();
		}

		QtAudioPlot::~QtAudioPlot()
		{
		}

		void QtAudioPlot::InitAudioPlot()
		{
			mPanel = new QHBoxLayout;
			
			lefthole = new QFrame(this);
			lefthole->setFixedSize(50,30);
			mPanel->addWidget(lefthole);

			mPlayer = new QtAudioPlayer(this);
			mPlayer->setFixedSize(75,30);
			((QtAudioPlayer*)mPlayer)->SetSlotPlayingTime(mSlotPlayingTimeReceived);
			((QtAudioPlayer*)mPlayer)->SetSlotStopPlaying(mSlotStopPlayingReceived);
			mPanel->addWidget(mPlayer);

			mPanel->addStretch(1);

			mLeftAmpLab = new SingleLabel(this,"","Amplitude");
			mRightAmpLab = new SingleLabel(this,"","Amplitude Right");
			mPanel->addWidget(mLeftAmpLab);
			mPanel->addWidget(mRightAmpLab);

			mPanel->addStretch(1);

			mLabelsGroup = new TimeSegmentLabelsGroup(this);
			mLabelsGroup->setMinimumSize(186,25);
			mPanel->addWidget(mLabelsGroup);

			righthole = new QFrame(this);
			righthole->setFixedSize(20,30);
			mPanel->addWidget(righthole);

			AddToMainLayout(mPanel);	
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

		void QtAudioPlot::SetForegroundColor(Color c)
		{
			((AudioPlotController*)mController)->SetDataColor(c);
		}

		void QtAudioPlot::SetDialColor(Color c)
		{
			((AudioPlotController*)mController)->SetDialColor(c);
		}

		void QtAudioPlot::SetRegionColor(Color c)
		{
			((AudioPlotController*)mController)->SetRegionColor(c);
		}

		void QtAudioPlot::keyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
					((AudioPlotController*)mController)->SetKeyShiftPressed(true);
					break;

				case Qt::Key_Insert:
					((AudioPlotController*)mController)->SetKeyInsertPressed(true); 
					break;
						
				case Qt::Key_Delete:
					((AudioPlotController*)mController)->SetKeyDeletePressed(true); 
					break;
				    
				default:
					break;
			}
		}

		void QtAudioPlot::keyReleaseEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
					((AudioPlotController*)mController)->SetKeyShiftPressed(false);
					break;

				case Qt::Key_Insert:
					((AudioPlotController*)mController)->SetKeyInsertPressed(false); 
					break;
						
				case Qt::Key_Delete:
					((AudioPlotController*)mController)->SetKeyDeletePressed(false); 
					break;

				default:
					break;
			}
		}

		void QtAudioPlot::updateRegion(MediaTime time)
		{
			if(	time.GetBegin()==mPlayBounds.GetBegin() &&
				time.GetEnd()==mPlayBounds.GetEnd()) return;
	
			mPlayBounds=time;
			mPlayer->stop();
			mPlayer->SetPlaySegment(time);
			mLabelsGroup->UpdateLabels(time);

			UpdateAmpLabels(time);
			emit regionTime(time);
			emit regionTime(float(time.GetBegin()), float(time.GetEnd()));
		}

		void QtAudioPlot::UpdateAmpLabels(MediaTime time)
		{
			if(time.HasDuration())
			{
				mLeftAmpLab->Update(((AudioPlotController*)mController)->GetAmp(time.GetBegin()));
				mLeftAmpLab->SetToolTip("Amplitude Left");
				mRightAmpLab->Update(((AudioPlotController*)mController)->GetAmp(time.GetEnd()));
				if(mShowRightAmp)
				{
					mRightAmpLab->show();
				}
			}
			else
			{
				mLeftAmpLab->Update(((AudioPlotController*)mController)->GetAmp(time.GetBegin()));
				mLeftAmpLab->SetToolTip("Amplitude");
				mRightAmpLab->clear();
				mRightAmpLab->hide();
			}
		}

		void QtAudioPlot::SetPlotController()
		{
			SetController(new AudioPlotController());
		}

		void QtAudioPlot::Connect()
		{
			// Connections
			connect(((AudioPlotController*)mController),SIGNAL(initialYRulerRange(double,double)),this,SLOT(initialYRulerRange(double,double)));
			connect(((AudioPlotController*)mController),SIGNAL(selectedRegion(MediaTime)),this,SLOT(updateRegion(MediaTime)));
			connect(((AudioPlotController*)mController),SIGNAL(currentPlayingTime(float)),this,SIGNAL(currentPlayingTime(float)));
			connect(((AudioPlotController*)mController),SIGNAL(stopPlaying(float)),this,SIGNAL(stopPlaying(float)));
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
			SetBackgroundColor(VMColor::Black());
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Orange());
		}

		void QtAudioPlot::DisplayBackgroundWhite()
		{
			SetBackgroundColor(VMColor::White());
			SetForegroundColor(VMColor::Blue());
			SetDialColor(VMColor::Black());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Red());
		}

		void QtAudioPlot::hideEvent(QHideEvent* e)
		{
			((QtAudioPlayer*)mPlayer)->stop();
			QWidget::hideEvent(e);
		}

		void QtAudioPlot::closeEvent(QCloseEvent *e)
		{
			RemoveFromPlayList();
			e->accept();
		}

		void QtAudioPlot::RemovePlayPanel()
		{
			RemoveFromMainLayout(mPanel);
			lefthole->hide();
			righthole->hide();
			mPlayer->hide();
			mLeftAmpLab->hide();
			mRightAmpLab->hide();
			mLabelsGroup->hide();
			mShowRightAmp=false;
		}

		void QtAudioPlot::SetKeyPressed(QKeyEvent* e)
		{
			keyPressEvent(e);
		}

		void QtAudioPlot::SetKeyReleased(QKeyEvent* e)
		{
			keyReleaseEvent(e);
		}

		void QtAudioPlot::SetMarks(std::vector<unsigned>& marks)
		{
			((AudioPlotController*)mController)->SetMarks(marks);
		}
	    
		std::vector<unsigned>& QtAudioPlot::GetMarks()
		{
			return ((AudioPlotController*)mController)->GetMarks();
		}


		void QtAudioPlot::SetMarksColor(Color c)
		{
			((AudioPlotController*)mController)->SetMarksColor(c);
		}

		void QtAudioPlot::initialYRulerRange(double min, double max)
		{
			QtPresentation::initialYRulerRange(min,max);
			lefthole->setFixedSize(YRulerWidth(),lefthole->height());
		}

		void QtAudioPlot::setSelectedXPos(double xpos)
		{
			((AudioPlotController*)mController)->SetSelectedXPos(xpos);
		}

		void QtAudioPlot::PlayingTime(TData time)
		{
			((AudioPlotController*)mController)->UpdateTimePos(time);
		}

		void QtAudioPlot::StopPlaying(TData time)
		{
			((AudioPlotController*)mController)->StopPlaying(time);
		}

		void QtAudioPlot::setCurrentPlayingTime(float t)
		{
			((AudioPlotController*)mController)->UpdateTimePos(TData(t));
		}

		void QtAudioPlot::receivedStopPlaying(float t)
		{
			((AudioPlotController*)mController)->StopPlaying(TData(t));
		}

		std::vector<QString> QtAudioPlot::GetSegmentationTags()
		{
			return ((AudioPlotController*)mController)->GetTags();
		}

    }	
}

// END

