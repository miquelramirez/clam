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

#ifndef __GENERICPLOT__
#define __GENERICPLOT__

#include "PlotBase.hxx"
#include <string>

namespace CLAMVM
{
	class Fl_Plot;
	
	class GenericPlot : public PlotBase
	{

	public:
		virtual ~GenericPlot();

		virtual void SetPosition( int x, int y );
		virtual void SetSize( int w, int h );
		virtual void SetLabel( const char* label );
		virtual void SetYRange( double ymin, double ymax );
		virtual void SetTooltipFormat( const char* tooltipFmt );
		
	protected:
		void RegisterAnonymously();
		void RegisterAs( std::string name );

		Fl_Plot* GetWidget();
		void     SetWidget( Fl_Plot* );

		virtual void CreateWidget();
		virtual  void DestroyWidget();
	private:
		Fl_Plot*    mWidget;
	};

	// Inline definitions
	
	inline Fl_Plot* GenericPlot::GetWidget() {
		return mWidget;
	}

	inline void GenericPlot::SetWidget( Fl_Plot* w) {
		mWidget = w;
	}

}

#endif // GenericPlot.hxx
