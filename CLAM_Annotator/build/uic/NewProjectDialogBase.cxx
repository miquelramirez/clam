/****************************************************************************
** Form implementation generated from reading ui file '../src/NewProjectDialogBase.ui'
**
** Created: Mon Mar 14 17:56:13 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "NewProjectDialogBase.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qheader.h>
#include <qlistview.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../../src/NewProjectDialogBase.ui.h"
/*
 *  Constructs a NewProjectDialogBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
NewProjectDialogBase::NewProjectDialogBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "NewProjectDialogBase" );
    NewProjectDialogBaseLayout = new QVBoxLayout( this, 11, 6, "NewProjectDialogBaseLayout"); 

    textLabel1 = new QLabel( this, "textLabel1" );
    NewProjectDialogBaseLayout->addWidget( textLabel1 );

    mFoldersView = new QListView( this, "mFoldersView" );
    mFoldersView->addColumn( QString::null );
    mFoldersView->header()->setClickEnabled( FALSE, mFoldersView->header()->count() - 1 );
    mFoldersView->header()->setResizeEnabled( FALSE, mFoldersView->header()->count() - 1 );
    mFoldersView->setSelectionMode( QListView::Extended );
    NewProjectDialogBaseLayout->addWidget( mFoldersView );

    mRecursivelyCheckBox = new QCheckBox( this, "mRecursivelyCheckBox" );
    NewProjectDialogBaseLayout->addWidget( mRecursivelyCheckBox );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

    mOkButton = new QPushButton( this, "mOkButton" );
    layout1->addWidget( mOkButton );
    QSpacerItem* spacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer );

    mCancelButton = new QPushButton( this, "mCancelButton" );
    layout1->addWidget( mCancelButton );
    NewProjectDialogBaseLayout->addLayout( layout1 );
    languageChange();
    resize( QSize(424, 341).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( mOkButton, SIGNAL( clicked() ), this, SLOT( okClicked() ) );
    connect( mCancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
NewProjectDialogBase::~NewProjectDialogBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void NewProjectDialogBase::languageChange()
{
    setCaption( tr( "Select files" ) );
    textLabel1->setText( tr( "Please, select the files to add :" ) );
    mFoldersView->header()->setLabel( 0, QString::null );
    mFoldersView->clear();
    QListViewItem * item_2 = new QListViewItem( mFoldersView, 0 );
    item_2->setOpen( TRUE );
    QListViewItem * item_3 = new QListViewItem( item_2, 0 );
    item_3->setOpen( TRUE );
    QListViewItem * item = new QListViewItem( item_3, 0 );
    item->setText( 0, tr( "song 1" ) );
    item_3->setOpen( TRUE );
    item = new QListViewItem( item_3, item );
    item->setText( 0, tr( "song 2" ) );
    item_3->setText( 0, tr( "songs 1970-2004" ) );
    item_3->setPixmap( 0, QPixmap::fromMimeSource( "stock_open.png" ) );
    item_2->setText( 0, tr( "my music" ) );
    item_2->setPixmap( 0, QPixmap::fromMimeSource( "stock_open.png" ) );

    mRecursivelyCheckBox->setText( tr( "Select files into folders recursively" ) );
    mOkButton->setText( tr( "&Ok" ) );
    mOkButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    mCancelButton->setText( tr( "&Cancel" ) );
    mCancelButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

