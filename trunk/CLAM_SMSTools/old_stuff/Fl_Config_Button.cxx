#include "Fl_Config_Button.hxx"
#include <FL/fl_draw.H>

Fl_Config_Button::Fl_Config_Button( int x, int y, int w, int h, char* label ) 
	: Fl_Button( x, y, w, h, label ) 
{
	box( FL_NO_BOX );
}

Fl_Config_Button::~Fl_Config_Button()
{
}

int handle( int event )
{
	if( active( ) ) 
	{
		if ( event == FL_ENTER  ) 
		{
			fl_cursor( FL_CURSOR_PENCIL, FL_BLACK, FL_WHITE );			
		}
		else if ( event == FL_LEAVE  ) 
		{
			fl_cursor( FL_CURSOR_DEFAULT, FL_BLACK, FL_WHITE );			
		}
	}
	
	return Fl_Button::handle( event );
}
