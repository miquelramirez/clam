#ifndef _EWindowingMethod_
#define _EWindowingMethod_

#include "Enum.hxx"

namespace MTG {

	class EWindowingMethod : public MTG::Enum {
	public:
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EWindowingMethod() : MTG::Enum(sEnumValues, sDefault) {}
		EWindowingMethod(tValue v) : MTG::Enum(sEnumValues, v) {}
		EWindowingMethod(std::string s) : MTG::Enum(sEnumValues, s) {}

		virtual MTG::Component * Species() const {return new EWindowingMethod();}

		typedef enum {
			ANALYSIS,
			SYNTHESIS,
		} tEnum;
	};
}; //end namspace MTG

#endif //_EWindowingMethod_