/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "CLAMGL.hxx"
#include "vmGrid.hxx"
#include "vmPlot2D.hxx"

namespace CLAM
{
	namespace VM
	{
		Grid::Grid()
			: mShowGrid(false)
			, mSnapToGrid(false)
			, mGridXStep(1.0)
			, mGridYStep(1.0)
			, mGridColor(0,0,0)
		{
		}

		Grid::~Grid()
		{
		}

		void Grid::ShowGrid(bool show)
		{
			mShowGrid = show;
			_container->needUpdate();
		}

		void Grid::SnapToGrid(bool snap)
		{
			mSnapToGrid = snap;
		}

		void Grid::SetGridSteps(double xstep, double ystep)
		{
			mGridXStep = xstep;
			mGridYStep = ystep;
			_container->needUpdate();
		}

		void Grid::SetGridColor(const QColor& color)
		{
			mGridColor = color;
			_container->needUpdate();
		}

		bool Grid::ShowGrid() const
		{
			return mShowGrid;
		}

		bool Grid::SnapToGrid() const
		{
			return mSnapToGrid;
		}

		double Grid::XStep() const
		{
			return mGridXStep;
		}

		double Grid::YStep() const
		{
			return mGridYStep;
		}

		void Grid::Render()
		{
			if(!mEnabled) return;
			if(!mShowGrid) return;
			
			glColor3ub(mGridColor.red(),mGridColor.green(),mGridColor.blue());
			glLineWidth(1);
			glBegin(GL_LINES);
			// vertical lines
			double pos = mXRange.min;
			while(pos < mView.right)
			{
				if(pos > mView.left)
				{
					glVertex2d(pos,mView.bottom);
					glVertex2d(pos,mView.top);
				}
				pos += mGridXStep;
			}
			// horizontal lines
			pos = mYRange.min;
			while(pos < mView.top)
			{
				if(pos > mView.bottom)
				{
					glVertex2d(mView.left,pos);
					glVertex2f(mView.right,pos);
				}
				pos += mGridYStep;
			}
			glEnd();
		}
	}
}

// END

