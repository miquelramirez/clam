/****************************************************************************
** Form interface generated from reading ui file '../src/ConfigurationDialogBase.ui'
**
** Created: Mon Mar 14 18:43:07 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CONFIGURATIONDIALOGBASE_H
#define CONFIGURATIONDIALOGBASE_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>
#include "TableItem.hxx"
#include "ListViewItem.hxx"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class Table;
class QListView;
class QListViewItem;
class QSplitter;
class QWidgetStack;
class QWidget;
class QPushButton;

class ConfigurationDialogBase : public QDialog
{
    Q_OBJECT

public:
    ConfigurationDialogBase( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConfigurationDialogBase();

    QListView* mListView;
    QSplitter* mSplitter;
    QWidgetStack* mWidgetStack;
    QWidget* WStackPage;
    Table* Table7;
    QWidget* WStackPage_2;
    QPushButton* pushButton5;
    QPushButton* pushButton6;

protected:
    QVBoxLayout* ConfigurationDialogBaseLayout;
    QHBoxLayout* layout15;
    QVBoxLayout* WStackPageLayout;
    QHBoxLayout* layout63;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

    void init();

};

#endif // CONFIGURATIONDIALOGBASE_H
