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

#ifndef __DEBUGGERCP__
#define __DEBUGGERCP__

#include <FL/Fl_Window.H>
#include "FLCB_Button.hxx"
#include "GeometryKit.hxx"

namespace CLAMGUI
{

class DebuggerCP
{
public:

	DebuggerCP()
		: mCPWindow( NULL ), mNextBtn( NULL ), mContBtn( NULL ), mExitBtn( NULL )
	{
	}

	void BuildWidgets( const Rect<int>& scrspace );

	void ShowAll();

	void HideAll();

	virtual ~DebuggerCP()
	{
		if ( mCPWindow != NULL )
			delete mCPWindow;
	}

public:

	Fl_Window*     mCPWindow;
	FLCB_Button*   mNextBtn;
	FLCB_Button*   mContBtn;
	FLCB_Button*   mExitBtn;

};

}

#endif // DebuggerCP.hxx
