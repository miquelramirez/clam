#include "NetFundPlot.hxx"
#include "FundPlotProcessing.hxx"

namespace CLAM
{
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
		: mInput("Fundamental Input", this)
	{
		FundPlotProcessingConfig cfg;
		Configure(cfg);

		mPlot = NULL;
	}

	FundPlotProcessing::FundPlotProcessing(const FundPlotProcessingConfig& cfg)
		: mInput("Fundamental Input", this)
	{
		Configure(cfg);

		mPlot = NULL;
	}

	FundPlotProcessing::~FundPlotProcessing()
	{
		if(mPlot) delete mPlot;
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
		if(!mPlot->isVisible()) mPlot->Show();
		return true;
	}

	bool FundPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	void FundPlotProcessing::InitFundPlot()
	{
		mPlot = new VM::NetFundPlot();
		mPlot->Label(mConfig.GetCaption());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetDataColor(VM::VMColor::Green());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
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
		    delete mPlot;
		    mPlot = NULL;
		}
		return true;
	}
}

// END

