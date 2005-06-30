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
			: SingleDisplayPlot(parent,name,f)
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

		void QtMultiPlot::SetXRange(const double& xmin, const double& xmax)
		{
			((MultiPlotController*)mController)->SetXRulerRange(xmin,xmax);
		}
		
		void QtMultiPlot::SetYRange(const double& ymin, const double& ymax)
		{
			SingleDisplayPlot::initialYRulerRange(ymin,ymax);
			((MultiPlotController*)mController)->SetYRulerRange(ymin,ymax);
		}

		void QtMultiPlot::SetPlotController()
		{
			SetController(new MultiPlotController());
		}

		void QtMultiPlot::Connect()
		{
			connect(((MultiPlotController*)mController),SIGNAL(sendXYValues(double,double)),
					this,SLOT(updateLabels(double,double)));
		}

		void QtMultiPlot::DisplayBackgroundBlack()
		{
			SetDialColor(VMColor::Red());
			SetMarksColor(VMColor::Orange());
			SetBackgroundColor(VMColor::Black());
		}

		void QtMultiPlot::DisplayBackgroundWhite()
		{
			SetDialColor(VMColor::Black());
			SetMarksColor(VMColor::Red());
			SetBackgroundColor(VMColor::White());
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

		void QtMultiPlot::updateLabels(double xvalue, double yvalue)
		{
			mXLabel->Update(xvalue,2);
			mYLabel->Update(yvalue,2);
		}

    }
}

// END

