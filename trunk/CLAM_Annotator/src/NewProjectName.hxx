#ifndef NEW_PROJECT_NAME_HXX
#define NEW_PROJECT_NAME_HXX

#include "AnnotatorDataFacade.hxx"

#include "NewProjectNameBase.h"

class NewProjectName : public NewProjectNameBase
{
	public:
		NewProjectName(AnnotatorDataFacade & data,AnnotatorDataFacade::StringList *, QWidget * parent = 0, const char * name = 0, WFlags f = 0);
		void okClicked();
	private:
		AnnotatorDataFacade::StringList * mFiles;
		AnnotatorDataFacade & mData;
};

#endif
