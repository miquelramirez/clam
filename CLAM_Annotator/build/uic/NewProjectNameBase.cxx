/****************************************************************************
** Form implementation generated from reading ui file '../src/NewProjectNameBase.ui'
**
** Created: Mon Mar 14 17:56:13 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "NewProjectNameBase.h"

#include <qvariant.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a NewProjectNameBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
NewProjectNameBase::NewProjectNameBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "NewProjectNameBase" );
    setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)4, 0, 0, sizePolicy().hasHeightForWidth() ) );
    NewProjectNameBaseLayout = new QVBoxLayout( this, 11, 6, "NewProjectNameBaseLayout"); 

    mEdit = new QLineEdit( this, "mEdit" );
    mEdit->setFrameShape( QLineEdit::LineEditPanel );
    mEdit->setFrameShadow( QLineEdit::Sunken );
    NewProjectNameBaseLayout->addWidget( mEdit );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

    mOkButton = new QPushButton( this, "mOkButton" );
    layout1->addWidget( mOkButton );
    QSpacerItem* spacer = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer );

    mCancelButton = new QPushButton( this, "mCancelButton" );
    layout1->addWidget( mCancelButton );
    NewProjectNameBaseLayout->addLayout( layout1 );
    languageChange();
    resize( QSize(419, 76).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( mCancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( mOkButton, SIGNAL( clicked() ), this, SLOT( okClicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
NewProjectNameBase::~NewProjectNameBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void NewProjectNameBase::languageChange()
{
    setCaption( tr( "new project" ) );
    mEdit->setText( tr( "write the project name here" ) );
    mOkButton->setText( tr( "&Ok" ) );
    mOkButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    mCancelButton->setText( tr( "&Cancel" ) );
    mCancelButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

void NewProjectNameBase::okClicked()
{
    qWarning( "NewProjectNameBase::okClicked(): Not implemented yet" );
}

