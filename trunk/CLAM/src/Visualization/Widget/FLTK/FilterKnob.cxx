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

#include "FilterKnob.hxx"
#include "Assert.hxx"

using namespace CLAMGUI;

void FilterKnob::SetInitialValue( TData initial )
{
	value( initial );
}

void FilterKnob::SetValueRange( TData min, TData max )
{
	maximum(max);
	minimum(min);
}

void FilterKnob::SignalSender( Fl_Widget* knob )
{
	Fl_Knob*    superfknob = dynamic_cast< Fl_Knob* >( knob );
	FilterKnob* fknob = dynamic_cast< FilterKnob* >( knob );
	
	CLAM_ASSERT( fknob!=NULL , " Whoa! where is the virtual function table pointer?");
	
	if ( fknob->mIsConstraining) 
		(fknob->mConstraintSignal)( superfknob->value() );
	
	fknob->mInputSignal();

	superfknob->redraw();
}

