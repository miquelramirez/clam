#include "SaveProjectAsDialog.hxx"
#include <qlineedit.h>

SaveProjectAsDialog::SaveProjectAsDialog( QWidget * parent, const char * name, WFlags f) : SaveProjectAsDialogBase( parent, name, f)
{
}

void SaveProjectAsDialog::okClicked()
{
	emit nameToBeSaveEmited( mNameToBeSaved->text() );
}
