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

#ifndef __SPECTRALDELAYPRESENTATION__
#define __SPECTRALDELAYPRESENTATION__

#include "GView.hxx"
#include "FilterKnob.hxx"
#include "CBL.hxx"
#include "ProcObjPresentation.hxx"
#include "DataTypes.hxx"
#include "GeometryKit.hxx"
#include "SpectralDelayView.hxx"
#include <FL/Fl_Double_Window.H>

using CLAM::TData;

namespace CLAMGUI
{

class DelayKnob
	: public FilterKnob
{

public:

	DelayKnob( const Geometry& g, const char* label = "Delay" )
		: FilterKnob( g, label )
	{
		InitKnob();
	}

	inline void SetMaxDelay( unsigned max_delay )
	{
		mMaxDelay = max_delay;
		InitKnob();
		redraw();
	}

	void InitKnob();

private:

	unsigned        mMaxDelay;

};

template<>
class ProcObjPresentation< SpectralDelay >
	: public Presentation
{
public:

	ProcObjPresentation( const char* label = 0 )
		: Presentation()
	{
		Geometry g( 100, 100, 80, 80 );
		Init( g, label );
	}

	ProcObjPresentation( const Geometry& g, const char* label )
		: Presentation()
	{
		Init( g, label );
	}

	~ProcObjPresentation()
	{
		delete mWindow;
	}

	void PublishCallbacks();

	void Show();

protected:

	void Init( const Geometry& g, const char* label );

	void RenderMaxDelay( unsigned max );

	void RenderRealDelay( unsigned real_delay );

	void NotifyView();

private:

	DelayKnob*   mKnob;
	Fl_Window*   mWindow;

	unsigned     mMaximumDelay;

	CBL::Functor1< unsigned > mGetMaxDelay;
	CBL::Functor1< unsigned > mGetRealDelay;

};

}

#endif // SpectralDelayPresentation.hxx
