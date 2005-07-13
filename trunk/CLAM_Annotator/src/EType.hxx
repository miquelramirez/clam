
#ifndef _EType_hxx__
#define _EType_hxx_

#include "Enum.hxx"

namespace CLAM_Annotator
{
	/**
	 * Specifies the kind of descriptors
	 */
	class EType: public CLAM::Enum
	{
	public:
		EType() : Enum(ValueTable(), eNone) {}
		EType(tValue v) : Enum(ValueTable(), v) {}
		EType(const std::string & s) : Enum(ValueTable(), s) {}
		virtual CLAM::Component* Species() const { return new EType; }

		typedef enum {
			eInt,
			eFloat,
			eString,
			eRestrictedString,
			eNone
		} tEnum;

		static tEnumValue * ValueTable()
		{
			static tEnumValue sValueTable[] = 
			{
				{eInt,"Int"},
				{eFloat,"Float"},
				{eString,"String"},
				{eRestrictedString,"RestrictedString"},
				{eNone,"None"},
				{0,NULL}
			};
			return sValueTable;
		}
	};
}

#endif//_EType_hxx_
