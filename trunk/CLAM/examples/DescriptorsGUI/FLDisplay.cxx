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

#include "FLDisplay.hxx"
#include "FLMultiDisplay.hxx"

#include <FL/fl_draw.H>


using namespace CLAMGUI;

FLDisplay::FLDisplay(int x,int y,int w,int h)
:Fl_Widget(x,y,w,h)
{
	mInMultiDisplay = 0;
}

void FLDisplay::draw(void) { 
	if (mInMultiDisplay) {
		mInMultiDisplay->draw();
	}
	else
	{
		fl_push_clip(x(),y(),w(),h());
		DrawSelf(); 
		fl_pop_clip();
	}
}
