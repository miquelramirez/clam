#include "MediaTime.hxx"

namespace CLAM {

	/* Enum */
	Enum::tEnumValue EMediaTimeType::sEnumValues[] = {
		{EMediaTimeType::eBeginEnd,"BeginEnd"},
		{EMediaTimeType::eBeginDuration,"BeginDuration"},
		{0,NULL}
	};

	Enum::tValue EMediaTimeType::sDefault = EMediaTimeType::eBeginEnd;

	
	/* MediaTime Config */
	void MediaTimeConfig::DefaultInit()
	{
		Addtype();
		UpdateData();
		Settype(EMediaTimeType::eBeginEnd); // default
	}
	
	/* MediaTime */
	MediaTime::MediaTime(const MediaTimeConfig &newConfig) : ProcessingData(4)
	{
		MandatoryInit();
		Configure(newConfig);
	}
	
	const MediaTimeConfig& MediaTime::GetConfig() const {	return GetprConfig();}
	 	
	void MediaTime::DefaultInit() 
	{ Configure(MediaTimeConfig()); }
	
	void MediaTime::Configure(const MediaTimeConfig &newConfig)
	{
		Addbegin();
		UpdateData();

		if(newConfig.Gettype()==EMediaTimeType::eBeginEnd) {
			 Addend(); UpdateData();		}

		if(newConfig.Gettype()==EMediaTimeType::eBeginDuration) {
			 Addduration();	 UpdateData();	}

		if (!HasprConfig()){
			AddprConfig();
			UpdateData();
		}
		SetprConfig(newConfig);
}



}; // namespace MTG
