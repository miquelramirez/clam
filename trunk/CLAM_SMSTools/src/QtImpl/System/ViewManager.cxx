#include <qlayout.h>
#include <qwidget.h>
#include "Message.hxx"
#include "SMSTimeMultiDisplay.hxx"
#include "SMSFreqMultiDisplay.hxx"
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

	ViewManager::ViewManager():mCurrentTime(0.0f),mLastFrame(0){}
	ViewManager::~ViewManager(){ mInstance=0; mPlotList.clear(); }

	void ViewManager::SetAudio(eView id)
	{
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
				break;
		}
	}

	void ViewManager::SetAnalyzedData()
	{
		CLAM::Segment segment = Engine::Instance()->GetOriginalSegment();
		CLAM::Frame frame = segment.GetFrame(GetFrameNumber());

		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->SetAnalyzedSegment(segment);
		 
		((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->SetSpectrumAndPeaks(
			frame.GetSinusoidalAnalSpectrum(),
			frame.GetSpectralPeakArray() );
		
		// TODO: At this moment the frame hasn't sinusoidal spectrum attribute, change it later
		((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->SetSinusoidalSpectrum(
			frame.GetSinusoidalAnalSpectrum() );
		
		((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->SetResidualSpectrum(frame.GetResidualSpec());
	}

	void ViewManager::SetSynthesizedData()
	{
		for(int i=SYNTH_AUDIO; i <= SYNTH_RESIDUAL; i++)
		{
			SetAudio((eView)i);
		}
	}
		
	void ViewManager::ShowConcreteView(eView group_id, eView view_id)
	{
		switch(group_id)
		{
			case TIME_GROUP_VIEW:
				((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->ShowDisplay(view_id);
				break;
			case SPECTRUM_GROUP_VIEW:
				((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->ShowDisplay(view_id);
				break;
			default:
				break;
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
	}
	
	QWidget* ViewManager::GetView(QWidget* parent)
	{
		QWidget* view = new QWidget(parent);

		mPlotList.push_back(new CLAM::VM::SMSTimeMultiDisplay(view));
		mPlotList.push_back(new CLAM::VM::SMSFreqMultiDisplay(view));

		for(unsigned i=0; i < mPlotList.size(); i++)
		{
			mPlotList[i]->SetToggleColorOn(true);
			mPlotList[i]->switchColors();
		}
		((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW])->ShowDisplayOnNewData(false);
		((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->ShowDisplayOnNewData(false);
		
		QBoxLayout* layout = new QVBoxLayout(view);
		for(unsigned i=0; i < mPlotList.size(); i++)
		{
			layout->addWidget(mPlotList[i]);
		}
		
		connect(((CLAM::VM::SMSTimeMultiDisplay*)mPlotList[TIME_GROUP_VIEW]),
				SIGNAL(currentTime(float)),this,SLOT(onNewTime(float)));

		return view;
	}

	void ViewManager::onNewTime(float time)
	{
		mCurrentTime = time;
		if(!Engine::Instance()->State().GetHasAnalysis()) return;
		int current_frame = GetFrameNumber();
		if(current_frame != mLastFrame)
		{
			mLastFrame = current_frame;
			CLAM::Frame frame = Engine::Instance()->GetOriginalSegment().GetFrame(mLastFrame);

			((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->SetSpectrumAndPeaks(
				frame.GetSinusoidalAnalSpectrum(),
				frame.GetSpectralPeakArray() );
		
			// TODO: At this moment the frame hasn't sinusoidal spectrum attribute, change it later
			((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->SetSinusoidalSpectrum(
				frame.GetSinusoidalAnalSpectrum() );
		
			((CLAM::VM::SMSFreqMultiDisplay*)mPlotList[SPECTRUM_GROUP_VIEW])->SetResidualSpectrum(
				frame.GetResidualSpec() );
		}
	}

	int ViewManager::GetFrameNumber()
	{
		CLAM::Segment segment = Engine::Instance()->GetOriginalSegment();
		float duration = float(segment.GetEndTime());
		float n_frames = float(segment.GetnFrames());
		int current_frame = int(mCurrentTime*n_frames/duration);
		if(current_frame >= n_frames) current_frame--;
		return current_frame;
	}

}

// END
