/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __GRIDLAYOUT__
#define __GRIDLAYOUT__

#include "GeometryKit.hxx"
#include "Layout.hxx"

namespace CLAMGUI
{
	/**
	 *  This class provides the functionality required to make that a given rect
	 *  adapts to the layout provided by the GridLayout object
	 */
	class GridLayout : public Layout
	{
	private:
		/**
		 *  Number of rows in the layout
		 */
		int mRows;
		/**
		 *  Number of columns in the layout
		 */
		int	mCols;
		
		/**
		 *  The area the GridLayout is managing
		 */
		Recti mArea;
		
	public:
		/**
		 *  Constructor for GridLayout
		 *  @param cols the number of columns in the new GridLayout
		 *  @param rows the number of rows in the new GridLayout
		 */
		
		GridLayout() 
			: mRows(0), mCols(0)
		{
		}
		
		GridLayout(int cols, int rows)
			: mRows(rows), mCols(cols)
		{
		}
		
		GridLayout(int cols, int rows, const Recti& r) 
			: mRows(rows), mCols(cols), mArea(r)
		{
		}
		
		GridLayout(int cols, int rows, const Recti* r) 
			: mRows(rows), mCols(cols)
		{
			mArea.SetX(r->GetX());
			mArea.SetY(r->GetY());
			mArea.SetW(r->GetW());
			mArea.SetH(r->GetH());
		}

		/** Copy Constructor
		 *
		 */
		
		GridLayout(const GridLayout& g)
		{
			mRows = g.GetRows();
			mCols = g.GetCols();
			mArea = g.GetArea();
		}
		
		/**
		 *  Destructor for GridLayout
		*/
		virtual ~GridLayout()
		{
		}
		
		void SetCols(int cols) { mCols = cols; }
		void SetRows(int rows) { mRows = rows; }
		int GetCols() const { return mCols; }
		int GetRows() const { return mRows; }
		
		/**
		 *  Modifier for setting the Layout Area Of Action
		 *  @param r the rect that containes the LAOA
		 */
		void SetArea(const Recti& r)
		{
			mArea = r;
		}
		
		
		Recti GetArea() const
		{
			return mArea;
		}
		
		/**
		 *  This operator is overloaded in a uncommon way since in this case:
		 *  
		 *  Rect r(1,1,1,2);
		 *  Rect gridArea(100, 100, 300, 300)
		 *  GridLayout g(3,3,gridArea);
		 *  Rect result;
		 *  
		 *  result = g << r;
		 *
		 *  Result will have the following coordinates on screen:
		 *  (200, 200, 100, 200) roughly
		 *
		 *  @param r the Rect to be adapted
		 *  @return the resulting Rect
		 */
		
		Recti operator<< (const Recti& r);
	 
	};
	
}

#endif // GridLayout.hxx

