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
#include "SingleLabel.hxx"
#include "SpectrumPlotController.hxx"
#include "QtSpectrumPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		QtSpectrumPlot::QtSpectrumPlot(QWidget* parent, const char * name, WFlags f) 
			: QtPresentation(parent,name,f)
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
			QHBoxLayout* panel = new QHBoxLayout;
			panel->addStretch(1);
			
			mMagLabel = new SingleLabel(this," dB ","Magnitude");
			panel->addWidget(mMagLabel);

			mFreqLabel = new SingleLabel(this," Hz ","Frequency");
			panel->addWidget(mFreqLabel);

			QFrame* righthole = new QFrame(this);
			righthole->setFixedSize(20,mFreqLabel->height());
			panel->addWidget(righthole);

			AddToMainLayout(panel);
		}

		void QtSpectrumPlot::SetData(const Spectrum& spec)
		{
			((SpectrumPlotController*)mController)->SetData(spec);
		}

		void QtSpectrumPlot::SetForegroundColor(Color c)
		{
			((SpectrumPlotController*)mController)->SetDataColor(c);
		}

		void QtSpectrumPlot::SetVLineColor(Color c)
		{
			((SpectrumPlotController*)mController)->SetDialColor(c);
		}

		void QtSpectrumPlot::updateMagLabel(TData value)
		{
			mMagLabel->Update(value);
		}

		void QtSpectrumPlot::updateFreqLabel(TData value)
		{
			mFreqLabel->Update(value);
		}

		void QtSpectrumPlot::SetPlotController()
		{			
			SetController(new SpectrumPlotController());
		}

		void QtSpectrumPlot::Connect()
		{
			// Connections
			connect(((SpectrumPlotController*)mController),SIGNAL(mag(TData)),this,SLOT(updateMagLabel(TData)));
			connect(((SpectrumPlotController*)mController),SIGNAL(freq(TData)),this,SLOT(updateFreqLabel(TData)));
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

		void QtSpectrumPlot::SetMarks(std::vector<unsigned>& marks)
		{			
			((SpectrumPlotController*)mController)->SetMarks(marks);
		}

		std::vector<unsigned>& QtSpectrumPlot::GetMarks()
		{
			return ((SpectrumPlotController*)mController)->GetMarks();
		}
	
		void QtSpectrumPlot::SetMarksColor(Color c)
		{
			((SpectrumPlotController*)mController)->SetMarksColor(c);
		}

		void QtSpectrumPlot::keyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Insert:
					((SpectrumPlotController*)mController)->SetKeyInsertPressed(true); 
					break;
						
				case Qt::Key_Delete:
					((SpectrumPlotController*)mController)->SetKeyDeletePressed(true); 
					break;
				    
				default:
					break;

			}
		}

		void QtSpectrumPlot::keyReleaseEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Insert:
					((SpectrumPlotController*)mController)->SetKeyInsertPressed(false); 
					break;
						
				case Qt::Key_Delete:
					((SpectrumPlotController*)mController)->SetKeyDeletePressed(false); 
					break;
				    
				default:
					break;
			}
		}

		std::vector<QString> QtSpectrumPlot::GetSegmentationTags()
		{
			return  ((SpectrumPlotController*)mController)->GetTags();
		}

    }
}

// END


