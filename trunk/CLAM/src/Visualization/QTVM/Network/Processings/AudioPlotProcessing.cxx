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
		: mInput("Audio Input", this)
	{
		AudioPlotProcessingConfig cfg;
		Configure(cfg);

		mPlot = NULL;
	}

	AudioPlotProcessing::AudioPlotProcessing(const AudioPlotProcessingConfig& cfg)
		: mInput("Audio Input", this)
	{
		Configure(cfg);
		
		mPlot = NULL;
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
		mPlot->Label(mConfig.GetCaption().c_str());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetDataColor(VM::VMColor::Green());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
	}

	bool AudioPlotProcessing::ConcreteStart()
	{
		if(!mPlot) InitAudioPlot();
		return true;
	}

	bool AudioPlotProcessing::ConcreteStop()
	{
		if(mPlot) delete mPlot;
		mPlot = NULL;
		return true;
	}
}

// END

