#ifndef __FL_SMS_ARROW__
#define __FL_SMS_ARROW__

#include <FL/Fl_Button.H>

class Fl_SMS_Arrow : public Fl_Button
{
public:
	Fl_SMS_Arrow( int X, int Y, int W, int H, char* L );
	
	virtual ~Fl_SMS_Arrow();

	virtual void draw( void );
};


#endif // Fl_SMS_Arrow.hxx
