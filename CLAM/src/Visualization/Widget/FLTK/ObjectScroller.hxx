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

#ifndef __OBJECTSCROLLER__
#define __OBJECTSCROLLER__

#include <FL/Fl_Scroll.H>
#include "GridLayout.hxx"
#include "GeometryKit.hxx"
#include "ObjectIcon.hxx"
#include <string>
using std::string;

namespace CLAMGUI
{
class ObjectScroller : public Fl_Scroll
{
public:

	ObjectScroller ( int objAtOnce, const Rect<int>& geometry, const char* label );

	virtual ~ObjectScroller()
	{
	}

	void draw();
	int handle( int event );
	void resize( int x, int y, int w, int h );
	void HandleLighting( std::string& s );
	void HandleAction( std::string& s );

	void AddObject( ObjectIcon* icon );
	void RemoveAllObjects();
	void RemoveObject( ObjectIcon* icon );

protected:
	
	void Init();
private:

	GridLayout       mGenLayout;
	GridLayout       mObjLayout;
	int              mObjAtOnce;
	unsigned int     mObjCnt;
};

}

#endif // ObjectScroller.hxx
