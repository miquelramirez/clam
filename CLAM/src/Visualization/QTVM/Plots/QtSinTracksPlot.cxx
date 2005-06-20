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
		QtSinTracksPlot::QtSinTracksPlot(QWidget* parent, const char * name, WFlags f) 
			: QtPresentation(parent,name,f)
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
			((SinTracksPlotController*)mController)->SetData(segment);
		}

		void QtSinTracksPlot::SetData(const Array< SpectralPeakArray >& peakMtx, 
									  const TData& sr, const TData& dur)
		{
			((SinTracksPlotController*)mController)->SetData(peakMtx,sr,dur);
		}

		void QtSinTracksPlot::SetDialColor(Color c)
		{
			((SinTracksPlotController*)mController)->SetDialColor(c);
		}

		void QtSinTracksPlot::SetRegionColor(Color c)
		{
			((SinTracksPlotController*)mController)->SetRegionColor(c);
		}

		void QtSinTracksPlot::updateRegion(MediaTime time)
		{
			mLabelsGroup->UpdateLabels(time);
		}

		void QtSinTracksPlot::keyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
					((SinTracksPlotController*)mController)->SetKeyShiftPressed(true);
					break;

				case Qt::Key_Insert:
					((SinTracksPlotController*)mController)->SetKeyInsertPressed(true); 
					break;
						
				case Qt::Key_Delete:
					((SinTracksPlotController*)mController)->SetKeyDeletePressed(true); 
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
					((SinTracksPlotController*)mController)->SetKeyShiftPressed(false);			
					break;

				case Qt::Key_Insert:
					((SinTracksPlotController*)mController)->SetKeyInsertPressed(false); 
					break;
						
				case Qt::Key_Delete:
					((SinTracksPlotController*)mController)->SetKeyDeletePressed(false); 
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
			connect(((SinTracksPlotController*)mController),SIGNAL(selectedRegion(MediaTime)),this,SLOT(updateRegion(MediaTime)));
		}

		void QtSinTracksPlot::InitSinTracksPlot()
		{
			QHBoxLayout* panel = new QHBoxLayout;
			panel->addStretch(1);
			
			mLabelsGroup = new TimeSegmentLabelsGroup(this);
			mLabelsGroup->setMinimumSize(186,25);
			panel->addWidget(mLabelsGroup);

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
			((SinTracksPlotController*)mController)->SetMarks(marks);
		}

		std::vector<unsigned>& QtSinTracksPlot::GetMarks()
		{
			return ((SinTracksPlotController*)mController)->GetMarks();
		}

		void QtSinTracksPlot::SetMarksColor(Color c)
		{
			((SinTracksPlotController*)mController)->SetMarksColor(c);
		}

		std::vector<QString> QtSinTracksPlot::GetSegmentationTags()
		{
			return ((SinTracksPlotController*)mController)->GetTags();
		}

    }
}

// END

