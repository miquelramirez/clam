#ifndef OPEN_PROJECT_DIALOG_HXX
#define OPEN_PROJECT_DIALOG_HXX

#include "OpenProjectDialogBase.h"

class AnnotatorDataFacade;
class Annotator;

class OpenProjectDialog : public OpenProjectDialogBase
{
	public:
		OpenProjectDialog(Annotator* ,AnnotatorDataFacade & , QWidget * parent = 0, const char * name = 0, WFlags f = 0);
	public slots:
			
		void itemClicked( QListViewItem * item);
	private:
		void insertItemsFromAnnotatorDataFacadeIntoListView();
		void removeCurrentListViewItems();
		AnnotatorDataFacade & mData;
                Annotator* mpAnnotator;
};

#endif
