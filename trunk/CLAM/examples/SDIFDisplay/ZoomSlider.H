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

#ifndef __ZOOMSLIDER__
#define __ZOOMSLIDER__

#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Valuator.H>

class ZoomSlider:public Fl_Widget
{
/*D/ 
	Slider widget that can be used for both zooming and scrolling.
	The thumb of the slider is divided into 3 parts, the top and bottom
	for adjusting the scale factor, the middle for scrolling.
*/
private:
	float scale_;
	float offset_;
	int v1,v2;
	
	void update1();
	void update2();
	void resize(int X,int Y,int W,int H);
public:
	float scale(void) { return scale_; }
	float offset(void) { return offset_; }

	void scale(float v) { scale_=v; update2(); }
	void offset(float v) { offset_=v; update2(); }

	ZoomSlider(int X,int Y,int W,int H,int T): Fl_Widget(X,Y,W,H)
	{
		box(FL_THIN_DOWN_FRAME);
		scale(1);
		offset(0);
		type(T);
	}	
	void draw(void);
	int handle(int evt);
};

inline void ZoomSlider::update1() {
//D/ Calculate scale and offset from the physical positions of the thumb-parts
	if (type()==FL_HORIZONTAL) {
		scale_=float(v2-v1+1)/float(w()-2);
		offset_=float(v1-1)/float(w()-2);
	}else{
		scale_=float(v2-v1+1)/float(h()-2);
		offset_=float(h()-2-v2)/float(h()-2);		
	}
}

inline void ZoomSlider::update2() {
//D/ Calculate the physical positions of the thumb-parts for scale and offset 
	if (type()==FL_HORIZONTAL) {
		v1=int(offset_*float(w()-2))+1;
		v2=int((scale_)*float(w()-2))+v1-1;
	}else{
		v2=h()-2-int(offset_*float(h()-2));		
		v1=v2+1-int((scale_)*float(h()-2));
	}
}

inline void ZoomSlider::resize(int X,int Y,int W,int H)
{
//D/ Resize the slider, and update the thumb positions
	Fl_Widget::resize(X,Y,W,H);
	update2();
}	

#endif
