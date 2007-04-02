#include "NetFundPlot.hxx"
#include "FundPlotProcessing.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<FundPlotProcessing> regtFundPlotProcessing("FundPlotProcessing");
	}
	
	
	void FundPlotProcessingConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetName("FundPlotProcessing");
		SetCaption("Fundamental");
		Setxpos(100);
		Setypos(100);
		Setwidth(200);
		Setheight(75);
	}

	FundPlotProcessing::FundPlotProcessing() 
		: mPlot(0)
		, mInput("Fundamental Input", this)
		, mOwnedPlot(false)
	{
		FundPlotProcessingConfig cfg;
		Configure(cfg);
	}

	FundPlotProcessing::FundPlotProcessing(const FundPlotProcessingConfig& cfg)
		: mPlot(0)
		, mInput("Fundamental Input", this)
		, mOwnedPlot(false)
	{
		Configure(cfg);
	}

	FundPlotProcessing::~FundPlotProcessing()
	{
		if(mOwnedPlot) delete mPlot;
	}

	bool FundPlotProcessing::Do()
	{
		bool res = Do(mInput.GetData());
		mInput.Consume();
		return res;
	}

	bool FundPlotProcessing::Do(const Fundamental& fund)
	{
		if(!AbleToExecute()) return true;
		mPlot->SetData(fund);
		if(!mPlot->isVisible()) mPlot->show();
		return true;
	}

	bool FundPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	bool FundPlotProcessing::ConcreteStart()
	{
		if(!mPlot) InitFundPlot();
		return true;
	}

	bool FundPlotProcessing::ConcreteStop()
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

	void FundPlotProcessing::SetPlot(VM::NetFundPlot * plot)
	{
		if (mOwnedPlot) delete mPlot;
		mOwnedPlot = false;
		mPlot = plot;
	}
	
	void FundPlotProcessing::InitFundPlot()
	{
		if (mOwnedPlot) delete mPlot;
		mPlot = new VM::NetFundPlot();
		mOwnedPlot = true;
		mPlot->Label(mConfig.GetCaption());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetDataColor(VM::VMColor::Green());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
	}

}

// END

