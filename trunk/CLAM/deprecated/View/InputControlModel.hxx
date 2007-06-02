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

#ifndef __INPUTCONTROLMODEL__
#define __INPUTCONTROLMODEL__

#include "Signalv1.hxx"
#include "Signalv2.hxx"
#include "Slotv1.hxx"
#include "OutControl.hxx"

namespace CLAMVM
{
	using SigSlot::Signalv1;
	using SigSlot::Signalv2;
	using SigSlot::Slotv1;

	class InputControlModel
	{
	protected:


	public:
		virtual void DispatchControlValue( CLAM::TControlData value ) = 0;

		InputControlModel( );

		virtual ~InputControlModel();

		// Signals
		Signalv1< CLAM::TControlData >                      ValuePublished;
		Signalv2< CLAM::TControlData, CLAM::TControlData >  ValueRangePublished;
				
		// Slots
		Slotv1< CLAM::TControlData>                         UpdateValue;

	};
}

#endif // InputControlModel.hxx

