#ifndef SAVE_PROJECT_AS_DIALOG_HXX
#define SAVE_PROJECT_AS_DIALOG_HXX

#include "SaveProjectAsDialogBase.h"

class SaveProjectAsDialog : public SaveProjectAsDialogBase
{
	Q_OBJECT
	public:
		SaveProjectAsDialog( QWidget * parent = 0, const char * name = 0, WFlags f = 0);
	public slots:
		void okClicked();
	signals:
		void nameToBeSaveEmited(const QString &);
};

#endif
