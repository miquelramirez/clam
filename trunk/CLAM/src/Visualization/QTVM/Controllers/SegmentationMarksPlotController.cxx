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

#include "SegmentationMarksPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		SegmentationMarksPlotController::SegmentationMarksPlotController()
		{
			_mustProcessMarks=false;
		}

		SegmentationMarksPlotController::~SegmentationMarksPlotController()
		{
		}
			
		void SegmentationMarksPlotController::SetMarks(std::list<unsigned>& marks)
		{
			_marks = marks;
			_mustProcessMarks=true;
		}

		void SegmentationMarksPlotController::SetMarksColor(Color c)
		{
			_marksRenderer.SetColor(c);
		}

		void SegmentationMarksPlotController::Draw()
		{
			if(_marks.size() > 0)
			{
				if(_mustProcessMarks) ProcessMarksData();
				_marksRenderer.Render();
			}
			
		}

		void SegmentationMarksPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
			_mustProcessMarks=true;
		}

		void SegmentationMarksPlotController::SetHBounds(const TData& left, const TData& right)
		{
			PlotController::SetHBounds(left,right);
			_marksRenderer.SetHBounds(GetLeftBound(),GetRightBound());
			_mustProcessMarks=true;
		}

		void SegmentationMarksPlotController::SetVBounds(const TData& bottom, const TData& top)
		{
			PlotController::SetVBounds(bottom,top);
			_marksRenderer.SetVBounds(GetBottomBound(),GetTopBound());
			_mustProcessMarks=true;
		}

		void SegmentationMarksPlotController::ProcessMarksData()
		{
			unsigned left = unsigned(GetLeftBound());
			unsigned right = unsigned(GetRightBound());

			Array<unsigned> processedMarks;

			Marks::iterator it = _marks.begin();
			for(;it != _marks.end();it++)
			{
				if((*it) > right) break;
				if((*it) >= left) processedMarks.AddElem((*it));
			}

			_marksRenderer.SetData(processedMarks);
			_mustProcessMarks=false;
		}
	}
}

// END

