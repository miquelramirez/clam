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

#include "FLDisplayContainer.hxx"
#include "ErrGUI.hxx"

using namespace CLAMGUI;
using namespace CLAMVM;

void FLDisplayContainer::SliderCB(Fl_Slider* slider,Fl_Scrollbar* scrollbar)
{
	int v = scrollbar->value();
	int max = 400;

	double f = slider->value();

	if (f>0) 
		f = 0.5+(f/slider->maximum())/2.;
	else 
		f = 0.5/(1.+-f);

	int r = static_cast<int>( f*max );
	if (v+r > max) 
		v=max-r;
	scrollbar->value( v, r, 0, max);
	scrollbar->do_callback();	
}

void FLDisplayContainer::ScrollbarCB( Fl_Scrollbar* scrollbar, MRuler* ruler)
{
	ruler->FollowSlider( scrollbar );
	ruler->redraw();
}

void FLDisplayContainer::HorSliderCB(Fl_Slider* slider)
{
	FLDisplayContainer* c = dynamic_cast< FLDisplayContainer* > (slider->parent());

	if (c == NULL ) throw ( ErrGUI( "Pointer was messed" )  );

	SliderCB( c->mpHorSlider, c->mpHorScrollbar );

}
void FLDisplayContainer::HorScrollCB(Fl_Scrollbar* scrollbar)
{
	FLDisplayContainer* c = dynamic_cast< FLDisplayContainer* > ( scrollbar->parent() );

	if (c == NULL ) throw ( GUIException( "Pointer was messed" )  );

	ScrollbarCB( c->mpHorScrollbar, c->mpHorRuler );
	c->mpMultiDisplay->mHorRange = c->mpHorRuler->mRange;

	c->mpMultiDisplay->redraw();

}

void FLDisplayContainer::VerSliderCB( Fl_Slider* slider )
{
	FLDisplayContainer* c = dynamic_cast< FLDisplayContainer* > ( slider->parent() );

	if (c == NULL ) throw ( GUIException( "Pointer was messed" )  );

	SliderCB( c->mpVerSlider, c->mpVerScrollbar );

}

void FLDisplayContainer::VerScrollCB(Fl_Scrollbar* scrollbar)
{
	FLDisplayContainer* c = dynamic_cast< FLDisplayContainer* > ( scrollbar->parent() );

	if (c == NULL ) throw ( GUIException( "Pointer was messed" )  );

	ScrollbarCB( c->mpVerScrollbar, c->mpVerRuler );
	c->mpMultiDisplay->mVerRange = c->mpVerRuler->mRange;
	c->mpMultiDisplay->redraw();

}


FLDisplayContainer::FLDisplayContainer(int x,int y,int w,int h)
:Fl_Group(x,y,w,h)
{
	mpHorRuler = NULL;
	mpHorScrollbar = NULL;
	mpHorSlider = NULL;
	mpVerRuler = NULL;	
	mpVerScrollbar = NULL;
	mpVerSlider = NULL;

	mpHorRuler     = new MRuler(    x,y+h-60,w-70,20);
	mpHorScrollbar = new Fl_Scrollbar( x,y+h-40,w-70,20);
	mpHorSlider    = new Fl_Slider(    x,y+h-20,w-70,20);

	mpVerRuler     = new MRuler(    x+w-70,y,30,h-60);
	mpVerScrollbar = new Fl_Scrollbar( x+w-40,y,20,h-60);
	mpVerSlider    = new Fl_Slider(    x+w-20,y,20,h-60);

	mpHorRuler->type(FL_HORIZONTAL);
	mpHorSlider->type(FL_HORIZONTAL);
	mpHorScrollbar->type(FL_HORIZONTAL);

	mpVerRuler->type(FL_VERTICAL);
	mpVerSlider->type(FL_VERTICAL);
	mpVerScrollbar->type(FL_VERTICAL);

	mpHorSlider->box(FL_THIN_DOWN_BOX);
	mpVerSlider->box(FL_THIN_DOWN_BOX);

	mpHorSlider->range(-10,1);
	mpVerSlider->range(-10,1);

	mpHorSlider->value(1);
	mpVerSlider->value(1);

	mpMultiDisplay = new FLMultiDisplay(x,y,w-70,h-60);
	mpMultiDisplay->color(FL_BLACK);
	mpMultiDisplay->end();

	mpHorSlider->callback((Fl_Callback*)HorSliderCB);
	mpHorScrollbar->callback((Fl_Callback*)HorScrollCB);

	mpVerSlider->callback((Fl_Callback*)VerSliderCB);
	mpVerScrollbar->callback((Fl_Callback*)VerScrollCB);

	resizable(mpMultiDisplay);

	end();
}		

void FLDisplayContainer::SetHorRange( double top, double total )
{
	double pos = top, size = total<0?-total:total;

	mpMultiDisplay->mHorRange.Set(pos,size,top,total);
	mpHorScrollbar->value(int(pos),int(size),int(top),int(total));
	mpHorRuler->mRange = mpMultiDisplay->mHorRange;
}

void FLDisplayContainer::SetVerRange(double top,double total)
{
	double pos = top, size = total<0?-total:total;

	mpMultiDisplay->mVerRange.Set(pos,size,top,total);
	mpVerScrollbar->value(int(pos),int(size),int(top),int(total));
	mpVerRuler->mRange = mpMultiDisplay->mVerRange;
}

void FLDisplayContainer::Add( FLDisplay& display )
{
	mpMultiDisplay->mChildren.add( &display );
	display.resize(x(),y(),w()-70,h()-60);
}

void FLDisplayContainer::Add( CLAMGUI::GLPort& port )
{
	mpMultiDisplay->mChildren.add( &port );
	port.resize( x(), y(), w() - 70, h() - 60 );
}
