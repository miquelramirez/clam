#ifndef __FL_FILE_BUTTON__
#define __FL_FILE_BUTTON__

#include <FL/Fl_Button.H>

class Fl_File_Button : public  Fl_Button 
{
public:

	Fl_File_Button( int x, int y, int w, int h, char* label = "" );

	virtual ~Fl_File_Button();

	virtual int handle( int event );

};

#endif //Fl_File_Button.hxx
