#include "QtAppWrapper.hxx"
#include "QtSpectrumAndPeaksPlot.hxx"
#include "DPSpectrumAndPeaks.hxx"

namespace CLAM
{
	namespace VM
	{

		void PlotSpectrumAndPeaks(	const Spectrum& spec,
									const SpectralPeakArray& peaks,
									const std::string& label,
									int x, int y, int w, int h )
		{
			QtAppWrapper::Init();

			QtSpectrumAndPeaksPlot plot;
			plot.Label(label);
			plot.Geometry(x,y,w,h);
			plot.SetData(spec,peaks);
			plot.SwitchDisplayColors(true);
			plot.Show();

			QtAppWrapper::Run();
		}
	}
}

// END

