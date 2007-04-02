#include "NetSinTracksPlot.hxx"
#include "SinTracksPlotProcessing.hxx"
#include "Factory.hxx"


typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

namespace CLAM
{
	namespace detail
	{
		static ProcessingFactory::Registrator<SinTracksPlotProcessing> regtSinTracksPlotProcessing("SinTracksPlotProcessing");
	}
	
	
    void SinTracksPlotProcessingConfig::DefaultInit()
    {
		AddAll();
		UpdateData();
		SetName("SinTracksPlotProcessing");
		SetCaption("Sinusoidal tracks");
		Setxpos(100);
		Setypos(100);
		Setwidth(200);
		Setheight(75);
    }

    SinTracksPlotProcessing::SinTracksPlotProcessing()
		: mPlot(0)
		, mInput("Peaks Input", this)
		, mOwnedPlot(false)
    {
		SinTracksPlotProcessingConfig cfg;
		Configure(cfg);
    }

    SinTracksPlotProcessing::SinTracksPlotProcessing(const SinTracksPlotProcessingConfig& cfg)
		: mPlot(0)
		, mInput("Peaks Input", this)
		, mOwnedPlot(false)
    {
		Configure(cfg);
    }

    SinTracksPlotProcessing::~SinTracksPlotProcessing()
    {
		if(mOwnedPlot) delete mPlot;
    }

    bool SinTracksPlotProcessing::Do()
    {
		bool res = Do(mInput.GetData());
		mInput.Consume();
		return res;
    }

    bool SinTracksPlotProcessing::Do(const SpectralPeakArray& peaks)
    {
		if(!AbleToExecute()) return true;
		mPlot->SetData(peaks);
		if(!mPlot->isVisible()) mPlot->show();
		return true;
    }

    bool SinTracksPlotProcessing::ConcreteConfigure(const ProcessingConfig& c)
    {
		CopyAsConcreteConfig(mConfig, c);
		return true;
    }

    bool SinTracksPlotProcessing::ConcreteStart()
    {
		if(!mPlot) InitSinTracksPlot();
		return true;
    }

    bool SinTracksPlotProcessing::ConcreteStop()
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
    
    void SinTracksPlotProcessing::SetPlot(VM::NetSinTracksPlot * plot)
    {
		if (mOwnedPlot) delete mPlot;
		mOwnedPlot = false;
		mPlot = plot;
    }

    void SinTracksPlotProcessing::InitSinTracksPlot()
    {
		if (mOwnedPlot) delete mPlot;
		mPlot = new VM::NetSinTracksPlot();
		mOwnedPlot = true;
		mPlot->Label(mConfig.GetCaption());
		mPlot->SetBackgroundColor(VM::VMColor::Black());
		mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
    }

}

// END

