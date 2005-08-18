#include <qlayout.h>
#include <qframe.h>
#include <qcombobox.h>
#include <qtooltip.h>
#include "Ruler.hxx"
#include "VScrollGroup.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "DisplaySurface.hxx"
#include "QtSMSPlayer.hxx"
#include "AudioPlotController.hxx"
#include "FundPlotController.hxx"
#include "SinTracksPlotController.hxx"
#include "SMSTimeMultiDisplay.hxx"

namespace CLAM
{
	namespace VM
	{
		SMSTimeMultiDisplay::SMSTimeMultiDisplay(QWidget* parent, const char * name, WFlags f)
			: MultiDisplayPlot(parent,name,f)
			, mShowOnNewData(true)
			, mHasMasterData(false)
			, mHasAudioData(false)
			, mHasFundamentalData(false)
			, mHasEnqueuedPlayListItem(false)
		{
			mSlotPlayingTimeReceived.Wrap(this,&SMSTimeMultiDisplay::PlayingTime);
			mSlotStopPlayingReceived.Wrap(this,&SMSTimeMultiDisplay::StopPlaying);

			InitSMSTimePlot();
		}

		SMSTimeMultiDisplay::~SMSTimeMultiDisplay()
		{
			mYRulers.clear();
			mAudioPtrs.clear();
		}

		void SMSTimeMultiDisplay::InitSMSTimePlot()
		{
				// x ruler
			CreateXRuler();

			// surfaces
			CreateSurfaces();

			QFont ref = mYRulers[0]->Font();
			QFontMetrics fm(ref);       
			int y_ruler_width = fm.width("X:-0.0e+00");
			
			SetYRulersWidth(y_ruler_width);
			
			// holes
			QFrame* topHole = new QFrame(this);
			QFrame* sleftHole = new QFrame(this);
			QFrame* srightHole = new QFrame(this);
			
			leftHole = new QFrame(this);
			rightHole = new QFrame(this);

			topHole->setFixedSize(20,GetXRuler()->height());
			sleftHole->setFixedSize(y_ruler_width,20);
			srightHole->setFixedSize(20,20);
			leftHole->setFixedSize(y_ruler_width,20);
			rightHole->setFixedSize(20,20);

			// toggle frame
			CreateToggleColorFrame();
			SetToggleColorFrameWidth(y_ruler_width,true);

			// scrolling
			CreateHScrollGroup();

			// bind controllers to surfaces
			CreateControllers();
			BindToSurfaces();

			// layout
			QGridLayout* innerLayout = new QGridLayout(this,5,3,2);
			innerLayout->addWidget(GetToggleColorFrame(),0,0);
			innerLayout->addWidget(GetXRuler(),0,1);
			innerLayout->addWidget(topHole,0,2);
			innerLayout->addMultiCellLayout(CreateAudioDisplays(),1,1,0,2);		
			innerLayout->addMultiCellLayout(CreateFrequencyDisplays(),2,2,0,2);
			innerLayout->addWidget(sleftHole,3,0);
			innerLayout->addWidget(GetHScrollGroup(),3,1);
			innerLayout->addWidget(srightHole,3,2);
			innerLayout->addWidget(leftHole,4,0);
			innerLayout->addLayout(CreatePlayer(),4,1);
			innerLayout->addWidget(rightHole,4,2);

			ConnectXRuler();
			ConnectHScrollGroup();
			ConnectVScrollGroup();
			ConnectEditTagSlot();
			ConnectDataTypeSignal();
			SynchronizeHZoom();
			SynchronizeVZoom();
			SynchronizeDialPos();
			SynchronizeInsertMark();
			SynchronizeRemoveMark();
			SynchronizeUpdateMark();
			SynchronizeUpdateTag();
			SynchronizeYRulers();
			SynchronizeRegionTime();
			SynchronizePlayingPos();

			InitPlayList();
			InitAudioPtrs();

			HideDisplays();
		}


		void SMSTimeMultiDisplay::SetOriginalAudio(const Audio& audio)
		{
			((AudioPlotController*)mControllers[MASTER])->SetData(audio);
			mAudioPtrs[MASTER] = &audio;
			mHasMasterData = true;
			if(mShowOnNewData) ShowDisplay(MASTER);
		}

		void SMSTimeMultiDisplay::SetSynthesizedAudio(const Audio& audio)
		{
			if(!mHasMasterData) 
			{
				// dummy
				((AudioPlotController*)mControllers[MASTER])->SetData(audio);
				mHasMasterData = true;
			}
			((AudioPlotController*)mControllers[SYNTHESIZED])->SetData(audio);
			mAudioPtrs[SYNTHESIZED] = &audio;
			if(mShowOnNewData) ShowDisplay(SYNTHESIZED);
		}

		void SMSTimeMultiDisplay::SetSynthesizedSinusoidal(const Audio& audio)
		{
			if(!mHasMasterData) 
			{
				// dummy
				((AudioPlotController*)mControllers[MASTER])->SetData(audio);
				mHasMasterData = true;
			}
			((AudioPlotController*)mControllers[SINUSOIDAL])->SetData(audio);
			mAudioPtrs[SINUSOIDAL] = &audio;
			if(mShowOnNewData) ShowDisplay(SINUSOIDAL);
		}

		void SMSTimeMultiDisplay::SetSynthesizedResidual(const Audio& audio)
		{
			if(!mHasMasterData) 
			{
				// dummy
				((AudioPlotController*)mControllers[MASTER])->SetData(audio);
				mHasMasterData = true;
			}
			((AudioPlotController*)mControllers[RESIDUAL])->SetData(audio);
			mAudioPtrs[RESIDUAL] = &audio;
			if(mShowOnNewData) ShowDisplay(RESIDUAL);
		}

		void SMSTimeMultiDisplay::SetAnalyzedSegment(const Segment& segment)
		{
			if(!mHasMasterData) 
			{
				// dummy
				Audio audio;
				audio.SetSize(TSize(segment.GetEndTime()*segment.GetSamplingRate()));
				audio.SetSampleRate(segment.GetSamplingRate());
				((AudioPlotController*)mControllers[MASTER])->SetData(audio);
				mHasMasterData = true;
			}
			((FundPlotController*)mControllers[FUNDAMENTAL])->SetData(segment);
			((SinTracksPlotController*)mControllers[SINTRACKS])->SetData(segment);
			// data to player
			((QtSMSPlayer*)mPlayer)->SetData(segment);
			mHasFundamentalData = true;
			if(mShowOnNewData)
			{
				ShowDisplay(FUNDAMENTAL);
				ShowDisplay(SINTRACKS);
			}
			
		}

		void SMSTimeMultiDisplay::SetForegroundColor(Color c)
		{
			for(int i=MASTER; i <= RESIDUAL; i++)
			{
				((AudioPlotController*)mControllers[i])->SetDataColor(c);
			}
			((FundPlotController*)mControllers[FUNDAMENTAL])->SetDataColor(c);
		}

		void SMSTimeMultiDisplay::SetRegionColor(Color c)
		{
			for(int i=MASTER; i <= FUNDAMENTAL; i++)
			{
				((PlayablePlotController*)mControllers[i])->SetRegionColor(c);
			}
		}

		void SMSTimeMultiDisplay::ShowDisplay(int id)
		{
			if(id < MASTER || id > SINTRACKS) return;
			mControllers[id]->ActiveRendering(true);
			mYRulers[id]->show();
			mSurfaces[id]->show();
			switch(id)
			{
				case MASTER:
				case SYNTHESIZED:
				case SINUSOIDAL:
				case RESIDUAL:
					if(!mAudioVScroll->isVisible()) mAudioVScroll->show();
					AddAudioItem(id);
					if(!mPlayer->isVisible()) ShowPlayer();
					break;
				case FUNDAMENTAL:
					if(!mFrequencyVScroll->isVisible()) mFrequencyVScroll->show();
					AddFundFreqItem(id);
					if(!mPlayer->isVisible()) ShowPlayer();
					break;
				case SINTRACKS:
					if(!mFrequencyVScroll->isVisible()) mFrequencyVScroll->show();
					break;
				default:
					break;
			}
			if(!isVisible()) show();
		}

		void SMSTimeMultiDisplay::HideDisplay(int id)
		{
			if(id < MASTER || id > SINTRACKS) return;
			mControllers[id]->ActiveRendering(false);
			mYRulers[id]->hide();
			mSurfaces[id]->hide();
			switch(id)
			{
				case MASTER:
				case SYNTHESIZED:
				case SINUSOIDAL:
				case RESIDUAL:
					CheckAudioVScrollVisibility();
					break;
				case FUNDAMENTAL:
				case SINTRACKS:
					CheckFrequencyVScrollVisibility();
					break;
				default:
					break;
			}
			RemovePlayListItem(id);
			CheckVisibility();
			CheckPlayerVisibility();
		}

		void SMSTimeMultiDisplay::ShowDisplayOnNewData(bool show)
		{
			mShowOnNewData = show;
		}

		void SMSTimeMultiDisplay::Flush()
		{
			for(unsigned i=0; i < mAudioPtrs.size(); i++)
			{
				if(mAudioPtrs[i])
				{
					delete mAudioPtrs[i];
					mAudioPtrs[i] = 0;
				}
			}
			mHasMasterData = false;
			mHasAudioData = false;
			mHasFundamentalData = false;
			mCBPlayList->clear();
			((QtSMSPlayer*)mPlayer)->Flush();
		}

		void SMSTimeMultiDisplay::CreateControllers()
		{
			for(int i=MASTER; i <= RESIDUAL; i++)
			{
				mControllers.push_back(new AudioPlotController());
			}
			mControllers.push_back(new FundPlotController());
			mControllers.push_back(new SinTracksPlotController());
			SetMasterId(MASTER);
		}

		void SMSTimeMultiDisplay::CreateSurfaces()
		{
			for(int i=MASTER; i <= SINTRACKS; i++)
			{
				mSurfaces.push_back(new DisplaySurface(this));
				mYRulers.push_back(new Ruler(this,CLAM::VM::Left));
				mSurfaces[i]->setMinimumSize(200,20);
			}
		}

		void SMSTimeMultiDisplay::SynchronizeDialPos()
		{
			for(int i=MASTER; i <= SINTRACKS; i++)
			{
				SyncDialPos(i);
			}
		}

		void SMSTimeMultiDisplay::SynchronizeInsertMark()
		{
			for(int i=MASTER; i <= SINTRACKS; i++)
			{
				SyncInsertMarkBySampleIndex(i);
			}
		}

		void SMSTimeMultiDisplay::SynchronizeRemoveMark()
		{
			for(int i=MASTER; i <= SINTRACKS; i++)
			{
				SyncRemoveMarkBySampleIndex(i);
			}
		}

		void SMSTimeMultiDisplay::SynchronizeUpdateMark()
		{
			for(int i=MASTER; i <= SINTRACKS; i++)
			{
				SyncUpdateMarkBySampleIndex(i);
			}
		}

		void SMSTimeMultiDisplay::SynchronizeUpdateTag()
		{
			for(int i=MASTER; i <= SINTRACKS; i++)
			{
				SyncUpdateTag(i);
			}
		}

		void SMSTimeMultiDisplay::DisplayBackgroundBlack()
		{
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetMarksColor(VMColor::Orange());
			SetBackgroundColor(VMColor::Black());
		}

		void SMSTimeMultiDisplay::DisplayBackgroundWhite()
		{
			SetForegroundColor(VMColor::Blue());
			SetDialColor(VMColor::Black());
			SetMarksColor(VMColor::Red());
			SetBackgroundColor(VMColor::White());
		}

		void SMSTimeMultiDisplay::hideEvent(QHideEvent* e)
		{
			((QtSMSPlayer*)mPlayer)->stop();
			QWidget::hideEvent(e);
		}

		void SMSTimeMultiDisplay::closeEvent(QCloseEvent* e)
		{
			RemoveFromPlayList();
			e->accept();
		}

		void SMSTimeMultiDisplay::setMaxAudioVScroll(int value)
		{
			int max = value-mSurfaces[MASTER]->height();
			if(max < 0) max=0;
			if(max >= 0 && mAudioVScroll->GetMaxScrollValue() != max)
			{
				mAudioVScroll->setMaxScrollValue(max);
			}
		}

		void SMSTimeMultiDisplay::setMaxFrequencyVScroll(int value)
		{
			int max = value-mSurfaces[FUNDAMENTAL]->height();
			if(max < 0) max=0;
			if(max >= 0 && mFrequencyVScroll->GetMaxScrollValue() != max)
			{
				mFrequencyVScroll->setMaxScrollValue(max);
			}
		}

		void SMSTimeMultiDisplay::originalAudio(bool over)
		{
			(over) ? emit dataType("original audio") : emit dataType("");
		}

		void SMSTimeMultiDisplay::synthesizedAudio(bool over)
		{
			(over) ? emit dataType("synthesized audio") : emit dataType("");
		}

		void SMSTimeMultiDisplay::sinusoidalAudio(bool over)
		{
			(over) ? emit dataType("synthesized sinusoidal") : emit dataType("");
		}

		void SMSTimeMultiDisplay::residualAudio(bool over)
		{
			(over) ? emit dataType("synthesized residual") : emit dataType("");
		}

		void SMSTimeMultiDisplay::fundamentalFreq(bool over)
		{
			(over) ? emit dataType("fundamental frequency") : emit dataType("");
		}

		void SMSTimeMultiDisplay::sinusoidalTracks(bool over)
		{
			(over) ? emit dataType("sinusoidal tracks") : emit dataType("");
		}

		void SMSTimeMultiDisplay::updateRegion(MediaTime time)
		{
			mPlayer->stop();
			mPlayer->SetPlaySegment(time);
			mLabelsGroup->UpdateLabels(time);
		}

		void SMSTimeMultiDisplay::setCurrentPlayer(int id)
		{
			int dataId = GetPlayDataId(mCBPlayList->text(id));
			int playerId = (dataId == FUNDAMENTAL) ? FUND_PLAYER : AUDIO_PLAYER;
			bool isAudioData = true;
			mHasEnqueuedPlayListItem = false;
			std::vector<const Audio*> data(1);
			switch(dataId)
			{
				case MASTER:
					data[0] = mAudioPtrs[MASTER];
					break;
				case SYNTHESIZED:
					data[0] = mAudioPtrs[SYNTHESIZED];
					break;
				case SINUSOIDAL:
					data[0] = mAudioPtrs[SINUSOIDAL];
					break;
				case RESIDUAL:
					data[0] = mAudioPtrs[RESIDUAL];
					break;
				default:
					isAudioData = false;
					break;
			}
			if(mPlayer->IsRunning())
			{
				mHasEnqueuedPlayListItem = true;
				mEnqueuedDataId = dataId;
				mEnqueuedPlayerId = playerId;
			}
			else
			{		
				((QtSMSPlayer*)mPlayer)->SetCurrentPlayer(playerId);
				if(isAudioData)
				{
					((QtSMSPlayer*)mPlayer)->SetData(data,!mHasAudioData);
				}
			}
		}

		void SMSTimeMultiDisplay::SynchronizeVZoom()
		{
			for(int i=SYNTHESIZED; i <= RESIDUAL; i++)
			{
				connect(mControllers[MASTER],SIGNAL(yRulerRange(double,double)),
						mControllers[i],SLOT(setVBounds(double,double)));
			}
			connect(mControllers[FUNDAMENTAL],SIGNAL(yRulerRange(double,double)),
					mControllers[SINTRACKS],SLOT(setVBounds(double,double)));
			
		}

		void SMSTimeMultiDisplay::CheckVisibility()
		{
			bool flag = false;
			for(unsigned i=0; i < mYRulers.size(); i++)
			{
				if(mYRulers[i]->isVisible()) 
				{
					flag = true;
					break;
				}
			}
			if(!flag) hide();
		}

		void SMSTimeMultiDisplay::SetYRulersWidth(int w)
		{
			for(unsigned i=0; i < mYRulers.size(); i++)
			{
				mYRulers[i]->setFixedWidth(w);
			}
		}

		void SMSTimeMultiDisplay::ConnectVScrollGroup()
		{
			// audio group vertical scrolling
			connect(mAudioVScroll,SIGNAL(zoomIn()),mControllers[MASTER],SLOT(vZoomIn()));
			connect(mAudioVScroll,SIGNAL(zoomOut()),mControllers[MASTER],SLOT(vZoomOut()));
			connect(mAudioVScroll,SIGNAL(scrollValueChanged(int)),mControllers[MASTER],SLOT(updateVScrollValue(int)));

			connect(mControllers[MASTER],SIGNAL(vZoomRatio(double)),mAudioVScroll,SLOT(updateZoomRatio(double)));
			connect(mControllers[MASTER],SIGNAL(vScrollValue(int)),mAudioVScroll,SLOT(updateScrollValue(int)));
			connect(mControllers[MASTER],SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxAudioVScroll(int)));

			// frequency group vertical scrolling
			connect(mFrequencyVScroll,SIGNAL(zoomIn()),mControllers[FUNDAMENTAL],SLOT(vZoomIn()));
			connect(mFrequencyVScroll,SIGNAL(zoomOut()),mControllers[FUNDAMENTAL],SLOT(vZoomOut()));
			connect(mFrequencyVScroll,SIGNAL(scrollValueChanged(int)),
					mControllers[FUNDAMENTAL],SLOT(updateVScrollValue(int)));

			connect(mControllers[FUNDAMENTAL],SIGNAL(vZoomRatio(double)),
					mFrequencyVScroll,SLOT(updateZoomRatio(double)));
			connect(mControllers[FUNDAMENTAL],SIGNAL(vScrollValue(int)),mFrequencyVScroll,SLOT(updateScrollValue(int)));
			connect(mControllers[FUNDAMENTAL],SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxFrequencyVScroll(int)));	
		}

		void SMSTimeMultiDisplay::SynchronizeYRulers()
		{
			for(int i=MASTER; i <= RESIDUAL; i++)
			{
				SyncAudioYRuler(i);
			}
			for(int i=FUNDAMENTAL; i <= SINTRACKS; i++)
			{
				SyncFrequencyYRuler(i);
			}
		}

		void SMSTimeMultiDisplay::SyncAudioYRuler(int id)
		{
			for(int i=MASTER; i <= RESIDUAL; i++)
			{
				if(i == id) continue;
				connect(mControllers[id],SIGNAL(yRulerRange(double,double)),
						mYRulers[i],SLOT(updateRange(double,double)));
			}
		}

		void SMSTimeMultiDisplay::SyncFrequencyYRuler(int id)
		{
			for(int i=FUNDAMENTAL; i <= SINTRACKS; i++)
			{
				if(i == id) continue;
				connect(mControllers[id],SIGNAL(yRulerRange(double,double)),
						mYRulers[i],SLOT(updateRange(double,double)));
			}
		}

		void SMSTimeMultiDisplay::HideDisplays()
		{
			for(unsigned i=0; i < mSurfaces.size(); i++)
			{
				HideDisplay(i);
			}
		}

		void SMSTimeMultiDisplay::ConnectDataTypeSignal()
		{
			connect(mControllers[MASTER],SIGNAL(mouseOverDisplay(bool)),this,SLOT(originalAudio(bool)));
			connect(mControllers[SYNTHESIZED],SIGNAL(mouseOverDisplay(bool)),this,SLOT(synthesizedAudio(bool)));
			connect(mControllers[SINUSOIDAL],SIGNAL(mouseOverDisplay(bool)),this,SLOT(sinusoidalAudio(bool)));
			connect(mControllers[RESIDUAL],SIGNAL(mouseOverDisplay(bool)),this,SLOT(residualAudio(bool)));
			connect(mControllers[FUNDAMENTAL],SIGNAL(mouseOverDisplay(bool)),this,SLOT(fundamentalFreq(bool)));
			connect(mControllers[SINTRACKS],SIGNAL(mouseOverDisplay(bool)),this,SLOT(sinusoidalTracks(bool)));
		}

		void SMSTimeMultiDisplay::PlayingTime(TData time)
		{
			((AudioPlotController*)mControllers[MASTER])->updateTimePos(time);
		}

		void SMSTimeMultiDisplay::StopPlaying(TData time)
		{
			((AudioPlotController*)mControllers[MASTER])->stopPlaying(time);
			if(mHasEnqueuedPlayListItem)
			{
				mHasEnqueuedPlayListItem = false;
				((QtSMSPlayer*)mPlayer)->SetCurrentPlayer(mEnqueuedPlayerId);
				if(mEnqueuedDataId >= MASTER && mEnqueuedDataId <= RESIDUAL)
				{
					std::vector<const Audio*> data(1);
					data[0] = mAudioPtrs[mEnqueuedDataId];
					((QtSMSPlayer*)mPlayer)->SetData(data,!mHasAudioData);
				}
			}
		}

		void SMSTimeMultiDisplay::SynchronizeRegionTime()
		{
			for(int i=MASTER; i <= FUNDAMENTAL; i++)
			{
				connect(((PlayablePlotController*)mControllers[i]),SIGNAL(selectedRegion(MediaTime)),
						this,SLOT(updateRegion(MediaTime)));
			}
		}

		void SMSTimeMultiDisplay::SynchronizePlayingPos()
		{
			for(int i=SYNTHESIZED; i <= FUNDAMENTAL; i++)
			{
				connect(((PlayablePlotController*)mControllers[MASTER]),SIGNAL(currentPlayingTime(float)),
						((PlayablePlotController*)mControllers[i]),SLOT(updateTimePos(float)));
				connect(((PlayablePlotController*)mControllers[MASTER]),SIGNAL(stopPlayingTime(float)),
						((PlayablePlotController*)mControllers[i]),SLOT(stopPlaying(float)));
			}
		}

		void SMSTimeMultiDisplay::InitAudioPtrs()
		{
			mAudioPtrs.resize(4);
			for(unsigned i=0; i < mAudioPtrs.size(); i++) mAudioPtrs[i]=0;
		}

		void SMSTimeMultiDisplay::CheckAudioVScrollVisibility()
		{
			bool flag = false;
			for(int i=MASTER; i <= RESIDUAL; i++)
			{
				if(mYRulers[i]->isVisible()) 
				{
					flag = true;
					break;
				}
			}
			if(!flag) mAudioVScroll->hide();
		}

		void SMSTimeMultiDisplay::CheckPlayerVisibility()
		{
			bool flag = false;
			for(int i=MASTER; i <= FUNDAMENTAL; i++)
			{
				if(mYRulers[i]->isVisible())
				{
					flag = true;
					break;
				}
			}
			if(!flag) HidePlayer();
		}

		void SMSTimeMultiDisplay::CheckFrequencyVScrollVisibility()
		{ 
			bool flag = false;
			for(int i=FUNDAMENTAL; i <= SINTRACKS; i++)
			{
				if(mYRulers[i]->isVisible()) 
				{
					flag = true;
					break;
				}
			}
			if(!flag) mFrequencyVScroll->hide();
		}

		QLayout* SMSTimeMultiDisplay::CreateAudioDisplays()
		{
			mAudioVScroll = new VScrollGroup(this);

			QBoxLayout* displayLayout = new QHBoxLayout;
			QBoxLayout* rulersLayout = new QVBoxLayout(displayLayout);
			QBoxLayout* surferLayout = new QVBoxLayout(displayLayout);
			QBoxLayout* scrollLayout = new QVBoxLayout(displayLayout); 
			// add widgets
			for(unsigned i=MASTER; i <= RESIDUAL; i++)
			{
				rulersLayout->addWidget(mYRulers[i]);
				surferLayout->addWidget(mSurfaces[i]);
			}
			scrollLayout->addWidget(mAudioVScroll);

			return displayLayout;
		}

		QLayout* SMSTimeMultiDisplay::CreateFrequencyDisplays()
		{
			mFrequencyVScroll = new VScrollGroup(this);

			QBoxLayout* displayLayout = new QHBoxLayout;
			QBoxLayout* rulersLayout = new QVBoxLayout(displayLayout);
			QBoxLayout* surferLayout = new QVBoxLayout(displayLayout);
			QBoxLayout* scrollLayout = new QVBoxLayout(displayLayout); 
			// add widgets
			rulersLayout->addWidget(mYRulers[FUNDAMENTAL]);
			rulersLayout->addWidget(mYRulers[SINTRACKS]);

			surferLayout->addWidget(mSurfaces[FUNDAMENTAL]);			
			surferLayout->addWidget(mSurfaces[SINTRACKS]);

			scrollLayout->addWidget(mFrequencyVScroll);

			return displayLayout;
		}

		QLayout* SMSTimeMultiDisplay::CreatePlayer()
		{
			mPlayer = new QtSMSPlayer(this);
			mPlayer->setFixedSize(75,30);
			((QtSMSPlayer*)mPlayer)->SetSlotPlayingTime(mSlotPlayingTimeReceived);
			((QtSMSPlayer*)mPlayer)->SetSlotStopPlaying(mSlotStopPlayingReceived);
			
			middleHole = new QFrame(this);
			middleHole->setFixedSize(50,30);

			mCBPlayList = new QComboBox(this);
			mCBPlayList->setDuplicatesEnabled(false);
			QToolTip::add(mCBPlayList,"Play List");
			
			mLabelsGroup = new TimeSegmentLabelsGroup(this);
			mLabelsGroup->setMinimumSize(186,25);

			QBoxLayout* play_panel = new QHBoxLayout;
			play_panel->addWidget(mPlayer);
			play_panel->addWidget(middleHole);
			play_panel->addWidget(mCBPlayList);
			play_panel->addStretch();
			play_panel->addWidget(mLabelsGroup);

			connect(mCBPlayList,SIGNAL(highlighted(int)),this,SLOT(setCurrentPlayer(int)));

			return play_panel;

		}
		
		void SMSTimeMultiDisplay::ShowPlayer()
		{
			// player
			mPlayer->show();
			// combo
			mCBPlayList->show();
			// labels group
			mLabelsGroup->show();
			// holes
			leftHole->show();
			middleHole->show();
			rightHole->show();
		}

		void SMSTimeMultiDisplay::HidePlayer()
		{
			// player
			mPlayer->hide();
			// combo
			mCBPlayList->hide();
			// labels group
			mLabelsGroup->hide();
			// holes
			leftHole->hide();
			middleHole->hide();
			rightHole->hide();
		}

		void SMSTimeMultiDisplay::InitPlayList()
		{
			mPlayList.push_back("Original Audio");
			mPlayList.push_back("Synthesized Audio");
			mPlayList.push_back("Synthesized Sinusoidal");
			mPlayList.push_back("Synthesized Residual");
			mPlayList.push_back("Fundamental Frequency");
		}

		void SMSTimeMultiDisplay::AddAudioItem(int id)
		{
			if(id < MASTER || id > RESIDUAL) return;
			if(!mAudioPtrs[id]) return;
			bool flag = false;
			if(!mCBPlayList->count())
			{
				flag = true;
				std::vector<const Audio*> data(1);
				data[0] = mAudioPtrs[id];
				((QtSMSPlayer*)mPlayer)->SetData(data,!mHasAudioData);
				((QtSMSPlayer*)mPlayer)->SetCurrentPlayer(AUDIO_PLAYER);
			}
			if(!mHasAudioData) mHasAudioData = true;
			mCBPlayList->insertItem(mPlayList[id].c_str());
			if(flag)
			{
				mCBPlayList->setCurrentItem(0);
				setCurrentPlayer(mCBPlayList->currentItem());
			}
			
		}

		void SMSTimeMultiDisplay::AddFundFreqItem(int id)
		{
			if(!mHasFundamentalData || id != FUNDAMENTAL) return;
			bool flag = false;
			if(!mCBPlayList->count()) 
			{
				flag = true;
				((QtSMSPlayer*)mPlayer)->SetCurrentPlayer(FUND_PLAYER);
			}
			mCBPlayList->insertItem(mPlayList[id].c_str());
			if(flag)
			{
				mCBPlayList->setCurrentItem(0);
				setCurrentPlayer(mCBPlayList->currentItem());
			}
		}

		void SMSTimeMultiDisplay::RemovePlayListItem(int id)
		{
			if(id < MASTER || id > FUNDAMENTAL) return;
			const QString& item = mPlayList[id];
			int index=-1;
			for(int i=0; i < mCBPlayList->count(); i++)
			{
				if(!mCBPlayList->text(i).compare(item))
				{
					index = i;
					break;
				}
			}
			if(index != -1) mCBPlayList->removeItem(index);
			if(mCBPlayList->count()) 
			{
				mCBPlayList->setCurrentItem(0);
				setCurrentPlayer(mCBPlayList->currentItem());
			}
		}

		int SMSTimeMultiDisplay::GetPlayDataId(const QString& item)
		{
			if(item.contains("Original")) return MASTER;
			if(item.contains("Sinusoidal")) return SINUSOIDAL;
			if(item.contains("Residual")) return RESIDUAL;
			if(item.contains("Fundamental")) return FUNDAMENTAL;
			return SYNTHESIZED;
		}

	}
}

// END
