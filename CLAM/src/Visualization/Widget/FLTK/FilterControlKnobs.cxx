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

#include "FilterControlKnobs.hxx"
using namespace CLAMGUI;

void GainKnob::InitKnob()
{
	mIsConstraining = false;
	
	value( 3.0f );
	maximum( 3.0f );
	minimum( 0.0f );

	labelsize(10);
	box(FL_OVAL_BOX);
	color(FL_GRAY);
	selection_color(1);
	type(Fl_Knob::DOTLIN);
	callback( FilterKnob::SignalSender );
}

void SlopeKnob::InitKnob()
{
	mIsConstraining = false;
	
	value( 3.0f );
	maximum( 10.0f );
	minimum( 3.0f );
	
	labelsize(10);
	box(FL_OVAL_BOX);
	color(FL_GRAY);
	selection_color(1);
	type(Fl_Knob::DOTLIN);
	callback( FilterKnob::SignalSender );
}

void LowcutoffKnob::InitKnob()
{
	mIsConstraining = false;
	
	value( mSampleRate );
	maximum( mSampleRate );
	minimum( 0.0 );
	
	labelsize(10);
	box(FL_OVAL_BOX);
	color(FL_GRAY);
	selection_color(1);
	type(Fl_Knob::DOTLIN);
	callback( FilterKnob::SignalSender );
}

void LowcutoffKnob::SatisfyConstriction( TData new_value )
{
	maximum( new_value );
	Do();
}

void LowcutoffKnob::ConstrainKnob( Fl_Knob* k )
{
	HighcutoffKnob* knob = dynamic_cast< HighcutoffKnob* >( k );
	
	mConstraintSignal = makeMemberFunctor1( TData, *knob, HighcutoffKnob, SatisfyConstriction );

	mIsConstraining = true;
}

void HighcutoffKnob::InitKnob()
{
	mIsConstraining = false;
	
	value( mSampleRate );
	maximum( mSampleRate );
	minimum( 0.0 );
	
	labelsize(10);
	box(FL_OVAL_BOX);
	color(FL_GRAY);
	selection_color(1);
	type(Fl_Knob::DOTLIN);
	callback( FilterKnob::SignalSender );
}

void HighcutoffKnob::SatisfyConstriction( TData new_value )
{
	minimum( new_value );
	Do();
}

void HighcutoffKnob::ConstrainKnob( Fl_Knob* k )
{
	LowcutoffKnob* knob = dynamic_cast< LowcutoffKnob* >( k );
	
	mConstraintSignal = makeMemberFunctor1( TData, *knob, LowcutoffKnob, SatisfyConstriction );

	mIsConstraining = true;
}
