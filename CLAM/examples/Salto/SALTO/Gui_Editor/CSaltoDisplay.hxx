#ifndef __CSALTODISPLAY__
#define __CSALTODISPLAY__

#include "DataTypes.hxx"
#include <FL/Fl_Widget.H>

namespace CLAM
{

class CSaltoDisplay:public Fl_Widget
{
public:
	CSaltoDisplay(int X,int Y,int W,int H);	
	TData *ptr;
	int size;

	void draw(void);
};

} // end of namespace CLAM

#endif //CSaltoDisplay.hxx