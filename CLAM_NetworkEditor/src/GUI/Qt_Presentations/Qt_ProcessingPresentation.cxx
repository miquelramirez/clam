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
#include "InPortAdapter.hxx"
#include "OutPortAdapter.hxx"
#include "Qt_InPortPresentation.hxx"
#include "Qt_OutPortPresentation.hxx"
#include "Qt_ProcessingConfigPresentation.hxx"

#include "InControlAdapter.hxx"
#include "OutControlAdapter.hxx"
#include "Qt_InControlPresentation.hxx"
#include "Qt_OutControlPresentation.hxx"
#include <qtooltip.h> 
#include <qpainter.h>
#include <cmath>

namespace NetworkGUI
{

Qt_ProcessingPresentation::Qt_ProcessingPresentation( std::string nameFromNetwork, QWidget *parent, const char *name)
	: QWidget( parent, name ),
	  ProcessingPresentation(nameFromNetwork),
	  mDown(false)
{
	QWidget * top = topLevelWidget();
	QString s(mNameFromNetwork.c_str());

	//we calculate width of name
	QFont font( "Helvetica" ,8 );
	QFontMetrics fm( font );
	int pixelsWide = fm.width( s );
	int pixelsHigh = fm.height();
	setFixedSize(pixelsWide + 30, pixelsHigh*3);

	// now we position the processings in the gui with a random function
	// we put them inside 3/4 of the network, in order to avoid processings in the borders
	QPoint position(rand()%(3*top->width()/4), rand()%(3*top->height()/4));
	move(position);

	// port slots
	SetInPortClicked.Wrap( this, &Qt_ProcessingPresentation::OnNewInPortClicked);
	SetOutPortClicked.Wrap( this, &Qt_ProcessingPresentation::OnNewOutPortClicked);
	SetOutPortAfterClickInPort.Wrap(this, &Qt_ProcessingPresentation::OnNewOutPortAfterClickInPort);
	SetInPortAfterClickOutPort.Wrap(this, &Qt_ProcessingPresentation::OnNewInPortAfterClickOutPort);

	// control slots
	SetInControlClicked.Wrap( this, &Qt_ProcessingPresentation::OnNewInControlClicked);
	SetOutControlClicked.Wrap( this, &Qt_ProcessingPresentation::OnNewOutControlClicked);
	SetOutControlAfterClickInControl.Wrap(this, &Qt_ProcessingPresentation::OnNewOutControlAfterClickInControl);
	SetInControlAfterClickOutControl.Wrap(this, &Qt_ProcessingPresentation::OnNewInControlAfterClickOutControl);
}

Qt_ProcessingPresentation::~Qt_ProcessingPresentation()
{
}

void Qt_ProcessingPresentation::OnNewInPortAfterClickOutPort( const QPoint & p)
{
	if (!geometry().contains(p))
		return;

	const QPoint real = mapFromParent(p);
	if (real.x() >= 10) // must be minor to be an inport
		return;

	InPortPresentationIterator itin;
	for ( itin=mInPortPresentations.begin(); itin!=mInPortPresentations.end(); itin++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*itin);
		if (in->geometry().contains(real))
			in->AcquireInPortClicked.Emit(in);
	}
}

void Qt_ProcessingPresentation::OnNewOutPortAfterClickInPort( const QPoint & p)
{
	if (!geometry().contains(p))
		return;
	const QPoint real = mapFromParent(p);
	if (real.x() <width()- 10) // must be major to be an outport
		return;
	
	OutPortPresentationIterator itout;
	for ( itout=mOutPortPresentations.begin(); itout!=mOutPortPresentations.end(); itout++)
	{
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*itout);
		if (out->geometry().contains(real))
			out->AcquireOutPortClicked.Emit(out);
	}
}	

void Qt_ProcessingPresentation::OnNewInPortClicked( Qt_InPortPresentation * inport)
{
	AcquireInPortClicked.Emit( inport );
}

void Qt_ProcessingPresentation::OnNewOutPortClicked( Qt_OutPortPresentation * outport)
{
	AcquireOutPortClicked.Emit( outport );
}


void Qt_ProcessingPresentation::OnNewInControlAfterClickOutControl( const QPoint & p)
{
	if (!geometry().contains(p))
		return;

	const QPoint real = mapFromParent(p);
	if (real.y() >= 5) // must be minor to be an inport
		return;

	InControlPresentationIterator itin;
	for ( itin=mInControlPresentations.begin(); itin!=mInControlPresentations.end(); itin++)
	{
		Qt_InControlPresentation * in = (Qt_InControlPresentation*)(*itin);
		if (in->geometry().contains(real))
			in->AcquireInControlClicked.Emit(in);
	}
}

void Qt_ProcessingPresentation::OnNewOutControlAfterClickInControl( const QPoint & p)
{
	if (!geometry().contains(p))
		return;
	const QPoint real = mapFromParent(p);
	if (real.y() <height()- 5) // must be major to be an outport
		return;
	
	OutControlPresentationIterator itout;
	for ( itout=mOutControlPresentations.begin(); itout!=mOutControlPresentations.end(); itout++)
	{
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*itout);
		if (out->geometry().contains(real))
			out->AcquireOutControlClicked.Emit(out);
	}
}	

void Qt_ProcessingPresentation::OnNewInControlClicked( Qt_InControlPresentation * incontrol)
{
	AcquireInControlClicked.Emit( incontrol );
}

void Qt_ProcessingPresentation::OnNewOutControlClicked( Qt_OutControlPresentation * outcontrol)
{
	AcquireOutControlClicked.Emit( outcontrol );
}




void Qt_ProcessingPresentation::OnNewObservedClassName(const std::string& name)
{
	mObservedClassName = name;
	QToolTip::add( this, QString( mObservedClassName.c_str() ));
}

void Qt_ProcessingPresentation::OnNewInPort( CLAMVM::InPortAdapter* adapter )
{
	Qt_InPortPresentation* presentation = 
		new Qt_InPortPresentation( mInPortPresentations.size(),this );

	presentation->AttachTo(*adapter);
	presentation->AcquireInPortClicked.Connect( SetInPortClicked );
	adapter->Publish();
	mInPortPresentations.push_back(presentation);
	int heightPorts = mInPortPresentations.size()*7+14;
	if (height() < heightPorts)
	{
		setFixedSize(width(),heightPorts);
		updateOutControlsPosition();
	}
}

void Qt_ProcessingPresentation::OnNewOutPort( CLAMVM::OutPortAdapter* adapter )
{
	Qt_OutPortPresentation* presentation = 
		new Qt_OutPortPresentation( mOutPortPresentations.size(), this );

	presentation->AttachTo(*adapter);
	presentation->AcquireOutPortClicked.Connect( SetOutPortClicked );
	adapter->Publish();
	mOutPortPresentations.push_back(presentation);
	int heightPorts = mOutPortPresentations.size()*7+14;
	if (height() < heightPorts)
	{
		setFixedSize(width(),heightPorts);
		updateOutControlsPosition();
	}
}



void Qt_ProcessingPresentation::OnNewInControl( CLAMVM::InControlAdapter* adapter )
{
	Qt_InControlPresentation* presentation = 
		new Qt_InControlPresentation( mInControlPresentations.size(),this );

	presentation->AttachTo(*adapter);
	presentation->AcquireInControlClicked.Connect( SetInControlClicked );
	adapter->Publish();
	mInControlPresentations.push_back(presentation);
	int widthControls = mInControlPresentations.size()*13+24;
	if (width() < widthControls)
	{
		setFixedSize(widthControls, height());
		updateOutPortsPosition();
	}
}

void Qt_ProcessingPresentation::OnNewOutControl( CLAMVM::OutControlAdapter* adapter )
{
	Qt_OutControlPresentation* presentation = 
		new Qt_OutControlPresentation( mOutControlPresentations.size(), this );

	presentation->AttachTo(*adapter);
	presentation->AcquireOutControlClicked.Connect( SetOutControlClicked );
	adapter->Publish();
	mOutControlPresentations.push_back(presentation);
	int widthControls = mOutControlPresentations.size()*13+24;
	if (width() < widthControls)
	{
		setFixedSize(widthControls, height());
		updateOutPortsPosition();
	}
}

void Qt_ProcessingPresentation::updateOutPortsPosition()
{
	OutPortPresentationIterator it;
	for(it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end(); it++)
	{
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*it);
		out->updatePosition();
	}
}

void Qt_ProcessingPresentation::updateOutControlsPosition()
{
	OutControlPresentationIterator it;
	for(it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end(); it++)
	{
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*it);
		out->updatePosition();
	}
}


void Qt_ProcessingPresentation::Show()
{

	InPortPresentationIterator itInPort;
	for ( itInPort=mInPortPresentations.begin(); itInPort!=mInPortPresentations.end(); itInPort++)
		(*itInPort)->Show();

	OutPortPresentationIterator itOutPort;
	for ( itOutPort=mOutPortPresentations.begin(); itOutPort!=mOutPortPresentations.end(); itOutPort++)
		(*itOutPort)->Show();

	InControlPresentationIterator itInControl;
	for ( itInControl=mInControlPresentations.begin(); itInControl!=mInControlPresentations.end(); itInControl++)
		(*itInControl)->Show();

	OutControlPresentationIterator itOutControl;
	for ( itOutControl=mOutControlPresentations.begin(); itOutControl!=mOutControlPresentations.end(); itOutControl++)
		(*itOutControl)->Show();
	show();

}

void Qt_ProcessingPresentation::Hide()
{
	InPortPresentationIterator itInPort;
	for ( itInPort=mInPortPresentations.begin(); itInPort!=mInPortPresentations.end(); itInPort++)
		(*itInPort)->Hide();

	OutPortPresentationIterator itOutPort;
	for ( itOutPort=mOutPortPresentations.begin(); itOutPort!=mOutPortPresentations.end(); itOutPort++)
		(*itOutPort)->Hide();

	InControlPresentationIterator itInControl;
	for ( itInControl=mInControlPresentations.begin(); itInControl!=mInControlPresentations.end(); itInControl++)
		(*itInControl)->Hide();

	OutControlPresentationIterator itOutControl;
	for ( itOutControl=mOutControlPresentations.begin(); itOutControl!=mOutControlPresentations.end(); itOutControl++)
		(*itOutControl)->Hide();

	hide();

	if(mConfig)
		mConfig->Hide();

}

void Qt_ProcessingPresentation::paintEvent( QPaintEvent * )
{

	QColor c(220, 220, 170);
	QPainter p( this );
        p.setBrush( c );


	QRegion reg(12,7,width()-24, height()-14);
	InPortPresentationIterator itInPort;
	for (itInPort=mInPortPresentations.begin(); itInPort!=mInPortPresentations.end();itInPort++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*itInPort);
		reg += in->geometry();
	}
	OutPortPresentationIterator itOutPort;
	for (itOutPort=mOutPortPresentations.begin(); itOutPort!=mOutPortPresentations.end();itOutPort++)
	{
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*itOutPort);
		reg += out->geometry();
	}
	InControlPresentationIterator itInControl;
	for (itInControl=mInControlPresentations.begin(); itInControl!=mInControlPresentations.end();itInControl++)
	{
		Qt_InControlPresentation * in = (Qt_InControlPresentation*)(*itInControl)
;		reg += in->GetRegion();
	}
	OutControlPresentationIterator itOutControl;
	for (itOutControl=mOutControlPresentations.begin(); itOutControl!=mOutControlPresentations.end();itOutControl++)
	{
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*itOutControl);
		reg += out->GetRegion();
	}

	setMask(reg);
	

	p.drawRect( 12,7, width()-24,height()-14); // draw a rectangle
	p.setPen( QPen( blue, 1 ));
	p.drawRect( 12,7, width()-24,height()-14); // draw a rectangle
	p.setPen( QPen( black,1 ));
	p.setFont( QFont( "Helvetica" ,8) );
	p.drawText( QRect(12,7,width()-24, height()-14),
		    Qt::AlignHCenter+Qt::AlignVCenter ,	
		    QString( mNameFromNetwork.c_str() ));
	adjustSize();
 
}

void Qt_ProcessingPresentation::mousePressEvent( QMouseEvent *m)
{
	if(m->button() == LeftButton )
	{
		mDown = true;
		mClickPos = m->pos();
	}
	else
	{
		mConfig->Show();
	}
	grabKeyboard();
}

void Qt_ProcessingPresentation::mouseReleaseEvent( QMouseEvent *m)
{
	mDown = false;
	releaseKeyboard();
}

void Qt_ProcessingPresentation::mouseMoveEvent( QMouseEvent *m)
{
	if(!mDown)
		return;

	QPoint difference(mapFromGlobal(m->globalPos()));
	difference = mapToParent(difference) - mClickPos;
	move(difference);
	
	// emit movement to update connections
	InPortPresentationIterator itInPort;
	for (itInPort=mInPortPresentations.begin(); itInPort!=mInPortPresentations.end();itInPort++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*itInPort);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->AcquirePos.Emit( difference.x()+ posX, difference.y()+posY);
	}
	OutPortPresentationIterator itOutPort;
	for (itOutPort=mOutPortPresentations.begin(); itOutPort!=mOutPortPresentations.end();itOutPort++)
	{	
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*itOutPort);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->AcquirePos.Emit( difference.x() + posX , difference.y()+ posY );
	}
	InControlPresentationIterator itInControl;
	for (itInControl=mInControlPresentations.begin(); itInControl!=mInControlPresentations.end();itInControl++)
	{
		Qt_InControlPresentation * in = (Qt_InControlPresentation*)(*itInControl);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->AcquirePos.Emit( difference.x()+ posX +5  , difference.y()+posY -2 );
	}
	OutControlPresentationIterator itOutControl;
	for (itOutControl=mOutControlPresentations.begin(); itOutControl!=mOutControlPresentations.end();itOutControl++)
	{	
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*itOutControl);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->AcquirePos.Emit( difference.x() + posX - 4, difference.y()+ posY +2 );
	}
	QWidget * parent = parentWidget();
	parent->repaint();
}

void Qt_ProcessingPresentation::EmitPositionOfChildren()
{		
	InPortPresentationIterator itInPort;
	for (itInPort=mInPortPresentations.begin(); itInPort!=mInPortPresentations.end();itInPort++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*itInPort);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->AcquirePos.Emit( pos().x() + posX, pos().y() + posY);
	}
	OutPortPresentationIterator itOutPort;
	for (itOutPort=mOutPortPresentations.begin(); itOutPort!=mOutPortPresentations.end();itOutPort++)
	{	
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*itOutPort);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->AcquirePos.Emit( pos().x() + posX , pos().y() + posY );
	}
	InControlPresentationIterator itInControl;
	for (itInControl=mInControlPresentations.begin(); itInControl!=mInControlPresentations.end();itInControl++)
	{
		Qt_InControlPresentation * in = (Qt_InControlPresentation*)(*itInControl);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->AcquirePos.Emit( pos().x() + posX + 5, pos().y() + posY - 2 );
	}
	OutControlPresentationIterator itOutControl;
	for (itOutControl=mOutControlPresentations.begin(); itOutControl!=mOutControlPresentations.end();itOutControl++)
	{	
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*itOutControl);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->AcquirePos.Emit( pos().x() + posX - 4 , pos().y() + posY +2);
	}
}

void Qt_ProcessingPresentation::keyPressEvent( QKeyEvent *k )
{
	switch ( tolower(k->ascii()) ) 
	{
        case 'x': 

		RemoveProcessing.Emit( this );
		Hide();
		mDown = false;
		releaseKeyboard();
		break;
	}
}


} // namespace NetworkGUI
