#include "NetSpectrumPlot.hxx"
#include "SpectrumPlotProcessing.hxx"
#include "Factory.hxx"


	typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{

	namespace detail
	{
		static ProcessingFactory::Registrator<SpectrumPlotProcessing> regtSpectrumPlotProcessing("SpectrumPlotProcessing");
	}
	
	
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
		: mPlot(0)
		, mInput("Spectrum Input", this)
		, mOwnedPlot(false)
	{
		SpectrumPlotProcessingConfig cfg;
		Configure(cfg);
	}

	SpectrumPlotProcessing::SpectrumPlotProcessing(const SpectrumPlotProcessingConfig& cfg)
		: mPlot(0)
		, mInput("Spectrum Input", this)
		, mOwnedPlot(false)
	{
		Configure(cfg);
	}

	SpectrumPlotProcessing::~SpectrumPlotProcessing()
	{
		if(mOwnedPlot) delete mPlot;
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
		if(!mPlot->isVisible()) mPlot->show();
		return true;
	}

	bool SpectrumPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
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
			if (mOwnedPlot)
			{
				delete mPlot;
				mPlot = NULL;
			}
		}
		return true;
	}

	void SpectrumPlotProcessing::SetPlot(VM::NetSpectrumPlot * plot )
	{
		if (mOwnedPlot) delete mPlot;
		mOwnedPlot = false;
		mPlot = plot;
	}

	void SpectrumPlotProcessing::InitSpectrumPlot()
	{
		if (mOwnedPlot) delete mPlot;
		mPlot = new VM::NetSpectrumPlot();
		mOwnedPlot = true;
		mPlot->Label(mConfig.GetCaption());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetDataColor(VM::VMColor::Green());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
	}

}

// END

