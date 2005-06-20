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
#include "MultiPlotController.hxx"
#include "QtMultiPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		QtMultiPlot::QtMultiPlot(QWidget* parent, const char * name, WFlags f)
			: QtPresentation(parent,name,f)
		{
			SetPlotController();
			InitMultiPlot();
			Connect();
		}

		QtMultiPlot::~QtMultiPlot()
		{
		}

		void QtMultiPlot::InitMultiPlot()
		{
			QHBoxLayout* panel = new QHBoxLayout;
			panel->addStretch(1);
			
			mXLabel = new SingleLabel(this);
			panel->addWidget(mXLabel);

			mYLabel = new SingleLabel(this);
			panel->addWidget(mYLabel);

			QFrame* righthole = new QFrame(this);
			righthole->setFixedSize(20,mYLabel->height());
			panel->addWidget(righthole);

			AddToMainLayout(panel);
		}

		void QtMultiPlot::AddData(std::string key, const DataArray& array)
		{
			((MultiPlotController*)mController)->AddData(key,array);
		}
		
		void QtMultiPlot::AddData(std::string key, const BPF& data, int samples)
		{
			((MultiPlotController*)mController)->AddData(key,data,samples);
		}
		
		void QtMultiPlot::RemoveData( std::string key )
		{
			((MultiPlotController*)mController)->RemoveData(key);
		}
		
		void QtMultiPlot::RemoveAllData()
		{
			((MultiPlotController*)mController)->RemoveAllData();
		}

		void QtMultiPlot::SetColor(std::string key, Color c)
		{
			((MultiPlotController*)mController)->SetColor(key,c);
		}

		void QtMultiPlot::SetXRange(const TData& xmin, const TData& xmax)
		{
			((MultiPlotController*)mController)->SetXRange(xmin,xmax);
		}
		
		void QtMultiPlot::SetYRange(const TData& ymin, const TData& ymax)
		{
			QtPresentation::initialYRulerRange(double(ymin),double(ymax));
			((MultiPlotController*)mController)->SetYRange(ymin,ymax);
		}

		void QtMultiPlot::SetPlotController()
		{
			SetController(new MultiPlotController());
		}

		void QtMultiPlot::Connect()
		{
			connect(((MultiPlotController*)mController),SIGNAL(xvalue(TData)),this,SLOT(updateXLabel(TData)));
			connect(((MultiPlotController*)mController),SIGNAL(yvalue(TData)),this,SLOT(updateYLabel(TData)));
			connect(((MultiPlotController*)mController),SIGNAL(leavingMouse()),this,SLOT(clearLabels()));
		}

		void QtMultiPlot::DisplayBackgroundBlack()
		{
			SetBackgroundColor(VMColor::Black());
			SetDialColor(VMColor::Red());
			SetMarksColor(VMColor::Orange());
		}

		void QtMultiPlot::DisplayBackgroundWhite()
		{
			SetBackgroundColor(VMColor::White());
			SetDialColor(VMColor::Black());
			SetMarksColor(VMColor::Red());
		}

		void QtMultiPlot::SetUnits(const std::string& xunits, const std::string& yunits)
		{
			mXLabel->SetUnits(xunits.c_str());
			mYLabel->SetUnits(yunits.c_str());
		}

		void QtMultiPlot::SetToolTips(const std::string& xtooltip, const std::string& ytooltip)
		{
			mXLabel->SetToolTip(xtooltip.c_str());
			mYLabel->SetToolTip(ytooltip.c_str());
		}

		void QtMultiPlot::updateXLabel(TData value)
		{
			mXLabel->Update(value);
		}

		void QtMultiPlot::updateYLabel(TData value)
		{
			mYLabel->Update(value);
		}

		void QtMultiPlot::clearLabels()
		{
			mXLabel->setText("");
			mYLabel->setText("");
		}

		void QtMultiPlot::SetMarks(std::vector<unsigned>& marks)
		{
			((MultiPlotController*)mController)->SetMarks(marks);
		}
	    
		std::vector<unsigned>& QtMultiPlot::GetMarks()
		{
			return ((MultiPlotController*)mController)->GetMarks();
		}

		void QtMultiPlot::SetMarksColor(Color c)
		{
			((MultiPlotController*)mController)->SetMarksColor(c);
		}

		void QtMultiPlot::keyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Insert:
					((MultiPlotController*)mController)->SetKeyInsertPressed(true); 
					break;
						
				case Qt::Key_Delete:
					((MultiPlotController*)mController)->SetKeyDeletePressed(true); 
					break;
				    
				default:
					break;
			}
		}

		void QtMultiPlot::keyReleaseEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Insert:
					((MultiPlotController*)mController)->SetKeyInsertPressed(false); 
					break;
						
				case Qt::Key_Delete:
					((MultiPlotController*)mController)->SetKeyDeletePressed(false); 
					break;
				    
				default:
					break;
			}
		}

		void QtMultiPlot::SetDialColor(Color c)
		{
			((MultiPlotController*)mController)->SetDialColor(c);
		}

		std::vector<QString> QtMultiPlot::GetSegmentationTags()
		{
			return ((MultiPlotController*)mController)->GetTags();
		}

    }
}

// END

