#include "NetAudioPlot.hxx"
#include "AudioPlotProcessing.hxx"

namespace CLAM
{
	void AudioPlotProcessingConfig::DefaultInit()
	{
		AddAll();       
		UpdateData();	
		SetName("AudioPlotProcessing");
	}

	AudioPlotProcessing::AudioPlotProcessing() : mPlot(0)
	{
		AudioPlotProcessingConfig cfg;
		Configure(cfg);
	}

	AudioPlotProcessing::AudioPlotProcessing(const AudioPlotProcessingConfig& cfg)
		: mPlot(0)
	{
		Configure(cfg);
	}

	AudioPlotProcessing::~AudioPlotProcessing()
	{
		if(mPlot) delete mPlot;
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
		if(!mPlot) InitAudioPlot();
		mPlot->SetData(audio);
		if(!mPlot->isVisible()) mPlot->Show();
		return true;
	}

	bool AudioPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	void AudioPlotProcessing::InitAudioPlot()
	{
		mPlot = new VM::NetAudioPlot();
		mPlot->Label("Audio");
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetDataColor(VM::VMColor::Green());
		mPlot->Geometry(100,100,300,150);
	}
}

// END

