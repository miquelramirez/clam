#include "CSaltoDisplay.hxx"
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Slider.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include "Array.hxx"

namespace CLAM
{

CSaltoDisplay::CSaltoDisplay(int X,int Y,int W,int H):Fl_Widget(X,Y,W,H)
{
	ptr=0;
	size=0;
	this->box(FL_BORDER_FRAME);
}

void CSaltoDisplay::draw(void)
{
	fl_push_clip(x(),y(),w(),h());
		int tmp;
		fl_color(FL_BLACK);
		fl_rectf(x(),y(),w(),h());
		if (size && ptr)
		{
			float dx = float(w())/float(size);
			float cx = x();
			for (int i=0;i<size;i++)
			{
//				fl_color(8);				
				fl_color(FL_RED);				
//				fl_point((int)cx,y()+h()+(int)(ptr[i]*0.001-60));
				tmp=(int)(ptr[i]*0.006);
				fl_line((int)cx,y()+h()/2,(int)cx,y()+h()/2+tmp);
				fl_color(FL_WHITE);
//				fl_point((int)cx,y()+h()+(int)(ptr[i]*0.006-60));
				fl_line((int)cx,y()+h()/2+tmp,(int)cx+dx,y()+h()/2+(int)(ptr[i+1]*0.006));
				cx+=dx;
			}
		}
 fl_pop_clip();
}

} // end of namespace CLAM