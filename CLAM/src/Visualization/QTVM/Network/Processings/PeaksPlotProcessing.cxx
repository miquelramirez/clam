#include "NetPeaksPlot.hxx"
#include "PeaksPlotProcessing.hxx"

namespace CLAM
{
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
	: mInput("Peaks Input", this)
    {
	PeaksPlotProcessingConfig cfg;
	Configure(cfg);

	mPlot = NULL;
    }

    PeaksPlotProcessing::PeaksPlotProcessing(const PeaksPlotProcessingConfig& cfg)
	: mInput("Peaks Input", this)
    {
	Configure(cfg);

	mPlot = NULL;
    }

    PeaksPlotProcessing::~PeaksPlotProcessing()
    {
	if(mPlot) delete mPlot;
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
	if(!mPlot->isVisible()) mPlot->Show();
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
	if(mPlot) delete mPlot;
	mPlot = NULL;
	return true;
    }

    void PeaksPlotProcessing::InitPeaksPlot()
    {
	mPlot = new VM::NetPeaksPlot();
	mPlot->Label(mConfig.GetCaption());
	mPlot->SetBackgroundColor(VM::VMColor::Black());
	mPlot->SetPeaksColor(VM::VMColor::Cyan(), VM::VMColor::Red());
	mPlot->Geometry(mConfig.Getxpos(),mConfig.Getypos(),mConfig.Getwidth(),mConfig.Getheight());
    }
}

// END

