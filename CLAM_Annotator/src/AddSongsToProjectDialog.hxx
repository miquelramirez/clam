#ifndef ADD_SONGS_TO_PROJECT_HXX
#define ADD_SONGS_TO_PROJECT_HXX

#include "NewProjectDialog.hxx"

class AnnotatorDataFacade;

class AddSongsToProjectDialog : public NewProjectDialog
{
	Q_OBJECT
	public:
		AddSongsToProjectDialog(AnnotatorDataFacade &, QWidget * parent , const char * name = 0, WFlags f = 0);
	protected:
		void doSomethingWithTheListOfFiles( AnnotatorDataFacade::StringList *) const;
};

#endif
