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

#ifndef __CLASSESPANEL__
#define __CLASSESPANEL__

#include <FL/Fl_Double_Window.H>
#include "ObjectScroller.hxx"
#include "GeometryKit.hxx"

namespace CLAMGUI
{

class PORepository;

class ClassesPanel
{
public:

	ClassesPanel()
		: mWindow( NULL ), mScroll( NULL ), mRepository( NULL )
	{
	}

	~ClassesPanel();

	void ShowAll();

	void HideAll();

	void BuildWidgets( const Rect<int>& scrspace );

protected:

	void ShowPresentClasses();

public:

	Fl_Double_Window*   mWindow;
	ObjectScroller*     mScroll;
	PORepository*       mRepository;

};

}

#endif // ClassesPanel.hxx
