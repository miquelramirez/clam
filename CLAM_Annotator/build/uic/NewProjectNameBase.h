/****************************************************************************
** Form interface generated from reading ui file '../src/NewProjectNameBase.ui'
**
** Created: Mon Mar 14 17:56:13 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef NEWPROJECTNAMEBASE_H
#define NEWPROJECTNAMEBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLineEdit;
class QPushButton;

class NewProjectNameBase : public QDialog
{
    Q_OBJECT

public:
    NewProjectNameBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~NewProjectNameBase();

    QLineEdit* mEdit;
    QPushButton* mOkButton;
    QPushButton* mCancelButton;

public slots:
    virtual void okClicked();

protected:
    QVBoxLayout* NewProjectNameBaseLayout;
    QHBoxLayout* layout1;

protected slots:
    virtual void languageChange();

};

#endif // NEWPROJECTNAMEBASE_H
