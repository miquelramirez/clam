#ifndef _Melody_
#define _Melody_

#include "DynamicType.hxx"
#include "List.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"

#include "Note.hxx"

namespace CLAM{

//////////////////////////////////////////////////////
// Class Melody:
//
/**class Melody, used to store the melodic description of an AudioSegment
@@see SegmentDescriptors
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
	/* MelodicContour: Parsons Code or different interval quantization */
	DYN_ATTRIBUTE (3, public, List<TData>, Contour); 

 protected:
	 void DefaultInit();	
 };
}; // namespace CLAM

#endif // _Melody _
