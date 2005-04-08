
#ifndef _EType_
#define _EType_

#include "Enum.hxx"

namespace CLAM_Annotator{

/** Interpolation
   *   Enumeration to use for specifying the type of interpolation used
   */
  class EType: public CLAM::Enum
  {
  public:
	
	static tEnumValue sEnumValues[];
	static tValue sDefault;
	EType() : Enum(sEnumValues, sDefault) {}
	EType(tValue v) : Enum(sEnumValues, v) {}
	EType(std::string s) : Enum(sEnumValues, s) {}

	typedef enum {
		eInt,
		eFloat,
		eString,
		eRestrictedString,
		eNone
	} tEnum;

	virtual CLAM::Component* Species() const
	{
		return (CLAM::Component*) new EType;
	}
  };
};

#endif
