/****************************************************************************
** Form implementation generated from reading ui file '../src/ConfigurationDialogBase.ui'
**
** Created: Mon Mar 14 18:43:07 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "ConfigurationDialogBase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qheader.h>
#include <qlistview.h>
#include <qsplitter.h>
#include <qwidgetstack.h>
#include <qwidget.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "Table.hxx"
#include "../../src/ConfigurationDialogBase.ui.h"
/*
 *  Constructs a ConfigurationDialogBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ConfigurationDialogBase::ConfigurationDialogBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "ConfigurationDialogBase" );
    setModal( TRUE );
    ConfigurationDialogBaseLayout = new QVBoxLayout( this, 11, 6, "ConfigurationDialogBaseLayout"); 

    layout15 = new QHBoxLayout( 0, 0, 6, "layout15"); 

    mListView = new QListView( this, "mListView" );
    mListView->addColumn( QString::null );
    mListView->setFrameShape( QListView::StyledPanel );
    mListView->setFrameShadow( QListView::Sunken );
    layout15->addWidget( mListView );

    mSplitter = new QSplitter( this, "mSplitter" );
    mSplitter->setOrientation( QSplitter::Horizontal );
    layout15->addWidget( mSplitter );

    mWidgetStack = new QWidgetStack( this, "mWidgetStack" );

    WStackPage = new QWidget( mWidgetStack, "WStackPage" );
    WStackPageLayout = new QVBoxLayout( WStackPage, 11, 6, "WStackPageLayout"); 

    Table7 = new Table( WStackPage, "Table7" );
    Table7->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, Table7->sizePolicy().hasHeightForWidth() ) );
    WStackPageLayout->addWidget( Table7 );
    mWidgetStack->addWidget( WStackPage, 0 );

    WStackPage_2 = new QWidget( mWidgetStack, "WStackPage_2" );
    mWidgetStack->addWidget( WStackPage_2, 1 );
    layout15->addWidget( mWidgetStack );
    ConfigurationDialogBaseLayout->addLayout( layout15 );

    layout63 = new QHBoxLayout( 0, 0, 6, "layout63"); 
    QSpacerItem* spacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout63->addItem( spacer );

    pushButton5 = new QPushButton( this, "pushButton5" );
    layout63->addWidget( pushButton5 );
    QSpacerItem* spacer_2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout63->addItem( spacer_2 );

    pushButton6 = new QPushButton( this, "pushButton6" );
    layout63->addWidget( pushButton6 );
    QSpacerItem* spacer_3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout63->addItem( spacer_3 );
    ConfigurationDialogBaseLayout->addLayout( layout63 );
    languageChange();
    resize( QSize(776, 529).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButton5, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( pushButton6, SIGNAL( clicked() ), this, SLOT( close() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ConfigurationDialogBase::~ConfigurationDialogBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ConfigurationDialogBase::languageChange()
{
    setCaption( tr( "Configuration" ) );
    mListView->header()->setLabel( 0, QString::null );
    pushButton5->setText( tr( "&OK" ) );
    pushButton5->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    pushButton6->setText( tr( "&Cancel" ) );
    pushButton6->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

