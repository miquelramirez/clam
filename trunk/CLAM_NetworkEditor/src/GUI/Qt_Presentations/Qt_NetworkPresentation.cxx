
#include "Qt_NetworkPresentation.hxx"
#include "ProcessingAdapter.hxx"
#include "ConnectionAdapter.hxx"
#include "Qt_ProcessingPresentation.hxx"
#include "Qt_ConnectionPresentation.hxx"
#include "Qt_InPortPresentation.hxx"
#include "Qt_OutPortPresentation.hxx"

#include <qpainter.h>
#include <iostream>
#include <qpixmap.h>

namespace NetworkGUI
{

Qt_NetworkPresentation::Qt_NetworkPresentation( QWidget *parent, const char *name)
	: QWidget( parent, name ),	  
	  mNameLabel(this),
	  mInPortSelected(0),
	  mOutPortSelected(0)
{
	resize(800,600);
	setPalette( QPalette( QColor( 250, 250, 200 )));
 	SetInPortClicked.Wrap( this, &Qt_NetworkPresentation::OnNewInPortClicked);
 	SetOutPortClicked.Wrap( this, &Qt_NetworkPresentation::OnNewOutPortClicked);
}

Qt_NetworkPresentation::~Qt_NetworkPresentation()
{
}

void Qt_NetworkPresentation::OnNewInPortClicked( Qt_InPortPresentation * inport)
{
	mInPortSelected = inport;	
	grabMouse();
}

void Qt_NetworkPresentation::OnNewOutPortClicked( Qt_OutPortPresentation * outport)
{
	mOutPortSelected = outport;	
	grabMouse();
}

void Qt_NetworkPresentation::OnNewName(const std::string& name)
{
	mName = name;
	mNameLabel.setFont(QFont( "Verdana", 10));
	mNameLabel.setFrameStyle( QFrame::Panel | QFrame::Sunken );
	mNameLabel.setAlignment( AlignCenter );
	mNameLabel.setText(QString(mName.c_str()));

	QFont font( "Verdana" ,10 );
	QFontMetrics fm( font );
	int pixelsWide = fm.width( QString(mName.c_str()));
	int pixelsHigh = fm.height();
	mNameLabel.setFixedSize(pixelsWide + 30, pixelsHigh*2);

}

void Qt_NetworkPresentation::OnNewProcessing( CLAMVM::ProcessingAdapter* adapter, const std::string & name)
{
	Qt_ProcessingPresentation* presentation = new Qt_ProcessingPresentation(name, this);
	presentation->AttachTo(*adapter);
	presentation->AcquireInPortClicked.Connect( SetInPortClicked );
	presentation->AcquireOutPortClicked.Connect( SetOutPortClicked );
	AcquireOutPortAfterClickInPort.Connect( presentation->SetOutPortAfterClickInPort );
	AcquireInPortAfterClickOutPort.Connect( presentation->SetInPortAfterClickOutPort );
	adapter->Publish();
	mProcessingPresentations.push_back(presentation);
	Show();	
	SendNewMessageToStatus.Emit( "Created " + presentation->GetNameFromNetwork() );
}

void Qt_NetworkPresentation::OnNewConnection( CLAMVM::ConnectionAdapter* adapter)
{
	Qt_ConnectionPresentation* presentation = new Qt_ConnectionPresentation(this);
	presentation->AttachTo(*adapter);
	presentation->RemoveConnection.Connect(SetRemoveConnection);
	adapter->Publish();
	// connectar presentation a outport i inport signals

	AttachConnectionToPortPresentations(presentation);
	mConnectionPresentations.push_back(presentation);
	Show();
	SendNewMessageToStatus.Emit( "Linked " + presentation->GetOutName() +
				     " to " + presentation->GetInName() );
}

void Qt_NetworkPresentation::AttachConnectionToPortPresentations( Qt_ConnectionPresentation * con)
{
	std::string outName = con->GetOutName();
	Qt_OutPortPresentation & out = (Qt_OutPortPresentation&)
		GetOutPortPresentationByCompleteName(outName);
	std::string inName = con->GetInName();
	Qt_InPortPresentation & in = (Qt_InPortPresentation&)
		GetInPortPresentationByCompleteName(inName);
	Qt_OutPortPresentation & concreteOut = (Qt_OutPortPresentation &)out;
	Qt_InPortPresentation & concreteIn = (Qt_InPortPresentation &)in;

	concreteOut.AcquirePos.Connect(con->SetOutPos);
	concreteIn.AcquirePos.Connect(con->SetInPos);	
}

void Qt_NetworkPresentation::Show()
{
	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
	{
		Qt_ProcessingPresentation * proc = (Qt_ProcessingPresentation*)(*it);
		proc->Show();
		proc->EmitPositionOfPorts();
		
	}	
	ConnectionPresentationIterator itc;
	for ( itc=mConnectionPresentations.begin(); itc!=mConnectionPresentations.end(); itc++)
	{
		(*itc)->Show();
	}
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
	        AcquireOutPortAfterClickInPort.Emit( m->pos() );
	}
	else if (mOutPortSelected)
	{
	        AcquireInPortAfterClickOutPort.Emit( m->pos() );
	}
	if (mInPortSelected && mOutPortSelected) //there is a new connection
	{
		const std::string inPort = GetCompleteNameFromInPortSelected();
		const std::string outPort = GetCompleteNameFromOutPortSelected();
		CreateNewConnectionFromGUI.Emit( outPort, inPort );
	}

	mInPortSelected = 0;
	mOutPortSelected = 0;

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

void Qt_NetworkPresentation::Hide()
{
	ProcessingPresentationIterator it;
	for ( it=mProcessingPresentations.begin(); it!=mProcessingPresentations.end(); it++)
	{
		(*it)->Hide();
	}
	hide();	
}

void Qt_NetworkPresentation::paintEvent( QPaintEvent * e)
{
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
}

} // namespace NetworkGUI
