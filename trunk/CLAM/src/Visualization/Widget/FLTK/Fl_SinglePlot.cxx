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

#include "Fl_SinglePlot.hxx"
#include "GLRenderingManager.hxx"
#include "Fl_Gl_Single_Display.hxx"

namespace CLAMVM
{

	Fl_SinglePlot::Fl_SinglePlot( )
		: mRenderer( NULL )
	{
	}

	Fl_SinglePlot::~Fl_SinglePlot( )
	{
	}

	void Fl_SinglePlot::SetRenderingManager( GLRenderingManager* mgr )
	{
		if ( mRenderer == NULL )
			mRenderer = mgr;
		else
		{
			delete mRenderer;
			mRenderer = mgr;
		}
		AddRenderersToDisplay( GetDisplay() );
	}

	Fl_Gl_2DSurface* Fl_SinglePlot::CreateDisplay( int X, int Y, int W, int H )
	{
		return new Fl_Gl_Single_Display(  X, Y, W, H );

	}

	void Fl_SinglePlot::AddRenderersToDisplay( Fl_Gl_2DSurface* display )
	{
		if ( display == NULL )
			return;
		else
		{
			Fl_Gl_Single_Display* disp = static_cast<Fl_Gl_Single_Display*>(display);

			disp->SetRenderer( *mRenderer );
		}
	}
}
