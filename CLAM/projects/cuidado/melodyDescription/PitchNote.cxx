#include "PitchNote.hxx"

namespace CLAM{

	/* Enum */
	Enum::tEnumValue EPitch::sEnumValues[] = {
		{EPitch::eNone,"NotSet"},
		{EPitch::eA,"A"},
		{EPitch::eB,"B"},
		{EPitch::eC,"C"},
		{EPitch::eD,"D"},
		{EPitch::eE,"E"},
		{EPitch::eF,"F"},
		{EPitch::eG,"G"},
		{0,NULL}
	};
	Enum::tValue EPitch::sDefault = EPitch::eNone;

	/* PitchNote Config */
	void PitchNoteConfig::DefaultInit()
	{
		UpdateData();
	}
	
	/* PitchNote */
	PitchNote::PitchNote(const PitchNoteConfig &newConfig) : ProcessingData(3)
	{
		MandatoryInit();
		Configure(newConfig);
	}
	
	const PitchNoteConfig& PitchNote::GetConfig() const {	return GetprConfig();}
	
	void PitchNote::DefaultInit() 
	{  
		Configure(PitchNoteConfig());
	}
	
	void PitchNote::Configure(const PitchNoteConfig &newConfig)
	{
		Addpitch();
		Addoctave();
		UpdateData();

		if (!HasprConfig()){
			AddprConfig();
			UpdateData();
		}
		SetprConfig(newConfig);
	}

}; // namespace MTG
