#include "NetSpectrumPlotController.hxx"
#include "NetSpectrumPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		NetSpectrumPlot::NetSpectrumPlot(QWidget* parent, const char * name)
			: NetPlot(parent,name)
		{
			InitNetSpectrumPlot();
		}

		NetSpectrumPlot::~NetSpectrumPlot()
		{
		}

		void NetSpectrumPlot::SetData(const Spectrum& spec)
		{
			((NetSpectrumPlotController*)mController)->SetData(spec);
		}

		void NetSpectrumPlot::SetMonitor(MonitorType & monitor)
		{
			((NetSpectrumPlotController*)mController)->SetMonitor(monitor);
		}

		void NetSpectrumPlot::SetDataColor(Color c)
		{
			((NetSpectrumPlotController*)mController)->SetDataColor(c);
		}

		void NetSpectrumPlot::SetPlotController()
		{
			SetController(new NetSpectrumPlotController());
		}

		void NetSpectrumPlot::InitNetSpectrumPlot()
		{
			SetPlotController();
		}
	}
}

// END

