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
		QtMultiPlot::QtMultiPlot(QWidget* parent)
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
			
			_xLabel = new SingleLabel(this);
			panel->addWidget(_xLabel);

			_yLabel = new SingleLabel(this);
			panel->addWidget(_yLabel);

			QFrame* righthole = new QFrame(this);
			righthole->setFixedSize(20,_yLabel->height());
			panel->addWidget(righthole);

			AddToMainLayout(panel);

		}

		void QtMultiPlot::AddData(std::string key, const DataArray& array)
		{
			((MultiPlotController*)_controller)->AddData(key,array);
		}
		
		void QtMultiPlot::AddData(std::string key, const BPF& data, int samples)
		{
			((MultiPlotController*)_controller)->AddData(key,data,samples);
		}
		
		void QtMultiPlot::RemoveData( std::string key )
		{
			((MultiPlotController*)_controller)->RemoveData(key);
		}
		
		void QtMultiPlot::RemoveAllData()
		{
			((MultiPlotController*)_controller)->RemoveAllData();
		}

		void QtMultiPlot::SetColor(std::string key, Color c)
		{
			((MultiPlotController*)_controller)->SetColor(key,c);
		}

		void QtMultiPlot::SetXRange(const TData& xmin, const TData& xmax)
		{
			((MultiPlotController*)_controller)->SetXRange(xmin,xmax);
		}
		
		void QtMultiPlot::SetYRange(const TData& ymin, const TData& ymax)
		{
			((MultiPlotController*)_controller)->SetYRange(ymin,ymax);
		}

		void QtMultiPlot::SetPlotController()
		{
			SetController(new MultiPlotController());
		}

		void QtMultiPlot::Connect()
		{
			connect(((MultiPlotController*)_controller),SIGNAL(xRulerScaleDiv(QwtScaleDiv)),this,SLOT(setXRulerScaleDiv(QwtScaleDiv)));
			connect(((MultiPlotController*)_controller),SIGNAL(yRulerScaleDiv(QwtScaleDiv)),this,SLOT(setYRulerScaleDiv(QwtScaleDiv)));
			connect(((MultiPlotController*)_controller),SIGNAL(xvalue(TData)),this,SLOT(updateXLabel(TData)));
			connect(((MultiPlotController*)_controller),SIGNAL(yvalue(TData)),this,SLOT(updateYLabel(TData)));
			connect(((MultiPlotController*)_controller),SIGNAL(leavingMouse()),this,SLOT(clearLabels()));
		}

		void QtMultiPlot::DisplayBackgroundBlack()
		{
			SetBackgroundColor(VMColor::Black());
			SetMarksColor(VMColor::Orange());
		}

		void QtMultiPlot::DisplayBackgroundWhite()
		{
			SetBackgroundColor(VMColor::White());
			SetMarksColor(VMColor::Red());
		}

		void QtMultiPlot::SetUnits(const std::string& xunits, const std::string& yunits)
		{
			_xLabel->SetUnits(xunits.c_str());
			_yLabel->SetUnits(yunits.c_str());
		}

		void QtMultiPlot::SetToolTips(const std::string& xtooltip, const std::string& ytooltip)
		{
			_xLabel->SetToolTip(xtooltip.c_str());
			_yLabel->SetToolTip(ytooltip.c_str());
		}

		void QtMultiPlot::updateXLabel(TData value)
		{
			_xLabel->Update(value);
		}

		void QtMultiPlot::updateYLabel(TData value)
		{
			_yLabel->Update(value);
		}

		void QtMultiPlot::clearLabels()
		{
			_xLabel->setText("");
			_yLabel->setText("");
		}

		void QtMultiPlot::SetMarks(std::list<unsigned>& marks)
		{
			((MultiPlotController*)_controller)->SetMarks(marks);
		}

		void QtMultiPlot::SetMarksColor(Color c)
		{
			((MultiPlotController*)_controller)->SetMarksColor(c);
		}
	}
}

// END

