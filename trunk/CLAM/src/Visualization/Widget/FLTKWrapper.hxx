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

#ifndef __FLTKWRAPPER__
#define __FLTKWRAPPER__

#include "WidgetTKWrapper.hxx"
#include <list>
#include <stack>

class Fl_Widget;

namespace CLAMVM
{

class FLTKWrapper
	: public WidgetTKWrapper
{

	friend class WidgetTKWrapper;

	struct Refreshee
	{
		Fl_Widget*   mpWidget;
		unsigned     mSlotAssigned;
	};

public:

	virtual ~FLTKWrapper()
	{
		CancelAllAsynchronousRefresh();
	}

	void DisableAsynchronousRefresh();

	bool IsClosing() const;
	
	void Tick() const;

	void Run() const;

	void SetFPS( unsigned desired_fps );
	
	void ActivateAsynchronousRefresh();
	
	unsigned RequestAsynchronousRefresh( Fl_Widget* pWidget );

	void CancelAsynchronousRefresh( unsigned freed_slot );

private:

	static FLTKWrapper& GetInstance();
	static void  sRefreshingCallback( void* );

	void CancelAllAsynchronousRefresh();

	unsigned mNextSlot;
	float    mTimeoutInterval;

	std::stack<unsigned>   mFreedSlots;
	std::list< Refreshee > mWidgetsToBeRefreshed;

	FLTKWrapper()
		: mNextSlot( 0 )
	{
	}

};

}

#endif // FLTKWrapper.hxx
