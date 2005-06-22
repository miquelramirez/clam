#include "NetAudioPlotController.hxx"
#include "NetAudioPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		NetAudioPlot::NetAudioPlot(QWidget* parent, const char * name)
			: NetPlot(parent,name)
		{
			InitNetAudioPlot();
		}

		NetAudioPlot::~NetAudioPlot()
		{
		}

		void NetAudioPlot::SetData(const Audio& audio)
		{
			((NetAudioPlotController*)mController)->SetData(audio);
		}

		void NetAudioPlot::SetMonitor(MonitorType & monitor)
		{
			((NetAudioPlotController*)mController)->SetMonitor(monitor);
		}

		void NetAudioPlot::SetDataColor(Color c)
		{
			((NetAudioPlotController*)mController)->SetDataColor(c);
		}

		void NetAudioPlot::SetPlotController()
		{
			SetController(new NetAudioPlotController());
		}

		void NetAudioPlot::InitNetAudioPlot()
		{
			SetPlotController();
		}
	}
}

// END

