#include "QtAppWrapper.hxx"
#include "QtStereoAudioPlot.hxx"
#include "DPAudioStereo.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotAudioStereo(	std::vector<Audio> data,
								const std::string& label,
								int x, int y, int w, int h )
		{
			QtAppWrapper::Init();

			QtStereoAudioPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(data);
			plot.SwitchDisplayColors(true);
			plot.Show();

			QtAppWrapper::Run();
		}
	}
}

// END

