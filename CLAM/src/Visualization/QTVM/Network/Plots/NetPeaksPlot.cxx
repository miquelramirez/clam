#include "NetPeaksPlotController.hxx"
#include "NetPeaksPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		NetPeaksPlot::NetPeaksPlot(QWidget* parent, const char * name)
			: NetPlot(parent,name)
		{
			InitNetPeaksPlot();
		}

		NetPeaksPlot::~NetPeaksPlot()
		{
		}

		void NetPeaksPlot::SetData(const SpectralPeakArray& peaks)
		{
			((NetPeaksPlotController*)mController)->SetData(peaks);
		}

		void NetPeaksPlot::SetMonitor(MonitorType & monitor)
		{
			((NetPeaksPlotController*)mController)->SetMonitor(monitor);
		}

		void NetPeaksPlot::SetPeaksColor(Color cline, Color cpoint)
		{
			((NetPeaksPlotController*)mController)->SetPeaksColor(cline, cpoint);
		}

		void NetPeaksPlot::SetPlotController()
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

