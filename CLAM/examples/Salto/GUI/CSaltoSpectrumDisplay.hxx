#ifndef __CSALTOSPECTRUMDISPLAY__
#define __CSALTOSPECTRUMDISPLAY__

#include <FL/Fl_Widget.H>
#include "Array.hxx"

class SDToolTip;

namespace CLAM
{


class CSaltoSpectrumDisplay:public Fl_Widget
{
public:
	CSaltoSpectrumDisplay(int X,int Y,int W,int H);	
	virtual ~CSaltoSpectrumDisplay();
//	float *ptr;
	float ptr[1000];
	int size;

	SDToolTip* mSDToolTip;

	int SetPtr(Array<float>& displaySpecArray);
	int handle(int);
	void draw(void);
	
};

} // end of namespace CLAM

#endif // CSaltoSpectrumDisplay.hxx
