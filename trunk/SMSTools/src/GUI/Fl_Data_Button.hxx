#ifndef __FL_DATA_BUTTON__
#define __FL_DATA_BUTTON__

#include <FL/Fl_Button.H>

// @todo: substitute this by some static const enum Fl_Cursor Fl_Data_Button::FL_CURSOR_EYE
#define FL_CURSOR_EYE (enum Fl_Cursor)65

class Fl_Data_Button : public  Fl_Button 
{
public:
	Fl_Data_Button( int x, int y, int w, int h, char* label = "" );

	virtual ~Fl_Data_Button();

	virtual int handle( int event );

};

#endif //Fl_Data_Button.hxx
