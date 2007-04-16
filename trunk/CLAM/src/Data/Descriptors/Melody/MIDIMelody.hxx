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

#ifndef _MIDIMelody_
#define _MIDIMelody_

#include "DynamicType.hxx"
#include "List.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"

#include "MIDINote.hxx"

namespace CLAM{

//////////////////////////////////////////////////////
// Class MIDIMelody:
//
/**class Melody, used to store the melodic description of a MIDI stream/file
@@see SegmentDescriptors
**/

 class MIDIMelody: public ProcessingData
 {
 public:
	DYNAMIC_TYPE_USING_INTERFACE (MIDIMelody, 2, ProcessingData);
	/** Array of notes belonging to the melody **/
	DYN_ATTRIBUTE (0, public, List<MIDINote>, NoteArray); 
	/** Higher-level descriptors computed from the noteArray information */
	DYN_ATTRIBUTE (1, public, TSize, NumberOfNotes); 
 protected:
	 void DefaultInit();	
 };
}; // namespace CLAM

#endif // _Melody _

