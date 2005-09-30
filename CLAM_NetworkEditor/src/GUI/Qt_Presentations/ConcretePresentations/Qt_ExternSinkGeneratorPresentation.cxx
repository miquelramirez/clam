#ifndef __Qt_ExternSinkGeneratorPresentation_hxx__
#define __Qt_ExternSinkGeneratorPresentation_hxx__

#include "Qt_ProcessingPresentation.hxx"
#include "Qt_InPortPresentation.hxx"
#include "Qt_OutPortPresentation.hxx"
#include "Qt_ProcessingPresentation.hxx"
#include <qtooltip.h>
#include <qpainter.h>
#include <qcursor.h>
#include <qlineedit.h>

class QWidget;

namespace NetworkGUI
{

class Qt_ExternSinkPresentation :  public Qt_ProcessingPresentation
{
protected:
	QColor GetColorOfState()
	{
		QColor c(0, 0, 0);
		switch( mProcessingState )
		{
			case CLAM::Processing::Ready:
				c.setRgb( 90, 180, 210 );
				QToolTip::add( this, "Processing is ready to run" );
				break;
			case CLAM::Processing::Unconfigured:
				c.setRgb( 210, 150, 150 );
				QToolTip::add( this, QString( mProcessingStatus.c_str() ));
				break;	
			case CLAM::Processing::Running:
				c.setRgb( 120, 120, 210 );
				QToolTip::add( this, "Processing running" );
				break;
			default:
				c.setRgb( 90, 90, 90 );
				QToolTip::add( this, "Unmanaged Processing State" );
				break;
		}
		return c;
	}

	virtual void Qt_ExternSinkPresentation::paintEvent( QPaintEvent * )
	{
		QPainter p( this );
		p.setBrush( GetColorOfState() );
	
		QRegion reg(12,7,width()-24, height()-14);
		ConnectionPointPresentationsList::iterator it;
		for (it=mInPortPresentations.begin(); it!=mInPortPresentations.end();it++)
		{
			Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*it);
			reg += in->geometry();
		}
	
		setMask(reg);
	
		p.setPen( QPen( blue, 1 ));
		p.drawRect( 12,7, width()-24,height()-14); // draw a rectangle
		p.drawLine( QPoint( 12, height()/2), QPoint( width()-12, height()/2 )); 
		
		p.setPen( QPen( black,1 ));
		
		if(mSelected)
			DrawSelectedRepresentation();
		p.setFont( QFont( "Helvetica" ,8) );
		p.drawText(  QRect(12,7,width()-24, height()/2 - 5 ),
			    Qt::AlignCenter ,	
			    QString( mName.c_str() ));
					
		p.drawText(  QRect(12,height()/2,width()-24, height()/2 - 5 ),
			    Qt::AlignCenter ,	
			    QString( mObservedClassName.c_str() ));
	}
		
public:
	Qt_ExternSinkPresentation(){}
};

class Qt_ExternGeneratorPresentation :  public Qt_ProcessingPresentation
{
protected:
	QColor GetColorOfState()
	{
		QColor c(0, 0, 0);
		switch( mProcessingState )
		{
			case CLAM::Processing::Ready:
				c.setRgb( 90, 180, 210 );
				QToolTip::add( this, "Processing is ready to run" );
				break;
			case CLAM::Processing::Unconfigured:
				c.setRgb( 210, 150, 150 );
				QToolTip::add( this, QString( mProcessingStatus.c_str() ));
				break;	
			case CLAM::Processing::Running:
				c.setRgb( 120, 120, 210 );
				QToolTip::add( this, "Processing running" );
				break;
			default:
				c.setRgb( 90, 90, 90 );
				QToolTip::add( this, "Unmanaged Processing State" );
				break;
		}
		return c;
	}
	virtual void Qt_ExternGeneratorPresentation::paintEvent( QPaintEvent * )
	{
		QPainter p( this );
		p.setBrush( GetColorOfState() );
	
	
		QRegion reg(12,7,width()-24, height()-14);
		ConnectionPointPresentationsList::iterator it;
		for (it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end();it++)
		{
			Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*it);
			reg += out->geometry();
		}
	
		setMask(reg);
	
		p.setPen( QPen( blue, 1 ));
		p.drawRect( 12,7, width()-24,height()-14); // draw a rectangle
		p.drawLine( QPoint( 12, height()/2), QPoint( width()-12, height()/2 )); 
		
		p.setPen( QPen( black,1 ));
		
		if(mSelected)
			DrawSelectedRepresentation();
		p.setFont( QFont( "Helvetica" ,8) );
		p.drawText(  QRect(12,7,width()-24, height()/2 - 5 ),
			    Qt::AlignCenter ,	
			    QString( mName.c_str() ));
					
		p.drawText(  QRect(12,height()/2,width()-24, height()/2 - 5 ),
			    Qt::AlignCenter ,	
			    QString( mObservedClassName.c_str() ));
	}

public:
	Qt_ExternGeneratorPresentation(){}
};

}
#include "Factory.hxx"
typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ExternSinkPresentation > 
	regtExternSink( "ExternSink" );

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ExternGeneratorPresentation > 
	regtExternGenerator( "ExternGenerator" );

#endif
