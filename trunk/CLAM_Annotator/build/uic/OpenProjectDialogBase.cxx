/****************************************************************************
** Form implementation generated from reading ui file '../src/OpenProjectDialogBase.ui'
**
** Created: Mon Mar 14 17:56:13 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "OpenProjectDialogBase.h"

#include <qvariant.h>
#include <qheader.h>
#include <qlistview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a OpenProjectDialogBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
OpenProjectDialogBase::OpenProjectDialogBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "OpenProjectDialogBase" );
    setCursor( QCursor( 13 ) );
    OpenProjectDialogBaseLayout = new QVBoxLayout( this, 11, 6, "OpenProjectDialogBaseLayout"); 

    mProjectsView = new QListView( this, "mProjectsView" );
    mProjectsView->addColumn( QString::null );
    OpenProjectDialogBaseLayout->addWidget( mProjectsView );
    languageChange();
    resize( QSize(195, 210).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( mProjectsView, SIGNAL( clicked(QListViewItem*) ), this, SLOT( itemClicked(QListViewItem*) ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
OpenProjectDialogBase::~OpenProjectDialogBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void OpenProjectDialogBase::languageChange()
{
    setCaption( tr( "Open project" ) );
    mProjectsView->header()->setLabel( 0, QString::null );
    mProjectsView->clear();
    QListViewItem * item = new QListViewItem( mProjectsView, 0 );
    item->setText( 0, tr( "Project 1" ) );

    item = new QListViewItem( mProjectsView, item );
    item->setText( 0, tr( "Project 2" ) );

}

void OpenProjectDialogBase::itemClicked(QListViewItem*)
{
    qWarning( "OpenProjectDialogBase::itemClicked(QListViewItem*): Not implemented yet" );
}

