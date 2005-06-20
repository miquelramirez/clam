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
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include "Assert.hxx"
#include "Ruler.hxx"
#include "HScrollGroup.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "QtAudioPlayer.hxx"
#include "QtAudioPlot.hxx"
#include "QtStereoAudioPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		QtStereoAudioPlot::QtStereoAudioPlot(QWidget* parent, const char * name, WFlags f) 
			: QtPlot(parent,name,f)
			, mMaxHZRatio(-1.0)
		{
			mSlotPlayingTimeReceived.Wrap(this,&QtStereoAudioPlot::PlayingTime);
			mSlotStopPlayingReceived.Wrap(this,&QtStereoAudioPlot::StopPlaying);

			InitStereoAudioPlot();
		}

		QtStereoAudioPlot::~QtStereoAudioPlot()
		{
		}

		void QtStereoAudioPlot::InitStereoAudioPlot()
		{
			QVBoxLayout* main = new QVBoxLayout(this);
			main->setMargin(5);
			main->setSpacing(0);

			//create player
			mPlayer = new QtAudioPlayer(this);
			((QtAudioPlayer*)mPlayer)->SetSlotPlayingTime(mSlotPlayingTimeReceived);
			((QtAudioPlayer*)mPlayer)->SetSlotStopPlaying(mSlotStopPlayingReceived);
			mPlayer->setFixedSize(75,30);

			// top area (x ruler)
			QHBoxLayout* top = new QHBoxLayout(main);
			QFrame* topLeftHole = new QFrame(this);
			mToggleColor = new QPushButton(topLeftHole);
			mToggleColor->setFixedSize(25,25);
			mToggleColor->setGeometry(0,8,25,25);
			mToggleColor->setToggleButton(true);
			mToggleColor->setText("CM");
			QToolTip::add(mToggleColor,"switch display colors");
			SwitchDisplayColors(false);
			top->addWidget(topLeftHole,0);
			mXRuler = new Ruler(this,CLAM::VM::Top);
			mXRuler->setFixedHeight(40);
			top->addWidget(mXRuler);

			// middle area - channels left and right
			QHBoxLayout* leftChannel = new QHBoxLayout(main);
			
			QVBoxLayout* left = new QVBoxLayout(leftChannel);
			QLabel* leftlabel= new QLabel(this);
			leftlabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			leftlabel->setFixedSize(25,20);
			leftlabel->setText("L");
			leftlabel->setAlignment(Qt::AlignCenter);
			QToolTip::add(leftlabel,"Left Channel");
			left->addWidget(leftlabel);

			mMuteLeft = new QPushButton(this);
			mMuteLeft->setFixedSize(25,20);
			mMuteLeft->setToggleButton(true);
			mMuteLeft->setDown(false);
			mMuteLeft->setOn(false);
			mMuteLeft->setText("M");
			QToolTip::add(mMuteLeft,"Mute Left Channel");
			left->addWidget(mMuteLeft);
			left->addStretch(1);

			mLeftChannel = new QtAudioPlot(this);
			QFont ref = mLeftChannel->RulerFont();
			QFontMetrics fm(ref);
			int yruler_width=fm.width("-0.00")+8;
			mLeftChannel->SetYRulerWidth(yruler_width);
			mLeftChannel->SetFlag(true);
			mLeftChannel->RemoveXRuler();
			mLeftChannel->RemoveVScrollGroup();
			mLeftChannel->RemoveHScrollGroup();
			mLeftChannel->RemovePlayPanel();
			leftChannel->addWidget(mLeftChannel);

			QHBoxLayout* rightChannel = new QHBoxLayout(main);

			QVBoxLayout* right = new QVBoxLayout(rightChannel);
			QLabel* rightlabel = new QLabel(this);
			rightlabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			rightlabel->setFixedSize(25,20);
			rightlabel->setText("R");
			rightlabel->setAlignment(Qt::AlignCenter);
			QToolTip::add(rightlabel,"Right Channel");
			right->addWidget(rightlabel);

			mMuteRight = new QPushButton(this);
			mMuteRight->setFixedSize(25,20);
			mMuteRight->setToggleButton(true);
			mMuteRight->setDown(false);
			mMuteRight->setOn(false);
			mMuteRight->setText("M");
			QToolTip::add(mMuteRight,"Mute Right Channel");
			right->addWidget(mMuteRight);

			right->addStretch(1);

			mRightChannel = new QtAudioPlot(this);
			mRightChannel->SetYRulerWidth(yruler_width);
			mRightChannel->SetFlag(true);
			mRightChannel->RemoveXRuler();
			mRightChannel->RemoveVScrollGroup();
			mRightChannel->RemoveHScrollGroup();
			mRightChannel->RemovePlayPanel();
			rightChannel->addWidget(mRightChannel);

			// bottom area (horizontal scroll and zoom group)
			QHBoxLayout* bottom = new QHBoxLayout(main);
			QFrame* bottomLeftHole = new QFrame(this);
			bottom->addWidget(bottomLeftHole,0);

			mHScrollBar= new HScrollGroup(this);
			bottom->addWidget(mHScrollBar);

			// play panel
			QHBoxLayout* panel = new QHBoxLayout(main);
			
			QFrame* leftpphole = new QFrame(this);
			panel->addWidget(leftpphole);

			topLeftHole->setFixedSize(yruler_width+25,40);
			bottomLeftHole->setFixedSize(topLeftHole->width(),20);
			leftpphole->setFixedSize(topLeftHole->width(),30);

			// add player to panel
			panel->addWidget(mPlayer);

			panel->addStretch(1);

			mLabelsGroup = new TimeSegmentLabelsGroup(this);
			mLabelsGroup->setMinimumSize(186,25);
			panel->addWidget(mLabelsGroup);
			
			// connection to switch colors 
			connect(mToggleColor,SIGNAL(clicked()),this,SLOT(switchColors()));

			// connections for select channel buttons
			connect(mMuteLeft,SIGNAL(clicked()),this,SLOT(leftChannelClicked()));
			connect(mMuteRight,SIGNAL(clicked()),this,SLOT(rightChannelClicked()));

			// xRuler
			connect(mLeftChannel,SIGNAL(xRulerRange(double,double)),mXRuler,SLOT(updateRange(double,double)));

			// horizontal zoom ratio 
			connect(mLeftChannel,SIGNAL(HZoomRatio(double)),this,SLOT(receivedHZoomRatio(double)));

			// zoom in/out
			connect(mLeftChannel,SIGNAL(HZoomIn()),mRightChannel,SLOT(hZoomIn()));
			connect(mLeftChannel,SIGNAL(HZoomOut()),mRightChannel,SLOT(hZoomOut()));

			// max and updated horizontal scroll values
			connect(mRightChannel,SIGNAL(HMaxScroll(int)),this,SLOT(hMaxScroll(int)));
			connect(mRightChannel,SIGNAL(UpdatedHScroll(int)),this,SLOT(updateHScroll(int)));

			// Connections horizontal scroll and zoom group
			connect(mHScrollBar,SIGNAL(zoomIn()),this,SLOT(hZoomIn()));
			connect(mHScrollBar,SIGNAL(zoomOut()),this,SLOT(hZoomOut()));
			connect(mHScrollBar,SIGNAL(scrollValueChanged(int)),this,SLOT(hScrollValue(int)));

			// region time labels
			connect(mLeftChannel,SIGNAL(regionTime(MediaTime)),this,SLOT(updateRegion(MediaTime)));
			connect(mRightChannel,SIGNAL(regionTime(MediaTime)),this,SLOT(updateRegion(MediaTime)));

			// sel pos sync
			connect(mLeftChannel,SIGNAL(selPos(TData)),mRightChannel,SLOT(setSelPos(TData)));
			connect(mRightChannel,SIGNAL(selPos(TData)),mLeftChannel,SLOT(setSelPos(TData)));

			// segmentation marks sync
			connect(mLeftChannel,SIGNAL(insertedMark(unsigned)),mRightChannel,SLOT(insertMark(unsigned)));
			connect(mRightChannel,SIGNAL(insertedMark(unsigned)),mLeftChannel,SLOT(insertMark(unsigned)));

			connect(mLeftChannel,SIGNAL(removedMark(int,unsigned)),mRightChannel,SLOT(removeMark(int,unsigned)));
			connect(mRightChannel,SIGNAL(removedMark(int,unsigned)),mLeftChannel,SLOT(removeMark(int,unsigned)));

			connect(mLeftChannel,SIGNAL(updatedMark(int,unsigned)),mRightChannel,SLOT(updateMark(int,unsigned)));
			connect(mRightChannel,SIGNAL(updatedMark(int,unsigned)),mLeftChannel,SLOT(updateMark(int,unsigned)));

			// playing pos sync
			connect(mLeftChannel,SIGNAL(currentPlayingTime(float)),mRightChannel,SLOT(setCurrentPlayingTime(float)));
			connect(mLeftChannel,SIGNAL(stopPlaying(float)),mRightChannel,SLOT(receivedStopPlaying(float)));

			// segmentation tags sync
			connect(mLeftChannel,SIGNAL(updatedTag(int,QString)),mRightChannel,SLOT(updateTag(int,QString)));
			connect(mRightChannel,SIGNAL(updatedTag(int,QString)),mLeftChannel,SLOT(updateTag(int,QString)));
		}

		void QtStereoAudioPlot::SetData(const Audio& leftChannel, const Audio& rightChannel)
		{
			CLAM_ASSERT(leftChannel.GetSize() == rightChannel.GetSize(),"Size of channels left and right must be the same!");
			mLeftChannel->SetData(leftChannel);
			mRightChannel->SetData(rightChannel);
			SetPData(leftChannel, rightChannel);
		}

		void QtStereoAudioPlot::SetBackgroundColor(Color c)
		{
			mLeftChannel->SetBackgroundColor(c);
			mRightChannel->SetForegroundColor(c);
		}

		void QtStereoAudioPlot::SetForegroundColor(Color c)
		{
			mLeftChannel->SetForegroundColor(c);
			mRightChannel->SetForegroundColor(c);
		}

		void QtStereoAudioPlot::SetDialColor(Color c)
		{
			mLeftChannel->SetDialColor(c);
			mRightChannel->SetDialColor(c);
		}

		void QtStereoAudioPlot::SetRegionColor(Color c)
		{
			mLeftChannel->SetRegionColor(c);
			mRightChannel->SetRegionColor(c);
		}

		void QtStereoAudioPlot::Label(const std::string& label)
		{
			setCaption(label.c_str());
		}

		void QtStereoAudioPlot::Geometry(int x,int y,int w,int h)
		{
			setGeometry(x,y,w,h);
		}

		void QtStereoAudioPlot::Show()
		{
			show();
		}

		void QtStereoAudioPlot::Hide()
		{
			hide();
		}

		void QtStereoAudioPlot::hZoomIn()
		{
			if(mCurrentHZRatio != 1)
			{
				mLeftChannel->hZoomIn();
			}
		}

		void QtStereoAudioPlot::hZoomOut()
		{
			if(mCurrentHZRatio != mMaxHZRatio)
			{
				mLeftChannel->hZoomOut();
			}
		}

		void QtStereoAudioPlot::hScrollValue(int value)
		{
			mLeftChannel->hScrollValue(value);
			mRightChannel->hScrollValue(value);	
		}

		void QtStereoAudioPlot::hMaxScroll(int value)
		{
			if(value >=0 && mHScrollBar->GetMaxScrollValue() != value)
			{
				mHScrollBar->setMaxScrollValue(value);
			}
		}

		void QtStereoAudioPlot::updateHScroll(int value)
		{
			if(mHScrollBar->GetScrollValue() != value)
			{
				mHScrollBar->updateScrollValue(value);
			}
		}

		void QtStereoAudioPlot::receivedHZoomRatio(double zr)
		{
			if(mMaxHZRatio < zr) mMaxHZRatio = zr;
			// update horizontal zoom ratio indicator
			mHScrollBar->updateZoomRatio(zr);
			mCurrentHZRatio = zr;
		}
		
		void QtStereoAudioPlot::leftChannelClicked()
		{
			((QtAudioPlayer*)mPlayer)->SetLeftChannelMuted(mMuteLeft->isOn());
		}

		void QtStereoAudioPlot::rightChannelClicked()
		{
			((QtAudioPlayer*)mPlayer)->SetRightChannelMuted(mMuteRight->isOn());
		}

		void QtStereoAudioPlot::updateRegion(MediaTime time)
		{
			mPlayer->stop();
			mPlayer->SetPlaySegment(time);
			mLabelsGroup->UpdateLabels(time);
		}

		void QtStereoAudioPlot::keyPressEvent(QKeyEvent* e)
		{
			mLeftChannel->SetKeyPressed(e);
			mRightChannel->SetKeyPressed(e);
		}

		void QtStereoAudioPlot::keyReleaseEvent( QKeyEvent* e)
		{
			mLeftChannel->SetKeyReleased(e);
			mRightChannel->SetKeyReleased(e);
		}

		void QtStereoAudioPlot::hideEvent(QHideEvent* e)
		{
			((QtAudioPlayer*)mPlayer)->stop();
			QWidget::hideEvent(e);
		}

		void QtStereoAudioPlot::closeEvent(QCloseEvent* e)
		{
			RemoveFromPlayList();
			e->accept();
		}

		void QtStereoAudioPlot::SetPData(const Audio& leftChannel, const Audio& rightChannel)
		{
			std::vector<const Audio*> dataPtr;
			dataPtr.resize(2);
			dataPtr[0]=&leftChannel;
			dataPtr[1]=&rightChannel;
			((QtAudioPlayer*)mPlayer)->SetData(dataPtr,true);
		}

		void QtStereoAudioPlot::SwitchDisplayColors(bool b)
		{
			(b) ? mToggleColor->show() : mToggleColor->hide();
		}
		
		void QtStereoAudioPlot::switchColors()
		{
			mLeftChannel->SetToggleColorOn(mToggleColor->isOn());
			mRightChannel->SetToggleColorOn(mToggleColor->isOn());
			mLeftChannel->switchColors();
			mRightChannel->switchColors();
		}

		void QtStereoAudioPlot::SetMarks(std::vector<unsigned>& marks)
		{
			mLeftChannel->SetMarks(marks);
			mRightChannel->SetMarks(marks);
		}

		std::vector<unsigned>& QtStereoAudioPlot::GetMarks()
		{
			return mLeftChannel->GetMarks();
		}

		void QtStereoAudioPlot::SetMarksColor(Color c)
		{
			mLeftChannel->SetMarksColor(c);
			mRightChannel->SetMarksColor(c);
		}

		void QtStereoAudioPlot::PlayingTime(TData time)
		{
			mLeftChannel->setCurrentPlayingTime(float(time));
		}

		void QtStereoAudioPlot::StopPlaying(TData time)
		{
			mLeftChannel->receivedStopPlaying(float(time));
		}

		std::vector<QString> QtStereoAudioPlot::GetSegmentationTags()
		{
			return mLeftChannel->GetSegmentationTags();
		}

    }	
}

// END

