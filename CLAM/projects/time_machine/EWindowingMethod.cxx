#include "EWindowingMethod.hxx"

using namespace MTG;
//using namespace TM;

MTG::Enum::tEnumValue EWindowingMethod::sEnumValues[] = {
	{MTG::EWindowingMethod::ANALYSIS,"ANALYSIS"},
	{MTG::EWindowingMethod::SYNTHESIS,"SYNTHESIS"},
	{0,NULL}
};

MTG::Enum::tValue MTG::EWindowingMethod::sDefault = MTG::EWindowingMethod::ANALYSIS;