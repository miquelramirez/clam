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

#include "SpectralDelayPresentation.hxx"
#include "GridLayout.hxx"
using namespace CLAMGUI;

// Knob implementation

void DelayKnob::InitKnob()
{
	mIsConstraining = false;

	value ( 0 );
	maximum( mMaxDelay );
	minimum( 0.0 );

	labelsize( 10 );
	box( FL_OVAL_BOX );
	color( FL_GRAY );
	selection_color( 1 );
	step( 1.0 );
	type( Fl_Knob::DOTLIN );

	callback( FilterKnob::SignalSender );
}

void ProcObjPresentation< SpectralDelay >::PublishCallbacks()
{

	Aspect cfg;

	ProcObjView<SpectralDelay>::Services s;

	s = ProcObjView<SpectralDelay>::Services::SetDelay;
	cfg.AddRequest( s, &(mKnob->GetReceptionSignal() ) );

	s = ProcObjView<SpectralDelay>::Services::GetDelay;
	cfg.AddRequest( s, &mGetRealDelay );
	
	s = ProcObjView<SpectralDelay>::Services::GetMaxDelay;
	cfg.AddRequest( s, &mGetMaxDelay );
	
	mLinkedView->Configure( cfg );

}

void ProcObjPresentation< SpectralDelay >::Show()
{
	mWindow->show();
}

void ProcObjPresentation< SpectralDelay >::Init( const Geometry& g, const char* label )
{
	mWindow = new Fl_Window ( g.GetX(), g.GetY(), g.GetW(), g.GetH(), label );

	GridLayout gl ( 6, 6, Geometry( 0, 0, g.GetW(), g.GetH() ) );

	Geometry coords;

	coords = gl << Geometry( 1, 1, 4, 4 );

	mKnob = new DelayKnob( coords, "Delay" );

	mKnob->ConnectKnob( makeMemberFunctor0( *this, ProcObjPresentation<SpectralDelay>, NotifyView ) );

	mWindow->end();

	mGetMaxDelay = makeMemberFunctor1( unsigned, *this, ProcObjPresentation<SpectralDelay>, RenderMaxDelay );
	mGetRealDelay = makeMemberFunctor1( unsigned, *this, ProcObjPresentation<SpectralDelay>, RenderRealDelay );

}

void ProcObjPresentation< SpectralDelay >::RenderMaxDelay( unsigned max )
{
	mKnob->SetMaxDelay( max );
}

void ProcObjPresentation< SpectralDelay >::RenderRealDelay( unsigned real_delay )
{
	mKnob->value( real_delay );
}

void ProcObjPresentation< SpectralDelay >::NotifyView()
{
	mLinkedView->NotifyUpdate();
}
