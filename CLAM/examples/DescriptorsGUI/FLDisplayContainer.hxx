/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _FLDisplayContainer_
#define _FLDisplayContainer_

#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>
#include "FLMultiDisplay.hxx"
#include "MRuler.hxx"


class Fl_Scrollbar;
class Fl_Slider;

namespace CLAMGUI
{

class FLDisplayContainer:public Fl_Group
{
public:
	FLMultiDisplay* mpMultiDisplay;

	MRuler*        mpHorRuler;
	Fl_Scrollbar* mpHorScrollbar;
	Fl_Slider*    mpHorSlider;
	MRuler*        mpVerRuler;	
	Fl_Scrollbar* mpVerScrollbar;
	Fl_Slider*    mpVerSlider;

	static void SliderCB( Fl_Slider* slider, Fl_Scrollbar* scrollbar );
	static void ScrollbarCB( Fl_Scrollbar* scrollbar, MRuler* ruler );
	static void HorSliderCB( Fl_Slider* slider );
	static void HorScrollCB( Fl_Scrollbar* scrollbar );
	static void VerSliderCB( Fl_Slider* slider );
	static void VerScrollCB( Fl_Scrollbar* scrollbar );
	
	FLDisplayContainer(int x,int y,int w,int h);

	void SetHorRange(double top,double total);
	void SetVerRange(double top,double total);
	void Add( FLDisplay& display );
	void Add( CLAMGUI::GLPort& port );
};

}

#endif
