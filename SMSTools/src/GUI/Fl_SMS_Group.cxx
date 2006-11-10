#include "Fl_SMS_Group.hxx"
#include <FL/fl_draw.H>


Fl_SMS_Group::Fl_SMS_Group( int x, int y, int w, int h, char* label ) 
	: Fl_Group( x, y, w, h, label ) 
{
}

Fl_SMS_Group::~Fl_SMS_Group()
{
}

int Fl_SMS_Group::handle( int event )
{
	if ( event == FL_ENTER  ) 
		fl_cursor( FL_CURSOR_DEFAULT, FL_BLACK, FL_WHITE );			
	
	return Fl_Group::handle( event );
}

