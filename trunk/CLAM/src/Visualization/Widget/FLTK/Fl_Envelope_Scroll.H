/*****************************************************************************h
Fl_Envelope Widgets

Filename:   Fl_Envelope_Scroll.H
Programmer: Maarten de Boer
            mdeboer@iua.upf.es / maarten@things.nl
Date:       20 Jun 1999

This is free software; you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation;
either version 2 of the License, or (at your option) any later version.
******************************************************************************/
#ifndef __FL_ENVELOPE_SCROLL__
#define __FL_ENVELOPE_SCROLL__

#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Scrollbar.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Slider.H>

#include "Fl_Envelope.H"

class Fl_EnvelopeControl
{
private:
	Fl_Scrollbar* hscroll;
	Fl_Scrollbar* vscroll;
	Fl_Slider*	  hrange;
	Fl_Slider*	  vrange;
	Fl_Envelope*  envelope;
	Fl_Widget*	  callbacktarget;
	
	float hdiv,htotal,hmul;
	float vdiv,vtotal,vmul;
public:
	Fl_EnvelopeControl(
		Fl_Envelope* e,
		Fl_Scrollbar* hs,Fl_Scrollbar* vs,
		Fl_Slider* 	hr,Fl_Slider* vr,Fl_Widget* _callbacktarget=0);
	static void h_cb(Fl_Scrollbar* sb,Fl_EnvelopeControl* ec);
	static void v_cb(Fl_Scrollbar* sb,Fl_EnvelopeControl* ec);

	/**
	*   Controls the exacts coords of the WCS window:
	*   \param position Starting x point
	*   \param size     The portion of the range visible ( counting from the 'position' value )
	*   \param top      The minimum x point value
	*   \param total    The total range supported
	*/
    void hvalue(float position, float size, float top, float total);

	/**
	*   Controls the exacts coords of the WCS window:
	*   \param position The 'topmost' y point
	*   \param size     The portion of the range visible ( counting from the 'position' value )
	*   \param top      The minimum y point value
	*   \param total    The total range supported
	*/
    void vvalue(float position, float size, float top, float total);

	static void e_cb(Fl_Envelope* ev,Fl_EnvelopeControl* ec);
};

class Fl_Envelope_Scroll : public Fl_Group
{
public:
	Fl_Scrollbar* hscroll;
	Fl_Scrollbar* vscroll;
	Fl_Slider*    hslider;
	Fl_Slider*    vslider;
private:
	Fl_Double_Window* window;
	bool          mIsActive;
public:
	Fl_Envelope*  envelope;	
	Fl_EnvelopeControl* control;

	Fl_Envelope_Scroll(int x,int y,int w,int h);

	int handle( int event );

	void margin_adjust(void);
};

#endif
