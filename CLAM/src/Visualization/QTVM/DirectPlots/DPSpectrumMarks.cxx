#include "QtAppWrapper.hxx"
#include "QtSpectrumPlot.hxx"
#include "DPSpectrumMarks.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotSpectrumMarks(	const Spectrum& spec,
								std::list<unsigned>& marks,
								const std::string& label,
								int x, int y, int w, int h )
		{
			QtAppWrapper::Init();

			QtSpectrumPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(spec);
			plot.SetMarks(marks);
			plot.SetMarksColor(VMColor::Red());
			plot.SwitchDisplayColors(true);
			plot.Show();

			QtAppWrapper::Run();
		}
	}
}

// END

