/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************
 *   Maarten de Boer <maarten.deboer@iua.upf.es>, 1999                     *
 *   Music Technology Group                                                *
 *   Institut de l'Audiovisual, Universitat Pompeu Fabra, Barcelona, Spain *
 *   http://www.iua.upf.es/mtg/                                            *
 ***************************************************************************/

#include "ZoomSlider.H"
#include <FL/fl_draw.H>
#include <FL/Fl.H>

void ZoomSlider::draw(void)
{
	draw_box();

	int w1 = v1;	
	int w2 = v2;

	if (type()==FL_HORIZONTAL) {
		if (w2<w1+17) w2=w1+17;
		fl_draw_box(FL_THIN_UP_FRAME,x()+w1,y()+1,w2-w1+1,h()-2,color());
		fl_draw_box(FL_THIN_DOWN_FRAME,x()+w1+5,y()+3,2,h()-6,color());
		fl_draw_box(FL_THIN_DOWN_FRAME,x()+w2-6,y()+3,2,h()-6,color());

		fl_color(color());
		fl_rectf(x()+1,y()+1,x()+w1-(x()+1),h()-2);
		fl_xyline(x()+w1+1,y()+2,x()+w2-1);
		fl_xyline(x()+w1+1,y()+h()-3,x()+w2-1);
		fl_rectf(x()+w1+1,y()+3,4,h()-6);
		fl_rectf(x()+w2-4,y()+3,4,h()-6);
		fl_rectf(x()+w2+1,y()+1,w()-2-(x()+w2-x()),h()-2);

		fl_rectf(x()+w1+7,y()+3,x()+w2-(x()+w1)-13,h()-6);
	}else{
		if (w1>w2-17) w1=w2-17;
		fl_draw_box(FL_THIN_UP_FRAME,x()+1,y()+w1,w()-2,w2-w1+1,color());
		fl_draw_box(FL_THIN_DOWN_FRAME,x()+3,y()+w1+5,w()-6,2,color());
		fl_draw_box(FL_THIN_DOWN_FRAME,x()+3,y()+w2-6,w()-6,2,color());

		fl_color(color());
		fl_rectf(x()+1,y()+1,w()-2,y()+w1-(y()+1));
		fl_yxline(x()+2,y()+w1+1,y()+w2-1);
		fl_yxline(x()+w()-3,y()+w1+1,y()+w2-1);
		fl_rectf(x()+3,y()+w1+1,w()-6,4);
		fl_rectf(x()+3,y()+w2-4,w()-6,4);
		fl_rectf(x()+1,y()+w2+1,w()-2,h()-2-(y()+w2-y()));

		fl_rectf(x()+3,y()+w1+7,w()-6,y()+w2-(y()+w1)-13);
	}
	return;
}

int ZoomSlider::handle(int evt)
{
	static int prev=0;
	static int dragging=0;	

	int w1=v1;
	int w2=v2;
	
	if (type()==FL_HORIZONTAL) {
		if (w2<w1+17) w2=w1+17;
	}else{
		if (w1>w2-17) w1=w2-17;	
	}
	
	switch (evt) {
		case FL_SHOW:
			update2();
			return 1;
		case FL_PUSH:
		{
			int o=0;
			if (type()==FL_HORIZONTAL) {
				prev=Fl::event_x();
				o=x();
			}else{
				prev=Fl::event_y();
				o=y();
			}

			dragging=0;	
			if (prev>o+w1 && prev<o+w2) {
				if (prev<o+w1+5) {
					dragging=1;
				}else if (prev>o+w2-5) {
					dragging=2;
				}else{
					dragging=3;
				}
			}
			return 1;
		}
		case FL_DRAG:
		{
			int cur,o,r;
			if (type()==FL_HORIZONTAL) {
				cur=Fl::event_x();
				o=x();
				r=w();
			}else{
				cur=Fl::event_y();
				o=y();
				r=h();
			}
			int d = cur-prev;
			if (dragging&1) {
					if (o+w1+d<o+1) d=(o+1)-(o+w1);
					if (dragging==1)
						if (o+w1+d>o+w2-17) d=o+w2-17-(o+w1);
			}
			if (dragging&2) {
					if (o+w2+d>o+r-2) d=o+r-2-(o+w2);
					if (dragging==2)
						if (o+w2+d<o+w1+17) d=o+w1+17-(o+w2);
			}
			if (dragging&1)
					v1+=d;
			if (dragging&2)
					v2+=d;

			if (dragging && d) {
				update1();
				do_callback();
				redraw();
			}
			prev=cur;

			return 1;
		}
	}
	return 0;
}
