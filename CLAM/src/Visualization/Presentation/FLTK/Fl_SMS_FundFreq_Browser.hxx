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

#ifndef __FL_SMS_FUNDFREQ_BROWSER__
#define __FL_SMS_FUNDFREQ_BROWSER__

#include <string>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include "Presentation.hxx"
#include "Array.hxx"
#include "TimeFreqMagPoint.hxx"
#include "FundFreqRM.hxx"
#include "TooltipTracker2D.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"
#include "Slotv2.hxx"
#include "BoundingBoxes.hxx"
#include "DataTypes.hxx"

namespace CLAMVM
{

	class Fl_X_Axis;
	class Fl_Y_Axis;
	class Fl_ZoomSlider;
	class Fl_SMS_Gl_Single_Browsable_Display;

	class Fl_SMS_FundFreq_Browser : public Presentation, public Fl_Group
	{
	private:
		
		Fl_X_Axis*                          mXAxis;
		Fl_Y_Axis*                          mYAxis;
		Fl_ZoomSlider*                      mXSlider;
		Fl_ZoomSlider*                      mYSlider;
		Fl_SMS_Gl_Single_Browsable_Display* mDisplay;
		FundFreqRM                          mDrawMgr;
		Fl_Box*                             mImposterBox;
		TooltipTracker2D                    mTooltipTracker;
		DataBoundBox                        mWorldSpaceCoords;
		CLAM::TSize                         mFrames;
		CLAM::TTime                         mBeginTime;
		CLAM::TTime                         mEndTime;
		std::string                         mTooltipFmtStr;
		double                              mMinFreq;
		double                              mMaxFreq;

	protected:

		virtual void OnNewTrajectory( const CLAM::Array< TimeFreqMagPoint >& trajectory, CLAM::TData sRate );
		virtual void OnNewTimeSpan( CLAM::TTime begin, CLAM::TTime end );
		virtual void OnNewFreqRangeHint( CLAM::TData lower, CLAM::TData higher );

		void OnRefreshTooltip( int sx, int sy, char* txtBuffer, int maxLen );

		virtual void OnDisplaySelectedXValue( double value );
		virtual void OnSetSelectedXValue( double value );

		SigSlot::Signalv1< double > ChangeSelectedXValue;
		SigSlot::Slotv1< double >   HandleDisplaySelection;		

	public:
		Fl_SMS_FundFreq_Browser( int X, int Y, int W, int H, const char* label = 0 );
		virtual ~Fl_SMS_FundFreq_Browser();

		int handle( int event );

		virtual void Show();
		virtual void Hide();

		SigSlot::Slotv2< const CLAM::Array<TimeFreqMagPoint>&, CLAM::TData >  NewTrajectory;
		SigSlot::Slotv2< CLAM::TTime, CLAM::TTime >                           NewTimeSpan;
		SigSlot::Slotv2< CLAM::TData, CLAM::TData >                           NewFreqRangeHint;

		SigSlot::Slotv1<double>        SetSelectedXValue;
		SigSlot::Signalv1<double>      SelectedXValue;
		
		void SetTooltipFormat( const char* fmtStr );
		void SetYRange( double ymin, double ymax );

	};

	// inlines
	inline void Fl_SMS_FundFreq_Browser::SetTooltipFormat( const char* fmtStr )
	{
		mTooltipFmtStr = fmtStr;
	}

	inline void Fl_SMS_FundFreq_Browser::SetYRange( double ymin, double ymax )
	{
		mMinFreq = ymin;
		mMaxFreq = ymax;
	}
	
}

#endif // Fl_SMS_FundFreq_Browser.hxx
