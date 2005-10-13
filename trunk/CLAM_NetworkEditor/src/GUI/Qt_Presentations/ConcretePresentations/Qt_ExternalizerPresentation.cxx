#ifndef __Qt_ExternalizerPresentation_hxx__
#define __Qt_ExternalizerPresentation_hxx__

#include "Qt_ProcessingPresentation.hxx"
#include "Qt_InPortPresentation.hxx"
#include "Qt_OutControlPresentation.hxx"
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
		
public:
	Qt_ExternSinkPresentation(){}
};

class Qt_ExternGeneratorPresentation :  public Qt_ProcessingPresentation
{
protected:
	virtual QColor GetColorOfState()
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

public:
	Qt_ExternGeneratorPresentation(){}
};

class Qt_ExternInControlPresentation :  public Qt_ProcessingPresentation
{
protected:
	virtual QColor GetColorOfState()
	{
		QColor c(0, 0, 0);
		switch( mProcessingState )
		{
			case CLAM::Processing::Ready:
				c.setRgb( 90, 210, 180 );
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

public:
	Qt_ExternInControlPresentation(){}
};

}
#include "Factory.hxx"
typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ExternSinkPresentation > 
	regtExternSink( "ExternSink" );

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ExternGeneratorPresentation > 
	regtExternGenerator( "ExternGenerator" );

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ExternInControlPresentation > 
	regtExternInControl( "ExternInControl" );

#endif
