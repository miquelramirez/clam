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

#ifndef _FLDisplay_
#define _FLDisplay_

#include <FL/Fl_Widget.H>
#include "Range.hxx"


namespace CLAMGUI
{

class FLMultiDisplay;

class FLDisplay:public Fl_Widget
{
public:
	Range mVerRange;
	Range mHorRange;
	FLMultiDisplay* mInMultiDisplay;
public:
	FLDisplay(int x,int y,int w,int h);

	virtual void draw(void); 

	virtual void DrawSelf(void)
	{
	}
	void SetHorRange(const Range& range)
	{
		mHorRange = range;
	}
	void SetVerRange(const Range& range)
	{
		mVerRange = range;
	}
};

}

#endif
