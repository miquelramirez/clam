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

#include "DebuggerCP.hxx"
#include "GridLayout.hxx"

using namespace CLAMGUI;

void DebuggerCP::BuildWidgets( const Rect<int>& scrspace )
{
	GridLayout layout( 13, 4, scrspace );
	Rect<int>  nextBtnLogicalCoords( 1,1,3,2 );
	Rect<int>  contBtnLogicalCoords( 5,1,3,2 );
	Rect<int>  exitBtnLogicalCoords( 9,1,3,2 );
	Rect<int>  physCoords;


	mCPWindow = new Fl_Window( scrspace.GetX(), scrspace.GetY(), scrspace.GetW(), scrspace.GetH(), "CLAM Debugger Control Panel" );

	physCoords = layout << nextBtnLogicalCoords;

	mNextBtn = new FLCB_Button( physCoords, "Next" );

	mNextBtn->labelfont( FL_SCREEN_BOLD );
	mNextBtn->labelsize( 9 );

	physCoords = layout << contBtnLogicalCoords;

	mContBtn = new FLCB_Button( physCoords, "Continue" );

	mContBtn->labelfont( FL_SCREEN_BOLD );
	mContBtn->labelsize( 9 );

	physCoords = layout << exitBtnLogicalCoords;

	mExitBtn = new FLCB_Button( physCoords, "Exit" );

	mExitBtn->labelfont( FL_SCREEN_BOLD );
	mExitBtn->labelsize( 9 );


	mCPWindow->end();
}

void DebuggerCP::ShowAll()
{
	mCPWindow->show();
	mNextBtn->show();
	mContBtn->show();
	mExitBtn->show();
}

void DebuggerCP::HideAll()
{
	mCPWindow->hide();
	mCPWindow->hide();
	mCPWindow->hide();
	mCPWindow->hide();
}
