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

#include "ObjectBrowser.hxx"
using namespace CLAMGUI;

ObjectBrowser::ObjectBrowser( int objatonce, const Rect<int>& g, const char* label )
	: Fl_Group( g.GetX(), g.GetY(), g.GetW(), g.GetH() , label ), mObjAtOnce( objatonce ), mObjCnt( 0 )
{
	box( FL_UP_BOX );
	end();
	mScroll = new Fl_Scrollbar( 0, 0, 10, 10 );
	mCanvas = new Fl_Box( 0, 0, 10, 10 );
	mCanvas->box( FL_THIN_DOWN_BOX );
	mCanvas->color ( FL_WHITE );
	Init();
	resize( g.GetX(), g.GetY(), g.GetW(), g.GetH() );
}

void ObjectBrowser::resize( int x, int y, int w, int h )
{
	Fl_Group::resize( x, y, w, h );
	mGenLayout = GridLayout( 10, 15, Rect<int>( x, y, w, h ) );

	Rect<int> scrollcoords = mGenLayout << Rect<int>( 8,0,2,15);
	mScroll->resize( scrollcoords.GetX(), scrollcoords.GetY(), scrollcoords.GetW(), scrollcoords.GetH() );

	Rect<int> canvascoords = mGenLayout << Rect<int>( 0, 0, 8, 15 );
	mCanvas->resize( canvascoords.GetX(), canvascoords.GetY(), canvascoords.GetW(), canvascoords.GetH() );

	int first_widget = int( mScroll->value() );
	int last_widget = int( mScroll->maximum()*mScroll->slider_size() );


	Rect<int> boxescoords = mGenLayout << Rect<int>( 0,0, 8, 15 );
	GridLayout boxesLayout = GridLayout( 1, mObjAtOnce, boxescoords );
	Rect<int> boxcoord;

	if ( mObjCnt > 0 )
	{
		for ( int k= first_widget; k <= last_widget; k++ )
		{
			boxcoord = boxesLayout << Rect<int>( 0, k, 1, 1 );
			child(k)->resize( boxcoord.GetX(), boxcoord.GetY(), boxcoord.GetW(), boxcoord.GetH() );
		}
	}

}

void ObjectBrowser::Init()
{

	color( FL_WHITE );
	align( FL_ALIGN_CENTER | FL_ALIGN_TOP );
	mScroll->range( 0.0, double( mObjCnt ) );
	mScroll->value( 0, mObjAtOnce, mObjCnt, mObjCnt );
	mScroll->slider_size( 1.0 );
	mGenLayout = GridLayout( 10, 15, Rect<int>( x(), y(), w(), h() ) );

}

int ObjectBrowser::handle( int event )
{
	return Fl_Group::handle( event );
}

void ObjectBrowser::draw()
{
	int first_widget = int( mScroll->value() );
	int last_widget = int( mScroll->maximum()*mScroll->slider_size() );
	 
	if ( mObjCnt > 0 )
	{
		for ( int k= first_widget; k <= last_widget; k++ )
		{
			child( k )->draw(); 
		}
	}
}

void ObjectBrowser::AddObject( ObjectIcon* obj )
{
	mObjCnt++;
	add( obj );
	mScroll->slider_size( ( mObjCnt==0 ) ? 1.0  : mObjAtOnce / double( mObjCnt ) );
	mScroll->maximum( double( mObjCnt ) );
}

void ObjectBrowser::RemoveObject( ObjectIcon* obj )
{
	mObjCnt--;
	remove( *obj );
	mScroll->slider_size( ( mObjCnt==0 ) ? 1.0  : mObjAtOnce / double( mObjCnt ) );
	mScroll->maximum( double( mObjCnt ) );
}
