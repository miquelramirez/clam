/*****************************************************************************h
Fl_Envelope Widgets

Filename:   Fl_Envelope_Scroll.cxx
Programmer: Maarten de Boer
            mdeboer@iua.upf.es / maarten@things.nl
Date:       20 Jun 1999

This is free software; you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.
******************************************************************************/
#include "Fl_Envelope_Scroll.H"

// class Fl_EnvelopeControl

Fl_EnvelopeControl::Fl_EnvelopeControl(
	Fl_Envelope* e,
	Fl_Scrollbar* hs,Fl_Scrollbar* vs,
	Fl_Slider* 	hr,Fl_Slider* vr,
	Fl_Widget* _callbacktarget)
{
	envelope=e;
	hscroll=hs;
	vscroll=vs;
	hrange=hr;
	vrange=vr;
	callbacktarget=_callbacktarget;

	hscroll->callback((Fl_Callback*)h_cb,this);
	vscroll->callback((Fl_Callback*)v_cb,this);
	hrange->callback((Fl_Callback*)h_cb,this);
	vrange->callback((Fl_Callback*)v_cb,this);

	envelope->callback((Fl_Callback*)e_cb,this);
}

void Fl_EnvelopeControl::h_cb(Fl_Scrollbar* sb,Fl_EnvelopeControl* ec)
{
/*	printf("\n%f %f %f %f\n",
		(float) ec->hscroll->value(),
		(float) ec->hrange->value(),
		(float) ec->hscroll->minimum(),
		(float) ec->htotal);
*/		
	ec->hvalue(
		ec->hscroll->value()*ec->hdiv,
		ec->hrange->value(),
		ec->hscroll->minimum()*ec->hdiv,
		ec->htotal);
}

void Fl_EnvelopeControl::v_cb(Fl_Scrollbar* sb,Fl_EnvelopeControl* ec)
{
	ec->vvalue(
		ec->vscroll->value()*ec->vdiv,
		ec->vrange->value(),
		ec->vscroll->minimum()*ec->vdiv,
		ec->vtotal);
}

void Fl_EnvelopeControl::hvalue(float position, float size, float top, float total)
{
//	printf("%f %f %f %f\n",position,size,top,total);
	if (total<1000) {
		hmul=1000.;
		hdiv=0.001f;
	}else{
		hmul=1;
		hdiv=1;
	}
	
//	printf("%f\n",hmul);

	htotal=total;
	if (position+size>(top+total)) {
		position=(top+total)-size;
	}
	envelope->xbound(top,top+total);
	envelope->xscale(position,position+size);
	hscroll->value(position*hmul,size*hmul,top*hmul,total*hmul);
	hrange->range(0.01,total);
	hrange->value(size);
	envelope->redraw();
}

void Fl_EnvelopeControl::vvalue(float position, float size, float top, float total)
{
	if (total<1000) {
		vmul=1000.;
		vdiv=0.001f;
	}else{
		vmul=1;
		vdiv=1;
	}
	vtotal=total;
	if (position+size>(top+total)) {
		position=(top+total)-size;
	}

	float position2=(top+total)-(position-top)-size;

	envelope->ybound(top,top+total);
	envelope->yscale(position2,position2+size);
	vscroll->value(position*vmul,size*vmul,top*vmul,total*vmul);
	vrange->range(0.01,total);
	vrange->value(size);
	envelope->redraw();
}

void Fl_EnvelopeControl::e_cb(Fl_Envelope* ev,Fl_EnvelopeControl* ec)
{
	FLPOINT* p=ec->envelope->getPointPtr(ec->envelope->dragging());
	float dx=0;
	float dy=0;
	
	if (p) {
		if (p->x > ec->envelope->xmax()) dx=p->x - ec->envelope->xmax();
		else if (p->x < ec->envelope->xmin()) dx=p->x - ec->envelope->xmin();

		if (p->y > ec->envelope->ymax()) dy=p->y - ec->envelope->ymax();
		else if (p->y < ec->envelope->ymin()) dy=p->y - ec->envelope->ymin();

		if (dx) 
			ec->hvalue(
				ec->hscroll->value()*ec->hdiv+dx,
				ec->hrange->value(),
				ec->hscroll->minimum()*ec->hdiv,
				ec->htotal);

		if (dy) 
			ec->vvalue(
				ec->vscroll->value()*ec->vdiv+dy,
				ec->vrange->value(),
				ec->vscroll->minimum()*ec->vdiv,
				ec->vtotal);
	}
	if (ec->callbacktarget) {
		ec->callbacktarget->do_callback();
	}
}


// class Fl_Envelope_Scroll

Fl_Envelope_Scroll::Fl_Envelope_Scroll(int x,int y,int w,int h)
	: Fl_Group(x,y,w,h), mIsActive( true )
{
	hscroll = new Fl_Scrollbar( x, y+h-36, w-36, 18);
	vscroll = new Fl_Scrollbar( x+w-36, y, 18, h-36);
	hslider = new Fl_Slider(x,y+h-18, w-36,18);
	vslider = new Fl_Slider(x+w-18, y, 18, h-36);
	window = new Fl_Double_Window( x,y,w-36,h-36);
	envelope = new Fl_Envelope( x,y,w-36,h-36);
	control = new Fl_EnvelopeControl( envelope,hscroll,vscroll,hslider,vslider,this);
	window->end();
	hscroll->type(FL_HOR_SLIDER);
	hslider->type(FL_HOR_SLIDER);
	end();
	//resizable( envelope );
	resizable( window );
	window->resizable(envelope);
}

int Fl_Envelope_Scroll::handle( int event )
{
	int retValue = Fl_Group::handle( event );

	return retValue;
}

void Fl_Envelope_Scroll::margin_adjust(void)
{
	hscroll->resize(
		x()+envelope->leftmargin(),y()+h()-36,
		w()-36-envelope->hmargin(),18);
	vscroll->resize(
		x()+w()-36,y()+envelope->topmargin(),
		18,h()-36-envelope->vmargin());
	hslider->resize(
		x()+envelope->leftmargin(),y()+h()-18,
		w()-36-envelope->hmargin(),18);
	vslider->resize(
		x()+w()-18,y()+envelope->topmargin(),
		18,h()-36-envelope->vmargin());
}


