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

#ifndef _Melody_
#define _Melody_

#include "DynamicType.hxx"
#include "List.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"

#include "Note.hxx"

namespace CLAM
{

//////////////////////////////////////////////////////
// Class Melody:
//
/**class Melody, used to store the melodic description of an AudioSegment
@@see SegmentDescriptors
@@see FFHDescriptors
@@see DistributionDescriptors
@@see Key
**/

class Melody: public ProcessingData
{
public:
	DYNAMIC_TYPE_USING_INTERFACE (Melody, 4, ProcessingData);
	/** Array of notes belonging to the melody **/
	DYN_ATTRIBUTE (0, public, List<Note>, NoteArray); 
	/** Higher-level descriptors computed from the noteArray information */
	DYN_ATTRIBUTE (1, public, TSize, NumberOfNotes); 
	/** Tessitura in Hz: difference between the minimum and maximum pitch **/
	DYN_ATTRIBUTE (2, public, TData, Tessitura); 
	/** MelodicContour: Parsons Code or different interval quantization */
	DYN_ATTRIBUTE (3, public, List<TData>, Contour); 

protected:
	void DefaultInit();	
};

} // namespace CLAM

#endif // _Melody _

