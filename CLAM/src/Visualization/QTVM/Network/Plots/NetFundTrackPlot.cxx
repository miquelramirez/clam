#include "NetFundTrackPlotController.hxx"
#include "NetFundTrackPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		NetFundTrackPlot::NetFundTrackPlot(QWidget* parent, const char * name)
			: NetPlot(parent,name)
		{
			InitNetFundTrackPlot();
		}

		NetFundTrackPlot::~NetFundTrackPlot()
		{
		}

		void NetFundTrackPlot::SetData(const Fundamental& fund)
		{
			((NetFundTrackPlotController*)_controller)->SetData(fund);
		}

		void NetFundTrackPlot::SetDataColor(Color c)
		{
			((NetFundTrackPlotController*)_controller)->SetDataColor(c);
		}

		void NetFundTrackPlot::SetPlotController()
		{
			SetController(new NetFundTrackPlotController());
		}

		void NetFundTrackPlot::InitNetFundTrackPlot()
		{
			SetPlotController();
		}
	}
}

// END

