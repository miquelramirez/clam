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

#include "Fl_Gl_Multi_Display.hxx"
#include "GLRenderingManager.hxx"
#include "Assert.hxx"

namespace CLAMVM
{

	Fl_Gl_Multi_Display::Fl_Gl_Multi_Display( int X, int Y, int W, int H, const char* label )
		: Fl_Gl_2DSurface( X, Y, W, H, label )
	{
	}

	Fl_Gl_Multi_Display::~Fl_Gl_Multi_Display()
	{
	}

	void Fl_Gl_Multi_Display::AddRenderer( GLRenderingManager& mgr )
	{
		mRenderers.push_back( &mgr );
	}				

	void Fl_Gl_Multi_Display::ClearRenderers( )
	{
		mRenderers.clear();
	}

	void Fl_Gl_Multi_Display::DrawContents()
	{
		CLAM_DEBUG_ASSERT( mRenderers.size() > 0, "No renderers associated!" );

		tContainer::iterator i, end = mRenderers.end();

		if ( MustReproject() )
		{
			WCSProject();
			DataBoundBox dataBBox;
			QueryDataBoundBox( dataBBox );
			ScreenBoundBox scrBBox = { 0,0, w(), h() };

			for ( i = mRenderers.begin(); i != end; i++ )
			{
				(*i)->SetWorldBounds( dataBBox );
				(*i)->SetPixelBounds( scrBBox );
			}
		}

		for ( i = mRenderers.begin(); i != end; i++ )
		{
			(*i)->RenderData();
		}
	}
}
