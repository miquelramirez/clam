#ifndef __FL_DISPLAY_BUTTON__
#define __FL_DISPLAY_BUTTON__

#include <FL/Fl_Button.H>

// @todo: this macro is defined in several files!!!!
#ifndef FL_CURSOR_EYE
#define FL_CURSOR_EYE (enum Fl_Cursor)65
#endif

class Fl_Display_Button : public  Fl_Button 
{

public:
	Fl_Display_Button( int x, int y, int w, int h, char* label = "" );

	virtual ~Fl_Display_Button();

	virtual int handle( int event );
};

#endif //Fl_Display_Button.hxx
