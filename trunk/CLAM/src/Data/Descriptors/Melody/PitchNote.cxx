#include "PitchNote.hxx"

using namespace CLAM;

/* Enum */
Enum::tEnumValue EPitch::sEnumValues[] = {	
	{EPitch::eC, "C"},
	{EPitch::eDb,"Db"},
	{EPitch::eD, "D"},
	{EPitch::eEb,"Eb"},
	{EPitch::eE, "E"},
	{EPitch::eF, "F"},
	{EPitch::eGb,"Gb"},
	{EPitch::eG, "G"},
	{EPitch::eAb,"Ab"},
	{EPitch::eA, "A"},
	{EPitch::eBb,"Bb"},
	{EPitch::eB, "B"},
	{EPitch::eNone,"NotSet"},
	{0,NULL}
};
Enum::tValue EPitch::sDefault = EPitch::eNone;

void PitchNote::DefaultInit() 
{  
	AddPitch();
	AddOctave();
	UpdateData();
	
	/* Default values */
	SetPitch(EPitch::eA);
	SetOctave(0);
}


