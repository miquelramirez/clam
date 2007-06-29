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

#ifndef __VMQT_GRID_H__
#define __VMQT_GRID_H__

#include "vmRenderer2D.hxx"
#include <QtGui/QColor>

namespace CLAM
{
	namespace VM
	{
		class Grid : public Renderer2D
		{
		public:
			Grid();
			~Grid();

			void ShowGrid(bool show);
			void SnapToGrid(bool snap);
			void SetGridSteps(double xstep, double ystep);
		    void SetGridColor(const QColor& color);

			bool ShowGrid() const;
			bool SnapToGrid() const;

			double XStep() const;
			double YStep() const;

		protected:
			void Render();

		private:
			bool   mShowGrid;
			bool   mSnapToGrid;
			double mGridXStep;
			double mGridYStep;
			
			QColor mGridColor;
		};
	}
}

#endif

