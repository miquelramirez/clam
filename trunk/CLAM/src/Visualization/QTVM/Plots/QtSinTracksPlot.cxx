#include <qlayout.h>
#include <qframe.h>
#include "SinTracksPlotController.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "QtSinTracksPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		QtSinTracksPlot::QtSinTracksPlot(QWidget* parent) : QtPresentation(parent)
		{
			SetPlotController();
			InitSinTracksPlot();
			Connect();
		}

		QtSinTracksPlot::~QtSinTracksPlot()
		{
		}

		void QtSinTracksPlot::SetData(const Segment& segment)
		{
			((SinTracksPlotController*)_controller)->SetData(segment);
		}

		void QtSinTracksPlot::SetData(const Array< SpectralPeakArray >& peakMtx, 
										const TData& sr, const TData& dur)
		{
			((SinTracksPlotController*)_controller)->SetData(peakMtx,sr,dur);
		}

		void QtSinTracksPlot::SetDialColor(Color c)
		{
			((SinTracksPlotController*)_controller)->SetDialColor(c);
		}

		void QtSinTracksPlot::SetRegionColor(Color c)
		{
			((SinTracksPlotController*)_controller)->SetRegionColor(c);
		}

		void QtSinTracksPlot::updateRegion(MediaTime time)
		{
			_labelsGroup->UpdateLabels(time);
		}

		void QtSinTracksPlot::keyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
						((SinTracksPlotController*)_controller)->SetKeyShiftPressed(true);
						break;
				default:
						break;
			}
		}

		void QtSinTracksPlot::keyReleaseEvent( QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
						((SinTracksPlotController*)_controller)->SetKeyShiftPressed(false);			
						break;
				default:
						break;
			}
		}

		void QtSinTracksPlot::SetPlotController()
		{
			SetController(new SinTracksPlotController());
		}

		void QtSinTracksPlot::Connect()
		{
			// Connections
			connect(((SinTracksPlotController*)_controller),SIGNAL(xRulerScaleDiv(QwtScaleDiv)),this,SLOT(setXRulerScaleDiv(QwtScaleDiv)));
			connect(((SinTracksPlotController*)_controller),SIGNAL(yRulerScaleDiv(QwtScaleDiv)),this,SLOT(setYRulerScaleDiv(QwtScaleDiv)));
			connect(((SinTracksPlotController*)_controller),SIGNAL(selectedRegion(MediaTime)),this,SLOT(updateRegion(MediaTime)));
		}

		void QtSinTracksPlot::InitSinTracksPlot()
		{
			SetYRulerLabelFormat('f',0,1);

			QHBoxLayout* panel = new QHBoxLayout;
			
			QFrame* lefthole = new QFrame(this);
			lefthole->setFixedSize(50,30);
			panel->addWidget(lefthole);

			panel->addStretch(1);
			
			_labelsGroup = new TimeSegmentLabelsGroup(this);
			_labelsGroup->setFixedSize(186,25);
			panel->addWidget(_labelsGroup);

			QFrame* righthole = new QFrame(this);
			righthole->setFixedSize(20,30);
			panel->addWidget(righthole);

			AddToMainLayout(panel);
		}

		void QtSinTracksPlot::DisplayBackgroundBlack()
		{
			SetBackgroundColor(VMColor::Black());
			SetDialColor(VMColor::Red());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Orange());
		}

		void QtSinTracksPlot::DisplayBackgroundWhite()
		{
			SetBackgroundColor(VMColor::White());
			SetDialColor(VMColor::Black());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Red());
		}

		void QtSinTracksPlot::SetMarks(std::list<unsigned>& marks)
		{
			((SinTracksPlotController*)_controller)->SetMarks(marks);
		}

		void QtSinTracksPlot::SetMarksColor(Color c)
		{
			((SinTracksPlotController*)_controller)->SetMarksColor(c);
		}
	}
}

// END

