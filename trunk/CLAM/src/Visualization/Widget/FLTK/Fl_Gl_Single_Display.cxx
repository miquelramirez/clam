/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "Fl_Gl_Single_Display.hxx"
#include "Assert.hxx"
#include "CLAMGL.hxx"

namespace CLAMVM
{
	Fl_Gl_Single_Display::Fl_Gl_Single_Display( int X, int Y, int W, int H, const char* label)
		: Fl_Gl_2DSurface( X, Y, W, H, label ), mRenderer( NULL )
	{
	}

	Fl_Gl_Single_Display::~Fl_Gl_Single_Display()
	{
	}

	void Fl_Gl_Single_Display::DrawContents()
	{
		CLAM_DEBUG_ASSERT( mRenderer!=NULL, "No renderer was assigned to this display!" );

		if ( MustReproject() )
		{
			WCSProject();
			DataBoundBox dataBBox;
			QueryDataBoundBox( dataBBox );
			ScreenBoundBox scrBBox = { x(), y(), w(), h() };
						
			mRenderer->SetWorldBounds( dataBBox );
			mRenderer->SetPixelBounds( scrBBox );
		}

		glClear( GL_COLOR_BUFFER_BIT );

		mRenderer->RenderData();

	}
}
