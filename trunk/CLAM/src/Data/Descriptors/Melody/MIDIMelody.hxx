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
