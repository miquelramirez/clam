/****************************************************************************
** Form implementation generated from reading ui file '../src/SaveProjectAsDialogBase.ui'
**
** Created: Mon Mar 14 17:56:14 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "SaveProjectAsDialogBase.h"

#include <qvariant.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

/*
 *  Constructs a SaveProjectAsDialogBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
SaveProjectAsDialogBase::SaveProjectAsDialogBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "SaveProjectAsDialogBase" );
    SaveProjectAsDialogBaseLayout = new QVBoxLayout( this, 11, 6, "SaveProjectAsDialogBaseLayout"); 

    mNameToBeSaved = new QLineEdit( this, "mNameToBeSaved" );
    mNameToBeSaved->setFrameShape( QLineEdit::LineEditPanel );
    mNameToBeSaved->setFrameShadow( QLineEdit::Sunken );
    SaveProjectAsDialogBaseLayout->addWidget( mNameToBeSaved );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 

    mOkButton = new QPushButton( this, "mOkButton" );
    layout1->addWidget( mOkButton );

    mCancelButton = new QPushButton( this, "mCancelButton" );
    layout1->addWidget( mCancelButton );
    SaveProjectAsDialogBaseLayout->addLayout( layout1 );
    languageChange();
    resize( QSize(267, 87).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( mOkButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( mCancelButton, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( mOkButton, SIGNAL( clicked() ), this, SLOT( okClicked() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
SaveProjectAsDialogBase::~SaveProjectAsDialogBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SaveProjectAsDialogBase::languageChange()
{
    setCaption( tr( "Save project as" ) );
    mNameToBeSaved->setText( tr( "write the name for the project here" ) );
    mOkButton->setText( tr( "&Ok" ) );
    mOkButton->setAccel( QKeySequence( tr( "Alt+O" ) ) );
    mCancelButton->setText( tr( "&Cancel" ) );
    mCancelButton->setAccel( QKeySequence( tr( "Alt+C" ) ) );
}

void SaveProjectAsDialogBase::okClicked()
{
    qWarning( "SaveProjectAsDialogBase::okClicked(): Not implemented yet" );
}

