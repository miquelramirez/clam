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

#include "FLTKWrapper.hxx"
#include <FL/Fl.H>

using namespace CLAMGUI;

FLTKWrapper* FLTKWrapper::GetInstance()
{
	Fl::visual( FL_DOUBLE );
	return new FLTKWrapper;
}

bool FLTKWrapper::IsClosing() const
{
	return ( Fl::first_window() == NULL );
}

// GUI open loop 
void FLTKWrapper::Tick() const
{
	// MRJ: There seems to exist some kind
	// of race condition while using X11, since
	// seems that somehow FLTK tries to route
	// user events to already destroyed X11 clients.
	// Let's try to make sure that there any suitable
	// X11 client before forcing redrawing and event
	// processing ( I should have checked this before,
	// since although these functions are public 
	// it seems that nobody uses them so explicitly... so
	// maybe some essential check is not done etc. )
	
	if ( Fl::ready() )
		{
			if ( !IsClosing() )
				Fl::check();
		}

	if ( !IsClosing() )
		Fl::flush();
	

}

// GUI closed loop
void FLTKWrapper::Run() const
{
	Fl::run();
}
