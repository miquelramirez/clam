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

#include <FL/Fl.H>
#include "CLAMGL.hxx"
#include "Fl_SMS_Gl_Single_Browsable_Display.hxx"
#include "Assert.hxx"
#include "AudioRenderingManager.hxx"
#include <iostream>

namespace CLAMVM
{
	Fl_SMS_Gl_Single_Browsable_Display::Fl_SMS_Gl_Single_Browsable_Display( int X, int Y, int W, int H, const char* label)
		: Fl_Gl_Single_Display( X, Y, W, H, label )
	{
		mPainting = false;
		mPos = -1;
	
		SetSelectedXValue.Wrap( this, &Fl_SMS_Gl_Single_Browsable_Display::OnSetSelectedXValue );
	}

	Fl_SMS_Gl_Single_Browsable_Display::~Fl_SMS_Gl_Single_Browsable_Display()
	{
	}

	void Fl_SMS_Gl_Single_Browsable_Display::SetPainting(  ) {
		mPainting = true;
	}

	void Fl_SMS_Gl_Single_Browsable_Display::UnsetPainting(  ) {
		mPainting = true;
	}

	void Fl_SMS_Gl_Single_Browsable_Display::OnSetSelectedXValue( double pos ) {
		mPos = pos;
		redraw_overlay();
	}

	int Fl_SMS_Gl_Single_Browsable_Display::handle( int event ) {
		if( mPainting ) {
			if ( event == FL_PUSH && Fl::event_button() == FL_LEFT_MOUSE ) {

				DataBoundBox dataBBox;
				QueryDataBoundBox( dataBBox );
				
				mPos = ( Fl::event_x(  ) - x(  ) ) * (dataBBox.mRight-dataBBox.mLeft) / w(  ) + dataBBox.mLeft;
				
				//DrawContents();
				redraw_overlay();
				// notify SLOTTED class to act
				SelectedXValue.Emit( mPos );
				return 1;
			}
		}	
		return Fl_Gl_2DSurface::handle( event );
	}

	void Fl_SMS_Gl_Single_Browsable_Display::draw_overlay(  ) 
	{
		Fl_Gl_2DSurface::draw_overlay();

		DataBoundBox dataBBox;
		QueryDataBoundBox( dataBBox );

		if( dataBBox.mLeft < mPos && mPos < dataBBox.mRight ) 
	    {
			
			glColor3f( 1.0, 0.0, 0.0 );
			glEnable( GL_LINE_STIPPLE );
			glLineWidth(2);
			//glLineStipple( 1, 0xfc3f );
			//glLineStipple( 1, 0xe83e );
			glBegin( GL_LINES );
			glVertex2d( mPos, dataBBox.mTop );
			glVertex2d( mPos, dataBBox.mBottom );
			glEnd();
			glLineWidth( 1 );
			glDisable( GL_LINE_STIPPLE );
		}

	}

	void Fl_SMS_Gl_Single_Browsable_Display::DrawContents()
	{
		Fl_Gl_Single_Display::DrawContents();

		if( mPainting ) {
			redraw_overlay(  );
		}

	}
}






