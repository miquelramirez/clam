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

#ifndef __FLCB_BUTTON__
#define __FLCB_BUTTON__

#include <FL/Fl_Button.H>
#include "GeometryKit.hxx"
#include "CBL.hxx"


namespace CLAMGUI
{

class FLCB_Button : public Fl_Button
{
public:
	FLCB_Button( const CLAMGUI::Rect<int>& geometry, const char* label = 0);
	virtual ~FLCB_Button();

	int value( int val );
	int handle( int event );

	char value() const 
	{
		return Fl_Button::value();
	}

	void SetChangedCb( const CBL::Functor0& callback );
	void SetNotChangedCb( const CBL::Functor0& callback );
	void SetReleaseCb( const CBL::Functor0& callback );

protected:

	void FreeCallbacks();

private:
	CBL::Functor0*  mChangedCb;
	CBL::Functor0*  mNotChangedCb;
	CBL::Functor0*  mReleaseCb;
	char oldval;
};

}

#endif // FLCB_Button.hxx
