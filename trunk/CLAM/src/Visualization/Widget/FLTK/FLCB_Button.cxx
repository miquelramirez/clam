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

#include "FLCB_Button.hxx"
#include <FL/Fl.H>
#include <FL/Fl_Group.H>

using namespace CLAMGUI;

FLCB_Button::FLCB_Button( const CLAMGUI::Rect<int>& geometry, const char* label ) :
	Fl_Button( geometry.GetX(), geometry.GetY(), geometry.GetW(), geometry.GetH(), label ),
	mChangedCb ( NULL ), mNotChangedCb( NULL ), mReleaseCb( NULL )
{
	oldval = 0;
}

FLCB_Button::~FLCB_Button()
{
	FreeCallbacks();
}

int FLCB_Button::value(int v) 
{
	v = v ? 1 : 0;
	oldval = v;
	clear_changed();
	if (Fl_Button::value() != v) 
	{
		Fl_Button::value( v );
		redraw();
		return 1;
	}
	else return 0;
}

void FLCB_Button::FreeCallbacks()
{
	if ( mChangedCb != NULL )
		delete mChangedCb;
	if ( mNotChangedCb != NULL )
		delete mNotChangedCb;
	if ( mReleaseCb != NULL )
		delete mReleaseCb;
}

void FLCB_Button::SetChangedCb( const CBL::Functor0& callback )
{
	if ( mChangedCb != NULL )
		delete mChangedCb;

	when( FL_WHEN_CHANGED );

	mChangedCb = new CBL::Functor0( callback );
}

void FLCB_Button::SetNotChangedCb( const CBL::Functor0& callback )
{
	if ( mNotChangedCb != NULL )
		delete mChangedCb;

	when( FL_WHEN_NOT_CHANGED );

	mNotChangedCb = new CBL::Functor0( callback );
}

void FLCB_Button::SetReleaseCb( const CBL::Functor0& callback )
{
	if ( mReleaseCb != NULL )
		delete mReleaseCb;

	when( FL_WHEN_RELEASE );

	mReleaseCb = new CBL::Functor0( callback );
}

int FLCB_Button::handle( int event )
{
	int newval;
	switch (event) 
	{
		case FL_ENTER:
		case FL_LEAVE:
			//  if ((value_?selection_color():color())==FL_GRAY) redraw();
			return 1;
		case FL_PUSH:
		case FL_DRAG:
			if (Fl::event_inside(this)) 
			{
				if (type() == FL_RADIO_BUTTON) 
					newval = 1;
				else 
					newval = !oldval;
			} 
			else
				newval = oldval;
			if (newval != value()) 
			{
				value( newval );
				redraw();
				if (when() & FL_WHEN_CHANGED) 
				{
					if ( mChangedCb != NULL )
						(*mChangedCb)();
				}
			}
			return 1;
		case FL_RELEASE:
			if (!value(0)) 
			{
				if (when() & FL_WHEN_NOT_CHANGED) 
				{
					if ( mNotChangedCb != NULL )
						(*mNotChangedCb)();
				}
				return 1;
			}
			if (type() == FL_RADIO_BUTTON)
				setonly();
			else if (type() == FL_TOGGLE_BUTTON)
				oldval = value();
			else 
			{
				value(oldval);
				if (when() & FL_WHEN_CHANGED) 
				{
					if ( mChangedCb != NULL )
						(*mChangedCb)();
				}

			}
			if (when() & FL_WHEN_RELEASE) 
			{
				if ( mReleaseCb != NULL )
					(*mReleaseCb)();
			}
			else 
				set_changed();
			return 1;
		case FL_SHORTCUT:
			if (!(shortcut() ?
				Fl::test_shortcut(shortcut()) : test_shortcut())) 
				return 0;
			if (type() == FL_RADIO_BUTTON && !value()) 
			{
				setonly();
				if (when() & FL_WHEN_CHANGED) 
				{
					if ( mChangedCb != NULL )
						(*mChangedCb)();
				}

			} 
			else if (type() == FL_TOGGLE_BUTTON) 
			{
				value(!value());
				if (when() & FL_WHEN_CHANGED) 
				{
					if ( mChangedCb != NULL )
						(*mChangedCb)();
				}
			}
			if (when() & FL_WHEN_RELEASE) 
			{
				if ( mReleaseCb != NULL )
					(*mReleaseCb)();
			}
			else 
				set_changed();
			return 1;
		default:
			return 0;
	}
}

