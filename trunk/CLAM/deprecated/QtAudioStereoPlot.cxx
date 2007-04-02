#include <qlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include "Ruler.hxx"
#include "DisplaySurface.hxx"
#include "AudioPlotController.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "QtAudioPlayer.hxx"
#include "QtAudioStereoPlot.hxx"
#include "Audio.hxx"


namespace CLAM
{
	namespace VM
	{
		QtAudioStereoPlot::QtAudioStereoPlot(QWidget* parent, const char * name, WFlags f )
			: MultiDisplayPlot(parent,name,f)
		{
			mSlotPlayingTimeReceived.Wrap(this,&QtAudioStereoPlot::PlayingTime);
			mSlotStopPlayingReceived.Wrap(this,&QtAudioStereoPlot::StopPlaying);

			InitAudioStereoPlot();
		}

		QtAudioStereoPlot::~QtAudioStereoPlot()
		{
		}

		void QtAudioStereoPlot::SetData(const Audio& leftChannel, const Audio& rightChannel)
		{
			CLAM_ASSERT(leftChannel.GetSize() == rightChannel.GetSize(),
						"Size of channels left and right must be the same!");
			((AudioPlotController*)mControllers[MASTER])->SetData(leftChannel);
			((AudioPlotController*)mControllers[SLAVE])->SetData(rightChannel);
			SetPlayerData(leftChannel, rightChannel);
		}

		void QtAudioStereoPlot::SetForegroundColor(Color c)
		{
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				((AudioPlotController*)mControllers[i])->SetDataColor(c);
			}
		}

		void QtAudioStereoPlot::SetRegionColor(Color c)
		{
			for(unsigned i=0; i < mControllers.size(); i++)
			{
				((AudioPlotController*)mControllers[i])->SetRegionColor(c);
			}
		}

		void QtAudioStereoPlot::CreateControllers()
		{
			mControllers.push_back(new AudioPlotController());
			mControllers.push_back(new AudioPlotController());
			SetMasterId(MASTER);
		}

		void QtAudioStereoPlot::CreateSurfaces()
		{
			mSurfaces.push_back(new DisplaySurface(this));
			mSurfaces.push_back(new DisplaySurface(this));
			for(unsigned i=0; i < mSurfaces.size(); i++)
			{
				mSurfaces[i]->setMinimumSize(200,20);
			}
		}
			
		void QtAudioStereoPlot::SynchronizeDialPos()
		{
			SyncDialPos(MASTER);
			SyncDialPos(SLAVE);
		}

		void QtAudioStereoPlot::SynchronizeInsertMark()
		{
			SyncInsertMarkBySampleIndex(MASTER);
			SyncInsertMarkBySampleIndex(SLAVE);
		}

		void QtAudioStereoPlot::SynchronizeRemoveMark()
		{
			SyncRemoveMarkBySampleIndex(MASTER);
			SyncRemoveMarkBySampleIndex(SLAVE);
		}

		void QtAudioStereoPlot::SynchronizeUpdateMark()
		{
			SyncUpdateMarkBySampleIndex(MASTER);
			SyncUpdateMarkBySampleIndex(SLAVE);
		}

		void QtAudioStereoPlot::SynchronizeUpdateTag()
		{
			SyncUpdateTag(MASTER);
			SyncUpdateTag(SLAVE);
		}

		void QtAudioStereoPlot::DisplayBackgroundBlack()
		{
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Orange());
			SetBackgroundColor(VMColor::Black());
		}

		void QtAudioStereoPlot::DisplayBackgroundWhite()
		{
			SetForegroundColor(VMColor::Blue());
			SetDialColor(VMColor::Black());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Red());
			SetBackgroundColor(VMColor::White());
		}

		void QtAudioStereoPlot::hideEvent(QHideEvent* e)
		{
			((QtAudioPlayer*)mPlayer)->stop();
			MultiDisplayPlot::hideEvent(e);
		}

		void QtAudioStereoPlot::closeEvent(QCloseEvent* e)
		{
			RemoveFromPlayList();
			e->accept();
		}

		void QtAudioStereoPlot::muteLeftChannel()
		{
			((QtAudioPlayer*)mPlayer)->SetLeftChannelMuted(mMuteL->isOn());
		}

		void QtAudioStereoPlot::muteRightChannel()
		{
			((QtAudioPlayer*)mPlayer)->SetRightChannelMuted(mMuteR->isOn());
		}

		void QtAudioStereoPlot::updateRegion(MediaTime time)
		{
			mPlayer->stop();
			mPlayer->SetPlaySegment(time);
			mLabelsGroup->UpdateLabels(time);
		}

		void QtAudioStereoPlot::InitAudioStereoPlot()
		{
			// mute controls
			QLabel* labelL = new QLabel(this);
			labelL->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			labelL->setFixedSize(25,20);
			labelL->setText("L");
			labelL->setAlignment(Qt::AlignCenter);
			QToolTip::add(labelL,"Left Channel");

			mMuteL = new QPushButton(this);
			mMuteL->setFixedSize(25,20);
			mMuteL->setToggleButton(true);
			mMuteL->setDown(false);
			mMuteL->setOn(false);
			mMuteL->setText("M");
			QToolTip::add(mMuteL,"Mute Left Channel");
			
			QBoxLayout* mute_left_panel = new QVBoxLayout;
			mute_left_panel->addWidget(labelL);
			mute_left_panel->addWidget(mMuteL);
			mute_left_panel->addStretch();

			QLabel* labelR = new QLabel(this);
			labelR->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			labelR->setFixedSize(25,20);
			labelR->setText("R");
			labelR->setAlignment(Qt::AlignCenter);
			QToolTip::add(labelR,"Right Channel");

			mMuteR = new QPushButton(this);
			mMuteR->setFixedSize(25,20);
			mMuteR->setToggleButton(true);
			mMuteR->setDown(false);
			mMuteR->setOn(false);
			mMuteR->setText("M");
			QToolTip::add(mMuteR,"Mute Right Channel");

			QBoxLayout* mute_right_panel = new QVBoxLayout;
			mute_right_panel->addWidget(labelR);
			mute_right_panel->addWidget(mMuteR);
			mute_right_panel->addStretch();
			
			// y rulers
			mYRulerL = new Ruler(this,CLAM::VM::Left);
			mYRulerR = new Ruler(this,CLAM::VM::Left);

			QFont ref = mYRulerL->Font();
			QFontMetrics fm(ref);
			int left_hole_width = fm.width("X:-0.0e+00")+5;
			int y_ruler_width = left_hole_width - 25;
			
			mYRulerL->setFixedWidth(y_ruler_width);
			mYRulerR->setFixedWidth(y_ruler_width);
			
			// holes
			QFrame* sLeftHole = new QFrame(this);
			QFrame* pLeftHole = new QFrame(this);

			sLeftHole->setFixedSize(left_hole_width,20);
			pLeftHole->setFixedSize(left_hole_width,20);

			// toggle frame
			CreateToggleColorFrame();
			SetToggleColorFrameWidth(left_hole_width);

			// x ruler
			CreateXRuler();

			// horizontal scroll
			CreateHScrollGroup();

			// surfaces 
			CreateSurfaces();
			
			// play panel
			mPlayer = new QtAudioPlayer(this);
			mPlayer->setFixedSize(75,30);
			((QtAudioPlayer*)mPlayer)->SetSlotPlayingTime(mSlotPlayingTimeReceived);
			((QtAudioPlayer*)mPlayer)->SetSlotStopPlaying(mSlotStopPlayingReceived);
			AddToPlayList();
			
			mLabelsGroup = new TimeSegmentLabelsGroup(this);
			mLabelsGroup->setMinimumSize(186,25);

			QBoxLayout* play_panel = new QHBoxLayout;
			play_panel->addWidget(mPlayer);
			play_panel->addStretch();
			play_panel->addWidget(mLabelsGroup);

			// layout
			QGridLayout* innerLayout = new QGridLayout(this,5,3,1);
			innerLayout->addMultiCellWidget(GetToggleColorFrame(),0,0,0,1);
			innerLayout->addWidget(GetXRuler(),0,2);
			innerLayout->addLayout(mute_left_panel,1,0);
			innerLayout->addWidget(mYRulerL,1,1);
			innerLayout->addWidget(mSurfaces[MASTER],1,2);
			innerLayout->addLayout(mute_right_panel,2,0);
			innerLayout->addWidget(mYRulerR,2,1);
			innerLayout->addWidget(mSurfaces[SLAVE],2,2);
			innerLayout->addMultiCellWidget(sLeftHole,3,3,0,1);
			innerLayout->addWidget(GetHScrollGroup(),3,2);
			innerLayout->addMultiCellWidget(pLeftHole,4,4,0,1);
			innerLayout->addLayout(play_panel,4,2);
		
			// bind controllers to surfaces
			CreateControllers();
			BindToSurfaces();

			// connections
			ConnectXRuler();
			ConnectHScrollGroup();
			ConnectEditTagSlot();
			SynchronizeHZoom();
			SynchronizeDialPos();
			SynchronizeInsertMark();
			SynchronizeRemoveMark();
			SynchronizeUpdateMark();
			SynchronizeUpdateTag();
			SynchronizeRegionTime();
			SynchronizePlayingPos();
			SynchronizeFocusIn();

			connect(mControllers[MASTER],SIGNAL(yRulerRange(double,double)),mYRulerL,SLOT(updateRange(double,double)));
			connect(mControllers[SLAVE],SIGNAL(yRulerRange(double,double)),mYRulerR,SLOT(updateRange(double,double)));
			connect(mMuteL,SIGNAL(clicked()),this,SLOT(muteLeftChannel()));
			connect(mMuteR,SIGNAL(clicked()),this,SLOT(muteRightChannel()));
		}

		void QtAudioStereoPlot::PlayingTime(TData time)
		{
			((AudioPlotController*)mControllers[MASTER])->updateTimePos(time);
		}

		void QtAudioStereoPlot::StopPlaying(TData time)
		{
			((AudioPlotController*)mControllers[MASTER])->stopPlaying(time);
		}

		void QtAudioStereoPlot::SynchronizeRegionTime()
		{
			connect(((AudioPlotController*)mControllers[MASTER]),SIGNAL(selectedRegion(MediaTime)),
					this,SLOT(updateRegion(MediaTime)));
			connect(((AudioPlotController*)mControllers[SLAVE]),SIGNAL(selectedRegion(MediaTime)),
					this,SLOT(updateRegion(MediaTime)));
		}

		void QtAudioStereoPlot::SynchronizePlayingPos()
		{
			connect(((AudioPlotController*)mControllers[MASTER]),SIGNAL(currentPlayingTime(float)),
					((AudioPlotController*)mControllers[SLAVE]),SLOT(updateTimePos(float)));
			connect(((AudioPlotController*)mControllers[MASTER]),SIGNAL(stopPlayingTime(float)),
					((AudioPlotController*)mControllers[SLAVE]),SLOT(stopPlaying(float)));
		}

		void QtAudioStereoPlot::SetPlayerData(const Audio& leftChannel, const Audio& rightChannel)
		{
			std::vector<const Audio*> dataPtr;
			dataPtr.resize(2);
			dataPtr[0]=&leftChannel;
			dataPtr[1]=&rightChannel;
			((QtAudioPlayer*)mPlayer)->SetData(dataPtr,true);
		}

	}
}

// END



