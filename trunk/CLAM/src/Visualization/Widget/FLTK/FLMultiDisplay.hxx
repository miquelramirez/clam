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

#ifndef _FLMultiDisplay_
#define _FLMultiDisplay_

#include <FL/Fl_Group.H>
#include "FLDisplay.hxx"
#include "GLPortNew.hxx"

namespace CLAMGUI
{

class FLDisplayGroup : public Fl_Group
{
public:
	FLDisplayGroup(int x,int y,int w,int h)
	: Fl_Group(x,y,w,h)
	{
	}

	void draw(void) { }
};

class FLMultiDisplay : public FLDisplay
{
public:
	FLDisplayGroup mChildren; // TODO: As soon as I figure out why the hell the attributte is public it will be made private and 
	                          // accessors will be provided
public:
	FLMultiDisplay( int x, int y, int w, int h );
	void end(void) { mChildren.end(); }
	void draw(void);
};

}

#endif
