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

#include "InControl.hxx"
#include "OutControl.hxx"
#include "Processing.hxx"

//TODO: review this include!!!!
#include "MIDIMessage.hxx"

namespace CLAM 
{

template<>
const std::string InControl<MIDI::Message>::GetLastValueAsString() // TODO: Use plugins as soon we start to use non streamable types
{
// 	TODO: move MIDI::Message as streameable type?. Warning: TypeInfo<ControlData>::StorableAsLeaf affects DynamicTypes Store/Load
	StaticTrue staticTrue;
	return GetLastValueAsString(&staticTrue);
}

}; // namespace CLAM

