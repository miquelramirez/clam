#include "QtAppWrapper.hxx"
#include "QtSpectrumPlot.hxx"
#include "DPSpectrum.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotSpectrum(	const Spectrum& spec,
							const std::string& label,
							int x, int y, int w, int h )
		{
			QtAppWrapper::Init();

			QtSpectrumPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(spec);
			plot.SwitchDisplayColors(true);
			plot.Show();

			QtAppWrapper::Run();
		}
	}
}

// END

