#include "NetSpectrumPlot.hxx"
#include "SpectrumPlotProcessing.hxx"

namespace CLAM
{
	void SpectrumPlotProcessingConfig::DefaultInit()
	{
		AddAll();       
		UpdateData();	
		SetName("SpectrumPlotProcessing");
		SetCaption("Spectrum");
		Setxpos(100);
		Setypos(100);
		Setwidth(200);
		Setheight(75);
	}

	SpectrumPlotProcessing::SpectrumPlotProcessing() 
		: mInput("Spectrum Input", this)
	{
		SpectrumPlotProcessingConfig cfg;
		Configure(cfg);

		mPlot = NULL;
	}

	SpectrumPlotProcessing::SpectrumPlotProcessing(const SpectrumPlotProcessingConfig& cfg)
		: mInput("Spectrum Input", this)
	{
		Configure(cfg);

		mPlot = NULL;
	}

	SpectrumPlotProcessing::~SpectrumPlotProcessing()
	{
		if(mPlot) delete mPlot;
	}


	bool SpectrumPlotProcessing::Do()
	{
		bool res = Do(mInput.GetData()); 
		mInput.Consume();
		return res;
	}

	bool SpectrumPlotProcessing::Do(const Spectrum& spec)
	{
		if(!AbleToExecute()) return true;
		mPlot->SetData(spec);
		if(!mPlot->isVisible()) mPlot->Show();
		return true;
	}

	bool SpectrumPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	void SpectrumPlotProcessing::InitSpectrumPlot()
	{
		mPlot = new VM::NetSpectrumPlot();
		mPlot->Label(mConfig.GetCaption());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetDataColor(VM::VMColor::Green());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
	}

	bool SpectrumPlotProcessing::ConcreteStart()
	{
		if(!mPlot) InitSpectrumPlot();
		return true;
	}

	bool SpectrumPlotProcessing::ConcreteStop()
	{
		if(mPlot) 
		{
		    mPlot->StopRendering();
		    delete mPlot;
		    mPlot = NULL;
		}
		return true;
	}
}

// END

