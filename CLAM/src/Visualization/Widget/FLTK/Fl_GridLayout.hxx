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

#ifndef __FLGRIDLAYOUT__
#define __FLGRIDLAYOUT__

#include <FL/Fl_Group.H>
#include <list>

namespace CLAMVM
{

class Fl_GridLayout 
	: public Fl_Group
{

	struct TChildLayouts
	{
		Fl_Widget*    pChild;
		int           row_idx;
		int           col_idx;
		int           row_span;
		int           col_span;
		bool          first_resize;
		bool          width_fixed;
		bool          height_fixed;
	};

	typedef std::list<TChildLayouts> ListChildren;

public:

	Fl_GridLayout( int rows, int cols );

	virtual void resize( int x, int y, int w, int h );

	void AddMultiCellWidget( Fl_Widget* w, int xc, int yc, int xs, int ys );

	void MakeHeightFixed()
	{
		if ( !theChildren.empty() )
			theChildren.back().height_fixed=true;
	}

	void MakeWidthFixed()
	{
		if ( !theChildren.empty() )
			theChildren.back().width_fixed=true;
	}

	int handle( int evt );

protected:

	void ResizeChild( TChildLayouts& child);
	void draw();


private:

	int           mRows;
	int           mCols;
	int           mRow_dpx;
	int           mCol_dpx;
	ListChildren  theChildren;
};

}
#endif // Fl_GridLayout.hxx
