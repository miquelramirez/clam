#include "QtAppWrapper.hxx"
#include "QtAudioPlot.hxx"
#include "DPAudio.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotAudio(	const Audio& audio,
						const std::string& label,
						int x, int y, int w, int h )
		{
			QtAppWrapper::Init();

			QtAudioPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(audio);
			plot.SwitchDisplayColors(true);
			plot.Show();

			QtAppWrapper::Run();
		}
	}
}

// END

