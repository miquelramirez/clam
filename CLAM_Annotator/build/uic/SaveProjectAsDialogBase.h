/****************************************************************************
** Form interface generated from reading ui file '../src/SaveProjectAsDialogBase.ui'
**
** Created: Mon Mar 14 17:56:14 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef SAVEPROJECTASDIALOGBASE_H
#define SAVEPROJECTASDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLineEdit;
class QPushButton;

class SaveProjectAsDialogBase : public QDialog
{
    Q_OBJECT

public:
    SaveProjectAsDialogBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SaveProjectAsDialogBase();

    QLineEdit* mNameToBeSaved;
    QPushButton* mOkButton;
    QPushButton* mCancelButton;

public slots:
    virtual void okClicked();

protected:
    QVBoxLayout* SaveProjectAsDialogBaseLayout;
    QHBoxLayout* layout1;

protected slots:
    virtual void languageChange();

};

#endif // SAVEPROJECTASDIALOGBASE_H
