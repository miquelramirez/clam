#include "NetAudioPlot.hxx"
#include "AudioPlotProcessing.hxx"

namespace CLAM
{
	void AudioPlotProcessingConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetName("AudioPlotProcessing");
		SetCaption("Audio");
		Setxpos(100);
		Setypos(100);
		Setwidth(200);
		Setheight(75);
	}

	AudioPlotProcessing::AudioPlotProcessing() 
		: mPlot(0)
		, mInput("Audio Input", this)
		, mOwnedPlot(false)
	{
		AudioPlotProcessingConfig cfg;
		Configure(cfg);
	}

	AudioPlotProcessing::AudioPlotProcessing(const AudioPlotProcessingConfig& cfg)
		: mPlot(0)
		, mInput("Audio Input", this)
		, mOwnedPlot(false)
	{
		Configure(cfg);
	}

	AudioPlotProcessing::~AudioPlotProcessing()
	{
		if(mOwnedPlot) delete mPlot;
	}

	bool AudioPlotProcessing::Do()
	{
		bool res = Do(mInput.GetAudio()); 
		mInput.Consume();
		return res;
	}

	bool AudioPlotProcessing::Do(const Audio& audio)
	{
		if(!AbleToExecute()) return true;
		mPlot->SetData(audio);
		if(!mPlot->isVisible()) mPlot->Show();
		return true;
	}

	bool AudioPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	bool AudioPlotProcessing::ConcreteStart()
	{
		if(!mPlot) InitAudioPlot();
		return true;
	}

	bool AudioPlotProcessing::ConcreteStop()
	{
		if(mPlot) 
		{
			mPlot->StopRendering();
			if (mOwnedPlot)
			{
				delete mPlot;
				mPlot = NULL;
			}
		}
		return true;
	}

	void AudioPlotProcessing::SetPlot(VM::NetAudioPlot * plot)
	{
		if (mOwnedPlot) delete mPlot;
		mOwnedPlot = false;
		mPlot = plot;
	}

	void AudioPlotProcessing::InitAudioPlot()
	{
		if (mOwnedPlot) delete mPlot;
		mPlot = new VM::NetAudioPlot();
		mOwnedPlot = true;
		mPlot->Label(mConfig.GetCaption());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetDataColor(VM::VMColor::Green());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
	}

}

// END

