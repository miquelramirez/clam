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

#include "ObjectScroller.hxx"
#include <list>
#include <typeinfo>
#include <iostream>
using std::cout;
using std::endl;
using namespace CLAMGUI;

ObjectScroller::ObjectScroller( int objAtOnce, const Rect<int>& g, const char* label )
	: Fl_Scroll( g.GetX(), g.GetY(), g.GetW(), g.GetH(), label ), mObjAtOnce( objAtOnce ), mObjCnt( 0 )
{
	type( Fl_Scroll::VERTICAL_ALWAYS );
	scrollbar.align( FL_ALIGN_RIGHT );
	color( FL_WHITE );
	position( 0, 0 );
	resize( g.GetX(), g.GetY(), g.GetW(), g.GetH() );
}

void ObjectScroller::resize( int x, int y, int w, int h )
{
	Fl_Scroll::resize( x, y, w, h );

}

void ObjectScroller::Init()
{
	color( FL_WHITE );
	align( FL_ALIGN_CENTER | FL_ALIGN_TOP );
}

int ObjectScroller::handle( int event )
{
	return Fl_Scroll::handle( event );
}

void ObjectScroller::draw()
{
	Fl_Scroll::draw();
}

void ObjectScroller::AddObject( ObjectIcon* obj )
{
	mObjCnt++;
	add( obj );
	unsigned int ico_cnt = 0;
	if ( mObjCnt > 0 )
	{
		int dH = (this->h()) / mObjAtOnce; 
		for ( int k = 0; k < children(); k++ )
		{
			try
			{

				ObjectIcon& placeholder = dynamic_cast< ObjectIcon& >( *child(k) );
				placeholder.resize( this->x(), this->y() + dH*ico_cnt, this->w()-scrollbar.w(), dH );
				ico_cnt++;

			}
			catch ( ... )
			{
				// Nothing to do, it is not such a catastrophe
			}
		} 
	}
}

void ObjectScroller::HandleAction( std::string& s )
{
	cout << "Callback called for object: " << s << endl;
}

void ObjectScroller::HandleLighting( std::string& s )
{
	for ( int k = 0; k < children(); k++ )
	{
		try
		{
			 
			ObjectIcon& placeholder = dynamic_cast< ObjectIcon& >( *child(k) );
			if ( placeholder.IsSelected() )
			{
				if ( placeholder.GetName() == s )
				{
					placeholder.Highlight();
				}
				else
					placeholder.Unlight();
			}

		}
		catch ( ... )
		{
			// Nothing to do, it is not such a catastrophe
		}

	}
}

void ObjectScroller::RemoveAllObjects()
{
	mObjCnt = 0;
	std::list< ObjectIcon* > icons;
	std::list< ObjectIcon* >::iterator list_it;
	ObjectIcon*  placeholder;

	for ( int k = 0; k < children(); k++ )
	{
		try
		{
			placeholder = dynamic_cast< ObjectIcon* >( child(k) );
			icons.push_back( placeholder );
		}
		catch ( ... )
		{
			// Nothing to do, it is not such a catastrophe
		}
	} 

	for ( list_it = icons.begin(); list_it!= icons.end(); list_it++ )
	{
		remove( (*list_it) );
	}
}

void ObjectScroller::RemoveObject( ObjectIcon* obj )
{
	mObjCnt--;
	remove( *obj );
	if ( mObjCnt > 0 )
	{
		int dH = (this->h()) / mObjAtOnce; 
		for ( int k = 0; k < children(); k++ )
		{
			try
			{
				ObjectIcon& placeholder = dynamic_cast< ObjectIcon& >( *child(k) );	  
				placeholder.resize( this->x(), dH*k, this->w()-scrollbar.w(), dH );
			}
			catch ( ... )
			{
				// Nothing to do, it is not such a catastrophe
			}
		} 
	}

}
