#include "Note.hxx"


namespace CLAM{

	/* Note Config */
	void NoteConfig::DefaultInit()
	{
		AddhasPitch();
		AddtimeType();

		UpdateData();
		SethasPitch(1); // By default is defined
		SettimeType(EMediaTimeType::eBeginEnd); // default
	}
	
	/* Note */
	Note::Note(const NoteConfig &newConfig) : ProcessingData(7)
	{
		MandatoryInit();
		Configure(newConfig);
	}
	
	const NoteConfig& Note::GetConfig() const {	return GetprConfig();}
	 	
	void Note::DefaultInit() 
	{ 
		Configure(NoteConfig()); 
	}
	
	void Note::Configure(const NoteConfig &newConfig)
	{
		AddfundFreq();
		Addtime();
		Addattack();
		Addsustain();
		Addrelease();
		UpdateData();		
		
		// configure time type
		MediaTimeConfig mtcnf;
		mtcnf.Settype(newConfig.GettimeType());
		Gettime().Configure(mtcnf);
		Getattack().Configure(mtcnf);
		Getsustain().Configure(mtcnf);
		Getrelease().Configure(mtcnf);

		if(newConfig.GethasPitch()==1) {
			 AddpitchNote();
			 UpdateData();
		}

		if (!HasprConfig()){
			AddprConfig();
			UpdateData();
		}
		SetprConfig(newConfig);
}

}; // namespace MTG
