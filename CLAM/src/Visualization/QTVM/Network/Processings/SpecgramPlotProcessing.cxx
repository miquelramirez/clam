#include "NetSpecgramPlot.hxx"
#include "SpecgramPlotProcessing.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<SpecgramPlotProcessing> regtSpecgramPlotProcessing("SpecgramPlotProcessing");
	}
	
	
	void SpecgramPlotProcessingConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetName("SpecgramPlotProcessing");
		SetCaption("Specgram");
		Setxpos(100);
		Setypos(100);
		Setwidth(200);
		Setheight(75);
	}

	SpecgramPlotProcessing::SpecgramPlotProcessing() 
		: mPlot(0)
		, mInput("Spectrum Input", this)
		, mOwnedPlot(false)
	{
		SpecgramPlotProcessingConfig cfg;
		Configure(cfg);
	}

	SpecgramPlotProcessing::SpecgramPlotProcessing(const SpecgramPlotProcessingConfig& cfg)
		: mPlot(0)
		, mInput("Spectrum Input", this)
		, mOwnedPlot(false)
	{
		Configure(cfg);
	}

	SpecgramPlotProcessing::~SpecgramPlotProcessing()
	{
		if(mOwnedPlot) delete mPlot;
	}

	bool SpecgramPlotProcessing::Do()
	{
		bool res = Do(mInput.GetData());
		mInput.Consume();
		return res;
	}

	bool SpecgramPlotProcessing::Do(const Spectrum& spec)
	{
		if(!AbleToExecute()) return true;
		mPlot->SetData(spec);
		if(!mPlot->isVisible()) mPlot->show();
		return true;
	}

	bool SpecgramPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	bool SpecgramPlotProcessing::ConcreteStart()
	{
		if(!mPlot) InitSpecgramPlot();
		return true;
	}

	bool SpecgramPlotProcessing::ConcreteStop()
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

	void SpecgramPlotProcessing::SetPlot(VM::NetSpecgramPlot * plot )
	{
		if (mOwnedPlot) delete mPlot;
		mOwnedPlot = false;
		mPlot = plot;
	}

	void SpecgramPlotProcessing::InitSpecgramPlot()
	{
		if (mOwnedPlot) delete mPlot;
		mPlot = new VM::NetSpecgramPlot();
		mOwnedPlot = true;
		mPlot->Label(mConfig.GetCaption());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
	}

}

// END

