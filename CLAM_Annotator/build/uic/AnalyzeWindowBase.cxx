/****************************************************************************
** Form implementation generated from reading ui file '../src/AnalyzeWindowBase.ui'
**
** Created: Mon Mar 14 19:30:16 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "AnalyzeWindowBase.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../../src/AnalyzeWindowBase.ui.h"
/*
 *  Constructs a AnalyzeWindowBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AnalyzeWindowBase::AnalyzeWindowBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "AnalyzeWindowBase" );
    setModal( TRUE );
    AnalyzeWindowBaseLayout = new QVBoxLayout( this, 11, 6, "AnalyzeWindowBaseLayout"); 

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setFrameShape( QGroupBox::GroupBoxPanel );
    groupBox1->setFrameShadow( QGroupBox::Sunken );

    table1 = new QTable( groupBox1, "table1" );
    table1->setNumCols( table1->numCols() + 1 );
    table1->horizontalHeader()->setLabel( table1->numCols() - 1, tr( "Descriptors" ) );
    table1->setNumCols( table1->numCols() + 1 );
    table1->horizontalHeader()->setLabel( table1->numCols() - 1, tr( "Status" ) );
    table1->setNumCols( table1->numCols() + 1 );
    table1->horizontalHeader()->setLabel( table1->numCols() - 1, tr( "Analyze" ) );
    table1->setGeometry( QRect( 12, 12, 418, 328 ) );
    table1->setNumRows( 10 );
    table1->setNumCols( 3 );
    AnalyzeWindowBaseLayout->addWidget( groupBox1 );

    layout18 = new QHBoxLayout( 0, 0, 6, "layout18"); 
    QSpacerItem* spacer = new QSpacerItem( 72, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout18->addItem( spacer );

    layout17 = new QHBoxLayout( 0, 0, 6, "layout17"); 

    mAnalyzeButton = new QPushButton( this, "mAnalyzeButton" );
    layout17->addWidget( mAnalyzeButton );

    mAnalyzeAllButton = new QPushButton( this, "mAnalyzeAllButton" );
    layout17->addWidget( mAnalyzeAllButton );

    pushButton2 = new QPushButton( this, "pushButton2" );
    layout17->addWidget( pushButton2 );
    layout18->addLayout( layout17 );
    QSpacerItem* spacer_2 = new QSpacerItem( 72, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout18->addItem( spacer_2 );
    AnalyzeWindowBaseLayout->addLayout( layout18 );
    languageChange();
    resize( QSize(600, 506).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButton2, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( mAnalyzeButton, SIGNAL( clicked() ), this, SLOT( emit_analyze() ) );
    connect( mAnalyzeAllButton, SIGNAL( clicked() ), this, SLOT( emit_analyzeAll() ) );
    connect( mAnalyzeAllButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( mAnalyzeButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
AnalyzeWindowBase::~AnalyzeWindowBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AnalyzeWindowBase::languageChange()
{
    setCaption( tr( "Analyze" ) );
    groupBox1->setTitle( QString::null );
    table1->horizontalHeader()->setLabel( 0, tr( "Descriptors" ) );
    table1->horizontalHeader()->setLabel( 1, tr( "Status" ) );
    table1->horizontalHeader()->setLabel( 2, tr( "Analyze" ) );
    mAnalyzeButton->setText( tr( "&Analyze" ) );
    mAnalyzeButton->setAccel( QKeySequence( tr( "Alt+A" ) ) );
    mAnalyzeAllButton->setText( tr( "A&nalyze all" ) );
    mAnalyzeAllButton->setAccel( QKeySequence( tr( "Alt+N" ) ) );
    pushButton2->setText( tr( "&Cancel" ) );
    pushButton2->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

