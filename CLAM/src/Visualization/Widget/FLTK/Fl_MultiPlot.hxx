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

#ifndef __FL_MULTIPLOT__
#define __FL_MULTIPLOT__

#include "Fl_Plot.hxx"
#include <list>

namespace CLAMVM
{
	class GLRenderingManager;

	class Fl_MultiPlot : public Fl_Plot
	{
	public:
		Fl_MultiPlot();
		virtual ~Fl_MultiPlot();

		void AddRenderer( GLRenderingManager* mgr );
		void RemoveRenderer( GLRenderingManager* mgr );
		void ClearRenderers( );

	protected:

		virtual Fl_Gl_2DSurface* CreateDisplay( int X, int Y, int W, int H );
		virtual void AddRenderersToDisplay( Fl_Gl_2DSurface* display );

	protected:
		typedef std::list<GLRenderingManager* > tContainer;
		tContainer  mRenderers;
		
	};

}

#endif // Fl_MultiPlot.hxx
