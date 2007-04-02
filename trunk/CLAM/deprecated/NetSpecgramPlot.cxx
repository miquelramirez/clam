#include "NetSpecgramPlotController.hxx"
#include "NetSpecgramPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		NetSpecgramPlot::NetSpecgramPlot(QWidget* parent, const char * name)
			: NetPlot(parent,name)
		{
			InitNetSpecgramPlot();
		}

		NetSpecgramPlot::~NetSpecgramPlot()
		{
		}

		void NetSpecgramPlot::SetData(const Spectrum& spec)
		{
			((NetSpecgramPlotController*)mController)->SetData(spec);
		}

		void NetSpecgramPlot::SetMonitor(MonitorType & monitor)
		{
			((NetSpecgramPlotController*)mController)->SetMonitor(monitor);
		}

		void NetSpecgramPlot::SetPlotController()
		{
			SetController(new NetSpecgramPlotController());
		}
	
		void NetSpecgramPlot::InitNetSpecgramPlot()
		{
			SetPlotController();
		}
    }
}

// END

