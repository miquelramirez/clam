#include "NetFundPlotController.hxx"
#include "NetFundPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		NetFundPlot::NetFundPlot(QWidget* parent, const char * name)
			: NetPlot(parent,name)
		{
			InitNetFundPlot();
		}

		NetFundPlot::~NetFundPlot()
		{
		}

		void NetFundPlot::SetData(const Fundamental& fund)
		{
			((NetFundPlotController*)mController)->SetData(fund);
		}

		void NetFundPlot::SetMonitor(MonitorType & monitor)
		{
			((NetFundPlotController*)mController)->SetMonitor(monitor);
		}

		void NetFundPlot::SetDataColor(Color c)
		{
			((NetFundPlotController*)mController)->SetDataColor(c);
		}

		void NetFundPlot::SetPlotController()
		{
			SetController(new NetFundPlotController());
		}

		void NetFundPlot::InitNetFundPlot()
		{
			SetPlotController();
		}
	}
}

// END

