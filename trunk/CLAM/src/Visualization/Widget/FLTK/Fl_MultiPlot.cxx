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

#include "Fl_MultiPlot.hxx"
#include "Fl_Gl_Multi_Display.hxx"
#include "GLRenderingManager.hxx"
#include <algorithm>

namespace CLAMVM
{
	Fl_MultiPlot::Fl_MultiPlot()
	{
	}

	Fl_MultiPlot::~Fl_MultiPlot()
	{
	}

	void Fl_MultiPlot::AddRenderer( GLRenderingManager* mgr )
	{
		// first check it is not already in the renderers set

		tContainer::iterator i = std::find( mRenderers.begin(), mRenderers.end(), mgr );

		if ( i != mRenderers.end() ) // already in renderers set
			return;

		mRenderers.push_back( mgr );

		AddRenderersToDisplay( GetDisplay() );
	}

	void Fl_MultiPlot::RemoveRenderer( GLRenderingManager* mgr )
	{
		tContainer::iterator i = std::find( mRenderers.begin(), mRenderers.end(), mgr );

		if ( i == mRenderers.end() )
			return;

		mRenderers.erase( i );

		AddRenderersToDisplay( GetDisplay() );
	}

	void Fl_MultiPlot::ClearRenderers()
	{
		mRenderers.clear();
		AddRenderersToDisplay( GetDisplay() );
	}

	Fl_Gl_2DSurface* Fl_MultiPlot::CreateDisplay( int X, int Y, int W, int H )
	{
		return new Fl_Gl_Multi_Display( X, Y, W, H );
	}

	void Fl_MultiPlot::AddRenderersToDisplay( Fl_Gl_2DSurface* display )
	{
		if ( display == NULL )
			return;
		else
		{
			Fl_Gl_Multi_Display* pDisplay = static_cast< Fl_Gl_Multi_Display* >( display );
			
			pDisplay->ClearRenderers();
			
			for ( tContainer::iterator i = mRenderers.begin();
			      i != mRenderers.end();
			      i++ )
			{
				pDisplay->AddRenderer( **i );
			}
		}		
	}
}
