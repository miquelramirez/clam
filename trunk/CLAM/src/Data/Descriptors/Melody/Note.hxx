/* Note type definition */

#ifndef _Note_
#define _Note_

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
// Class Note:
//
/**class Note used to store note information. Pitch and Intranote information is not instantiated by default, 
	 and time information.
**/

	class Note: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (Note, 7, ProcessingData);
		/* Pitch information (not by default) */
		DYN_ATTRIBUTE (0, public, PitchNote, PitchNote);   
		/* Note Fundamental frequency (by default) */
		DYN_ATTRIBUTE (1, public, TData, FundFreq); 
		/* Note energy (by default) */
		DYN_ATTRIBUTE (2, public, TData, Energy);
		DYN_ATTRIBUTE (3, public, MediaTime, Time);
		/* Intranote duration (not by default) */
		DYN_ATTRIBUTE (4, public, MediaTime, Attack);
		DYN_ATTRIBUTE (5, public, MediaTime, Sustain);
		DYN_ATTRIBUTE (6, public, MediaTime, Release);

		/** Helper accessor for getting the corresponding MIDI note number
		*/
		int GetNoteNumber()
		{
			return 69+Round(log(GetFundFreq()/440)*17.31234);
		}
		int GetVelocity()
		{
		//this mapping is experimental and should be revisited
			TData velocity=65*log10(GetEnergy())+65;
			if(velocity<25) velocity=25;
			return (int)velocity;//65;
		}
	private:
		void DefaultInit();	
	};

}; // namespace CLAM


#endif // _Note_
