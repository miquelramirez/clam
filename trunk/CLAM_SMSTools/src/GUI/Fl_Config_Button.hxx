#ifndef __FL_CONFIG_BUTTON__
#define __FL_CONFIG_BUTTON__

#include <FL/Fl_Button.H>

// @todo: substitute this by static const Fl_Config_Button::FL_CURSOR_PENCIL
#define FL_CURSOR_PENCIL (enum Fl_Cursor)44

class Fl_Config_Button : public  Fl_Button 
{
public:
	Fl_Config_Button( int x, int y, int w, int h, char* label = "" );

	virtual ~Fl_Config_Button();

	virtual int handle( int event );

};

#endif // Fl_Config_Button.hxx
