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

#include "QtAppWrapper.hxx"
#include "QtMultiPlot.hxx"
#include "DPMultiPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		DPMultiPlot::DPMultiPlot()
			: mPlot(0)
		{
			QtAppWrapper::Init();
			mPlot = new QtMultiPlot();
			mPlot->Geometry(100,100,500,225);
			mPlot->SetDialColor(VMColor::Black());
			mPlot->SwitchDisplayColors(true);
			mPlot->Show();
		}

		DPMultiPlot::~DPMultiPlot()
		{
			delete mPlot;
		}

		void DPMultiPlot::Label(const std::string& label)
		{
			mPlot->Label(label);
		}
				
		void DPMultiPlot::Geometry(int x, int y, int w, int h)
		{
			mPlot->Geometry(x,y,w,h);
		}

		void DPMultiPlot::AddData(const std::string& key, const DataArray& array)
		{
			mPlot->AddData(key,array);
		}

		void DPMultiPlot::SetColor(const std::string& key, const Color& c)
		{
			mPlot->SetColor(key,c);
		}
				
		void DPMultiPlot::SetXRange(const TData& xmin, const TData& xmax)
		{
			mPlot->SetXRange(xmin,xmax);
		}

		void DPMultiPlot::SetYRange(const TData& ymin, const TData& ymax)
		{
			mPlot->SetYRange(ymin,ymax);
		}

		void DPMultiPlot::SetUnits(const std::string& xunits, const std::string& yunits)
		{
			mPlot->SetUnits(xunits,yunits);
		}

		void DPMultiPlot::SetToolTips(const std::string& xtooltip, const std::string& ytooltip)
		{
			mPlot->SetToolTips(xtooltip,ytooltip);
		}

		void DPMultiPlot::Run()
		{
			QtAppWrapper::Run();
		}

		void DPMultiPlot::SetMarks(std::vector<unsigned>& marks)
		{
			mPlot->SetMarks(marks);
			mPlot->SetMarksColor(VMColor::Red());
		}
	}
}

// END


