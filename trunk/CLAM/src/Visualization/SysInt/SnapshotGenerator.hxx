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

#ifndef __SNAPSHOTGENERATOR__
#define __SNAPSHOTGENERATOR__

/*
  Such strange #ifdef (the next two ones) are used to avoid the instantiation of the templates,
  which is done by the gcc compiler in its 2.95 version, even if you don't use them.
 */

#include "Array.hxx"
#include "WidgetTKWrapper.hxx"
#include "ProcObjView.hxx"
#include "ProcDataView.hxx"
#include "ProcObjPresentation.hxx"
#include "ProcDataPresentation.hxx"
#include <FL/Fl.H>

namespace CLAMGUI
{


template < typename ProcObjType > 
void showPOSnapshot( ProcObjType* obj, const char* label = 0 )
{
	WidgetTKWrapper& toolkit = WidgetTKWrapper::GetWrapperFor( "FLTK" );

	ProcObjView< ProcObjType > view;
	ProcObjPresentation< ProcObjType > presentation( label );

	view.BindTo( obj );
	presentation.LinkWithView( &view );

	presentation.Show();
	view.Refresh();

	do
	{
		toolkit.Tick();
	} while ( ! toolkit.IsClosing() );

}



template < typename ProcDataType >
void showPDSnapshot( ProcDataType* obj, const char* label = 0 )
{
	WidgetTKWrapper& toolkit = WidgetTKWrapper::GetWrapperFor( "FLTK" );

	ProcDataView< ProcDataType > view;
	ProcDataPresentation< ProcDataType > presentation ( label );

	view.BindTo( obj );
	presentation.LinkWithView( &view );

	presentation.Show();
	view.Refresh();

	do
	{

		toolkit.Tick();
	} while ( !toolkit.IsClosing() );
	
}


extern  void showSnapshotArray( const CLAM::DataArray& array );
extern  void showSnapshotArray( CLAM::DataArray& array );

}
#endif // SnapshotGenerator.hxx
