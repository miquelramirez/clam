/****************************************************************************
** Form implementation generated from reading ui file '../src/AboutBase.ui'
**
** Created: Mon Mar 14 18:33:39 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "AboutBase.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "../../src/AboutBase.ui.h"
/*
 *  Constructs a AboutBase as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
AboutBase::AboutBase( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "AboutBase" );
    setModal( TRUE );
    AboutBaseLayout = new QVBoxLayout( this, 11, 6, "AboutBaseLayout"); 

    pixmapLabel1 = new QLabel( this, "pixmapLabel1" );
    pixmapLabel1->setPixmap( QPixmap::fromMimeSource( "simac.png" ) );
    pixmapLabel1->setScaledContents( TRUE );
    AboutBaseLayout->addWidget( pixmapLabel1 );
    languageChange();
    resize( QSize(278, 241).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
AboutBase::~AboutBase()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AboutBase::languageChange()
{
    setCaption( tr( "About" ) );
}

