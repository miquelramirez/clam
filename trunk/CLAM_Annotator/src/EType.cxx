#include "EType.hxx"

namespace CLAM_Annotator{

//Initializer for Type enum
CLAM::Enum::tEnumValue EType::sEnumValues[] = {
	{EType::eInt,"Int"},
	{EType::eFloat,"Float"},
	{EType::eString,"String"},
	{EType::eRestrictedString,"RestrictedString"},
	{EType::eNone,"None"},
	{0,NULL}
};

CLAM::Enum::tValue EType::sDefault = EType::eNone;

};
