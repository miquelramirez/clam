#ifndef _EPhaseGeneration_
#define _EPhaseGeneration_

#include "Enum.hxx"
#include "DataTypes.hxx"

namespace CLAM{

/** PhaseGeneration
*   Enumeration to use for specifying the type of phase generation used
*/
class EPhaseGeneration: public Enum
{
public:
	
	static tEnumValue sEnumValues[];
	static tValue sDefault;
	EPhaseGeneration() : Enum(sEnumValues, sDefault) {}
	EPhaseGeneration(tValue v) : Enum(sEnumValues, v) {};
	EPhaseGeneration(std::string s) : Enum(sEnumValues, s) {};

	typedef enum {
		eAlign,
		eRandom,
		eContinuation
	} tEnum;

	virtual Component* Species() const
	{
		return (Component*) new EPhaseGeneration;
	};
};

}

#endif
