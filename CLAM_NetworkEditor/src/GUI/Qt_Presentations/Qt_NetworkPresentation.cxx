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

	SlotProcessingPresentationSelected.Wrap( this, &Qt_NetworkPresentation::ProcessingPresentationSelected );
	SlotConnectionPresentationSelected.Wrap( this, &Qt_NetworkPresentation::ConnectionPresentationSelected );
	SlotProcessingPresentationAddedToSelection.Wrap( this, &Qt_NetworkPresentation::ProcessingPresentationAddedToSelection );
	SlotConnectionPresentationAddedToSelection.Wrap( this, &Qt_NetworkPresentation::ConnectionPresentationAddedToSelection );
	
	SlotSendMessageToStatus.Wrap( this, &Qt_NetworkPresentation::SendMessageToStatus );
	SlotMovingMouseWithButtonPressed.Wrap( this, &Qt_NetworkPresentation::MovingMouseWithButtonPressed );
	setAcceptDrops(TRUE);
	setFocus();
	grabKeyboard();
}

void Qt_NetworkPresentation::SendMessageToStatus( const std::string & message )
{
	SignalSendMessageToStatus.Emit( message );
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

	QFont font( "Verdana" ,10 );
	QFontMetrics fm( font );
	int pixelsWide = fm.width( QString(mName.c_str()));
	int pixelsHigh = fm.height();

}

void Qt_NetworkPresentation::CreateProcessingPresentation( const std::string & name, CLAMVM::ProcessingController * controller )
{
	
	Qt_ProcessingPresentation* presentation = 0;
	
	presentation = new Qt_ProcessingPresentation(name, this);
	presentation->AttachTo(*controller);

	presentation->SignalAcquireInPortClicked.Connect( SlotSetInPortClicked );
	presentation->SignalAcquireOutPortClicked.Connect( SlotSetOutPortClicked );
	presentation->SignalAcquireInControlClicked.Connect( SlotSetInControlClicked );
	presentation->SignalAcquireOutControlClicked.Connect( SlotSetOutControlClicked );
	presentation->SignalRemoveProcessing.Connect( SlotRemoveProcessing );
	presentation->SignalProcessingPresentationSelected.Connect( SlotProcessingPresentationSelected );
	presentation->SignalSendMessageToStatus.Connect( SlotSendMessageToStatus );
	presentation->SignalMovingMouseWithButtonPressed.Connect( SlotMovingMouseWithButtonPressed );
	presentation->SignalProcessingPresentationAddedToSelection.Connect( SlotProcessingPresentationAddedToSelection );
		
	SignalAcquireOutPortAfterClickInPort.Connect( presentation->SlotSetOutPortAfterClickInPort );
	SignalAcquireInPortAfterClickOutPort.Connect( presentation->SlotSetInPortAfterClickOutPort );
	SignalAcquireOutControlAfterClickInControl.Connect( presentation->SlotSetOutControlAfterClickInControl );
	SignalAcquireInControlAfterClickOutControl.Connect( presentation->SlotSetInControlAfterClickOutControl );

	controller->Publish();
	mProcessingPresentations.push_back(presentation);

	presentation->Show();

	SignalSendMessageToStatus.Emit( "Created " + presentation->GetName() );
}

void Qt_NetworkPresentation::CreatePortConnectionPresentation( CLAMVM::ConnectionAdapter* adapter)
{
	Qt_PortConnectionPresentation* presentation = new Qt_PortConnectionPresentation(this);
	presentation->AttachTo(*adapter);
	presentation->SignalRemoveConnection.Connect( SlotRemovePortConnection);
	presentation->SignalConnectionPresentationSelected.Connect( SlotConnectionPresentationSelected );
	presentation->SignalConnectionPresentationAddedToSelection.Connect( SlotConnectionPresentationAddedToSelection );

	AttachConnectionToPortPresentations(presentation);
	mConnectionPresentations.push_back(presentation);

	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
	{
		Qt_ProcessingPresentation * proc = (Qt_ProcessingPresentation*)(*it);
		proc->EmitPositionOfChildren();
		
	}	
	presentation->Show();

	SignalSendMessageToStatus.Emit( "Linked " + presentation->GetOutName() +
				     " to " + presentation->GetInName() );
}


void Qt_NetworkPresentation::CreateControlConnectionPresentation( CLAMVM::ConnectionAdapter* adapter)
{
	Qt_ControlConnectionPresentation* presentation = new Qt_ControlConnectionPresentation(this);
	presentation->AttachTo(*adapter);
	presentation->SignalRemoveConnection.Connect( SlotRemoveControlConnection);
	presentation->SignalConnectionPresentationSelected.Connect( SlotConnectionPresentationSelected );
	presentation->SignalConnectionPresentationAddedToSelection.Connect( SlotConnectionPresentationAddedToSelection );
	
	AttachConnectionToControlPresentations(presentation);
	mConnectionPresentations.push_back(presentation);

	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
	{
		Qt_ProcessingPresentation * proc = (Qt_ProcessingPresentation*)(*it);
		proc->EmitPositionOfChildren();
		
	}	
	presentation->Show();

	SignalSendMessageToStatus.Emit( "Linked " + presentation->GetOutName() +
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
void Qt_NetworkPresentation::keyPressEvent( QKeyEvent * k)
{
	switch ( k->key() ) 
	{

	case Key_Delete:
		QtProcessingList::iterator it;
		for( it=mSelectedProcessingPresentations.begin(); it!=mSelectedProcessingPresentations.end(); it++ )
			(*it)->SignalRemoveProcessing.Emit( *it );
		mSelectedProcessingPresentations.clear();
		
		QtConnectionList::iterator itc;
		for( itc=mSelectedConnectionPresentations.begin(); itc!=mSelectedConnectionPresentations.end(); itc++ )
			(*itc)->SignalRemoveConnection.Emit( *itc );
		mSelectedConnectionPresentations.clear();
		
		repaint();
		break;

	}

}

void Qt_NetworkPresentation::mouseMoveEvent( QMouseEvent *m)
{
	mMousePos = m->pos();
	repaint();
}
void Qt_NetworkPresentation::mousePressEvent ( QMouseEvent * e )
{
	QtProcessingList::iterator it;
	for( it=mSelectedProcessingPresentations.begin(); it!=mSelectedProcessingPresentations.end(); it++ )
		(*it)->UnSelectProcessingPresentation();
	mSelectedProcessingPresentations.clear();

	QtConnectionList::iterator itc;
	for( itc=mSelectedConnectionPresentations.begin(); itc!=mSelectedConnectionPresentations.end(); itc++ )
		(*itc)->UnSelectConnectionPresentation();
	mSelectedConnectionPresentations.clear();

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
	return  inproc->GetName() + "." + mInPortSelected->GetName();
}

const std::string Qt_NetworkPresentation::GetCompleteNameFromOutPortSelected()
{
	Qt_ProcessingPresentation * outproc = (Qt_ProcessingPresentation*)
		mOutPortSelected->parentWidget();
	return outproc->GetName() + "." + mOutPortSelected->GetName();
}

const std::string Qt_NetworkPresentation::GetCompleteNameFromInControlSelected()
{
	Qt_ProcessingPresentation * inproc = (Qt_ProcessingPresentation*)
		mInControlSelected->parentWidget();
	return  inproc->GetName() + "." + mInControlSelected->GetName();
}

const std::string Qt_NetworkPresentation::GetCompleteNameFromOutControlSelected()
{
	Qt_ProcessingPresentation * outproc = (Qt_ProcessingPresentation*)
		mOutControlSelected->parentWidget();
	return outproc->GetName() + "." + mOutControlSelected->GetName();
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
	setFocus();
}

void Qt_NetworkPresentation::ProcessingPresentationSelected( Qt_ProcessingPresentation * proc )
{
	// remove selected presentations
	QtConnectionList::iterator it;
	for( it=mSelectedConnectionPresentations.begin(); it!=mSelectedConnectionPresentations.end(); it++ )
		(*it)->UnSelectConnectionPresentation();
	mSelectedConnectionPresentations.clear();

	// remove selected processsings
	QtProcessingList::iterator itc;
	for( itc=mSelectedProcessingPresentations.begin(); itc!=mSelectedProcessingPresentations.end(); itc++ )
		(*itc)->UnSelectProcessingPresentation();
	mSelectedProcessingPresentations.clear();

	mSelectedProcessingPresentations.push_back( proc );
}

void Qt_NetworkPresentation::ProcessingPresentationAddedToSelection( Qt_ProcessingPresentation * proc)
{
	mSelectedProcessingPresentations.push_back( proc );

}

void Qt_NetworkPresentation::ConnectionPresentationSelected( Qt_ConnectionPresentation * con )
{
	// remove selected presentations
	QtConnectionList::iterator it;
	for( it=mSelectedConnectionPresentations.begin(); it!=mSelectedConnectionPresentations.end(); it++ )
		(*it)->UnSelectConnectionPresentation();
	mSelectedConnectionPresentations.clear();
	// remove selected processsings
	QtProcessingList::iterator itc;
	for( itc=mSelectedProcessingPresentations.begin(); itc!=mSelectedProcessingPresentations.end(); itc++ )
		(*itc)->UnSelectProcessingPresentation();
		mSelectedProcessingPresentations.clear();

	mSelectedConnectionPresentations.push_back( con );

}

void Qt_NetworkPresentation::ConnectionPresentationAddedToSelection( Qt_ConnectionPresentation * con )
{	
	mSelectedConnectionPresentations.push_back( con );
}

void Qt_NetworkPresentation::MovingMouseWithButtonPressed( const QPoint & p)
{
	if(mSelectedProcessingPresentations.size())
	{
		QtProcessingList::iterator it;
		for( it=mSelectedProcessingPresentations.begin(); it!=mSelectedProcessingPresentations.end(); it++ )
			(*it)->Move(p);
	}
}




} // namespace NetworkGUI
