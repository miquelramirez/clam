#include "NetAudioPlotController.hxx"
#include "NetAudioPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		NetAudioPlot::NetAudioPlot(QWidget* parent)
		    : NetPlot(parent)
		{
			InitNetAudioPlot();
		}

		NetAudioPlot::~NetAudioPlot()
		{
		}

		void NetAudioPlot::SetData(const Audio& audio)
		{
			((NetAudioPlotController*)_controller)->SetData(audio);
		}

		void NetAudioPlot::SetDataColor(Color c)
		{
			((NetAudioPlotController*)_controller)->SetDataColor(c);
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

