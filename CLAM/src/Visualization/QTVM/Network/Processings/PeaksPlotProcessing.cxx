#include "NetPeaksPlot.hxx"
#include "PeaksPlotProcessing.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<PeaksPlotProcessing> regtPeaksPlotProcessing("PeaksPlotProcessing");
	}
	
	
	void PeaksPlotProcessingConfig::DefaultInit()
	{
		AddAll();
		UpdateData();
		SetName("PeaksPlotProcessing");
		SetCaption("Peaks");
		Setxpos(100);
		Setypos(100);
		Setwidth(200);
		Setheight(75);
	}

	PeaksPlotProcessing::PeaksPlotProcessing()
		: mPlot(0)
		, mInput("Peaks Input", this)
		, mOwnedPlot(false)
	{
		PeaksPlotProcessingConfig cfg;
		Configure(cfg);
	}

	PeaksPlotProcessing::PeaksPlotProcessing(const PeaksPlotProcessingConfig& cfg)
		: mPlot(0)
		, mInput("Peaks Input", this)
		, mOwnedPlot(false)
	{
		Configure(cfg);
	}

	PeaksPlotProcessing::~PeaksPlotProcessing()
	{
		if(mOwnedPlot) delete mPlot;
	}

	bool PeaksPlotProcessing::Do()
	{
		bool res = Do(mInput.GetData());
		mInput.Consume();
		return res;
	}

	bool PeaksPlotProcessing::Do(const SpectralPeakArray& peaks)
	{
		if(!AbleToExecute()) return true;
		mPlot->SetData(peaks);
		if(!mPlot->isVisible()) mPlot->show();
		return true;
	}

	bool PeaksPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		return true;
	}

	bool PeaksPlotProcessing::ConcreteStart()
	{
		if(!mPlot) InitPeaksPlot();
		return true;
	}

	bool PeaksPlotProcessing::ConcreteStop()
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

	void PeaksPlotProcessing::SetPlot(VM::NetPeaksPlot * plot)
	{
		if (mOwnedPlot) delete mPlot;
		mOwnedPlot = false;
		mPlot = plot;
	}

	void PeaksPlotProcessing::InitPeaksPlot()
	{
		if (mOwnedPlot) delete mPlot;
		mPlot = new VM::NetPeaksPlot();
		mOwnedPlot = true;
		mPlot->Label(mConfig.GetCaption());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->SetPeaksColor(VM::VMColor::Cyan(), VM::VMColor::Red());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
	}

}

// END

