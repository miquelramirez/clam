/****************************************************************************
** Form interface generated from reading ui file '../src/OpenProjectDialogBase.ui'
**
** Created: Mon Mar 14 17:56:13 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef OPENPROJECTDIALOGBASE_H
#define OPENPROJECTDIALOGBASE_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QListView;
class QListViewItem;

class OpenProjectDialogBase : public QDialog
{
    Q_OBJECT

public:
    OpenProjectDialogBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~OpenProjectDialogBase();

    QListView* mProjectsView;

public slots:
    virtual void itemClicked( QListViewItem * );

protected:
    QVBoxLayout* OpenProjectDialogBaseLayout;

protected slots:
    virtual void languageChange();

};

#endif // OPENPROJECTDIALOGBASE_H
