#include "NetSpectrumPlot.hxx"
#include "SpectrumPlotProcessing.hxx"

namespace CLAM
{
	void SpectrumPlotProcessingConfig::DefaultInit()
	{
		AddAll();       
		UpdateData();	
		SetName("SpectrumPlotProcessing");
	}

	SpectrumPlotProcessing::SpectrumPlotProcessing() 
		: mInput("Spectrum Input", this)
	{
		SpectrumPlotProcessingConfig cfg;
		Configure(cfg);

		InitSpectrumPlot();
	}

	SpectrumPlotProcessing::SpectrumPlotProcessing(const SpectrumPlotProcessingConfig& cfg)
		: mInput("Spectrum Input", this)
	{
		Configure(cfg);

		InitSpectrumPlot();
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
		mPlot->Label("Spectrum");
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetDataColor(VM::VMColor::Green());
		mPlot->Geometry(100,100,300,150);
	}
}

// END

