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

#include "GLPortNew.hxx"
#include "CLAMGL.hxx"
#include <iostream>

using std::cerr;
using std::endl;

using namespace CLAMGUI;

void GLPort::DrawSelf()
{
	if ( mIsConf )
	{
		make_current();
//		if ( !valid() )
	//	{
			ApplyProjection(); // Window has been resized or something so projection must change
	//	}
		
		glPushAttrib( GL_ALL_ATTRIB_BITS );
		
		mRenderingState->Apply(); // We apply our selected rendering environment onto the OpenGL stack
			
		mDrawCb();  // We make here the call to the rendering code
			
		glPopAttrib();
		
	}
	else
	{
		cerr << "Nothing drawn because GLPort wasn't properly configured" << endl;
	}

}

void GLPort::draw()
{

	if ( mInMultiDisplay )
	{
		mInMultiDisplay->draw();
	}
	else
	{
		DrawSelf();
	}
}

int GLPort::handle( int event )
{
	return Fl_Gl_Window::handle( event );
}

void GLPort::ApplyProjection()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0, 0, w(), h() );
	// Orthographic projection - we are engineers after all and scale should be preserved
	//glOrtho ( mOrthoVol.GetX(), mOrthoVol.GetW(), mOrthoVol.GetH(), mOrthoVol.GetY(), -1.0f, 1.0f );
	// Let's try with the rulers...
	glOrtho( mHorRange.mPosition, mHorRange.mPosition + mHorRange.mSize, mVerRange.mPosition - mVerRange.mSize, mVerRange.mPosition, -1.0f, 1.0f );
}

