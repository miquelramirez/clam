#ifndef __CSALTOSPECTRALDISPLAY__
#define __CSALTOSPECTRALDISPLAY__

#include <FL/Fl_Widget.H>
#include "Array.hxx"

class SDToolTip;

namespace CLAM
{


class CSaltoSpectralDisplay:public Fl_Widget
{
public:
	CSaltoSpectralDisplay(int X,int Y,int W,int H);	
	virtual ~CSaltoSpectralDisplay();
//	float *ptr;
	float ptr[1000]; //this is very ugly, we're assuming that we won't have more than 1000 peaks
	int size;

	SDToolTip* mSDToolTip;

	int SetPtr(Array<float>& displayPeakArray);
	int handle(int);
	void draw(void);
	
};

} // end of namespace CLAM

#endif // CSaltoSpectralDisplay.hxx
