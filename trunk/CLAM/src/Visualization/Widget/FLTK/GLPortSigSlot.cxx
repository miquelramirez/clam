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

#include "GLPortSigSlot.hxx"
#include <FL/fl_draw.H>
#include "CLAMGL.hxx"
#include <FL/gl.h>

using namespace CLAMGUI;

GLPortSigSlot::GLPortSigSlot( const Rect<int>& geometry, const char* label ) 
	: GLPort( geometry, label ) { 
	
	mPainting = false;
	mPos = -1;
	
	mFrameSlot.Wrap( this, &CLAMGUI::GLPortSigSlot::setPos );
	mPaintSlot.Wrap( this, &CLAMGUI::GLPortSigSlot::setPainting );
}

void GLPortSigSlot::setPainting( bool painting ) {
	mPainting = painting;
}

void GLPortSigSlot::setPos( CLAM::TData pos ) {
	mPos = pos;
	redraw_overlay();
}

void GLPortSigSlot::draw_overlay(  ) {
	if( mHorRange.mPosition < mPos && mPos < mHorRange.mPosition+mHorRange.mSize ) {
		int position = (int) ceil( ( ( mPos- mHorRange.mPosition ) * w() ) / mHorRange.mSize );

		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		ortho();
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity(  );
		
		//glClear(GL_COLOR_BUFFER_BIT );
		
		gl_color( FL_RED );
		glLineWidth(2);
		
		glBegin( GL_LINES );
		glVertex2i( position, y() );
		glVertex2i( position, y()+h() );
		glEnd();

		glMatrixMode( GL_PROJECTION );		
		glPopMatrix();		
		glMatrixMode( GL_MODELVIEW );
	}
}

void GLPortSigSlot::draw(  ) {
	GLPort::draw(  );
	redraw_overlay();
}

int GLPortSigSlot::handle( int event ) {
	if( mPainting ) {
		if ( event == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE ) {
			mPos = ( Fl::event_x(  ) - x(  ) ) * mHorRange.mSize / w(  ) + mHorRange.mPosition;

			// notify SLOTTED class to act
			mSignal.Emit( mPos );

			redraw();
			return 1;
		}
	}	
	return GLPort::handle( event );
}
