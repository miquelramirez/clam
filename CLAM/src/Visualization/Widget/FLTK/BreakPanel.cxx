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

#include "BreakPanel.hxx"
#include "GridLayout.hxx"
using namespace CLAMGUI;

void BreakPanel::BuildWidgets( const Rect<int>& scrspace )
{

	GridLayout layout( 1, 1, scrspace );
	Rect<int> scrollLogicalCoords( 0, 0, 1, 1 );
	Rect<int> physCoords;

	mBPWindow = new Fl_Double_Window( scrspace.GetX(), scrspace.GetY(), scrspace.GetW(), scrspace.GetH(), "Breakpoints Present" );

	physCoords = layout << scrollLogicalCoords;

	mScroll = new ObjectScroller( 5, physCoords, "Breakpoints present" );

	mBPWindow->end();
}

void BreakPanel::ShowAll()
{
	mBPWindow->show();
	mScroll->show();
}

void BreakPanel::HideAll()
{
	mBPWindow->hide();
	mScroll->hide();
}

