/****************************************************************************
** Form interface generated from reading ui file '../src/NewProjectDialogBase.ui'
**
** Created: Mon Mar 14 17:56:13 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef NEWPROJECTDIALOGBASE_H
#define NEWPROJECTDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QListView;
class QListViewItem;
class QCheckBox;
class QPushButton;

class NewProjectDialogBase : public QDialog
{
    Q_OBJECT

public:
    NewProjectDialogBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~NewProjectDialogBase();

    QLabel* textLabel1;
    QListView* mFoldersView;
    QCheckBox* mRecursivelyCheckBox;
    QPushButton* mOkButton;
    QPushButton* mCancelButton;

public slots:
    virtual void okClicked();

protected:
    QVBoxLayout* NewProjectDialogBaseLayout;
    QHBoxLayout* layout1;

protected slots:
    virtual void languageChange();

};

#endif // NEWPROJECTDIALOGBASE_H
