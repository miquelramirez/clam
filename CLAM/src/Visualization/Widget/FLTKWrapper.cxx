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
#include <FL/Fl_Widget.H>
#include <algorithm>

namespace CLAMVM
{

FLTKWrapper& FLTKWrapper::GetInstance()
{
	static FLTKWrapper wrapper;

	return wrapper;
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

void FLTKWrapper::SetFPS( unsigned desired_fps )
{
	mTimeoutInterval = 1.0 / float( desired_fps ); 
}

void FLTKWrapper::DisableAsynchronousRefresh()
{
	CancelAllAsynchronousRefresh();
}

unsigned FLTKWrapper::RequestAsynchronousRefresh( Fl_Widget* pWidget )
{
	unsigned assigned_slot;
	
	// determine if there are freed slots
	if ( mFreedSlots.empty() ) // no available slots
		{
			assigned_slot = mNextSlot;
			mNextSlot++;
		}
	else
		{
			assigned_slot = mFreedSlots.top();
			mFreedSlots.pop();
		}

	Refreshee new_refreshee = { pWidget, assigned_slot };

	mWidgetsToBeRefreshed.push_back( new_refreshee );

	return assigned_slot;
}

void FLTKWrapper::CancelAsynchronousRefresh( unsigned freed_slot ) 
{
	// Find wether the supplied slot is actually a valid one
	
	std::list<Refreshee>::iterator i = mWidgetsToBeRefreshed.begin();
	bool found = false;

	while ( i != mWidgetsToBeRefreshed.end() && !found )
		{
			if ( (*i).mSlotAssigned == freed_slot )
				found = true;
			else
				i++;
		}

	if ( found )
		{
			mWidgetsToBeRefreshed.erase(i);
			mFreedSlots.push( freed_slot );
		}
}

void FLTKWrapper::ActivateAsynchronousRefresh()
{
	Fl::add_timeout( mTimeoutInterval, FLTKWrapper::sRefreshingCallback, this );
}

void FLTKWrapper::CancelAllAsynchronousRefresh()
{
	mWidgetsToBeRefreshed.erase( mWidgetsToBeRefreshed.begin(), mWidgetsToBeRefreshed.end() );
}

void FLTKWrapper::sRefreshingCallback( void* data )
{

	FLTKWrapper* pFl = ( FLTKWrapper* ) data;

	if ( !pFl->mWidgetsToBeRefreshed.empty() )
		{

			std::list<Refreshee>::iterator i = pFl->mWidgetsToBeRefreshed.begin();
			Fl_Widget* childWidget = NULL;

			while ( i != pFl->mWidgetsToBeRefreshed.end() )
				{
					childWidget = (*i).mpWidget;
					childWidget->resize(childWidget->x(),childWidget->y(), childWidget->w(), childWidget->h() );
					childWidget->redraw();
					i++;
				}
		}

	Fl::repeat_timeout( pFl->mTimeoutInterval, FLTKWrapper::sRefreshingCallback, pFl );
}


}
