#include "QtAppWrapper.hxx"
#include "QtAudioPlot.hxx"
#include "DPAudioMarks.hxx"

namespace CLAM
{
	namespace VM
	{
		void PlotAudioMarks(	const Audio& audio,
								std::list<unsigned>& marks,
								const std::string& label,
								int x, int y, int w, int h	)
		{
			QtAppWrapper::Init();

			QtAudioPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(audio);
			plot.SetMarks(marks);
			plot.SetMarksColor(VMColor::Red());
			plot.SwitchDisplayColors(true);
			plot.Show();

			QtAppWrapper::Run();
		}
	}
}

// END

