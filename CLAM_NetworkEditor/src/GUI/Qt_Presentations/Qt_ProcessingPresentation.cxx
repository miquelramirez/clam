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
	SlotSetInPortClicked.Wrap( this, &Qt_ProcessingPresentation::SetInPortClicked);
	SlotSetOutPortClicked.Wrap( this, &Qt_ProcessingPresentation::SetOutPortClicked);
	SlotSetOutPortAfterClickInPort.Wrap(this, &Qt_ProcessingPresentation::SetOutPortAfterClickInPort);
	SlotSetInPortAfterClickOutPort.Wrap(this, &Qt_ProcessingPresentation::SetInPortAfterClickOutPort);

	// control slots
	SlotSetInControlClicked.Wrap( this, &Qt_ProcessingPresentation::SetInControlClicked);
	SlotSetOutControlClicked.Wrap( this, &Qt_ProcessingPresentation::SetOutControlClicked);
	SlotSetOutControlAfterClickInControl.Wrap(this, &Qt_ProcessingPresentation::SetOutControlAfterClickInControl);
	SlotSetInControlAfterClickOutControl.Wrap(this, &Qt_ProcessingPresentation::SetInControlAfterClickOutControl);
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
	QToolTip::add( this, QString( mObservedClassName.c_str() ));
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

void Qt_ProcessingPresentation::paintEvent( QPaintEvent * )
{

	QColor c(220, 220, 170);
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
	ConnectionPointPresentationsList::iterator it;
	for (it=mInPortPresentations.begin(); it!=mInPortPresentations.end();it++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*it);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->SignalAcquirePos.Emit( difference.x()+ posX, difference.y()+posY);
	}
	for (it=mOutPortPresentations.begin(); it!=mOutPortPresentations.end();it++)
	{	
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*it);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->SignalAcquirePos.Emit( difference.x() + posX , difference.y()+ posY );
	}
	for (it=mInControlPresentations.begin(); it!=mInControlPresentations.end();it++)
	{
		Qt_InControlPresentation * in = (Qt_InControlPresentation*)(*it);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->SignalAcquirePos.Emit( difference.x()+ posX +5  , difference.y()+posY -2 );
	}
	for (it=mOutControlPresentations.begin(); it!=mOutControlPresentations.end();it++)
	{	
		Qt_OutControlPresentation * out = (Qt_OutControlPresentation*)(*it);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->SignalAcquirePos.Emit( difference.x() + posX - 4, difference.y()+ posY +2 );
	}
	QWidget * parent = parentWidget();
	parent->repaint();
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

void Qt_ProcessingPresentation::keyPressEvent( QKeyEvent *k )
{
	switch ( tolower(k->ascii()) ) 
	{
        case 'x': 

		SignalRemoveProcessing.Emit( this );
		Hide();
		mDown = false;
		releaseKeyboard();
		break;
	}
}


} // namespace NetworkGUI
