#include "NetAudioBuffPlotController.hxx"
#include "NetAudioBuffPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		NetAudioBuffPlot::NetAudioBuffPlot(QWidget* parent, const char * name)
			: NetPlot(parent,name)
		{
			InitNetAudioBuffPlot();
		}

		NetAudioBuffPlot::~NetAudioBuffPlot()
		{
		}

		void NetAudioBuffPlot::SetData(const Audio& audio)
		{
			((NetAudioBuffPlotController*)mController)->SetData(audio);
		}

		void NetAudioBuffPlot::SetMonitor(MonitorType & monitor)
		{
			((NetAudioBuffPlotController*)mController)->SetMonitor(monitor);
		}

		void NetAudioBuffPlot::SetDataColor(Color c)
		{
			((NetAudioBuffPlotController*)mController)->SetDataColor(c);
		}

		void NetAudioBuffPlot::SetPlotController()
		{
			SetController(new NetAudioBuffPlotController());
		}

		void NetAudioBuffPlot::InitNetAudioBuffPlot()
		{
			SetPlotController();
		}
	}
}

// END

