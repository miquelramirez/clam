#include "SpectrumAndPeaksPlotController.hxx"
#include "QtSpectrumAndPeaksPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		QtSpectrumAndPeaksPlot::QtSpectrumAndPeaksPlot(QWidget* parent)
			: QtSpectrumPlot(parent)
		{
			SetPlotController();
			Connect();
		}
		
		QtSpectrumAndPeaksPlot::~QtSpectrumAndPeaksPlot()
		{
		}

		void QtSpectrumAndPeaksPlot::SetData(const Spectrum& spec,const SpectralPeakArray& peaks)
		{
			((SpectrumAndPeaksPlotController*)_controller)->SetData(spec,peaks);
		}
		void QtSpectrumAndPeaksPlot::SetPeaksColor(Color cline,Color cpoint)
		{
			((SpectrumAndPeaksPlotController*)_controller)->SetPeaksColor(cline,cpoint);
		}

		void QtSpectrumAndPeaksPlot::SetData(const Spectrum& spec)
		{
			// empty body for convenience
		}

		void QtSpectrumAndPeaksPlot::SetPlotController()
		{
			SetController(new SpectrumAndPeaksPlotController());
		}

		void QtSpectrumAndPeaksPlot::Connect()
		{
			// Connections
			connect(((SpectrumAndPeaksPlotController*)_controller),SIGNAL(xRulerScaleDiv(QwtScaleDiv)),this,SLOT(setXRulerScaleDiv(QwtScaleDiv)));
			connect(((SpectrumAndPeaksPlotController*)_controller),SIGNAL(yRulerScaleDiv(QwtScaleDiv)),this,SLOT(setYRulerScaleDiv(QwtScaleDiv)));
			connect(((SpectrumAndPeaksPlotController*)_controller),SIGNAL(mag(TData)),this,SLOT(updateMagLabel(TData)));
			connect(((SpectrumAndPeaksPlotController*)_controller),SIGNAL(freq(TData)),this,SLOT(updateFreqLabel(TData)));
		}

		void QtSpectrumAndPeaksPlot::DisplayBackgroundBlack()
		{
			QtSpectrumPlot::DisplayBackgroundBlack();
			SetPeaksColor(VMColor::Yellow(),VMColor::Red());
		}

		void QtSpectrumAndPeaksPlot::DisplayBackgroundWhite()
		{
			QtSpectrumPlot::DisplayBackgroundWhite();
			SetPeaksColor(VMColor::Cyan(),VMColor::Red());
		}
	}
}

// END



