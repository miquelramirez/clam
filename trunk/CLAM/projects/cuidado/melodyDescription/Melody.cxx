#include "Melody.hxx"

namespace CLAM{

	/* Melody Config */
	void MelodyConfig::DefaultInit()
	{
		UpdateData();
	}
	
	
	/* Melody */
	Melody::Melody(const MelodyConfig &newConfig) : ProcessingData(4)
	{
		MandatoryInit();
		Configure(newConfig);
	}
	
	const MelodyConfig& Melody::GetConfig() const {	return GetprConfig();}
	 
	
	void Melody::DefaultInit() { Configure(MelodyConfig()); }
	
	void Melody::Configure(const MelodyConfig &newConfig)
	{
		AddnoteArray();
		UpdateData();
		if (!HasprConfig()){
			AddprConfig();
			UpdateData();
		}
	SetprConfig(newConfig);
}

};
