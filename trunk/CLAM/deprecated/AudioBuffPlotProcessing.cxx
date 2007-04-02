#include "NetAudioBuffPlot.hxx"
#include "AudioBuffPlotProcessing.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<AudioBuffPlotProcessing> regtAudioBuffPlotProcessing("AudioBuffPlotProcessing");
	}

	
	void AudioBuffPlotProcessingConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetName("AudioBuffPlotProcessing");
		SetCaption("Audio");
		Setxpos(100);
		Setypos(100);
		Setwidth(200);
		Setheight(75);
	}

	AudioBuffPlotProcessing::AudioBuffPlotProcessing() 
		: mPlot(0)
		, mInput("Audio Input", this)
		, mOwnedPlot(false)
	{
		AudioBuffPlotProcessingConfig cfg;
		Configure(cfg);
	}

	AudioBuffPlotProcessing::AudioBuffPlotProcessing(const AudioBuffPlotProcessingConfig& cfg)
		: mPlot(0)
		, mInput("Audio Input", this)
		, mOwnedPlot(false)
	{
		Configure(cfg);
	}

	AudioBuffPlotProcessing::~AudioBuffPlotProcessing()
	{
		if(mOwnedPlot) delete mPlot;
	}

	bool AudioBuffPlotProcessing::Do()
	{
		bool res = Do(mInput.GetAudio()); 
		mInput.Consume();
		return res;
	}

	bool AudioBuffPlotProcessing::Do(const Audio& audio)
	{
		if(!AbleToExecute()) return true;
		mPlot->SetData(audio);
		if(!mPlot->isVisible()) mPlot->show();
		return true;
	}

	bool AudioBuffPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	bool AudioBuffPlotProcessing::ConcreteStart()
	{
		if(!mPlot) InitAudioBuffPlot();
		return true;
	}

	bool AudioBuffPlotProcessing::ConcreteStop()
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

	void AudioBuffPlotProcessing::SetPlot(VM::NetAudioBuffPlot * plot)
	{
		if (mOwnedPlot) delete mPlot;
		mOwnedPlot = false;
		mPlot = plot;
	}

	void AudioBuffPlotProcessing::InitAudioBuffPlot()
	{
		if (mOwnedPlot) delete mPlot;
		mPlot = new VM::NetAudioBuffPlot();
		mOwnedPlot = true;
		mPlot->Label(mConfig.GetCaption());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetDataColor(VM::VMColor::Green());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
	}

}

// END

