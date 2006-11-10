#include "Fl_Display_Button.hxx"
#include <FL/fl_draw.H>

Fl_Display_Button::Fl_Display_Button( int x, int y, int w, int h, char* label ) 
	: Fl_Button( x, y, w, h, label ) 
{
}

Fl_Display_Button::~Fl_Display_Button()
{
}

int Fl_Display_Button::handle( int event )
{
	if( active( ) ) 
	{
		if ( event == FL_ENTER  ) 
			fl_cursor( FL_CURSOR_EYE, FL_BLACK, FL_WHITE );			
		
		else if ( event == FL_LEAVE ) 
			fl_cursor( FL_CURSOR_DEFAULT, FL_BLACK, FL_WHITE );			
		
	}
	return Fl_Button::handle( event );
}

