#include <qlayout.h>
#include <qsplitter.h>
#include <qframe.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qtooltip.h>
#include <CLAM/Ruler.hxx>
#include <CLAM/VScrollGroup.hxx>
#include <CLAM/TimeSegmentLabelsGroup.hxx>
#include <CLAM/ColorScale.hxx>
#include <CLAM/DisplaySurface.hxx>
#include <CLAM/QtSMSPlayer.hxx>
#include <CLAM/AudioPlotController.hxx>
#include <CLAM/FundPlotController.hxx>
#include <CLAM/SinTracksPlotController.hxx>
#include <CLAM/SpectrogramPlotController.hxx>
#include <CLAM/SMSTimeMultiDisplay.hxx>

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
			, mIsPlaying(false)
			, mColorSonogram(true)
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
			QSplitter* splitter = new QSplitter(Qt::Vertical,this);

			mAudioDisplaysContainer = new QFrame(splitter);
			mFreqDisplaysContainer = new QFrame(splitter);

			mAudioDisplaysContainer->hide();
			mFreqDisplaysContainer->hide();

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

			CreateAudioDisplays();
			CreateFrequencyDisplays();

			// layout
			QGridLayout* innerLayout = new QGridLayout(this,5,3,1);
			innerLayout->addWidget(GetToggleColorFrame(),0,0);
			innerLayout->addWidget(GetXRuler(),0,1);
			innerLayout->addWidget(topHole,0,2);
			innerLayout->addMultiCellWidget(splitter,1,1,0,2);
			innerLayout->addWidget(sleftHole,2,0);
			innerLayout->addWidget(GetHScrollGroup(),2,1);
			innerLayout->addWidget(srightHole,2,2);
			innerLayout->addMultiCellLayout(CreateSpectrogramPanel(),3,3,0,2);
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
			SynchronizeFocusIn();
			
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
			// build spectrum matrix
			Array<Spectrum> specMtx;
			specMtx.Resize(segment.GetnFrames());
			specMtx.SetSize(segment.GetnFrames());
			for(TIndex i=0; i < segment.GetnFrames(); i++)
			{
				specMtx[i]=segment.GetFrame(i).GetSinusoidalAnalSpectrum();
			}
			double duration = (segment.HasAudio()) 
				? segment.GetAudio().GetSize()/segment.GetAudio().GetSampleRate() 
				: segment.GetEndTime();
			((SpectrogramPlotController*)mControllers[SONOGRAM])->SetData(specMtx,duration);
			FillRightGroupLabels();
			// data to player
			((QtSMSPlayer*)mPlayer)->SetData(segment);
			mHasFundamentalData = true;
			if(mShowOnNewData)
			{
				ShowDisplay(FUNDAMENTAL);
				ShowDisplay(SINTRACKS);
				ShowDisplay(SONOGRAM);
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

		void SMSTimeMultiDisplay::colorSonogram()
		{
			((SpectrogramPlotController*)mControllers[SONOGRAM])->SetRenderingMode(CLAM::VM::ColorSonogram);
			mColorScale->SetScale(((SpectrogramPlotController*)mControllers[SONOGRAM])->GetColorScale(mColorScale->width()));
			mColorSonogram = true;
		}

		void SMSTimeMultiDisplay::blackAndWhiteSonogram()
		{
			((SpectrogramPlotController*)mControllers[SONOGRAM])->SetRenderingMode(CLAM::VM::BlackWhiteSonogram);
			mColorScale->SetScale(((SpectrogramPlotController*)mControllers[SONOGRAM])->GetGrayScale(mColorScale->width()));
			mColorSonogram=false;
		}


		void SMSTimeMultiDisplay::ShowDisplay(int id)
		{
			if(id < MASTER || id > SONOGRAM) return;
			mControllers[id]->enableRendering();
			mYRulers[id]->show();
			mSurfaces[id]->show();
			if(id == SONOGRAM) ShowSpectrogramPanel();
			switch(id)
			{
				case MASTER:
				case SYNTHESIZED:
				case SINUSOIDAL:
				case RESIDUAL:
					if(!mAudioDisplaysContainer->isVisible()) mAudioDisplaysContainer->show();
					if(!mAudioVScroll->isVisible()) mAudioVScroll->show();
					AddAudioItem(id);
					if(!mPlayer->isVisible()) ShowPlayer();
					break;
				case FUNDAMENTAL:
					if(!mFreqDisplaysContainer->isVisible()) mFreqDisplaysContainer->show();
					if(!mFrequencyVScroll->isVisible()) mFrequencyVScroll->show();
					AddFundFreqItem(id);
					if(!mPlayer->isVisible()) ShowPlayer();
					break;
				case SINTRACKS:
				case SONOGRAM:
					if(!mFreqDisplaysContainer->isVisible()) mFreqDisplaysContainer->show();
					if(!mFrequencyVScroll->isVisible()) mFrequencyVScroll->show();
					break;
				default:
					break;
			}
			if(!isVisible()) show();
		}

		void SMSTimeMultiDisplay::HideDisplay(int id)
		{
			if(id < MASTER || id > SONOGRAM) return;
			mControllers[id]->disableRendering();
			mYRulers[id]->hide();
			mSurfaces[id]->hide();
			if(id == SONOGRAM) HideSpectrogramPanel();
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
				case SONOGRAM:
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
			HideDisplays();
			for(unsigned i=0; i < mAudioPtrs.size(); i++)
			{
				if(mAudioPtrs[i])
				{
					mAudioPtrs[i] = 0;
				}
			}
			for(unsigned i=0; i < mControllers.size(); i++) mControllers[i]->ClearMarks();
			mColorSonogram = true;
			mHasMasterData = false;
			mHasAudioData = false;
			mHasFundamentalData = false;
			mCBPlayList->clear();
			((QtSMSPlayer*)mPlayer)->Flush();
		}

		void SMSTimeMultiDisplay::setCurrentTime(double time)
		{
			((AudioPlotController*)mControllers[MASTER])->setSelectedXPos(time);
		}

		void SMSTimeMultiDisplay::CreateControllers()
		{
			for(int i=MASTER; i <= RESIDUAL; i++)
			{
				mControllers.push_back(new AudioPlotController());
			}
			mControllers.push_back(new FundPlotController());
			mControllers.push_back(new SinTracksPlotController());
			mControllers.push_back(new SpectrogramPlotController());
			SetMasterId(MASTER);
			connect(mControllers[MASTER],SIGNAL(selectedXPos(double)),this,SLOT(selectedXPos(double)));
		}

		void SMSTimeMultiDisplay::CreateSurfaces()
		{
			for(int i=MASTER; i <= RESIDUAL; i++)
			{
				mSurfaces.push_back(new DisplaySurface(mAudioDisplaysContainer));
				mYRulers.push_back(new Ruler(mAudioDisplaysContainer,CLAM::VM::Left));
				mSurfaces[i]->setMinimumSize(200,20);
			}
			for(int i=FUNDAMENTAL; i <= SONOGRAM; i++)
			{
				mSurfaces.push_back(new DisplaySurface(mFreqDisplaysContainer));
				mYRulers.push_back(new Ruler(mFreqDisplaysContainer,CLAM::VM::Left));
				mSurfaces[i]->setMinimumSize(200,20);
			}
		}

		void SMSTimeMultiDisplay::SynchronizeDialPos()
		{
			for(int i=MASTER; i <= SONOGRAM; i++)
			{
				SyncDialPos(i);
			}
		}

		void SMSTimeMultiDisplay::SynchronizeInsertMark()
		{
			for(int i=MASTER; i <= SONOGRAM; i++)
			{
				SyncInsertMarkBySampleIndex(i);
			}
		}

		void SMSTimeMultiDisplay::SynchronizeRemoveMark()
		{
			for(int i=MASTER; i <= SONOGRAM; i++)
			{
				SyncRemoveMarkBySampleIndex(i);
			}
		}

		void SMSTimeMultiDisplay::SynchronizeUpdateMark()
		{
			for(int i=MASTER; i <= SONOGRAM; i++)
			{
				SyncUpdateMarkBySampleIndex(i);
			}
		}

		void SMSTimeMultiDisplay::SynchronizeUpdateTag()
		{
			for(int i=MASTER; i <= SONOGRAM; i++)
			{
				SyncUpdateTag(i);
			}
		}

		void SMSTimeMultiDisplay::DisplayBackgroundBlack()
		{
			if(mControllers.size() >= SINTRACKS) ((SinTracksPlotController*)mControllers[SINTRACKS])->SaveScreen();
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetMarksColor(VMColor::Orange());
			SetBackgroundColor(VMColor::Black());
		}

		void SMSTimeMultiDisplay::DisplayBackgroundWhite()
		{
			if(mControllers.size() >= SINTRACKS) ((SinTracksPlotController*)mControllers[SINTRACKS])->SaveScreen();
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

		void SMSTimeMultiDisplay::spectrogram(bool over)
		{
			(over) ? emit dataType("sonogram") : emit dataType("");
		}


		void SMSTimeMultiDisplay::updateRegion(MediaTime time)
		{
			if(time.GetEnd() <= time.GetBegin()) return;
			mPlayer->stop();
			mPlayer->SetPlaySegment(time);
			mLabelsGroup->UpdateLabels(time);
		}

		void SMSTimeMultiDisplay::updateLabels(QString freq, QString dB, QString slice)
		{
			mFrequency->setText(freq);
			mDecibels->setText(dB);
			mSlice->setText(slice);
		}

		void SMSTimeMultiDisplay::updateColorScale(int width)
		{
			if(mColorSonogram)
				mColorScale->SetScale(((SpectrogramPlotController*)mControllers[SONOGRAM])->GetColorScale(width));
			else
				mColorScale->SetScale(((SpectrogramPlotController*)mControllers[SONOGRAM])->GetGrayScale(width));
		
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

		void SMSTimeMultiDisplay::selectedXPos(double xpos)
		{
			emit currentTime(float(xpos));
		}

		void SMSTimeMultiDisplay::SynchronizeVZoom()
		{
			for(int i=SYNTHESIZED; i <= RESIDUAL; i++)
			{
				connect(mControllers[MASTER],SIGNAL(yRulerRange(double,double)),
						mControllers[i],SLOT(setVBounds(double,double)));
			}
			for(int i=SINTRACKS; i <= SONOGRAM; i++)
			{
				connect(mControllers[FUNDAMENTAL],SIGNAL(yRulerRange(double,double)),
						mControllers[i],SLOT(setVBounds(double,double)));
			}		
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
			for(int i=FUNDAMENTAL; i <= SONOGRAM; i++)
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
			for(int i=FUNDAMENTAL; i <= SONOGRAM; i++)
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
			connect(mControllers[SONOGRAM],SIGNAL(mouseOverDisplay(bool)),this,SLOT(spectrogram(bool)));
		}

		void SMSTimeMultiDisplay::PlayingTime(TData time)
		{
			((AudioPlotController*)mControllers[MASTER])->updateTimePos(time);
			if(!mIsPlaying)
			{
				mIsPlaying = true;
				emit startPlaying();
			}
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
			mIsPlaying = false;
			emit stopPlaying();
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
			if(!flag) 
			{
				mAudioVScroll->hide();
				mAudioDisplaysContainer->hide();
			}
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
			for(int i=FUNDAMENTAL; i <= SONOGRAM; i++)
			{
				if(mYRulers[i]->isVisible()) 
				{
					flag = true;
					break;
				}
			}
			if(!flag) 
			{
				mFrequencyVScroll->hide();
				mFreqDisplaysContainer->hide();
			}
		}

	    void SMSTimeMultiDisplay::CreateAudioDisplays()
		{
			mAudioVScroll = new VScrollGroup(mAudioDisplaysContainer);

			QBoxLayout* displayLayout = new QHBoxLayout(mAudioDisplaysContainer,0,1);
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
		}

		void SMSTimeMultiDisplay::CreateFrequencyDisplays()
		{
			mFrequencyVScroll = new VScrollGroup(mFreqDisplaysContainer);

			QBoxLayout* displayLayout = new QHBoxLayout(mFreqDisplaysContainer,0,1);
			QBoxLayout* rulersLayout = new QVBoxLayout(displayLayout);
			QBoxLayout* surferLayout = new QVBoxLayout(displayLayout);
			QBoxLayout* scrollLayout = new QVBoxLayout(displayLayout); 
			// add widgets
			rulersLayout->addWidget(mYRulers[FUNDAMENTAL]);
			rulersLayout->addWidget(mYRulers[SINTRACKS]);
			rulersLayout->addWidget(mYRulers[SONOGRAM]);

			surferLayout->addWidget(mSurfaces[FUNDAMENTAL]);			
			surferLayout->addWidget(mSurfaces[SINTRACKS]);
			surferLayout->addWidget(mSurfaces[SONOGRAM]);

			scrollLayout->addWidget(mFrequencyVScroll);
		}

		QLayout* SMSTimeMultiDisplay::CreateSpectrogramPanel()
		{
			// left panel
			leftGroup = new QFrame(this);
			leftGroup->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			QFontMetrics fm(font());
			int width = fm.width("Frequency (Hz): 00000")+6;
			leftGroup->setFixedSize(width,66);

			QBoxLayout* lfMainLayout = new QVBoxLayout(leftGroup);
			lfMainLayout->setMargin(3);
			QBoxLayout* lfInnerTopLayout = new QHBoxLayout(lfMainLayout);
			QBoxLayout* lfInnerMiddleLayout = new QHBoxLayout(lfMainLayout);
			QBoxLayout* lfInnerBottomLayout = new QHBoxLayout(lfMainLayout);

			freqTxtLabel = new QLabel(leftGroup);
			freqTxtLabel->setText("Frequency (Hz): ");
			mFrequency = new QLabel(leftGroup);
			mFrequency->setFixedWidth(leftGroup->width()-freqTxtLabel->width()-6);
			mFrequency->setAlignment(Qt::AlignRight);
			mFrequency->setText("--");
			lfInnerTopLayout->addWidget(freqTxtLabel);
			lfInnerTopLayout->addWidget(mFrequency);
	
			decibelTxtLabel = new QLabel(leftGroup);
			decibelTxtLabel->setText("Decibels (dB): ");
			mDecibels = new QLabel(leftGroup);
			mDecibels->setFixedWidth(leftGroup->width()-decibelTxtLabel->width()-6);
			mDecibels->setAlignment(Qt::AlignRight);
			mDecibels->setText("--");
			lfInnerMiddleLayout->addWidget(decibelTxtLabel);
			lfInnerMiddleLayout->addWidget(mDecibels);

			sliceTxtLabel = new QLabel(leftGroup);
			sliceTxtLabel->setText("Slice: ");
			mSlice = new QLabel(leftGroup);
			mSlice->setFixedWidth(leftGroup->width()-sliceTxtLabel->width()-6);
			mSlice->setAlignment(Qt::AlignRight);
			mSlice->setText("--");
			lfInnerBottomLayout->addWidget(sliceTxtLabel);
			lfInnerBottomLayout->addWidget(mSlice);

			// right panel
			rightGroup = new QFrame(this);
			rightGroup->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			rightGroup->setFixedHeight(66);

			QBoxLayout* rfMainLayout = new QVBoxLayout(rightGroup);
			rfMainLayout->setMargin(3);
			QBoxLayout* rfInnerTopLayout = new QHBoxLayout(rfMainLayout);
			rfInnerTopLayout->setSpacing(5);
			QBoxLayout* rfInnerMiddleLayout = new QHBoxLayout(rfMainLayout);
			QBoxLayout* rfInnerBottomLayout = new QHBoxLayout(rfMainLayout);
			rfInnerBottomLayout->setSpacing(3);

			mFFTSize = new QLabel(rightGroup);
			mTotalSlices = new QLabel(rightGroup);
			rfInnerTopLayout->addWidget(mFFTSize);
			rfInnerTopLayout->addWidget(mTotalSlices);
			rfInnerTopLayout->addStretch();

			mSpectralRange = new QLabel(rightGroup);
			rfInnerMiddleLayout->addWidget(mSpectralRange);
			rfInnerMiddleLayout->addStretch();

				leftTag = new QLabel(rightGroup);
			leftTag->setText("0 dB");
			rightTag = new QLabel(rightGroup);
			rightTag->setText("-150 dB");
			mColorScale = new ColorScale(rightGroup);
			rfInnerBottomLayout->addWidget(leftTag);
			rfInnerBottomLayout->addWidget(mColorScale,1);
			rfInnerBottomLayout->addWidget(rightTag);
			rfInnerBottomLayout->addStretch();

			// main layout
			QBoxLayout* mainLayout = new QHBoxLayout;
		   
			mainLayout->addWidget(leftGroup);
			mainLayout->addWidget(rightGroup);

			// connectiona
			connect(((SpectrogramPlotController*)mControllers[SONOGRAM]),SIGNAL(sendLabels(QString,QString,QString)),
					this,SLOT(updateLabels(QString,QString,QString)));
			connect(mColorScale,SIGNAL(widthChanged(int)),this,SLOT(updateColorScale(int)));

			return mainLayout;
		}

		QLayout* SMSTimeMultiDisplay::CreatePlayer()
		{
			mPlayer = new QtSMSPlayer(this);
			mPlayer->setFixedSize(75,30);
			((QtSMSPlayer*)mPlayer)->SetSlotPlayingTime(mSlotPlayingTimeReceived);
			((QtSMSPlayer*)mPlayer)->SetSlotStopPlaying(mSlotStopPlayingReceived);
			AddToPlayList();

			middleHole = new QFrame(this);
			middleHole->setFixedSize(50,30);

			mCBPlayList = new QComboBox(this);

			QFontMetrics fm(mCBPlayList->font());
			int cbox_width=fm.width("Synthesized Sinusoidal")+35;
			mCBPlayList->setFixedWidth(cbox_width);

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
			Stop();
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

		void SMSTimeMultiDisplay::ShowSpectrogramPanel()
		{
			leftGroup->show();
			rightGroup->show();
			freqTxtLabel->show();
			mFrequency->show();
			decibelTxtLabel->show();
			mDecibels->show();
			sliceTxtLabel->show();
			mSlice->show();
			mFFTSize->show();
			mTotalSlices->show();
			mSpectralRange->show();
			leftTag->show();
			rightTag->show();
			mColorScale->show();
		}

		void SMSTimeMultiDisplay::HideSpectrogramPanel()
		{
			leftGroup->hide();
			rightGroup->hide();
			freqTxtLabel->hide();
			mFrequency->hide();
			decibelTxtLabel->hide();
			mDecibels->hide();
			sliceTxtLabel->hide();
			mSlice->hide();
			mFFTSize->hide();
			mTotalSlices->hide();
			mSpectralRange->hide();
			leftTag->hide();
			rightTag->hide();
			mColorScale->hide();
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
			QString item = mPlayList[id].c_str();
			std::vector<std::string> tmpLst;
			for(int i=0; i < mCBPlayList->count(); i++)
			{
				if(!mCBPlayList->text(i).compare(item)) continue;
				tmpLst.push_back(mCBPlayList->text(i).ascii());
			}
			mCBPlayList->clear();
			for(unsigned i=0; i < tmpLst.size(); i++)
			{
				mCBPlayList->insertItem(tmpLst[i].c_str());
			}
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

		void SMSTimeMultiDisplay::FillRightGroupLabels()
		{
			QString fftSize = "FFT Size: "+QString::number(((SpectrogramPlotController*)mControllers[SONOGRAM])->FFTSize());
			QString totalSlices = "Slices: "+QString::number(((SpectrogramPlotController*)mControllers[SONOGRAM])->TotalSlices());
			QString spectralRange = "Spectral Range (Hz): "+QString::number(((SpectrogramPlotController*)mControllers[SONOGRAM])->SpectralRange());
			mFFTSize->setText(fftSize);
			mTotalSlices->setText(totalSlices);
			mSpectralRange->setText(spectralRange);
		}

	}
}

// END

