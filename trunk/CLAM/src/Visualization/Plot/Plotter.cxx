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

#include "Plotter.hxx"
#include "ModelAdapter.hxx"
#include "Presentation.hxx"
#include "WidgetTKWrapper.hxx"
#include "Assert.hxx"

namespace CLAMVM
{
	Plotter::tPlotList Plotter::mPendingPlots;

	void Plotter::Hold( ModelAdapter* adap, Presentation* widget )
	{
		mPendingPlots.push_back( tPlotCmd( adap, widget) );
	}

	void Plotter::PlotAll( )
	{
		tPlotList::iterator i = mPendingPlots.begin();
		
		while ( i!= mPendingPlots.end() )
		{
			i->second->Show();
			i++;
		}

		// Run widget toolkit wrapper:
		WidgetTKWrapper& tk = WidgetTKWrapper::GetWrapperFor("FLTK");
		tk.Run();

	}

	void Plotter::Flush()
	{
		tPlotList::iterator i = mPendingPlots.begin();
		
		while ( i!= mPendingPlots.end() )
		{
			CLAM_ASSERT( i->first !=NULL, "Pointer was NULL!" );
			CLAM_ASSERT( i->second !=NULL, "Pointer was NULL!" );
			delete i->first;
			delete i->second;
			i++;
		}

		mPendingPlots.clear();
		
	}
}
