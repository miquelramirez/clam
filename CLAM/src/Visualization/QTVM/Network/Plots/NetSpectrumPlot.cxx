#include "NetSpectrumPlotController.hxx"
#include "NetSpectrumPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		NetSpectrumPlot::NetSpectrumPlot(QWidget* parent)
		    : NetPlot(parent)
		{
			InitNetSpectrumPlot();
		}

		NetSpectrumPlot::~NetSpectrumPlot()
		{
		}

		void NetSpectrumPlot::SetData(const Spectrum& spec)
		{
			((NetSpectrumPlotController*)_controller)->SetData(spec);
		}

		void NetSpectrumPlot::SetDataColor(Color c)
		{
			((NetSpectrumPlotController*)_controller)->SetDataColor(c);
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

