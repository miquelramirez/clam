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

#ifndef __GLPORTSIGSLOT__
#define __GLPORTSIGSLOT__

#include "GeometryKit.hxx"
#include "GLPortNew.hxx"
#include "FL/Fl.H"
#include "DataTypes.hxx"

#include "Signalv1.hxx"
#include "Slotv1.hxx"

namespace CLAMGUI
{
	
	class GLPortSigSlot : public GLPort
	{
		
	public:
		GLPortSigSlot( const Rect<int>& geometry, const char* label = 0 );

		void draw_overlay(  );
		
 		void draw();
		
 		int handle( int event ); // Fl_Gl_Window required interface

		void setPainting( bool painting );

		void setPos( CLAM::TData pos );

		Slotv1<CLAM::TData>* getFrameSlot(  ) { return &mFrameSlot; }
		Slotv1<bool>* getPaintSlot(  ) { return &mPaintSlot; }
		Signalv1<double>* getSignal(  ) { return &mSignal; }
		
	protected:
		bool mPainting;
		double mPos;

		Signalv1<double> mSignal;
		Slotv1<CLAM::TData> mFrameSlot;
		Slotv1<bool> mPaintSlot;
	};
	
}

#endif // GLPortSigSlot.hxx


