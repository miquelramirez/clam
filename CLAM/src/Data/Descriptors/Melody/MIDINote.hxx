/* Note type definition */

#ifndef _MIDINote_
#define _MIDINote_

#include "DynamicType.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"

#include "Enum.hxx"
#include "MediaTime.hxx"
#include "PitchNote.hxx"

namespace CLAM {

//////////////////////////////////////////////////////
// Class MIDINote:
//
/**class MIDINote used to store note information coming from a MIDI stream/file
**/

	class MIDINote: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (MIDINote, 3, ProcessingData);
		/* Pitch information (not by default) */
		DYN_ATTRIBUTE (0, public, int, Key);   
		/* Note Fundamental frequency (by default) */
		DYN_ATTRIBUTE (1, public, int, Velocity); 
		DYN_ATTRIBUTE (2, public, MediaTime, Time);
		
	private:
		void DefaultInit();	
	};

}; // namespace CLAM


#endif // _Note_
