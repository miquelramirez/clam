#include <qlayout.h>
#include <qsplitter.h>
#include <qframe.h>
#include <CLAM/Navigator.hxx>
#include <CLAM/SMSTimeMultiDisplay.hxx>
#include <CLAM/SMSFreqMultiDisplay.hxx>
#include "Engine.hxx"
#include "ViewManager.hxx"

namespace QtSMS
{
	ViewManager* ViewManager::mInstance = 0;

	ViewManager* ViewManager::Instance()
	{
		if(!mInstance)
		{
			mInstance = new ViewManager;
		}
		return mInstance;
	}

	ViewManager::ViewManager()
		: mCurrentTime(0.0f)
		, mLastFrame(0)
		, mSentinel(false)
		, mHasTimeViewFocus(false)
		, mHasSpecViewFocus(false)
		, mHasTransformedSegment(false)
	{}

	ViewManager::~ViewManager(){ mInstance=0; mPlotList.clear(); }

	void ViewManager::SetAudio(eView id)
	{
		bool hasAudio = true;
		switch (id)
		{
			case ORIGINAL_AUDIO:
				((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->SetOriginalAudio(
					Engine::Instance()->GetOriginalSegment().GetAudio() );
				break;
			case SYNTH_AUDIO:
				((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->SetSynthesizedAudio(
					Engine::Instance()->GetSynthesizedSound() );
				break;
			case SYNTH_SINUSOIDAL:
				((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->SetSynthesizedSinusoidal(
					Engine::Instance()->GetSynthesizedSinusoidal() );
				break;
			case SYNTH_RESIDUAL:
				((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->SetSynthesizedResidual(
					Engine::Instance()->GetSynthesizedResidual() );
				break;
			default:
				hasAudio = false;
				break;
		}
		emit sampleRateDuration(GetSampleRateDuration(hasAudio));
	}

	void ViewManager::SetAnalyzedData()
	{
		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->SetAnalyzedSegment(
			Engine::Instance()->GetOriginalSegment() );

		UpdateSpectrumView(0,false);

		mFrameNavigator->setRange(0,Engine::Instance()->GetOriginalSegment().GetnFrames()-1);
		mFrameNavigator->setValue(0);
		SetNavigatorEnabled(true);
		mHasTransformedSegment=false;
	}

	void ViewManager::SetTransformedData()
	{
		if(Engine::Instance()->GetTransformedSegment().HasAudio())
		{
			Engine::Instance()->GetTransformedSegment().RemoveAudio();
			Engine::Instance()->GetTransformedSegment().UpdateData();
		}
		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->SetAnalyzedSegment(
			Engine::Instance()->GetTransformedSegment());

		UpdateSpectrumView(0,false);

		mFrameNavigator->setRange(0,Engine::Instance()->GetTransformedSegment().GetnFrames()-1);
		mFrameNavigator->setValue(0);
		SetNavigatorEnabled(true);
		mHasTransformedSegment=true;
	}

	void ViewManager::SetSynthesizedData()
	{
		for(int i=SYNTH_AUDIO; i <= SYNTH_RESIDUAL; i++)
		{
			SetAudio((eView)i);
		}
		SetNavigatorEnabled(true);
	}
		
	void ViewManager::ShowConcreteView(eView group_id, eView view_id)
	{
		bool isVisible = true;
		switch(group_id)
		{
			case TIME_GROUP_VIEW:
				if(!mTimeViewContainer->isVisible()) mTimeViewContainer->show();
				((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->ShowDisplay(view_id);
				break;
			case SPECTRUM_GROUP_VIEW:
				if(!mSpecViewContainer->isVisible()) mSpecViewContainer->show();
				((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->ShowDisplay(view_id);
				break;
			default:
				isVisible = false;
				break;
		}
		if(isVisible)
		{
			if(!mFrameNavigator->isVisible())
			{
				SetNavigatorVisible(true);
			}
		}
	}

	void ViewManager::HideConcreteView(eView group_id, eView view_id)
	{
		switch(group_id)
		{
			case TIME_GROUP_VIEW:
				((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->HideDisplay(view_id);
				break;
			case SPECTRUM_GROUP_VIEW:
				((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->HideDisplay(view_id);
				break;
			default:
				break;
		}
		if(!((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->isVisible())
		{
			mTimeViewContainer->hide();
		}
		if(!((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->isVisible())
		{
			mSpecViewContainer->hide();
		}
		bool isVisible = ( ((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->isVisible() ||
						   ((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->isVisible() );
		if(!isVisible)
		{
			SetNavigatorVisible(false);
		}
	}

	void ViewManager::SetBackgroundBlack()
	{
		for(unsigned i=0; i < mPlotList.size(); i++)
		{
			mPlotList[i]->SetToggleColorOn(true);
			mPlotList[i]->switchColors();
		}
	}

	void ViewManager::SetBackgroundWhite()
	{
		for(unsigned i=0; i < mPlotList.size(); i++)
		{
			mPlotList[i]->SetToggleColorOn(false);
			mPlotList[i]->switchColors();
		}
	}

	void ViewManager::ColorSonogram()
	{
		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->colorSonogram();
	}

	void ViewManager::BlackAndWhiteSonogram()
	{
		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->blackAndWhiteSonogram();
	}

	void ViewManager::Flush()
	{
		for(int i=ORIGINAL_AUDIO; i <= SIN_TRACKS; i++ )
		{
			((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->HideDisplay(i);
		}
		for(int i=SPEC_AND_PEAKS; i <= RES_SPEC; i++)
		{
			((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->HideDisplay(i);
		}
		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->Flush();
		((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->Flush();
		mFrameNavigator->setValue(0);
		mTimeViewContainer->hide();
		mSpecViewContainer->hide();
		mFrameNavigator->setEnabled(false);
		SetNavigatorVisible(false);
		emit sampleRateDuration(GetSampleRateDuration(false));
	}
	
	QWidget* ViewManager::GetView(QWidget* parent)
	{
		QWidget* view = new QWidget(parent);
		QSplitter* splitter = new QSplitter(Qt::Vertical,view);

		mTimeViewContainer = new QFrame(splitter);
		mSpecViewContainer = new QFrame(splitter);

		mTimeViewContainer->hide();
		mSpecViewContainer->hide();

		mPlotList.push_back(new CLAM::VM::SMSTimeMultiDisplay(mTimeViewContainer));
		mPlotList.push_back(new CLAM::VM::SMSFreqMultiDisplay(mSpecViewContainer));

		timeViewLayout = new QHBoxLayout(mTimeViewContainer);
		specViewLayout = new QHBoxLayout(mSpecViewContainer);

		timeViewLayout->addWidget(mPlotList[TIME_GROUP_VIEW]);
		specViewLayout->addWidget(mPlotList[SPECTRUM_GROUP_VIEW]);

		// navigator
		mFrameNavigator = new CLAM::VM::Navigator(view);
		mFrameNavigator->setEnabled(false);
		mFrameNavigator->hide();
		hole = new QFrame(view);
		hole->setFixedSize(GetHoleWidth(),mFrameNavigator->height());
		hole->hide();

		SetBackgroundBlack();

		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->ShowDisplayOnNewData(false);
		((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->ShowDisplayOnNewData(false);

		QBoxLayout* navigator = new QHBoxLayout;
		navigator->addWidget(hole);
		navigator->addWidget(mFrameNavigator);
		navigator->addStretch();

		QBoxLayout* layout = new QVBoxLayout(view);
		layout->addWidget(splitter);
		layout->addLayout(navigator);

		connect(((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW]),
				SIGNAL(currentTime(float)),SLOT(onNewTime(float)));
		connect(((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW]),
				SIGNAL(startPlaying()),SLOT(onStartPlaying())); 
		connect(((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW]),
				SIGNAL(stopPlaying()),SLOT(onStopPlaying())); 
		connect(((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW]),
				SIGNAL(dataType(QString)),SIGNAL(dataType(QString)));
		connect(((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW]),
				SIGNAL(dataType(QString)),SIGNAL(dataType(QString)));
		connect(((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW]),
				SIGNAL(focusIn()),SLOT(onTimeViewFocusIn()));
		connect(((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW]),
				SIGNAL(focusIn()),SLOT(onSpectrumViewFocusIn()));
		connect(mFrameNavigator,SIGNAL(valueChanged(int)),SLOT(onNewFrame(int)));

		return view;
	}

	void ViewManager::onNewTime(float time)
	{
		if(mSentinel) 
		{
			mSentinel = false;
			return;
		}
		mCurrentTime = time;
		if(!Engine::Instance()->GetState().GetHasAnalysis()) return;
		int current_frame = GetFrameNumber();
		if(current_frame != mLastFrame && Engine::Instance()->GetState().GetHasAnalysis())
		{			
			mLastFrame = current_frame;
			UpdateSpectrumView(mLastFrame);
			mFrameNavigator->setValue(mLastFrame);
		}
	
	}

	void ViewManager::onNewFrame(int frameNumber)
	{
		if(frameNumber == mLastFrame) return;
		mSentinel = true;
		mLastFrame = frameNumber;
		UpdateSpectrumView(mLastFrame);
		CLAM::Segment segment = (mHasTransformedSegment) ? Engine::Instance()->GetTransformedSegment() : Engine::Instance()->GetOriginalSegment();
		float duration = float(segment.GetEndTime());
		float n_frames = float(segment.GetnFrames());
		float time = frameNumber*duration/n_frames;
		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->setCurrentTime(double(time));
	}

	void ViewManager::onStartPlaying()
	{
		SetNavigatorEnabled(false);
	}
	
	void ViewManager::onStopPlaying()
	{
		SetNavigatorEnabled(true);
	}

	void ViewManager::onTimeViewFocusIn()
	{
		if(mHasTimeViewFocus) return;
		mHasTimeViewFocus = true;
		mHasSpecViewFocus = false;
		specViewLayout->setMargin(0);
		mSpecViewContainer->setLineWidth(0);
		mSpecViewContainer->setFrameStyle(QFrame::MenuBarPanel | QFrame::Plain);
		timeViewLayout->setMargin(4);
		mTimeViewContainer->setLineWidth(4);
		mTimeViewContainer->setFrameStyle(QFrame::Box | QFrame::Plain);
	}

	void ViewManager::onSpectrumViewFocusIn()
	{
		if(mHasSpecViewFocus) return;
		mHasSpecViewFocus = true;
		mHasTimeViewFocus = false;
		timeViewLayout->setMargin(0);
		mTimeViewContainer->setLineWidth(0);
		mTimeViewContainer->setFrameStyle(QFrame::MenuBarPanel | QFrame::Plain);
		specViewLayout->setMargin(4);
		mSpecViewContainer->setLineWidth(4);
		mSpecViewContainer->setFrameStyle(QFrame::Box | QFrame::Plain);
	}

	int ViewManager::GetFrameNumber()
	{
		CLAM::Segment segment = (mHasTransformedSegment) ? Engine::Instance()->GetTransformedSegment() : Engine::Instance()->GetOriginalSegment();
		float duration = float(segment.GetEndTime());
		float n_frames = float(segment.GetnFrames());
		int frame_number = int(mCurrentTime*n_frames/duration);
		return (frame_number >= int(n_frames)) ? --frame_number : frame_number;
	}

	void ViewManager::UpdateSpectrumView(int frameNumber, bool update)
	{
		CLAM::Frame frame = (mHasTransformedSegment) ? Engine::Instance()->GetTransformedSegment().GetFrame(frameNumber) : Engine::Instance()->GetOriginalSegment().GetFrame(frameNumber);

		((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->SetSpectrumAndPeaks(
			frame.GetSinusoidalAnalSpectrum(),
			frame.GetSpectralPeakArray(), update );
		
		// TODO: At this moment the frame hasn't sinusoidal spectrum attribute, change it later
		((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->SetSinusoidalSpectrum(
			frame.GetSinusoidalAnalSpectrum(), update );
		
		((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->SetResidualSpectrum(
			frame.GetResidualSpec(), update );
	}

	void ViewManager::SetNavigatorEnabled(bool enabled)
	{
		if(!Engine::Instance()->GetState().GetHasAnalysis()) return;
		mFrameNavigator->setEnabled(enabled);
	}

	void ViewManager::SetNavigatorVisible(bool visible)
	{
		if(visible)
		{
			hole->show();
			mFrameNavigator->show();
		}
		else
		{
			hole->hide();
			mFrameNavigator->hide();
		}
	}

	int ViewManager::GetHoleWidth()
	{
		// no comment
		QFont f;
		f.setFamily("fixed");
		f.setPointSize(10);
		f.setBold(true);
		f.setStyleHint(QFont::Courier,QFont::NoAntialias);
		QFontMetrics fm(f);       
		return fm.width("X:-0.0e+00")+1;
	}

	QString ViewManager::GetSampleRateDuration(bool hasAudio)
	{
		if(!hasAudio) return "sr: --      dur: --:--,--- ";
		float sr = float(Engine::Instance()->GetOriginalSegment().GetAudio().GetSampleRate());
		float dur = float(Engine::Instance()->GetOriginalSegment().GetAudio().GetDuration())/1000.0f;
		return "sr: "+QString::number(sr,'f',0)+" Hz  "+"dur: "+TimeToStr(dur)+" ";
	}

	QString ViewManager::TimeToStr(float time)
	{
		QString s("");
		int tmp = int(time*1000);
		int sec = tmp/1000;
		int min = sec/60;
		sec %= 60;
		int msec = tmp%1000;
		s = s.sprintf("%02d:%02d,%03d",min,sec,msec);
		return s;
	}

	void ViewManager::HideDisplays()
	{
		for(int i=SYNTH_AUDIO; i <= SONOGRAM; i++)
		{
			HideConcreteView(TIME_GROUP_VIEW,(eView)i);
		}
		for(int i=SPEC_AND_PEAKS; i <= RES_SPEC; i++)
		{
			HideConcreteView(SPECTRUM_GROUP_VIEW,(eView)i);
		}
	}

	void ViewManager::Reset()
	{
		SetNavigatorEnabled(false);
		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->Stop();
		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->setCurrentTime(0.0);
	}

}

// END

