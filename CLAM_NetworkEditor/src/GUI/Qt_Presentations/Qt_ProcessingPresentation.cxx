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

#include "Qt_ProcessingPresentation.hxx"
#include "Qt_InPortPresentation.hxx"
#include "Qt_OutPortPresentation.hxx"
#include "Qt_ProcessingConfigPresentation.hxx"
#include "ProcessingController.hxx"
#include "Processing.hxx"

#include "Qt_InControlPresentation.hxx"
#include "Qt_OutControlPresentation.hxx"
#include <qtooltip.h> 
#include <qpainter.h>
#include <qlineedit.h>
#include <cmath>

namespace NetworkGUI
{

Qt_ProcessingPresentation::Qt_ProcessingPresentation( std::string nameFromNetwork, QWidget *parent, const char *name)
	: QWidget( parent, name ),
	  ProcessingPresentation(nameFromNetwork),
	  mSelected( false )
{
	QWidget * top = topLevelWidget();
	UpdateSize();
	// now we position the processings in the gui with a random function
	// we put them inside 3/4 of the network, in order to avoid processings in the borders
	QPoint position(rand()%(3*top->width()/4), rand()%(3*top->height()/4));
	move(position);

	// port slots
	SlotSetInPortClicked.Wrap( this, &Qt_ProcessingPresentation::SetInPortClicked);
	SlotSetOutPortClicked.Wrap( this, &Qt_ProcessingPresentation::SetOutPortClicked);
	SlotSetOutPortAfterClickInPort.Wrap(this, &Qt_ProcessingPresentation::SetOutPortAfterClickInPort);
	SlotSetInPortAfterClickOutPort.Wrap(this, &Qt_ProcessingPresentation::SetInPortAfterClickOutPort);

	// control slots
	SlotSetInControlClicked.Wrap( this, &Qt_ProcessingPresentation::SetInControlClicked);
	SlotSetOutControlClicked.Wrap( this, &Qt_ProcessingPresentation::SetOutControlClicked);
	SlotSetOutControlAfterClickInControl.Wrap(this, &Qt_ProcessingPresentation::SetOutControlAfterClickInControl);
	SlotSetInControlAfterClickOutControl.Wrap(this, &Qt_ProcessingPresentation::SetInControlAfterClickOutControl);

	SlotConfigurationUpdated.Wrap( this, &Qt_ProcessingPresentation::ConfigurationUpdated );
}

void Qt_ProcessingPresentation::UpdateSize()
{
	QString name(mName.c_str());
	QString className(mObservedClassName.c_str() );

	//we calculate width of name
	QFont font( "Helvetica" ,8 );
	QFontMetrics fm( font );
	int pixelsWide = std::max( fm.width( name ), fm.width( className ) );
	int pixelsHigh = fm.height();
	setFixedSize(pixelsWide + 35, pixelsHigh*2 + 30);

	int heightPorts = std::max( mInPortPresentations.size(), mOutPortPresentations.size() );
	heightPorts = heightPorts*7+14;
	if (height() < heightPorts)
		setFixedSize(width(),heightPorts);
	
	int widthControls = std::max( mInControlPresentations.size(), mOutControlPresentations.size());
	widthControls = widthControls*13+24;
	if (width() < widthControls)
		setFixedSize(widthControls, height());

}

void Qt_ProcessingPresentation::ConfigurationUpdated( bool ok )
{
	parentWidget()->setFocus();
	parentWidget()->grabKeyboard();
}

void Qt_ProcessingPresentation::SetInPortAfterClickOutPort( const QPoint & p)
{
	if (!geometry().contains(p))
		return;

	const QPoint real = mapFromParent(p);
	if (real.x() >= 10) // must be minor to be an inport
		return;

	ConnectionPointPresentationsList::iterator it;
	for ( it=mInPortPresentations.begin(); it!=mInPortPresentations.end(); it++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*it);
		if (in->geometry().contains(real))
			in->SignalAcquireInPortClicked.Emit(in);
	}
}

void Qt_ProcessingPresentation::SetOutPortAfterClickInPort( const QPoint & p)
{
	if (!geometry().contains(p))
		return;
	const QPoint real = mapFromParent(p);
	if (real.x() <width()- 10) // must be major to be an outport
		return;
	
	ConnectionPointPresentationsList::iterator it;
	for ( it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end(); it++)
	{
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*it);
		if (out->geometry().contains(real))
			out->SignalAcquireOutPortClicked.Emit(out);
	}
}	

void Qt_ProcessingPresentation::SetInPortClicked( Qt_InPortPresentation * inport)
{
	SignalAcquireInPortClicked.Emit( inport );
}

void Qt_ProcessingPresentation::SetOutPortClicked( Qt_OutPortPresentation * outport)
{
	SignalAcquireOutPortClicked.Emit( outport );
}


void Qt_ProcessingPresentation::SetInControlAfterClickOutControl( const QPoint & p)
{
	if (!geometry().contains(p))
		return;

	const QPoint real = mapFromParent(p);
	if (real.y() >= 5) // must be minor to be an inport
		return;

	ConnectionPointPresentationsList::iterator it;
	for ( it=mInControlPresentations.begin(); it!=mInControlPresentations.end(); it++)
	{
		Qt_InControlPresentation * in = (Qt_InControlPresentation*)(*it);
		if (in->geometry().contains(real))
			in->SignalAcquireInControlClicked.Emit(in);
	}
}

void Qt_ProcessingPresentation::SetOutControlAfterClickInControl( const QPoint & p)
{
	if (!geometry().contains(p))
		return;
	const QPoint real = mapFromParent(p);
	if (real.y() <height()- 5) // must be major to be an outport
		return;
	
	ConnectionPointPresentationsList::iterator it;
	for ( it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end(); it++)
	{
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*it);
		if (out->geometry().contains(real))
			out->SignalAcquireOutControlClicked.Emit(out);
	}
}	

void Qt_ProcessingPresentation::SetInControlClicked( Qt_InControlPresentation * incontrol)
{
	SignalAcquireInControlClicked.Emit( incontrol );
}

void Qt_ProcessingPresentation::SetOutControlClicked( Qt_OutControlPresentation * outcontrol)
{
	SignalAcquireOutControlClicked.Emit( outcontrol );
}

void Qt_ProcessingPresentation::SetObservedClassName(const std::string& name)
{
	mObservedClassName = name;
}

void Qt_ProcessingPresentation::SetInPort( const std::string & name )
{
	Qt_InPortPresentation* presentation = 
		new Qt_InPortPresentation( mInPortPresentations.size(),this );

	presentation->SetName(name);
	presentation->SignalAcquireInPortClicked.Connect( SlotSetInPortClicked );
	mInPortPresentations.push_back(presentation);
	int heightPorts = mInPortPresentations.size()*7+14;
	if (height() < heightPorts)
	{
		setFixedSize(width(),heightPorts);
		UpdateOutControlsPosition();
	}
}

void Qt_ProcessingPresentation::SetOutPort( const std::string & name )
{
	Qt_OutPortPresentation* presentation = 
		new Qt_OutPortPresentation( mOutPortPresentations.size(), this );

	presentation->SetName(name);
	presentation->SignalAcquireOutPortClicked.Connect( SlotSetOutPortClicked );
	mOutPortPresentations.push_back(presentation);
	int heightPorts = mOutPortPresentations.size()*7+14;
	if (height() < heightPorts)
	{
		setFixedSize(width(),heightPorts);
		UpdateOutControlsPosition();
	}
}



void Qt_ProcessingPresentation::SetInControl( const std::string & name )
{
	Qt_InControlPresentation* presentation = 
		new Qt_InControlPresentation( mInControlPresentations.size(),this );

	presentation->SetName(name);
	presentation->SignalAcquireInControlClicked.Connect( SlotSetInControlClicked );
	mInControlPresentations.push_back(presentation);
	int widthControls = mInControlPresentations.size()*13+24;
	if (width() < widthControls)
	{
		setFixedSize(widthControls, height());
		UpdateOutPortsPosition();
	}
}

void Qt_ProcessingPresentation::SetOutControl(  const std::string & name )
{
	Qt_OutControlPresentation* presentation = 
		new Qt_OutControlPresentation( mOutControlPresentations.size(), this );

	presentation->SetName(name);
	presentation->SignalAcquireOutControlClicked.Connect( SlotSetOutControlClicked );
	mOutControlPresentations.push_back(presentation);
	int widthControls = mOutControlPresentations.size()*13+24;
	if (width() < widthControls)
	{
		setFixedSize(widthControls, height());
		UpdateOutPortsPosition();
	}
}

void Qt_ProcessingPresentation::UpdateOutPortsPosition()
{
	ConnectionPointPresentationsList::iterator it;
	for(it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end(); it++)
	{
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*it);
		out->updatePosition();
	}
}

void Qt_ProcessingPresentation::UpdateOutControlsPosition()
{
	ConnectionPointPresentationsList::iterator it;
	for(it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end(); it++)
	{
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*it);
		out->updatePosition();
	}
}


void Qt_ProcessingPresentation::Show()
{
	Hide();
	UpdateSize();

	ConnectionPointPresentationsList::iterator it;
	for ( it=mInPortPresentations.begin(); it!=mInPortPresentations.end(); it++)
		(*it)->Show();
	for ( it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end(); it++)
		(*it)->Show();
	for ( it=mInControlPresentations.begin(); it!=mInControlPresentations.end(); it++)
		(*it)->Show();
	for ( it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end(); it++)
		(*it)->Show();
	show();

}

void Qt_ProcessingPresentation::Hide()
{
	ConnectionPointPresentationsList::iterator it;
	for ( it=mInPortPresentations.begin(); it!=mInPortPresentations.end(); it++)
		(*it)->Hide();
	for ( it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end(); it++)
		(*it)->Hide();
	for ( it=mInControlPresentations.begin(); it!=mInControlPresentations.end(); it++)
		(*it)->Hide();
	for ( it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end(); it++)
		(*it)->Hide();
	hide();

	if(mConfig)
		mConfig->Hide();

}

void Qt_ProcessingPresentation::DrawSelectedRepresentation()
{
	QColor c(200, 200, 200);
	QPainter p( this );
        p.setBrush( c );
		
	p.drawRect( QRect( 12, 7, 5, 5 ));  // up left
	p.drawRect( QRect( width()-17, 7, 5, 5 )); // up right
	p.drawRect( QRect( 12, height()-12, 5, 5 )); // down left
	p.drawRect( QRect( width()-17, height()-12, 5, 5 )); // down right
	
	p.drawRect( QRect( width()/2 - 5 , 7, 5, 5 )); // up center
	p.drawRect( QRect( width()/2 - 5, height()-12, 5, 5 )); // down center
	
}

void Qt_ProcessingPresentation::paintEvent( QPaintEvent * )
{

	QColor c(0, 0, 0);
	switch( mProcessingState )
	{
		case CLAM::Processing::Ready:
			c.setRgb( 30, 180, 50 );
			QToolTip::add( this, "Processing is ready to run" );
			break;
		case CLAM::Processing::Unconfigured:
			c.setRgb( 200, 10, 30 );
			QToolTip::add( this, QString( mProcessingStatus.c_str() ));
			break;	
		case CLAM::Processing::Running:
			c.setRgb( 0, 100, 200 );
			QToolTip::add( this, "Processing running" );
			break;

	}
	QPainter p( this );
        p.setBrush( c );


	QRegion reg(12,7,width()-24, height()-14);
	ConnectionPointPresentationsList::iterator it;
	for (it=mInPortPresentations.begin(); it!=mInPortPresentations.end();it++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*it);
		reg += in->geometry();
	}
	for (it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end();it++)
	{
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*it);
		reg += out->geometry();
	}
	for (it=mInControlPresentations.begin(); it!=mInControlPresentations.end();it++)
	{
		Qt_InControlPresentation * in = (Qt_InControlPresentation*)(*it);
		reg += in->GetRegion();
	}
	for (it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end();it++)
	{
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*it);
		reg += out->GetRegion();
	}

	setMask(reg);
	

	p.drawRect( 12,7, width()-24,height()-14); // draw a rectangle
	p.setPen( QPen( blue, 1 ));
	p.drawRect( 12,7, width()-24,height()-14); // draw a rectangle
	p.drawLine( QPoint( 12, height()/2), QPoint( width()-12, height()/2 )); 
	
	p.setPen( QPen( black,1 ));
	
	if(mSelected)
	{
		DrawSelectedRepresentation();
//		p.setFont( QFont( "Helvetica", 8, QFont::Light, true ));
	}
//	else
//	{
		p.setFont( QFont( "Helvetica" ,8) );
//	}
	p.drawText(  QRect(12,7,width()-24, height()/2 - 5 ),
		    Qt::AlignCenter ,	
		    QString( mName.c_str() ));
				
	p.drawText(  QRect(12,height()/2,width()-24, height()/2 - 5 ),
		    Qt::AlignCenter ,	
		    QString( mObservedClassName.c_str() ));

	adjustSize();
 
}

void Qt_ProcessingPresentation::mousePressEvent( QMouseEvent *m)
{
	mPrevPos = m->globalPos();
	if(!mSelected) // already selected
	{
		mSelected = true;
		if((m->button() & LeftButton) && (m->state() & ShiftButton))
			SignalProcessingPresentationAddedToSelection.Emit( this );
		else
			SignalProcessingPresentationSelected.Emit( this );
		repaint();
	}

	if(m->button() == RightButton )
	{
		if(SlotConfigurationUpdated.ActiveConnections() == 0) // not connected yet
		{
			Qt_ProcessingConfigPresentation * cfg = (Qt_ProcessingConfigPresentation*)mConfig;
			cfg->SignalConfigurationUpdated.Connect( SlotConfigurationUpdated );
		}
		parentWidget()->releaseKeyboard();
		mConfig->Show();
	}
}

void Qt_ProcessingPresentation::mouseMoveEvent( QMouseEvent *m)
{
	SignalMovingMouseWithButtonPressed.Emit( m->globalPos() - mPrevPos );
	mPrevPos = m->globalPos();
}

void Qt_ProcessingPresentation::Move( const QPoint & difference)
{
	QPoint toMove = difference + pos();
	move( toMove );
	
	// emit movement to update connections
	ConnectionPointPresentationsList::iterator it;
	for (it=mInPortPresentations.begin(); it!=mInPortPresentations.end();it++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*it);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->SignalAcquirePos.Emit( toMove.x()+ posX, toMove.y()+posY);
	}
	for (it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end();it++)
	{	
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*it);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->SignalAcquirePos.Emit( toMove.x() + posX , toMove.y()+ posY );
	}
	for (it=mInControlPresentations.begin(); it!=mInControlPresentations.end();it++)
	{
		Qt_InControlPresentation * in = (Qt_InControlPresentation*)(*it);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->SignalAcquirePos.Emit( toMove.x()+ posX +5  , toMove.y()+posY -2 );
	}
	for (it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end();it++)
	{	
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*it);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->SignalAcquirePos.Emit( toMove.x() + posX - 4, toMove.y()+ posY +2 );
	}
}

void Qt_ProcessingPresentation::EmitPositionOfChildren()
{		
	ConnectionPointPresentationsList::iterator it;
	for (it=mInPortPresentations.begin(); it!=mInPortPresentations.end();it++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*it);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->SignalAcquirePos.Emit( pos().x() + posX, pos().y() + posY);
	}
	for (it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end();it++)
	{	
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*it);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->SignalAcquirePos.Emit( pos().x() + posX , pos().y() + posY );
	}
	for (it=mInControlPresentations.begin(); it!=mInControlPresentations.end();it++)
	{
		Qt_InControlPresentation * in = (Qt_InControlPresentation*)(*it);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->SignalAcquirePos.Emit( pos().x() + posX + 5, pos().y() + posY - 2 );
	}
	for (it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end();it++)
	{	
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*it);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->SignalAcquirePos.Emit( pos().x() + posX - 4 , pos().y() + posY +2);
	}
}

void Qt_ProcessingPresentation::mouseDoubleClickEvent ( QMouseEvent * e )
{
	QLineEdit * nameEdit = new QLineEdit( this );
	nameEdit->setText( mName.c_str() );

	connect( nameEdit, SIGNAL( textChanged ( const QString & )),
		 this, SLOT( SlotTextChange( const QString & )));

	connect( nameEdit, SIGNAL( returnPressed() ), nameEdit, SLOT( close() ) );
	connect( nameEdit, SIGNAL( returnPressed() ), this, SLOT( SlotExecuteChangeName() ) );
	connect( this, SIGNAL(SignalEmitGeometryChange( const QRect &)), nameEdit, SLOT( setGeometry( const QRect & )) );

	nameEdit->setFont( QFont( "Helvetica" ,8) );
	nameEdit->setFocus();
	nameEdit->setGeometry( QRect(12,7,width()-24, height()-14) );
	nameEdit->show();

	SignalSendMessageToStatus.Emit( "Edit the processing name" );
	nameEdit->grabKeyboard();
}

void Qt_ProcessingPresentation::UnSelectProcessingPresentation()
{
	mSelected = false;
	repaint();
}

void Qt_ProcessingPresentation::SlotTextChange( const QString & newName )
{
	ChangeProcessingPresentationName( newName.latin1() );
	emit SignalEmitGeometryChange( QRect(12,7,width()-24, height()-14) );
	EmitPositionOfChildren();
}

void Qt_ProcessingPresentation::SlotExecuteChangeName()
{
	SignalProcessingNameChanged.Emit( mName );
	parentWidget()->grabKeyboard();
	parentWidget()->setFocus();
}

void Qt_ProcessingPresentation::UpdatePresentation()
{
	repaint();
}

void Qt_ProcessingPresentation::ChangeProcessingPresentationName( const std::string & name )
{
	mName = name;
	UpdateSize();
	UpdateOutControlsPosition();
	UpdateOutPortsPosition();
}

} // namespace NetworkGUI
