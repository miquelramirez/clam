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
