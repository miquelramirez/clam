
#include "Qt_ProcessingPresentation.hxx"
#include "InPortAdapter.hxx"
#include "OutPortAdapter.hxx"
#include "Qt_InPortPresentation.hxx"
#include "Qt_OutPortPresentation.hxx"


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
	setFixedSize(pixelsWide + 30, pixelsHigh*2);

	// now we position the processings in the gui with a random function
	// we put them inside 3/4 of the network, in order to avoid processings in the borders
	QPoint position(rand()%(3*top->width()/4), rand()%(3*top->height()/4));
	move(position);
	SetInPortClicked.Wrap( this, &Qt_ProcessingPresentation::OnNewInPortClicked);
	SetOutPortClicked.Wrap( this, &Qt_ProcessingPresentation::OnNewOutPortClicked);
	SetOutPortAfterClickInPort.Wrap(this, &Qt_ProcessingPresentation::OnNewOutPortAfterClickInPort);
	SetInPortAfterClickOutPort.Wrap(this, &Qt_ProcessingPresentation::OnNewInPortAfterClickOutPort);
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
	int heightPorts = mInPortPresentations.size()*7+3;
	if (height() < heightPorts)
		setFixedSize(width(),heightPorts);
}

void Qt_ProcessingPresentation::OnNewOutPort( CLAMVM::OutPortAdapter* adapter )
{
	Qt_OutPortPresentation* presentation = 
		new Qt_OutPortPresentation( mOutPortPresentations.size(), this );

	presentation->AttachTo(*adapter);
	presentation->AcquireOutPortClicked.Connect( SetOutPortClicked );
	adapter->Publish();
	mOutPortPresentations.push_back(presentation);
	int heightPorts = mOutPortPresentations.size()*7+3;
	if (height() < heightPorts)
		setFixedSize(width(),heightPorts);
}

void Qt_ProcessingPresentation::Show()
{

	InPortPresentationIterator itin;
	for ( itin=mInPortPresentations.begin(); itin!=mInPortPresentations.end(); itin++)
		(*itin)->Show();

	OutPortPresentationIterator itout;
	for ( itout=mOutPortPresentations.begin(); itout!=mOutPortPresentations.end(); itout++)
		(*itout)->Show();

	show();
}

void Qt_ProcessingPresentation::Hide()
{
	InPortPresentationIterator itin;
	for ( itin=mInPortPresentations.begin(); itin!=mInPortPresentations.end(); itin++)
		(*itin)->Hide();

	OutPortPresentationIterator itout;
	for ( itout=mOutPortPresentations.begin(); itout!=mOutPortPresentations.end(); itout++)
		(*itout)->Hide();

	hide();
}

void Qt_ProcessingPresentation::paintEvent( QPaintEvent * )
{

	QColor c(220, 220, 170);
	QPainter p( this );
        p.setBrush( c );


	QRegion reg(12,0,width()-24, height());
	InPortPresentationIterator itin;
	for (itin=mInPortPresentations.begin(); itin!=mInPortPresentations.end();itin++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*itin);
		reg += in->geometry();
	}
	OutPortPresentationIterator itout;
	for (itout=mOutPortPresentations.begin(); itout!=mOutPortPresentations.end();itout++)
	{
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*itout);
		reg += out->geometry();
	}
	setMask(reg);
	

	p.drawRect( 12,0, width()-24,height()); // draw a rectangle
	p.setPen( QPen( blue, 1 ));
	p.drawRect( 12,0, width()-24,height()); // draw a rectangle
	p.setPen( QPen( black,1 ));
	p.setFont( QFont( "Helvetica" ,8) );
	p.drawText( QRect(12,0,width()-24, height()),
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
		EditConfiguration.Emit( mConfig );
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
	InPortPresentationIterator itin;
	for (itin=mInPortPresentations.begin(); itin!=mInPortPresentations.end();itin++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*itin);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->AcquirePos.Emit( difference.x()+ posX, difference.y()+posY);
	}
	OutPortPresentationIterator itout;
	for (itout=mOutPortPresentations.begin(); itout!=mOutPortPresentations.end();itout++)
	{	
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*itout);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->AcquirePos.Emit( difference.x() + posX , difference.y()+ posY );
	}
	QWidget * parent = parentWidget();
	parent->repaint();
}

void Qt_ProcessingPresentation::EmitPositionOfPorts()
{		
	InPortPresentationIterator itin;
	for (itin=mInPortPresentations.begin(); itin!=mInPortPresentations.end();itin++)
	{
		Qt_InPortPresentation * in = (Qt_InPortPresentation*)(*itin);
		int posX = in->pos().x();
		int posY = in->pos().y() + in->height()/2;
		in->AcquirePos.Emit( pos().x() + posX, pos().y() + posY);
	}
	OutPortPresentationIterator itout;
	for (itout=mOutPortPresentations.begin(); itout!=mOutPortPresentations.end();itout++)
	{	
		Qt_OutPortPresentation * out = (Qt_OutPortPresentation*)(*itout);
		int posX = out->pos().x() + 10;
		int posY = out->pos().y() + out->height()/2;
		out->AcquirePos.Emit( pos().x() + posX , pos().y() + posY );
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
