/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

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

			        case Qt::Key_Insert:
				                ((SinTracksPlotController*)_controller)->SetKeyInsertPressed(true); 
						break;
						
			        case Qt::Key_Delete:
				                ((SinTracksPlotController*)_controller)->SetKeyDeletePressed(true); 
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

			        case Qt::Key_Insert:
				                ((SinTracksPlotController*)_controller)->SetKeyInsertPressed(false); 
						break;
						
			        case Qt::Key_Delete:
				                ((SinTracksPlotController*)_controller)->SetKeyDeletePressed(false); 
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
			_labelsGroup->setMinimumSize(186,25);
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

		void QtSinTracksPlot::SetMarks(std::vector<unsigned>& marks)
		{
			((SinTracksPlotController*)_controller)->SetMarks(marks);
		}

	        std::vector<unsigned>& QtSinTracksPlot::GetMarks()
		{
		    return ((SinTracksPlotController*)_controller)->GetMarks();
		}

		void QtSinTracksPlot::SetMarksColor(Color c)
		{
			((SinTracksPlotController*)_controller)->SetMarksColor(c);
		}
	}
}

// END

