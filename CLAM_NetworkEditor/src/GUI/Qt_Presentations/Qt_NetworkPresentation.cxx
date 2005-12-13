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
#include <CLAM/Factory.hxx>
#include "Qt_ProcessingPresentation.hxx"
#include "Qt_PortConnectionPresentation.hxx"
#include "Qt_ControlConnectionPresentation.hxx"
#include "Qt_InPortPresentation.hxx"
#include "Qt_OutPortPresentation.hxx"
#include "Qt_InControlPresentation.hxx"
#include "Qt_OutControlPresentation.hxx"

#include "NetworkController.hxx"

#include <CLAM/Text.hxx>
#include <CLAM/DynamicType.hxx>
#include <CLAM/XMLStorage.hxx>

#include <qpainter.h>
#include <qpixmap.h>
#include <qdragobject.h> 

#include <iostream> // TODO: remove
#include <fstream>
#include "MainWindow.hxx"

namespace NetworkGUI
{

typedef CLAM::Factory<CLAM::Processing> ProcessingFactory;
typedef CLAM::Factory<NetworkGUI::Qt_ProcessingPresentation> Qt_ProcessingPresentationFactory;

/////////Classes related to widget-position storage on Network Editor by using XML
class WidgetPosition : public CLAM::DynamicType
{
public:
	DYNAMIC_TYPE( WidgetPosition, 5);
	DYN_ATTRIBUTE( 0, public, CLAM::Text, Name);
	DYN_ATTRIBUTE( 1, public, CLAM::TSize, X);
	DYN_ATTRIBUTE( 2, public, CLAM::TSize, Y);
	DYN_ATTRIBUTE( 3, public, CLAM::TSize, Width);
	DYN_ATTRIBUTE( 4, public, CLAM::TSize, Height);
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
	}
};

class NetworkEditorPositions : public CLAM::DynamicType
{
public:
	DYNAMIC_TYPE( NetworkEditorPositions, 3 );
	DYN_ATTRIBUTE( 0, public, CLAM::TSize, WindowWidth );
	DYN_ATTRIBUTE( 1, public, CLAM::TSize, WindowHeight );
	DYN_CONTAINER_ATTRIBUTE( 2, public, std::list<WidgetPosition>, WidgetList, Widget);
 
protected:
	void DefaultInit()
	{
		AddAll();
		UpdateData();
	}
};
/////////


Qt_NetworkPresentation::Qt_NetworkPresentation( MainWindow *parent, const char *name)
	: QWidget( parent, name ),	  
	  mInPortSelected(0),
	  mOutPortSelected(0),
	  mInControlSelected(0),
	  mOutControlSelected(0)
{
	mMainWindow = parent;
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
	setFocusPolicy(QWidget::ClickFocus);
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
/* 
	QFont font( "Verdana" ,10 );
	QFontMetrics fm( font );
	int pixelsWide = fm.width( QString(mName.c_str()));
	int pixelsHigh = fm.height();
*/
}

Qt_ProcessingPresentation* Qt_NetworkPresentation::FindProcessingPresentation(const std::string& name)
{
 	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
	{
		Qt_ProcessingPresentation * proc = (Qt_ProcessingPresentation*)(*it);
		if (name==proc->GetName())
			return proc;
	}
	return 0;
}

void Qt_NetworkPresentation::SaveWidgetsPositions(const std::string& baseFilename)
{
	std::string positionsFilename = baseFilename + ".pos";

	NetworkEditorPositions pos;
	pos.SetWindowWidth( mMainWindow->size().width() );
	pos.SetWindowHeight( mMainWindow->size().height() );

	WidgetPosition wpos;
	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
	{
		Qt_ProcessingPresentation * proc = (Qt_ProcessingPresentation*)(*it);
		wpos.SetName( proc->GetName() );
		wpos.SetX( proc->pos().x() );
		wpos.SetY( proc->pos().y() );
		wpos.SetWidth( proc->size().width() );
		wpos.SetHeight( proc->size().height() );

		pos.GetWidgetList().push_back(wpos);
	}
	CLAM::XmlStorage::Dump(pos,"WidgetLocations",positionsFilename);
	
}
void Qt_NetworkPresentation::SetUpWidgetsPositions(const std::string& baseFilename)
{
	std::string positionsFilename = baseFilename+".pos";
	std::cout<<"opening file "<<positionsFilename<<std::endl;

	NetworkEditorPositions pos;
	try
	{
		CLAM::XmlStorage::Restore(pos,positionsFilename);
	}catch (std::exception e)
	{
		std::cerr << "Warning: Could not restore network-positions file.\n"
			<< e.what() << std::endl;
		return;
	}
	
	resize( pos.GetWindowWidth(),pos.GetWindowHeight() );
	mMainWindow->resize( pos.GetWindowWidth() , pos.GetWindowHeight() );
	std::string caption("CLAM Network Editor -- ");
	caption += baseFilename;
	mMainWindow->setCaption( caption.c_str() );

	for (std::list<WidgetPosition>::iterator it=pos.GetWidgetList().begin(); it!=pos.GetWidgetList().end(); it++)
	{
		Qt_ProcessingPresentation * proc=FindProcessingPresentation( it->GetName() );
		if (!proc)
		{
			std::cerr<<"Warning: found bad name in network-positions file: '"
				<< it->GetName() <<"'\n";
			continue;
		}
		proc->MoveAbsolute( QPoint( it->GetX() , it->GetY() ) );
		proc->resize( it->GetWidth() , it->GetHeight() );
		proc->ConfigurationUpdated(true);
	}
}

void Qt_NetworkPresentation::CreateProcessingPresentation( const std::string & name, CLAMVM::ProcessingController * controller )
{
	
	Qt_ProcessingPresentation* procPresent = 0;

	Qt_ProcessingPresentationFactory & factory =  Qt_ProcessingPresentationFactory::GetInstance();
	try
	{
		procPresent = factory.CreateSafe(controller->GetObservedClassName());
	}
	catch (CLAM::ErrFactory & err)
	{
		// No special presentation, just the standard one
		procPresent = new Qt_ProcessingPresentation;
	}
	procPresent->Initialize( name, this );
			
	procPresent->AttachToProcessingController(*controller);

	procPresent->SignalAcquireInPortClicked.Connect( SlotSetInPortClicked );
	procPresent->SignalAcquireOutPortClicked.Connect( SlotSetOutPortClicked );
	procPresent->SignalAcquireInControlClicked.Connect( SlotSetInControlClicked );
	procPresent->SignalAcquireOutControlClicked.Connect( SlotSetOutControlClicked );
	procPresent->SignalRemoveProcessing.Connect( SlotRemoveProcessing );
	procPresent->SignalProcessingPresentationSelected.Connect( SlotProcessingPresentationSelected );
	procPresent->SignalSendMessageToStatus.Connect( SlotSendMessageToStatus );
	procPresent->SignalMovingMouseWithButtonPressed.Connect( SlotMovingMouseWithButtonPressed );
	procPresent->SignalProcessingPresentationAddedToSelection.Connect( SlotProcessingPresentationAddedToSelection );
		
	SignalAcquireOutPortAfterClickInPort.Connect( procPresent->SlotSetOutPortAfterClickInPort );
	SignalAcquireInPortAfterClickOutPort.Connect( procPresent->SlotSetInPortAfterClickOutPort );
	SignalAcquireOutControlAfterClickInControl.Connect( procPresent->SlotSetOutControlAfterClickInControl );
	SignalAcquireInControlAfterClickOutControl.Connect( procPresent->SlotSetInControlAfterClickOutControl );

	controller->Publish();
	mProcessingPresentations.push_back(procPresent);

	procPresent->Show();

	SignalSendMessageToStatus.Emit( "Created " + procPresent->GetName() );
}

void Qt_NetworkPresentation::CreatePortConnectionPresentation( const std::string & out, const std::string & in )
{
	Qt_PortConnectionPresentation* presentation = new Qt_PortConnectionPresentation(this);
	presentation->AttachTo( out, in );
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


void Qt_NetworkPresentation::CreateControlConnectionPresentation( const std::string & out, const std::string & in ) 
{
	Qt_ControlConnectionPresentation* presentation = new Qt_ControlConnectionPresentation(this);
	presentation->AttachTo(out, in);
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
		QtConnectionList::iterator itc;
		for( itc=mSelectedConnectionPresentations.begin(); itc!=mSelectedConnectionPresentations.end(); itc++ )
			(*itc)->SignalRemoveConnection.Emit( *itc );
		mSelectedConnectionPresentations.clear();

		QtProcessingList::iterator it;
		for( it=mSelectedProcessingPresentations.begin(); it!=mSelectedProcessingPresentations.end(); it++ )
			(*it)->SignalRemoveProcessing.Emit( *it );
		mSelectedProcessingPresentations.clear();
		
		
		repaint();
		break;

	}

}

void Qt_NetworkPresentation::mouseMoveEvent( QMouseEvent *m)
{
	mMousePos = m->globalPos();
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

	mSelectionPos = e->globalPos();
	mMousePos = e->globalPos();

}

bool Qt_NetworkPresentation::CheckPortsSelection( QMouseEvent *m )
{
	bool selected = false;
	if( mInPortSelected ) //from inport to outport
	{
	        SignalAcquireOutPortAfterClickInPort.Emit( m->pos() );
		selected = true;
	}
	else if (mOutPortSelected)
	{
	        SignalAcquireInPortAfterClickOutPort.Emit( m->pos() );
		selected = true;
	}
	if (mInPortSelected && mOutPortSelected) //there is a new connection
	{
		const std::string inPort = GetCompleteNameFromInPortSelected();
		const std::string outPort = GetCompleteNameFromOutPortSelected();
		GetNetworkController().CreatePortConnection( outPort, inPort );
		selected = true;
	}

	mInPortSelected = 0;
	mOutPortSelected = 0;
	return selected;
}

bool Qt_NetworkPresentation::CheckControlsSelection( QMouseEvent *m )
{
	bool selected = false;
	if( mInControlSelected ) //from inport to outport
	{
	        SignalAcquireOutControlAfterClickInControl.Emit( m->pos() );
		selected = true;
	}
	else if (mOutControlSelected)
	{
	        SignalAcquireInControlAfterClickOutControl.Emit( m->pos() );
		selected = true;
	}
	if (mInControlSelected && mOutControlSelected) //there is a new connection
	{
		const std::string inControl = GetCompleteNameFromInControlSelected();
		const std::string outControl = GetCompleteNameFromOutControlSelected();
		GetNetworkController().CreateControlConnection( outControl, inControl );
		selected = true;		
	}

	mInControlSelected = 0;
	mOutControlSelected = 0;
	return selected;
}

void Qt_NetworkPresentation::mouseReleaseEvent( QMouseEvent *m )
{
	if( !CheckPortsSelection(m) && !CheckControlsSelection(m) )
	{
		CheckSelectionRectangle();
		mSelectionPos = QPoint(0,0);
		mMousePos = QPoint(0,0);
	}
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
	
	if(mMousePos==QPoint(0,0))
		return;
	
	QPainter p(this);
	p.setBrush(QColor(255, 255, 255) );
	
	if (mInPortSelected)
	{
		p.drawLine(mInPortSelected->pos() + mInPortSelected->parentWidget()->pos(), mapFromGlobal(mMousePos));
		return;
	}

	if (mOutPortSelected)
	{
		p.drawLine(mOutPortSelected->pos() + mOutPortSelected->parentWidget()->pos(), mapFromGlobal(mMousePos));
		return;
	}

	if (mInControlSelected)
	{
		p.drawLine(mInControlSelected->pos() + mInControlSelected->parentWidget()->pos(), mapFromGlobal(mMousePos));
		return;
	}

	if (mOutControlSelected)
	{
		p.drawLine(mOutControlSelected->pos() + mOutControlSelected->parentWidget()->pos(), mapFromGlobal(mMousePos));
		return;
	}

	if(mSelectionPos!=QPoint(0,0))
		DrawSelectionRectangle();

}

void Qt_NetworkPresentation::DrawSelectionRectangle()
{
	QPoint origin( std::min(mMousePos.x(), mSelectionPos.x()), std::min(mMousePos.y(), mSelectionPos.y()) );
	QPoint end( std::max(mMousePos.x(), mSelectionPos.x()), std::max(mMousePos.y(), mSelectionPos.y()) );

	QPainter p( this );
	p.drawRect( QRect( mapFromGlobal(origin), mapFromGlobal(end)) );
	
}

void Qt_NetworkPresentation::CheckSelectionRectangle()
{
	QPoint origin( std::min(mMousePos.x(), mSelectionPos.x()), std::min(mMousePos.y(), mSelectionPos.y()) );
	QPoint end( std::max(mMousePos.x(), mSelectionPos.x()), std::max(mMousePos.y(), mSelectionPos.y()) );
	QRect selection( mapFromGlobal(origin), mapFromGlobal(end) );

	// if shift
	//
	// else
	// 
	// remove selected presentations
	QtConnectionList::iterator itOld;
	for( itOld=mSelectedConnectionPresentations.begin(); itOld!=mSelectedConnectionPresentations.end(); itOld++ )
		(*itOld)->UnSelectConnectionPresentation();
	mSelectedConnectionPresentations.clear();

	// remove selected processsings
	QtProcessingList::iterator itcOld;
	for( itcOld=mSelectedProcessingPresentations.begin(); itcOld!=mSelectedProcessingPresentations.end(); itcOld++ )
		(*itcOld)->UnSelectProcessingPresentation();
	mSelectedProcessingPresentations.clear();

	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
	{
		Qt_ProcessingPresentation * proc = (Qt_ProcessingPresentation*)(*it);
		
		QRect processingR( proc->rect() );
		processingR.moveTopLeft( QPoint( proc->x(), proc->y()) );

		if(selection.contains( processingR ))
		{
			proc->SelectProcessingPresentation();
			ProcessingPresentationAddedToSelection( proc );
		}
	}
	ConnectionPresentationIterator itc;
	for ( itc=mConnectionPresentations.begin(); itc!=mConnectionPresentations.end(); itc++)
	{
		Qt_ConnectionPresentation * con = (Qt_ConnectionPresentation*)(*itc);
		
		QRect connectionR( con->rect() );
		connectionR.moveTopLeft( QPoint( con->x(), con->y()) );

		if(selection.contains( connectionR ))
		{
			con->SelectConnectionPresentation();
			ConnectionPresentationAddedToSelection( con );
		}
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
		std::string key = text.ascii();
		std::string name = AddProcessing( key );	
		SignalProcessingCreated.Emit(); 		// TODO: Get rid of this signal


		Qt_ProcessingPresentation& proc = (Qt_ProcessingPresentation&)GetProcessingPresentation( name );
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
