#ifndef _PitchNote_
#define _PitchNote_

#include "DynamicType.hxx"
#include "DataTypes.hxx"
#include "OSDefines.hxx"
#include "Err.hxx"
#include "ProcessingData.hxx"

#include "Enum.hxx"

namespace CLAM {

	// Class EPitch
	class EPitch : public Enum	{
	public:
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EPitch() : Enum(sEnumValues, sDefault) {}
		EPitch(tValue v) : Enum(sEnumValues, v) {};
		EPitch(std::string s) : Enum(sEnumValues, s) {};
		
		typedef enum {
			eC,eDb,eD,eEb,eE,eF,eGb,eG,eAb,eA,eBb,eB,eNone
		};	

		virtual Component* Species() const
		{
			return (Component*) new EPitch();
		};
	};
	

	///////////////////////////////////////////////////////
	// Class PitchNote:
	//
	
	
	/** Pitch note type definition = pitch + octave 
	 **/
	class PitchNote: public ProcessingData
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (PitchNote, 2, ProcessingData);
		DYN_ATTRIBUTE (0, public, EPitch, Pitch);   
		DYN_ATTRIBUTE (1, public, int,    Octave);
		
		void DefaultInit();
	};
	
}; // namespace CLAM

#endif // _PitchNote_


