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
	EPhaseGeneration() : Enum(ValueTable(), eAlign) {}
	EPhaseGeneration(tValue v) : Enum(ValueTable(), v) {};
	EPhaseGeneration(std::string s) : Enum(ValueTable(), s) {};

	typedef enum {
		eAlign,
		eRandom,
		eContinuation
	} tEnum;

	virtual Component* Species() const
	{
		return (Component*) new EPhaseGeneration;
	};
	static tEnumValue * ValueTable()
	{
		static tEnumValue sEnumValues[] = {
			{eAlign,"Align"},
			{eRandom,"Random"},
			{eContinuation,"Continuation"},
			{0,NULL}
		};
		return sEnumValues;
	}
};

}

#endif
