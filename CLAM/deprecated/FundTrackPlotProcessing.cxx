#include "NetFundTrackPlot.hxx"
#include "FundTrackPlotProcessing.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<FundTrackPlotProcessing> regtFundTrackPlotProcessing("FundTrackPlotProcessing");
	}
	
	
	void FundTrackPlotProcessingConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetName("FundTrackPlotProcessing");
		SetCaption("Fundamental track");
		Setxpos(100);
		Setypos(100);
		Setwidth(200);
		Setheight(75);
	}

	FundTrackPlotProcessing::FundTrackPlotProcessing() 
		: mPlot(0)
		, mInput("Fundamental Input", this)
		, mOwnedPlot(false)
	{
		FundTrackPlotProcessingConfig cfg;
		Configure(cfg);
	}

	FundTrackPlotProcessing::FundTrackPlotProcessing(const FundTrackPlotProcessingConfig& cfg)
		: mPlot(0)
		, mInput("Fundamental Input", this)
		, mOwnedPlot(false)
	{
		Configure(cfg);
	}

	FundTrackPlotProcessing::~FundTrackPlotProcessing()
	{
		if(mOwnedPlot) delete mPlot;
	}

	bool FundTrackPlotProcessing::Do()
	{
		bool res = Do(mInput.GetData());
		mInput.Consume();
		return res;
	}

	bool FundTrackPlotProcessing::Do(const Fundamental& fund)
	{
		if(!AbleToExecute()) return true;
		mPlot->SetData(fund);
		if(!mPlot->isVisible()) mPlot->show();
		return true;
	}

	bool FundTrackPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	bool FundTrackPlotProcessing::ConcreteStart()
	{
		if(!mPlot) InitFundTrackPlot();
		return true;
	}

	bool FundTrackPlotProcessing::ConcreteStop()
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

	void FundTrackPlotProcessing::SetPlot(VM::NetFundTrackPlot * plot)
	{
		if (mOwnedPlot) delete mPlot;
		mOwnedPlot = false;
		mPlot = plot;
	}

	void FundTrackPlotProcessing::InitFundTrackPlot()
	{
		if (mOwnedPlot) delete mPlot;
		mPlot = new VM::NetFundTrackPlot();
		mOwnedPlot = true;
		mPlot->Label(mConfig.GetCaption());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetDataColor(VM::VMColor::Green());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
	}

}

// END

