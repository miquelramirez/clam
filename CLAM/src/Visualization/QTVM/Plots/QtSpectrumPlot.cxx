#include <qlayout.h>
#include "SingleLabel.hxx"
#include "SpectrumPlotController.hxx"
#include "QtSpectrumPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		QtSpectrumPlot::QtSpectrumPlot(QWidget* parent) : QtPresentation(parent)
		{
			SetPlotController();
			InitSpectrumPlot();
			Connect();
		}

		QtSpectrumPlot::~QtSpectrumPlot()
		{
		}

		void QtSpectrumPlot::InitSpectrumPlot()
		{   
			SetXRulerLabelFormat('f',0,1);
			SetYRulerLabelFormat('f',0,1);

			QHBoxLayout* panel = new QHBoxLayout;
			panel->addStretch(1);
			
			_magLabel = new SingleLabel(this," dB ","Magnitude");
			panel->addWidget(_magLabel);

			_freqLabel = new SingleLabel(this," Hz ","Frequency");
			panel->addWidget(_freqLabel);

			QFrame* righthole = new QFrame(this);
			righthole->setFixedSize(20,_freqLabel->height());
			panel->addWidget(righthole);

			AddToMainLayout(panel);
		}

		void QtSpectrumPlot::SetData(const Spectrum& spec)
		{
			((SpectrumPlotController*)_controller)->SetData(spec);
		}

		void QtSpectrumPlot::SetForegroundColor(Color c)
		{
			((SpectrumPlotController*)_controller)->SetDataColor(c);
		}

		void QtSpectrumPlot::SetVLineColor(Color c)
		{
			((SpectrumPlotController*)_controller)->SetDialColor(c);
		}

		void QtSpectrumPlot::updateMagLabel(TData value)
		{
			_magLabel->Update(value);
		}

		void QtSpectrumPlot::updateFreqLabel(TData value)
		{
			_freqLabel->Update(value);
		}

		void QtSpectrumPlot::SetPlotController()
		{
			SetController(new SpectrumPlotController());
		}

		void QtSpectrumPlot::Connect()
		{
			// Connections
			connect(((SpectrumPlotController*)_controller),SIGNAL(xRulerScaleDiv(QwtScaleDiv)),this,SLOT(setXRulerScaleDiv(QwtScaleDiv)));
			connect(((SpectrumPlotController*)_controller),SIGNAL(yRulerScaleDiv(QwtScaleDiv)),this,SLOT(setYRulerScaleDiv(QwtScaleDiv)));
			connect(((SpectrumPlotController*)_controller),SIGNAL(mag(TData)),this,SLOT(updateMagLabel(TData)));
			connect(((SpectrumPlotController*)_controller),SIGNAL(freq(TData)),this,SLOT(updateFreqLabel(TData)));
		}

		void QtSpectrumPlot::DisplayBackgroundBlack()
		{
			SetBackgroundColor(VMColor::Black());
			SetForegroundColor(VMColor::Green());
			SetVLineColor(VMColor::Red());
			SetMarksColor(VMColor::Orange());
		}

		void QtSpectrumPlot::DisplayBackgroundWhite()
		{
			SetBackgroundColor(VMColor::White());
			SetForegroundColor(VMColor::Blue());
			SetVLineColor(VMColor::Black());
			SetMarksColor(VMColor::Red());
		}

		void QtSpectrumPlot::SetMarks(std::list<unsigned>& marks)
		{
			((SpectrumPlotController*)_controller)->SetMarks(marks);
		}

		void QtSpectrumPlot::SetMarksColor(Color c)
		{
			((SpectrumPlotController*)_controller)->SetMarksColor(c);
		}
	}
}

// END


