#ifndef NEW_PROJECT_DIALOG_HXX
#define NEW_PROJECT_DIALOG_HXX

#include "NewProjectDialogBase.h"
#include "AnnotatorDataFacade.hxx"

class NewProjectDialog : public NewProjectDialogBase
{
	Q_OBJECT
	public:
		NewProjectDialog(AnnotatorDataFacade &, QWidget * parent = 0, const char * name = 0, WFlags f = 0);
		void removeAllEntries();
	public slots:
		void okClicked();
	private:
		bool isAFolder( QListViewItem *) const;
		void listOfFiles( AnnotatorDataFacade::StringList &) const;
		void getFilesInTheFolder( AnnotatorDataFacade::StringList & list, QListViewItem * folder) const;
		bool recursive() const;
		void addNewItems( AnnotatorDataFacade &);
	protected:
		virtual void doSomethingWithTheListOfFiles( AnnotatorDataFacade::StringList * ) const;
	private:
		AnnotatorDataFacade & mData;
};

#endif
