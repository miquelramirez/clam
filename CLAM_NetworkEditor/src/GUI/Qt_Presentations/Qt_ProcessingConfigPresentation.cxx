/*
 * Copyright (c) 2001-2003 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "Qt_ProcessingConfigPresentation.hxx"
#include "ActionButton.hxx"
#include <qlabel.h>
#include <qgroupbox.h>  

#include "ProcessingConfig.hxx"

namespace NetworkGUI
{

Qt_ProcessingConfigPresentation::Qt_ProcessingConfigPresentation( QWidget *parent , const char *name )
	: QDialog(parent, name)
{
	if ( !name )
		setName( "ProcessingConfigDesigner" );
	
	mGroupBox = new QGroupBox( this, "mGroupBox" );
	mGroupBox->setGeometry( QRect( 10, 230, 190, 40 ) );
	
	mOkButton = new ActionButton( mGroupBox, "mOkButton" );
	mOkButton->setGeometry( QRect( 20, 10, 70, 20 ) );
	
	mCancelButton = new ActionButton( mGroupBox, "mCancelButton" );
	mCancelButton->setGeometry( QRect( 100, 10, 70, 20 ) );
	
	setCaption( tr( "Processing Config" ) );

	mGroupBox->setTitle( QString::null );
	mOkButton->setText( tr( "Ok" ) );
	mCancelButton->setText( tr( "Cancel" ) );

	resize( QSize(400, 275).expandedTo(minimumSizeHint()) );

	SlotOk.Wrap( this, &Qt_ProcessingConfigPresentation::Ok );
	SlotCancel.Wrap( this, &Qt_ProcessingConfigPresentation::Cancel );

	mOkButton->Pressed.Connect( SlotOk );
	mCancelButton->Pressed.Connect( SlotCancel );

}
	
void Qt_ProcessingConfigPresentation::Ok( bool )
{
	ConfigureProcessing();
	close();
}

void Qt_ProcessingConfigPresentation::Cancel( bool )
{
	close();
}

void Qt_ProcessingConfigPresentation::Show()
{
	show();
}

void Qt_ProcessingConfigPresentation::Hide()
{
	hide();
}


} // namespace NetworkGUI
