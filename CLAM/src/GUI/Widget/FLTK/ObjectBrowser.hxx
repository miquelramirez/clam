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

#ifndef __OBJECTBROWSER__
#define __OBJECTBROWSER__

#include <FL/Fl_Group.H>
#include <FL/Fl_Scrollbar.H>
#include "GridLayout.hxx"
#include "GeometryKit.hxx"
#include "ObjectIcon.hxx"


namespace CLAMGUI
{

class ObjectBrowser : public Fl_Group
{
public:

	ObjectBrowser( int objatonce, const Rect<int>& geometry, const char* label );
	
	virtual ~ObjectBrowser()
	{
		delete mScroll;
	}
	
	void resize( int x, int y, int w, int h );
	void draw();
	int handle ( int event );

	void AddObject( ObjectIcon* );

	void RemoveObject( ObjectIcon* obj );


protected:

	void Init();

private:

	GridLayout           mGenLayout;
	GridLayout           mObjLayout;
	int                  mObjAtOnce;
	unsigned int         mObjCnt;
	Fl_Scrollbar*        mScroll;
	Fl_Box*              mCanvas;
};

}


#endif // ObjectBrowser.hxx
