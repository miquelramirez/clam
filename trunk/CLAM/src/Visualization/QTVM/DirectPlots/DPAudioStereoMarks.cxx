#include "QtAppWrapper.hxx"
#include "QtStereoAudioPlot.hxx"
#include "DPAudioStereoMarks.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotAudioStereoMarks(	std::vector<Audio> data,
									std::list<unsigned>& marks,
									const std::string& label,
									int x, int y, int w, int h )
		{
			QtAppWrapper::Init();

			QtStereoAudioPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(data);
			plot.SetMarks(marks);
			plot.SetMarksColor(VMColor::Red());
			plot.SwitchDisplayColors(true);
			plot.Show();

			QtAppWrapper::Run();
		}
	}
}

// END

