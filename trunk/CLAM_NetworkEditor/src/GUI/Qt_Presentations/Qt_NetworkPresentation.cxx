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

#include "Qt_NetworkPresentation.hxx"
#include "ProcessingController.hxx"
#include "ConnectionAdapter.hxx"
#include "Factory.hxx"
#include "Qt_ProcessingPresentation.hxx"
#include "Qt_PortConnectionPresentation.hxx"
#include "Qt_ControlConnectionPresentation.hxx"
#include "Qt_InPortPresentation.hxx"
#include "Qt_OutPortPresentation.hxx"
#include "Qt_InControlPresentation.hxx"
#include "Qt_OutControlPresentation.hxx"

#include <qpainter.h>
#include <qpixmap.h>
#include <qdragobject.h> 

#include "ProcessingConfig.hxx"

namespace NetworkGUI
{

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;

Qt_NetworkPresentation::Qt_NetworkPresentation( QWidget *parent, const char *name)
	: QWidget( parent, name ),	  
	  mInPortSelected(0),
	  mOutPortSelected(0),
	  mInControlSelected(0),
	  mOutControlSelected(0)
{
	resize(800,600);
	setPalette( QPalette( QColor( 250, 250, 200 )));
 	SlotSetInPortClicked.Wrap( this, &Qt_NetworkPresentation::SetInPortClicked);
 	SlotSetOutPortClicked.Wrap( this, &Qt_NetworkPresentation::SetOutPortClicked);

 	SlotSetInControlClicked.Wrap( this, &Qt_NetworkPresentation::SetInControlClicked);
 	SlotSetOutControlClicked.Wrap( this, &Qt_NetworkPresentation::SetOutControlClicked);

	setAcceptDrops(TRUE);
}


void Qt_NetworkPresentation::SetInPortClicked( Qt_InPortPresentation * inport)
{
	mInPortSelected = inport;	
	grabMouse();
}

void Qt_NetworkPresentation::SetOutPortClicked( Qt_OutPortPresentation * outport)
{
	mOutPortSelected = outport;	
	grabMouse();
}

void Qt_NetworkPresentation::SetInControlClicked( Qt_InControlPresentation * incontrol)
{
	mInControlSelected = incontrol;	
	grabMouse();
}

void Qt_NetworkPresentation::SetOutControlClicked( Qt_OutControlPresentation * outcontrol)
{
	mOutControlSelected = outcontrol;	
	grabMouse();
}

void Qt_NetworkPresentation::SetName(const std::string& name)
{
	mName = name;

//	parent()->setCaption(QString(mName.c_str()));

	QFont font( "Verdana" ,10 );
	QFontMetrics fm( font );
	int pixelsWide = fm.width( QString(mName.c_str()));
	int pixelsHigh = fm.height();

}

void Qt_NetworkPresentation::CreateProcessingPresentation( const std::string & name, CLAMVM::ProcessingController * controller )
{
	Qt_ProcessingPresentation* presentation = new Qt_ProcessingPresentation(name, this);
	presentation->AttachTo(*controller);

	presentation->SignalAcquireInPortClicked.Connect( SlotSetInPortClicked );
	presentation->SignalAcquireOutPortClicked.Connect( SlotSetOutPortClicked );
	presentation->SignalAcquireInControlClicked.Connect( SlotSetInControlClicked );
	presentation->SignalAcquireOutControlClicked.Connect( SlotSetOutControlClicked );
	presentation->SignalRemoveProcessing.Connect( SlotRemoveProcessing );

	SignalAcquireOutPortAfterClickInPort.Connect( presentation->SlotSetOutPortAfterClickInPort );
	SignalAcquireInPortAfterClickOutPort.Connect( presentation->SlotSetInPortAfterClickOutPort );
	SignalAcquireOutControlAfterClickInControl.Connect( presentation->SlotSetOutControlAfterClickInControl );
	SignalAcquireInControlAfterClickOutControl.Connect( presentation->SlotSetInControlAfterClickOutControl );

	controller->Publish();
	mProcessingPresentations.push_back(presentation);

	presentation->Show();

	SignalSendNewMessageToStatus.Emit( "Created " + presentation->GetNameFromNetwork() );
}

void Qt_NetworkPresentation::CreatePortConnectionPresentation( CLAMVM::ConnectionAdapter* adapter)
{
	Qt_PortConnectionPresentation* presentation = new Qt_PortConnectionPresentation(this);
	presentation->AttachTo(*adapter);
	presentation->SignalRemoveConnection.Connect( SlotRemovePortConnection);
	adapter->Publish();
	// connectar presentation a outport i inport signals

	AttachConnectionToPortPresentations(presentation);
	mConnectionPresentations.push_back(presentation);

	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
	{
		Qt_ProcessingPresentation * proc = (Qt_ProcessingPresentation*)(*it);
		proc->EmitPositionOfChildren();
		
	}	
	presentation->Show();

	SignalSendNewMessageToStatus.Emit( "Linked " + presentation->GetOutName() +
				     " to " + presentation->GetInName() );
}


void Qt_NetworkPresentation::CreateControlConnectionPresentation( CLAMVM::ConnectionAdapter* adapter)
{
	Qt_ControlConnectionPresentation* presentation = new Qt_ControlConnectionPresentation(this);
	presentation->AttachTo(*adapter);
	presentation->SignalRemoveConnection.Connect( SlotRemoveControlConnection);
	adapter->Publish();
	// connectar presentation a outport i inport signals

	AttachConnectionToControlPresentations(presentation);
	mConnectionPresentations.push_back(presentation);

	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
	{
		Qt_ProcessingPresentation * proc = (Qt_ProcessingPresentation*)(*it);
		proc->EmitPositionOfChildren();
		
	}	
	presentation->Show();

	SignalSendNewMessageToStatus.Emit( "Linked " + presentation->GetOutName() +
				     " to " + presentation->GetInName() );
}

void Qt_NetworkPresentation::AttachConnectionToPortPresentations( Qt_PortConnectionPresentation * con)
{
	Qt_OutPortPresentation & out = (Qt_OutPortPresentation&)
		GetOutPortPresentationByCompleteName( con->GetOutName() );
	Qt_InPortPresentation & in = (Qt_InPortPresentation&)
		GetInPortPresentationByCompleteName( con->GetInName() );

	out.SignalAcquirePos.Connect(con->SlotSetOutPos);	
	in.SignalAcquirePos.Connect(con->SlotSetInPos);	
}


void Qt_NetworkPresentation::AttachConnectionToControlPresentations( Qt_ControlConnectionPresentation * con)
{
	Qt_OutControlPresentation & out = (Qt_OutControlPresentation&)
		GetOutControlPresentationByCompleteName( con->GetOutName() );
	Qt_InControlPresentation & in = (Qt_InControlPresentation&)
		GetInControlPresentationByCompleteName( con->GetInName() );

	out.SignalAcquirePos.Connect(con->SlotSetOutPos);	
	in.SignalAcquirePos.Connect(con->SlotSetInPos);	
}

void Qt_NetworkPresentation::Show()
{
	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
		(*it)->Show();

	ConnectionPresentationIterator itc;
	for ( itc=mConnectionPresentations.begin(); itc!=mConnectionPresentations.end(); itc++)
		(*itc)->Show();

	show();
}

void Qt_NetworkPresentation::mouseMoveEvent( QMouseEvent *m)
{
	mMousePos = m->pos();
	repaint();
}

void Qt_NetworkPresentation::mouseReleaseEvent( QMouseEvent *m)
{
	if( mInPortSelected ) //from inport to outport
	{
	        SignalAcquireOutPortAfterClickInPort.Emit( m->pos() );
	}
	else if (mOutPortSelected)
	{
	        SignalAcquireInPortAfterClickOutPort.Emit( m->pos() );
	}
	if (mInPortSelected && mOutPortSelected) //there is a new connection
	{
		const std::string inPort = GetCompleteNameFromInPortSelected();
		const std::string outPort = GetCompleteNameFromOutPortSelected();
		SignalCreatePortConnection.Emit( outPort, inPort );
	}

	mInPortSelected = 0;
	mOutPortSelected = 0;


	if( mInControlSelected ) //from inport to outport
	{
	        SignalAcquireOutControlAfterClickInControl.Emit( m->pos() );
	}
	else if (mOutControlSelected)
	{
	        SignalAcquireInControlAfterClickOutControl.Emit( m->pos() );
	}
	if (mInControlSelected && mOutControlSelected) //there is a new connection
	{
		const std::string inControl = GetCompleteNameFromInControlSelected();
		const std::string outControl = GetCompleteNameFromOutControlSelected();
		SignalCreateControlConnection.Emit( outControl, inControl );
	}

	mInControlSelected = 0;
	mOutControlSelected = 0;


	releaseMouse();
	repaint();
}

const std::string Qt_NetworkPresentation::GetCompleteNameFromInPortSelected()
{
	Qt_ProcessingPresentation * inproc = (Qt_ProcessingPresentation*)
		mInPortSelected->parentWidget();
	return  inproc->GetNameFromNetwork() + "." + mInPortSelected->GetName();
}

const std::string Qt_NetworkPresentation::GetCompleteNameFromOutPortSelected()
{
	Qt_ProcessingPresentation * outproc = (Qt_ProcessingPresentation*)
		mOutPortSelected->parentWidget();
	return outproc->GetNameFromNetwork() + "." + mOutPortSelected->GetName();
}

const std::string Qt_NetworkPresentation::GetCompleteNameFromInControlSelected()
{
	Qt_ProcessingPresentation * inproc = (Qt_ProcessingPresentation*)
		mInControlSelected->parentWidget();
	return  inproc->GetNameFromNetwork() + "." + mInControlSelected->GetName();
}

const std::string Qt_NetworkPresentation::GetCompleteNameFromOutControlSelected()
{
	Qt_ProcessingPresentation * outproc = (Qt_ProcessingPresentation*)
		mOutControlSelected->parentWidget();
	return outproc->GetNameFromNetwork() + "." + mOutControlSelected->GetName();
}


void Qt_NetworkPresentation::Hide()
{
	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
		(*it)->Hide();
	hide();	
}

void Qt_NetworkPresentation::paintEvent( QPaintEvent * e)
{
	UpdatePresentations();
	
	if (mInPortSelected)
	{
		QPainter p(this);
		p.setBrush(QColor(255, 255, 255) );
		p.drawLine(mInPortSelected->pos() + mInPortSelected->parentWidget()->pos(),
			   mMousePos);
	}

	if (mOutPortSelected)
	{
		QPainter p(this);
		p.setBrush(QColor(255, 255, 255) );
		p.drawLine(mOutPortSelected->pos() + mOutPortSelected->parentWidget()->pos(),
			   mMousePos);
	}

	if (mInControlSelected)
	{
		QPainter p(this);
		p.setBrush(QColor(255, 255, 255) );
		p.drawLine(mInControlSelected->pos() + mInControlSelected->parentWidget()->pos(),
			   mMousePos);
	}

	if (mOutControlSelected)
	{
		QPainter p(this);
		p.setBrush(QColor(255, 255, 255) );
		p.drawLine(mOutControlSelected->pos() + mOutControlSelected->parentWidget()->pos(),
			   mMousePos);
	}

}

void Qt_NetworkPresentation::dragEnterEvent(QDragEnterEvent* event)
{
	event->accept( QTextDrag::canDecode(event) );
}

void Qt_NetworkPresentation::dropEvent(QDropEvent* event)
{
	QString text;
	
	if ( QTextDrag::decode(event, text) ) 
	{
		ProcessingFactory & factory = ProcessingFactory::GetInstance();
		std::string completeName(text.ascii());
		
		std::string className(GetProcessingIdentifier(completeName));
		std::string concreteName(GetLastIdentifier(completeName));
		AddProcessing( concreteName, factory.Create(className) );
		SignalProcessingCreated.Emit();

		Qt_ProcessingPresentation & proc = (Qt_ProcessingPresentation&)GetProcessingPresentation(concreteName);
		proc.move(event->pos());
	}
}

 

} // namespace NetworkGUI
