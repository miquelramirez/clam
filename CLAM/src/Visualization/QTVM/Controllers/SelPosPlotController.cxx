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

#include "SelPosPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		SelPosPlotController::SelPosPlotController()
		{
		}

		SelPosPlotController::~SelPosPlotController()
		{
		}

		void SelPosPlotController::SetSelPos(const TData& value)
		{
			if(CanDrawSelectedPos())
			{
				PlotController::SetSelPos(value);
				UpdateDial(GetSelPos());
			}
		}

		void SelPosPlotController::Draw()
		{
			SegmentationMarksPlotController::Draw();
			mDial.Render();
		}

		void SelPosPlotController::SetDialColor(Color c)
		{
			mDial.SetColor(c);
		}
				
		void SelPosPlotController::SetHBounds(const TData& left,const TData& right)
		{
			SegmentationMarksPlotController::SetHBounds(left,right);
			mDial.SetHBounds(GetLeftBound(),GetRightBound());
		}

		void SelPosPlotController::SetVBounds(const TData& bottom,const TData& top)
		{
			SegmentationMarksPlotController::SetVBounds(bottom,top);
			mDial.SetVBounds(GetBottomBound(),GetTopBound());
		}

		void SelPosPlotController::UpdateDial(const TData& value)
		{
			mDial.Update(value);
		}

		TData SelPosPlotController::GetDialPos() const
		{
			return mDial.GetPos();
		}
    }
}

// END
