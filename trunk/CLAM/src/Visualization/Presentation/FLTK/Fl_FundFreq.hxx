/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef __FLFUNDFREQ__
#define __FLFUNDFREQ__

#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "Presentation.hxx"
#include "Array.hxx"
#include "TimeFreqMagPoint.hxx"
#include "FundFreqRM.hxx"
#include "TooltipTracker2D.hxx"
#include "Slotv1.hxx"
#include "Slotv2.hxx"
#include "BoundingBoxes.hxx"

namespace CLAMVM
{
	class Fl_X_Axis;
	class Fl_Y_Axis;
	class Fl_ZoomSlider;
	class Fl_Gl_Single_Display;

	class Fl_FundFreq : public Presentation, public Fl_Group
	{
	private:
		
		Fl_X_Axis*            mXAxis;
		Fl_Y_Axis*            mYAxis;
		Fl_ZoomSlider*        mXSlider;
		Fl_ZoomSlider*        mYSlider;
		Fl_Gl_Single_Display* mDisplay;
		FundFreqRM            mDrawMgr;
		Fl_Box*               mImposterBox;
		TooltipTracker2D      mTooltipTracker;
		DataBoundBox          mWorldSpaceCoords;

	protected:

		virtual void OnNewTrajectory( const CLAM::Array< TimeFreqMagPoint >& trajectory, CLAM::TData sRate );
		virtual void OnNewTimeSpan( CLAM::TTime begin, CLAM::TTime end );

		void OnRefreshTooltip( int sx, int sy, char* txtBuffer, int maxLen );

	public:
		Fl_FundFreq( int X, int Y, int W, int H, const char* label = 0 );
		virtual ~Fl_FundFreq();

		int handle( int event );

		virtual void Show();
		virtual void Hide();

		SigSlot::Slotv2< const CLAM::Array<TimeFreqMagPoint>&, CLAM::TData >  NewTrajectory;
		SigSlot::Slotv2< CLAM::TTime, CLAM::TTime >                           NewTimeSpan;

	};

}

#endif // Fl_FundFreq.hxx
