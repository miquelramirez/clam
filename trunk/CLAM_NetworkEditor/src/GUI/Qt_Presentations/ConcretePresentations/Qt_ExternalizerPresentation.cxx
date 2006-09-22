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

class Qt_AudioSinkPresentation :  public Qt_ProcessingPresentation
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
				QToolTip::add( this, "Processing needs to be configured" );
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
	Qt_AudioSinkPresentation(){}
};

class Qt_AudioSourcePresentation :  public Qt_ProcessingPresentation
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
				QToolTip::add( this, QString( "Processing needs to be configured" ));
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
	Qt_AudioSourcePresentation(){}
};

class Qt_ControlSourcePresentation :  public Qt_ProcessingPresentation
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
				QToolTip::add( this, "Processing needs to be configured" );
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
	Qt_ControlSourcePresentation(){}
};

class Qt_ControlSinkPresentation :  public Qt_ProcessingPresentation
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
				QToolTip::add( this, "Processing needs to be configured" );
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
	Qt_ControlSinkPresentation(){}
};

}
#include <CLAM/Factory.hxx>
typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_AudioSinkPresentation > 
	regtAudioSink( "AudioSink" );

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_AudioSourcePresentation > 
	regtAudioSource( "AudioSource" );

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ControlSourcePresentation > 
	regtControlSource( "ControlSource" );

static Qt_ProcessingPresentationFactory::Registrator< NetworkGUI::Qt_ControlSinkPresentation > 
	regtControlSink( "ControlSink" );
#endif
