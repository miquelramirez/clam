
#include "Qt_OutControlSenderPresentation.hxx"
#include "Qt_OutControlPresentation.hxx"

#include <cmath>
#include <qslider.h>
#include <qdial.h>
#include <qspinbox.h>
#include <qpainter.h>

namespace NetworkGUI
{

Qt_OutControlSenderPresentation::Qt_OutControlSenderPresentation()
	: mControlRepresentation(0)
{
}

void Qt_OutControlSenderPresentation::UpdateSize( bool hasToResize)
{	
	setMinimumSize( 50, 50 );
	if(hasToResize)
		resize( 50, 50 );

}

void Qt_OutControlSenderPresentation::paintEvent( QPaintEvent * )
{
	QPainter p( this );
	if(mControlRepresentation)
		mControlRepresentation->setPalette( QPalette( GetColorOfState()));
	p.setBrush( GetColorOfState() );

	QRegion reg(12,7,width()-24, height()-14);
		
	ConnectionPointPresentationsList::iterator it;
	for (it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end();it++)
	{
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*it);
		reg += out->GetRegion();
	}

	setMask(reg);
	p.setPen( QPen( blue, 1 ));
	p.drawRect( 12,7, width()-24,height()-14); // draw a rectangle

	p.setPen( QPen( black,1 ));
	
	if(mSelected)
		DrawSelectedRepresentation();
}

void Qt_OutControlSenderPresentation::ConfigurationUpdated( bool ok )
{
	if(mControlRepresentation)
	{
		delete mControlRepresentation;
		mControlRepresentation = 0;
	}

	CLAM::OutControlSenderConfig * config = dynamic_cast<CLAM::OutControlSenderConfig *>(mConfig->GetConfig());
	CLAM_ASSERT( config, " Qt_OutControlSenderPresentation::ConfigurationUpdated(), config has the wrong concrete type" );

	mMin = config->GetMin();
	mDefault = config->GetDefault();
	mMax = config->GetMax();
	mStep = config->GetStep();

	if(config->GetControlRepresentation() == CLAM::OutControlSenderConfig::EControlRepresentation::eUndetermined )
		return;

	CreateControlRepresentationWidget( config->GetControlRepresentation() );
	AdjustControlRepresentationValues();
	SignalSendOutControlValue.Emit( "out", mDefault );
	UpdateOutControlsPosition();
	EmitPositionOfChildren();
	repaint();
}

void Qt_OutControlSenderPresentation::AdjustControlRepresentationValues()
{
	QRangeControl * controlRepresentation = dynamic_cast<QRangeControl*>( mControlRepresentation );
	controlRepresentation->setRange( (int)(round(mMin/mStep)),
			   (int)(round(mMax/mStep)) );
	
	controlRepresentation->setValue( (int)(mDefault/mStep) );
}

void Qt_OutControlSenderPresentation::CreateControlRepresentationWidget( 
		const CLAM::OutControlSenderConfig::EControlRepresentation & representation )
{
	switch( representation )
	{
		case CLAM::OutControlSenderConfig::EControlRepresentation::eVerticalSlider:
			mControlRepresentation = new QSlider( Vertical, this );
			mControlRepresentation->setMinimumSize( 20, 50 );
			setMinimumSize( 56, 76 );
			resize( 56, 76 );
			break;
		case CLAM::OutControlSenderConfig::EControlRepresentation::eHorizontalSlider:
			mControlRepresentation = new QSlider( Horizontal, this );
			mControlRepresentation->setMinimumSize( 50, 20 );
			setMinimumSize( 86, 46 );
			resize( 86, 46 );
			break;
		case CLAM::OutControlSenderConfig::EControlRepresentation::eKnot:
			mControlRepresentation = new QDial( this );
			mControlRepresentation->setMinimumSize( 30, 30 );
			setMinimumSize( 66, 56 );
			resize( 66, 56 );
			break;
		case CLAM::OutControlSenderConfig::EControlRepresentation::eSpinBox:
			mControlRepresentation = new QSpinBox( this );
			mControlRepresentation->setMinimumSize( 45, 25 );
			setMinimumSize( 81, 51 );
			resize( 81, 51 );
			break;
		default:
			CLAM_ASSERT( false, "Qt_OutControlSenderPresentation::ConfigurationUpdated(), unknown type of widget" );
	}	
	
	connect( mControlRepresentation, SIGNAL( valueChanged( int ) ), 
		 this, SLOT( SlotValueChanged( int ) ) );

	mControlRepresentation->move( 18, 14 );
	mControlRepresentation->show();
	mControlRepresentation->resize( geometry().width() - 36, geometry().height() - 28 );
	UpdateOutControlsPosition();
	EmitPositionOfChildren();
	repaint();
}

void Qt_OutControlSenderPresentation::SlotValueChanged( int value )
{
	SignalSendOutControlValue.Emit( "out", (CLAM::TControlData)( value*mStep ) );
}

void Qt_OutControlSenderPresentation::ExecuteResize( const QPoint & difference )
{	
	QRect newGeometry = geometry();
	switch( mResizePosition )
	{
		case UpLeft:
			newGeometry.setTopLeft( difference + pos() );
			break;
		case Up:
			newGeometry.setTop( difference.y() + pos().y() );
			break;
		case UpRight:
			newGeometry.setTopRight( difference + pos() );	
			newGeometry.setWidth( difference.x() + width() );
			break;
		case DownLeft:
			newGeometry.setBottomLeft( difference + pos() );
			newGeometry.setHeight( difference.y() + height() );
			break;
		case Down:
			newGeometry.setHeight( difference.y() + height() );
			break;
		case DownRight:
			newGeometry.setWidth( difference.x() + width() );
			newGeometry.setHeight( difference.y() + height() );
			break;
	}
	setGeometry( newGeometry );
	if(mControlRepresentation)
		mControlRepresentation->resize( geometry().width() - 36, geometry().height() - 28 );
	UpdateOutControlsPosition();
	EmitPositionOfChildren();
	repaint();
}

} // namespace NetworkGUI

