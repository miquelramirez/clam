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
			int maxY = y()+h()-1;
			int maxX = x()+w()-1;
			float dx = float(w())/float(size);
			float cx = x();
			for (int i=0; i<size-1; i++)
			{
				tmp=(int)(ptr[i]*0.006);
				
				float currentY = y()+h()/2;
				float overCurrentY = currentY + tmp;
				float nextY = y()+h()/2+(int)(ptr[i+1]*0.006);

				int currentX = int(cx);
				int nextX = int(cx + dx);

				//lets clamp values that overpasses the limit:
				if (currentY > maxY) currentY = maxY;
				if (overCurrentY > maxY) overCurrentY = maxY;
				if (nextY > maxY) nextY = maxY;
				if (currentX > maxX) currentX = maxX;
				if (nextX > maxX) nextX = maxX;
				
		
				fl_color(FL_RED);				
				fl_line(currentX, currentY, currentX, overCurrentY);

				fl_color(FL_WHITE);
				fl_line(currentX, overCurrentY, nextX,nextY);
				cx+=dx;
			}
		}
 fl_pop_clip();
}

} // end of namespace CLAM