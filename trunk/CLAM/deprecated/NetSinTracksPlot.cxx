#include "NetSinTracksPlotController.hxx"
#include "NetSinTracksPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		NetSinTracksPlot::NetSinTracksPlot(QWidget* parent, const char * name)
			: NetPlot(parent,name)
		{
			InitNetSinTracksPlot();
		}

		NetSinTracksPlot::~NetSinTracksPlot()
		{
		}

		void NetSinTracksPlot::SetData(const SpectralPeakArray& peaks)
		{
			((NetSinTracksPlotController*)mController)->SetData(peaks);
		}
	
		void NetSinTracksPlot::SetMonitor(MonitorType & monitor)
		{
			((NetSinTracksPlotController*)mController)->SetMonitor(monitor);
		}

		void NetSinTracksPlot::SetPlotController()
		{
			SetController(new NetSinTracksPlotController());
		}
	
		void NetSinTracksPlot::InitNetSinTracksPlot()
		{
			SetPlotController();
		}
    }
}

// END

