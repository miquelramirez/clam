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

#ifndef __FDFILTERGENPRESENTATION__
#define __FDFILTERGENPRESENTATION__

#include "GView.hxx"
#include <FL/Fl.H>
#include "FilterPanel.hxx"
#include "CBL.hxx"
#include "ViewConfiguration.hxx"
#include "ProcObjPresentation.hxx"
#include "DataTypes.hxx"
#include "GeometryKit.hxx"
#include "FDFilterGen.hxx"

using CLAM::TData;



using CLAM::FDFilterGen;
using CLAM::EFDFilterType;

namespace CLAMGUI
{

template < >
class ProcObjPresentation< FDFilterGen >
	: public Presentation
{

public:
	
	ProcObjPresentation( const char* label = 0)
		: Presentation()
		{
			Geometry g( 100, 100, 400, 200 );
			Init( g, label );
		}
	ProcObjPresentation( const Geometry& g, const char* label )
		: Presentation()
		{
			Init( g, label );
		}
	~ProcObjPresentation()
		{
		}
	void PublishCallbacks();
	void Show();
	void UpdateFilterType( EFDFilterType type );
	
	
protected:
	
	void PublishLowPassCallbacks(Aspect& my_aspect);
	void PublishBandPassCallbacks(Aspect& my_aspect);
	void PublishHighPassCallbacks(Aspect& my_aspect);
	
	void Init( const Geometry& g, const char* label );
	void GetGain( TData gain );
	void GetHCutoff( TData hcutoff );
	void GetLCutoff( TData lcutoff );
	void GetPBSlope( TData pbslope );
	void GetSBSlope( TData sbslope );
	void NotifyView();
	
	// Knobs
	
	Fl_Knob*           mGainKnob;
	Fl_Knob*           mHCutoffKnob;
	Fl_Knob*           mLCutoffKnob;
	Fl_Knob*           mPBSlopeKnob;
	Fl_Knob*           mSBSlopeKnob;
	
	
	FilterPanel*                      mPanel;
	EFDFilterType                       mCurrentFilterType;
	
	// FLTK stuff
	Geometry                          mPresGeometry;
	
	// Callbacks
	CBL::Functor1< TData >            mDrawGain;
	CBL::Functor1< TData >            mDrawHCutoff;
	CBL::Functor1< TData >            mDrawLCutoff;
	CBL::Functor1< TData >            mDrawPBSlope;
	CBL::Functor1< TData >            mDrawSBSlope;
	CBL::Functor1< EFDFilterType >      mGetFiltertype;
};
	
}

#endif // FDFilterGenPresentation.hxx
