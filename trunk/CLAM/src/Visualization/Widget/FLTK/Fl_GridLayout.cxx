#include "Fl_GridLayout.hxx"
#include <iostream>

namespace CLAMVM
{

Fl_GridLayout::Fl_GridLayout( int rows, int cols )
	: Fl_Group( 0, 0, 0, 0 ), mRows( rows ), mCols( cols )
{

}

void Fl_GridLayout::AddMultiCellWidget( Fl_Widget* w, int xc, int yc, int xs, int ys )
{
	TChildLayouts l = { w, yc, xc, ys, xs, true, false, false };

	theChildren.push_back( l );

	add( *w );
}

void Fl_GridLayout::draw()
{

	Fl_Widget* const* a = array();

	if (damage() == FL_DAMAGE_CHILD) 
	{ 
		// only redraw some children
		for (int i = children(); i --; a ++) 
			update_child(**a);
	} 
	else 
	{ 
		// total redraw
		draw_box();
		// now draw all the children atop the background:
		for (int i = children(); i --; a ++) 
		{
			draw_child(**a);
		}
	}

}

int Fl_GridLayout::handle( int evt )
{
	if ( evt == FL_RELEASE )
	{
		redraw();
		return 1;
	}

	return Fl_Group::handle(evt);
}

void Fl_GridLayout::resize( int x, int y, int w, int h )
{
	// We assume the parent ( or the parent's parent etc. ) to
	// be a Fl_Window widget ( i.e. sub widgets positions are absolute with
	// relation to the parent's upper left corner)
	Fl_Widget::resize( 0, 0, w, h );

	mRow_dpx = this->h() / mRows;
	mCol_dpx = this->w() / mCols;


	ListChildren::iterator i = theChildren.begin();

	for ( ; i != theChildren.end(); i++ )
		ResizeChild( *i );


	redraw();
}

void Fl_GridLayout::ResizeChild( TChildLayouts& child)
{
	int child_x, child_y, child_w, child_h;

	child_x =  (child.col_idx * mCol_dpx );
	child_y =  (child.row_idx * mRow_dpx );
	if ( !child.first_resize ) 
	{
		if ( child.width_fixed )
			child_w = child.pChild->w();
		else
			child_w = child.col_span * mCol_dpx;
		
		if ( child.height_fixed )
			child_h = child.pChild->h();
		else
			child_h = child.row_span * mRow_dpx;
	}
	else
	{
		child_w = child.col_span * mCol_dpx;
		child_h = child.row_span * mRow_dpx;
		child.first_resize = false;

	}
	child.pChild->resize( child_x, child_y, child_w, child_h );
	child.pChild->redraw();
	
}

}
