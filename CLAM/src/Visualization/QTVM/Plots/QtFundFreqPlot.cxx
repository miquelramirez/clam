#include <qlayout.h>
#include <qframe.h>
#include "FundPlotController.hxx"
#include "QtFundPlayer.hxx"
#include "SingleLabel.hxx"
#include "TimeSegmentLabelsGroup.hxx"
#include "QtFundFreqPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		QtFundFreqPlot::QtFundFreqPlot(QWidget* parent) : QtPresentation(parent)
		{
			SetPlotController();
			InitFundFreqPlot();
			Connect();
		}

		QtFundFreqPlot::~QtFundFreqPlot()
		{
		}

		void QtFundFreqPlot::InitFundFreqPlot()
		{
			SetYRulerLabelFormat('f',0,1);

			QHBoxLayout* panel = new QHBoxLayout;
			
			QFrame* lefthole = new QFrame(this);
			lefthole->setFixedSize(50,30);
			panel->addWidget(lefthole);

			_player = new QtFundPlayer(this);
			_player->setFixedSize(75,30);
			panel->addWidget(_player);

			panel->addStretch(1);

			_leftFreqLab = new SingleLabel(this," Hz ","Frequency");
			_rightFreqLab = new SingleLabel(this," Hz ","Frequency Right");

			panel->addWidget(_leftFreqLab);
			panel->addWidget(_rightFreqLab);

			panel->addStretch(1);
			
			_labelsGroup = new TimeSegmentLabelsGroup(this);
			_labelsGroup->setFixedSize(186,25);
			panel->addWidget(_labelsGroup);

			QFrame* righthole = new QFrame(this);
			righthole->setFixedSize(20,30);
			panel->addWidget(righthole);

			AddToMainLayout(panel);
		}

		void QtFundFreqPlot::SetData(const Segment& segment)
		{
			((FundPlotController*)_controller)->SetData(segment);
			SetPData(segment);
		}

		void QtFundFreqPlot::SetForegroundColor(Color c)
		{
			((FundPlotController*)_controller)->SetDataColor(c);
		}

		void QtFundFreqPlot::SetDialColor(Color c)
		{
			((FundPlotController*)_controller)->SetDialColor(c);
		}

		void QtFundFreqPlot::SetRegionColor(Color c)
		{
			((FundPlotController*)_controller)->SetRegionColor(c);
		}

		void QtFundFreqPlot::keyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
						((FundPlotController*)_controller)->SetKeyShiftPressed(true);
						break;
				default:
						break;
			}
		}

		void QtFundFreqPlot::keyReleaseEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
						((FundPlotController*)_controller)->SetKeyShiftPressed(false);			
						break;
				default:
						break;
			}
		}

		void QtFundFreqPlot::updateRegion(MediaTime time)
		{
			_player->stop();
			_player->SetPlaySegment(time);
			_labelsGroup->UpdateLabels(time);

			UpdateFreqLabels(time);
		}

		void QtFundFreqPlot::UpdateFreqLabels(MediaTime time)
		{
			if(time.HasDuration())
			{
			    _leftFreqLab->Update(((FundPlotController*)_controller)->GetFreq(time.GetBegin()));
				_leftFreqLab->SetToolTip("Frequency Left");
				_rightFreqLab->Update(((FundPlotController*)_controller)->GetFreq(time.GetEnd()));
				_rightFreqLab->show();
			}
			else
			{
				_leftFreqLab->Update(((FundPlotController*)_controller)->GetFreq(time.GetBegin()));
				_leftFreqLab->SetToolTip("Frequency");
				_rightFreqLab->clear();
				_rightFreqLab->hide();
			}
		}

		void QtFundFreqPlot::SetPlotController()
		{
			SetController(new FundPlotController());
		}

		void QtFundFreqPlot::Connect()
		{
			// Connections
			connect(((FundPlotController*)_controller),SIGNAL(xRulerScaleDiv(QwtScaleDiv)),this,SLOT(setXRulerScaleDiv(QwtScaleDiv)));
			connect(((FundPlotController*)_controller),SIGNAL(yRulerScaleDiv(QwtScaleDiv)),this,SLOT(setYRulerScaleDiv(QwtScaleDiv)));
			connect(((FundPlotController*)_controller),SIGNAL(selectedRegion(MediaTime)),this,SLOT(updateRegion(MediaTime)));
		}

		void QtFundFreqPlot::SetPData(const Segment& seg)
		{
			((QtFundPlayer*)_player)->SetData(seg);
		}

		void QtFundFreqPlot::DisplayBackgroundBlack()
		{
			SetBackgroundColor(VMColor::Black());
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Orange());
		}

		void QtFundFreqPlot::DisplayBackgroundWhite()
		{
			SetBackgroundColor(VMColor::White());
			SetForegroundColor(VMColor::Blue());
			SetDialColor(VMColor::Black());
			SetRegionColor(VMColor::LightGray());
			SetMarksColor(VMColor::Red());
		}

		void QtFundFreqPlot::closeEvent(QCloseEvent *e)
		{
			RemoveFromPlayList();
			QtPlot::closeEvent(e);
		}

		void QtFundFreqPlot::SetMarks(std::list<unsigned>& marks)
		{
			((FundPlotController*)_controller)->SetMarks(marks);
		}

		void QtFundFreqPlot::SetMarksColor(Color c)
		{
			((FundPlotController*)_controller)->SetMarksColor(c);
		}
	}	
}

// END

