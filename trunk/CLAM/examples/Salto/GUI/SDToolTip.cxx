#include "SDToolTip.hxx"
#include <cstring>
#include <cstdio>

SDToolTip::SDToolTip( int x, int y, int w, int h )
	: Fl_Box( x, y, w, h )
{
	labelsize(10);
	align( FL_ALIGN_INSIDE|FL_ALIGN_CENTER );
	box( FL_ROUNDED_BOX );
	color( FL_YELLOW );
}

void SDToolTip::SetValue( float db, float freq )
{
	snprintf( mStr, 60, "%0.1f Db, %0.1f Hz", db, freq );
	label( mStr );
}
