
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

	resize( QSize(209, 275).expandedTo(minimumSizeHint()) );

	mOkButtonIsPressed.Wrap( this, &Qt_ProcessingConfigPresentation::Ok );
	mCancelButtonIsPressed.Wrap( this, &Qt_ProcessingConfigPresentation::Cancel );

	mOkButton->Pressed.Connect( mOkButtonIsPressed );
	mCancelButton->Pressed.Connect( mCancelButtonIsPressed );

}
	
Qt_ProcessingConfigPresentation::~Qt_ProcessingConfigPresentation()
{
}

void Qt_ProcessingConfigPresentation::Ok( bool )
{
	ApplyChangesToConfig();
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
