#include "Fl_Data_Button.hxx"
#include <FL/fl_draw.H>

Fl_Data_Button::Fl_Data_Button( int x, int y, int w, int h, char* label ) 
	: Fl_Button( x, y, w, h, label ) 
{
	box( FL_NO_BOX );
}

Fl_Data_Button::~Fl_Data_Button()
{
}

int Fl_Data_Button::handle( int event )
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

