#include "NetPeaksPlotController.hxx"
#include "NetPeaksPlot.hxx"

namespace CLAM
{
    namespace VM
    {
	NetPeaksPlot::NetPeaksPlot(QWidget* parent)
	    : NetPlot(parent)
	{
	    InitNetPeaksPlot();
	}

	NetPeaksPlot::~NetPeaksPlot()
	{
	}

	void NetPeaksPlot::SetData(const SpectralPeakArray& peaks)
	{
	    ((NetPeaksPlotController*)_controller)->SetData(peaks);
	}

	void NetPeaksPlot::SetPeaksColor(Color cline, Color cpoint)
	{
	    ((NetPeaksPlotController*)_controller)->SetPeaksColor(cline, cpoint);
	}

	void NetPeaksPlot::NetPeaksPlot::SetPlotController()
	{
	    SetController(new NetPeaksPlotController());
	}

	void NetPeaksPlot::InitNetPeaksPlot()
	{
	    SetPlotController();
	}
    }
}

// END

