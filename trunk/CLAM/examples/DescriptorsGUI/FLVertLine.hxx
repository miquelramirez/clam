#ifndef __FLVERTLINE__
#define __FLVERTLINE__

#include "GeometryKit.hxx"
#include <FL/fl_draw.H>

namespace CLAMGUI
{

struct FLVertLine
{

  Fl_Color myColor;

  void operator()( int x_value, const Recti& r )
  {
	Fl_Color prev = fl_color();
	fl_color( myColor );
    fl_line( x_value, r.GetY(), x_value, r.GetH() );
	fl_color( prev );
  }

};

}

#endif // FLVertLine.hxx
