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

#include "OscillatorPresentation.hxx"
#include "GeometryKit.hxx"
#include "GridLayout.hxx"
#include "OscillatorGView.hxx"
#include "DataTypes.hxx"
#include <iostream>
using std::cout;
using std::endl;
using CLAM::TData;
using namespace CLAMGUI;


void ProcObjPresentation<Oscillator>::PublishCallbacks()
{
	Aspect my_aspect;
	ProcObjView<Oscillator>::Services s;

	// s is by default AtomView<Oscillator>::Services::GetPitch
	my_aspect.AddRequest( s, &mDrawFreq );

	s = ProcObjView<Oscillator>::Services::GetAmp;
	my_aspect.AddRequest( s, &mDrawAmp );

	s = ProcObjView<Oscillator>::Services::GetModIdx;
	my_aspect.AddRequest( s, &mDrawModIndex );

	s = ProcObjView<Oscillator>::Services::GetPhase ;
	my_aspect.AddRequest( s, &mDrawPhase );

	s = ProcObjView<Oscillator>::Services::SetPitch;
	my_aspect.AddRequest( s, &mSendFreq );

	s = ProcObjView<Oscillator>::Services::SetAmp;
	my_aspect.AddRequest( s, &mSendAmp );

	s = ProcObjView<Oscillator>::Services::SetModIdx;
	my_aspect.AddRequest( s, &mSendModIndex );

	s = ProcObjView<Oscillator>::Services::SetPhase;
	my_aspect.AddRequest( s, &mSendPhase );

	mLinkedView->Configure( my_aspect );

}

void ProcObjPresentation<Oscillator>::Show()
{
	mWindow->show();
}

void ProcObjPresentation<Oscillator>::Init(const Geometry& g, const char* label)
{
	mWindow = new Fl_Window( g.GetX(), g.GetY(), g.GetW(), g.GetH(), label );
	
	GridLayout gl( 13, 4, Rect<int>(0,0,g.GetW(), g.GetH()) );
	Rect<int> coords;
	coords = gl << Rect<int>( 1, 1, 2, 2 );  // Freq knob positioning and resizing
	mFreqKnob = new Fl_Knob( coords.GetX(), coords.GetY(), coords.GetW(), coords.GetH(), "Pitch" );
	
	mFreqKnob->value( 440.0f );
	mFreqKnob->maximum( 440.0f );
	mFreqKnob->minimum( 0.0f );
	mFreqKnob->labelsize(10);
	mFreqKnob->box(FL_OVAL_BOX);
	mFreqKnob->color(FL_GRAY);
	mFreqKnob->selection_color(1);
	//mFreqKnob->step(1.0);
	mFreqKnob->type(Fl_Knob::DOTLIN);
	mFreqKnob->callback( ProcObjPresentation<Oscillator>::callback, this );
	coords = gl << Rect<int>( 4, 1, 2, 2 ); // Amplitude knob positioning and resizing
	
	mAmpKnob = new Fl_Knob( coords.GetX(), coords.GetY(), coords.GetW(), coords.GetH(), "Amp" );
	mAmpKnob->value( 0.5f );
	mAmpKnob->maximum( 1.0f );
	mAmpKnob->minimum( 0.0f );
	mAmpKnob->labelsize(10);
	mAmpKnob->box(FL_OVAL_BOX);
	mAmpKnob->color(FL_GRAY);
	mAmpKnob->selection_color(1);
	mAmpKnob->step(0.1);
	mAmpKnob->type(Fl_Knob::DOTLIN);
	mAmpKnob->callback( ProcObjPresentation<Oscillator>::callback, this );
	
	
	coords = gl << Rect<int>( 7, 1, 2, 2 ); // Modulation INdex knob positioning and resizing
	
	mModIdxKnob = new Fl_Knob( coords.GetX(), coords.GetY(), coords.GetW(), coords.GetH(), "Index" );
	mModIdxKnob->value( 0.0f );
	mModIdxKnob->maximum( 100.0f );
	mModIdxKnob->minimum( 0.0f );
	mModIdxKnob->labelsize(10);
	mModIdxKnob->box(FL_OVAL_BOX);
	mModIdxKnob->color(FL_GRAY);
	mModIdxKnob->selection_color(1);
	mModIdxKnob->step(1.0);
	mModIdxKnob->type(Fl_Knob::DOTLIN);
	mModIdxKnob->callback( ProcObjPresentation<Oscillator>::callback, this );
	mPhaseKnob = new Fl_Knob( coords.GetX(), coords.GetY(), coords.GetW(), coords.GetH(), "Phase" );
	mPhaseKnob->value( 0.0f );
	mPhaseKnob->maximum( 3.14159f );
	mPhaseKnob->minimum( -3.14159f );
	mPhaseKnob->labelsize(10);
	mPhaseKnob->box(FL_OVAL_BOX);
	mPhaseKnob->color(FL_GRAY);
	mPhaseKnob->selection_color(1);
	mPhaseKnob->step(0.1);
	mPhaseKnob->type(Fl_Knob::DOTLIN);
	mPhaseKnob->callback( ProcObjPresentation<Oscillator>::callback, this );
	
	mWindow->resizable( mFreqKnob );
	mWindow->resizable( mAmpKnob );
	mWindow->resizable( mModIdxKnob );
	mWindow->resizable( mPhaseKnob );
	
	mWindow->end();
	
	mDrawFreq = makeMemberFunctor1( TData, *this, ProcObjPresentation<Oscillator>, GetFreq );
	mDrawAmp = makeMemberFunctor1( TData, *this, ProcObjPresentation<Oscillator>, GetAmp );
	mDrawModIndex = makeMemberFunctor1( TData, *this, ProcObjPresentation<Oscillator>, GetModIndex );
	mDrawPhase = makeMemberFunctor1( TData, *this, ProcObjPresentation<Oscillator>, GetPhase );
	mSendFreq = makeMemberFunctor0R( TData, *this, ProcObjPresentation<Oscillator>, SendFreq );
	mSendAmp = makeMemberFunctor0R( TData, *this, ProcObjPresentation<Oscillator>, SendAmp );
	mSendModIndex = makeMemberFunctor0R( TData, *this, ProcObjPresentation<Oscillator>, SendModIndex );
	mSendPhase = makeMemberFunctor0R( TData, *this, ProcObjPresentation<Oscillator>, SendPhase );
	
}

void ProcObjPresentation<Oscillator>::GetFreq( TData freq )
{
	mFreqKnob->value( freq );
	mFreqKnob->redraw();
}


void ProcObjPresentation<Oscillator>::GetAmp( TData mag )
{
	mAmpKnob->value( mag );
	mAmpKnob->redraw();
}

void ProcObjPresentation<Oscillator>::GetModIndex( TData modidx )
{
	mModIdxKnob->value( modidx );
	mModIdxKnob->redraw();
}

void ProcObjPresentation<Oscillator>::GetPhase( TData phase )
{
	mPhaseKnob->value( phase );
	mPhaseKnob->redraw();
}

TData ProcObjPresentation<Oscillator>::SendFreq()
{
	return TData(mFreqKnob->value());
}

TData ProcObjPresentation<Oscillator>::SendAmp()
{
	return TData(mAmpKnob->value());
}

TData ProcObjPresentation<Oscillator>::SendModIndex()
{
	return TData(mModIdxKnob->value());
}

TData ProcObjPresentation<Oscillator>::SendPhase()
{
	return TData(mPhaseKnob->value());
}

void ProcObjPresentation<Oscillator>::callback(Fl_Widget* source, void* module)
{
	ProcObjPresentation<  Oscillator >* pModule = (ProcObjPresentation<Oscillator>*) module;

	pModule->mLinkedView->NotifyUpdate();

}

