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

#ifndef __FL_PLOT__
#define __FL_PLOT__

#include <Fl/Fl_Group.H>
#include "Presentation.hxx"
#include "TooltipTracker2D.hxx"
#include "Slotv1.hxx"
#include "Slotv2.hxx"
#include "Array.hxx"
#include "BPF.hxx"
#include "BoundingBoxes.hxx"
#include <Fl/Fl_Box.H>

namespace CLAMVM
{
	class Fl_X_Axis;
	class Fl_Y_Axis;
	class Fl_ZoomSlider;
	class Fl_Gl_2DSurface;

	class Fl_Plot : public Presentation, public Fl_Group
	{
	public:
		Fl_Plot();
		virtual ~Fl_Plot();

		virtual int handle( int eventCode );
		virtual void resize( int X, int y, int W, int H );

		virtual void Show();
		virtual void Hide();
		
		SigSlot::Slotv2< double, double >          SetXRange;
		SigSlot::Slotv2< double, double >          SetYRange;
		void SetTipFormatString( const char* fmtString );

	protected:
		void CreateContent();
		void PositionWidgets( int X, int Y, int W, int H );

		virtual void OnNewXRange( double xmin, double xmax );
		virtual void OnNewYRange( double ymin, double ymax );
		void OnRefreshTooltip( int sx, int sy, char* txtBuff, int maxLen );

		virtual Fl_Gl_2DSurface* CreateDisplay( int X, int Y, int W, int H ) = 0;
		virtual void AddRenderersToDisplay( Fl_Gl_2DSurface* display ) = 0;

		Fl_Gl_2DSurface* GetDisplay() {
			return mDisplay;
		}
		
	private:
		Fl_X_Axis*               mXAxis;
		Fl_Y_Axis*               mYAxis;
		Fl_ZoomSlider*           mXSlider;
		Fl_ZoomSlider*           mYSlider;
		Fl_Gl_2DSurface*         mDisplay;
		TooltipTracker2D         mTooltipTracker;
		Fl_Box*                  mImposterBox;
		DataBoundBox             mWSC;
		std::string              mTipFormatString;
	};
}

#endif // Fl_Plot.hxx
