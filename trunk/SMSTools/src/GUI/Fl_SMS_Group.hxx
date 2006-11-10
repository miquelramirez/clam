#ifndef __FL_SMS_GROUP__
#define __FL_SMS_GROUP__

#include <FL/Fl_Group.H>

class Fl_SMS_Group : public  Fl_Group 
{

public:
	Fl_SMS_Group( int x, int y, int w, int h, char* label = "" );

	virtual ~Fl_SMS_Group();

	virtual int handle( int event );
};

#endif //Fl_SMS_Group.hxx
