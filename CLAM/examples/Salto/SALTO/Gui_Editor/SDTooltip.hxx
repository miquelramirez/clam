#ifndef __SDTOOLTIP__
#define __SDTOOLTIP__

#include <FL/Fl_Box.H>

class SDToolTip : public Fl_Box
{
public:

	char mStr[60];


public:
	
	SDToolTip( int X, int Y, int W, int H );
	void SetValue( float db, float freq );
};


#endif // SDToolTip.hxx

