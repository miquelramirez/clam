#ifndef _EWindowType_
#define _EWindowType_

#include "Enum.hxx"

namespace MTG {

	class EWindowType : public MTG::Enum {
	public:
		static tEnumValue sEnumValues[];
		static tValue sDefault;
		EWindowType() : MTG::Enum(sEnumValues, sDefault) {}
		EWindowType(tValue v) : MTG::Enum(sEnumValues, v) {}
		EWindowType(std::string s) : MTG::Enum(sEnumValues, s) {}

		virtual MTG::Component * Species() const {return new EWindowType();}

		typedef enum {
			HAMMING,
			KAISERBESSEL17,
			KAISERBESSEL18,
			KAISERBESSEL19,
			KAISERBESSEL20,
			KAISERBESSEL25,
			KAISERBESSEL30,
			KAISERBESSEL35,
			BLACKMAN_HARRIS_62,
			BLACKMAN_HARRIS_70,
			BLACKMAN_HARRIS_74,
			BLACKMAN_HARRIS_92,
			TRIANGULAR,
			CONSTANT_1
		} tEnum;
	};
}; //end namspace MTG

#endif //_EWindowType_