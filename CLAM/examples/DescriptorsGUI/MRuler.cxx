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

#include "MRuler.hxx"
#include <FL/Fl_Slider.H>
#include <FL/fl_draw.H>
#include <cmath>
#include <cstdio>

#ifdef __MWERKS__
using std::fabs;
#endif


using namespace CLAMGUI;

MRuler::MRuler(int x,int y,int w,int h)
	: Fl_Widget(x,y,w,h)
{
	box(FL_FLAT_BOX);
	type(FL_VERTICAL);
	mInteger = false;
}

void MRuler::FollowSlider(Fl_Slider* s)
{
	float stotal = s->maximum()-s->minimum();
	if (stotal==0) 
		mRange.mPosition=mRange.mTop;
	else 
		mRange.mPosition = mRange.mTop + mRange.mTotal * s->value() * (1.-s->slider_size())/stotal;
	
	mRange.mSize = fabs(mRange.mTotal) * s->slider_size();
}

void MRuler::draw(void)
{
	fl_push_clip(x(),y(),w(),h());
	
	double valuePerPixel;
	double mul,every;
	int dec;
	double pRange;
	
	int maxcnt = 10;
	int longercnt = 1;

	if (type()==FL_HORIZONTAL) 
		pRange = double(w());
	else 
		pRange = double(h());
	
	draw_box();
	fl_color(FL_BLACK);
	fl_font(FL_HELVETICA,9);
	
	valuePerPixel = mRange.mSize/pRange; // position increment per pixel
	
	/* we can draw a number at most every 30 pixels. we calculate how often that is */
	every = valuePerPixel*30.; // this value has to be rounded to a nice divider
	
	dec = -int(log10(every)); // number of decimals
	if (every<1) dec++;
	
	mul = pow(10,-dec);
	if (every>5*mul) 
	{
		dec--;
		mul*=10;
	}
	
	if (every<=1*mul)
	{
		every = 1*mul;
	}
	else 
		every = 5*mul;

	if (dec<0) 
		dec=0;
	// every now is a nice divider for showing ruler numbers
	
	every/=10; // we want 10 lines per number
	
	if (mInteger)
	{
		maxcnt = int ( every*10 );
		if (maxcnt<1) maxcnt=1;
		else if (maxcnt>10) maxcnt=10;
		else if (maxcnt==5) longercnt = 0;
		if (every<1) every = 1;
		dec = 0;
		mul = 10;
	}
	
	double pixelsPerValue = pRange/mRange.mSize; // number of pixels per position increment
	double dp = every*pixelsPerValue; // every dp pixels we draw a line
	
	double p,v;
	p = mRange.mPosition;
	if (mRange.mTotal>0) 
	{
		v = ((int)(p / every + 0.5)) * every;
		p = int((v - p)*pixelsPerValue)+(type()==FL_HORIZONTAL?x():y())+0.5;
	}
	else
	{
		v = ((int)(p / every - 0.5)) * every;
		p = int((p - v)*pixelsPerValue)+(type()==FL_HORIZONTAL?x():y())+0.5;
	}
	
	/* we keep a counter to decide when to draw a number, and longer lines */
	int cnt; 
	double tmp = v/every;
	if (mRange.mTotal<0) 
		tmp = -tmp;
	
	if (tmp<0) 
	{
		cnt = maxcnt - 1 - int(-0.5-tmp)%maxcnt;
	}
	else 
	{
		cnt = int(tmp)%maxcnt;	
	}
	
	if (type()==FL_HORIZONTAL)
	{
		pRange = x()+pRange;
	}
	else
	{
		pRange = y()+pRange;
	}
	
	if (mRange.mTotal<0) 
		every=-every;
	
	int first = 1;
	float extra = every/100.;
	
	if (extra<0) 
		extra=-extra;
	
	while (p<pRange+30) 
	{
		char fmt[256];
		char txt[256];
		int to;
	 
		if (cnt==maxcnt) 
			cnt=0;
		
		if (cnt==0) 
		{
			sprintf(fmt,"%%.%df",dec);
			sprintf(txt,fmt,v+extra);
			fl_color(FL_BLACK);
		
			if (type()==FL_HORIZONTAL) 
			{
				fl_draw(txt,int(p)+1,y()+16);
				
				if (first) 
				{
					sprintf(txt,fmt,v-every*10+every/100.);
					fl_draw(txt,int(p-dp*10.)+1,y()+16);
					first = 0;
				}
			} 
			else
			{
				fl_draw(txt,x()+8,int(p)-2);			
			}
			to = 16;
		}
		else
		{
			to = 8-((cnt&longercnt)<<2);
		}
		
		if (type()==FL_HORIZONTAL)
		{
			fl_color(FL_WHITE);
			fl_yxline(int(p)-1,y(),y()+to);
			fl_color(FL_DARK3);
			fl_yxline(int(p),y(),y()+to);
		} 
		else
		{
			fl_color(FL_WHITE);
			fl_xyline(x(),int(p)-1,x()+to);		
			fl_color(FL_DARK3);
			fl_xyline(x(),int(p),x()+to);		
		}
		p+=dp;
		v+=every;
		cnt++;
	}
	
	fl_pop_clip();
}






