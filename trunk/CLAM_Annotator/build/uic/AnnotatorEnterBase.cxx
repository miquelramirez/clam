/****************************************************************************
** Form implementation generated from reading ui file '../src/AnnotatorEnterBase.ui'
**
** Created: Mon Mar 14 18:33:40 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "AnnotatorEnterBase.h"

#include <qvariant.h>
#include <qiconview.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../../src/AnnotatorEnterBase.ui.h"
/*
 *  Constructs a AnnotatorEnterBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AnnotatorEnterBase::AnnotatorEnterBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "AnnotatorEnterBase" );
    setModal( TRUE );
    AnnotatorEnterBaseLayout = new QVBoxLayout( this, 11, 6, "AnnotatorEnterBaseLayout"); 

    mChooseIconView = new QIconView( this, "mChooseIconView" );
    AnnotatorEnterBaseLayout->addWidget( mChooseIconView );
    languageChange();
    resize( QSize(287, 93).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( mChooseIconView, SIGNAL( clicked(QIconViewItem*) ), this, SLOT( clicked(QIconViewItem*) ) );
    connect( mChooseIconView, SIGNAL( onItem(QIconViewItem*) ), this, SLOT( handCursor() ) );
    connect( mChooseIconView, SIGNAL( onViewport() ), this, SLOT( arrowCursor() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
AnnotatorEnterBase::~AnnotatorEnterBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AnnotatorEnterBase::languageChange()
{
    setCaption( tr( "Choose an action" ) );
    mChooseIconView->clear();
    (void) new QIconViewItem( mChooseIconView, tr( "New project" ), QPixmap::fromMimeSource( "stock_open.png" ) );
    (void) new QIconViewItem( mChooseIconView, tr( "Open project" ), QPixmap::fromMimeSource( "stock_open.png" ) );
    (void) new QIconViewItem( mChooseIconView, tr( "Exit" ), QPixmap::fromMimeSource( "stock_exit.png" ) );
}

