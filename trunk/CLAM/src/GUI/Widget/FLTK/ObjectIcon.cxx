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

#include "ObjectIcon.hxx"
#include <FL/Fl.H>
using namespace CLAMGUI;


ObjectIcon::ObjectIcon()
	: Fl_Box( 0, 0, 0, 0, NULL ), mSelected ( false )
{
	Init();
}

void ObjectIcon::Init()
{
	align( FL_ALIGN_RIGHT | FL_ALIGN_INSIDE );
	labelfont( FL_COURIER_BOLD );
	labelsize( 10 );
	labelcolor( FL_BLACK );
	color( FL_WHITE ); 
	box( FL_THIN_UP_BOX );
}

void ObjectIcon::SetObjectName( const char* name )
{
	mObjectName = std::string( name );
	label( mObjectName.c_str() );
}

void ObjectIcon::SetActionCallback( const CBL::Functor1< std::string& >& cb )
{
	mActionCb = cb;
}

void ObjectIcon::SetSelectionCallback( const CBL::Functor1< std::string& >& cb )
{
	mSelCb = cb;
}

void ObjectIcon::draw()
{
	Fl_Box::draw();
}

void ObjectIcon::Highlight()
{
	labelcolor( FL_WHITE );
	color ( FL_BLUE );
	redraw();

}

void ObjectIcon::Unlight()
{
	mSelected = false;
	labelcolor( FL_BLACK );
	color( FL_WHITE );
	redraw();
}

int ObjectIcon::handle( int event )
{

	switch ( event )
	{
		case FL_PUSH:
			if ( Fl::event_clicks() )
			{
				mActionCb ( mObjectName );
			}
			else
			{
				if ( !mSelected )
				{
					mSelected = true;
					mSelCb( mObjectName );
				}
				else
				{
					mSelected = false;
					Unlight();
				}

				redraw();
			}
			break;
		default:
			return 0; // Widget is not interested in any other events
	};

	return 1;
}
