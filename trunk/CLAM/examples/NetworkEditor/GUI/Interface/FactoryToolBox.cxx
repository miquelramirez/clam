
#include "FactoryToolBox.hxx"
#include "ProcessingLabel.hxx"
#include <qpainter.h>
#include <iostream>

namespace NetworkGUI
{

FactoryToolBox::FactoryToolBox(  QWidget * parent , const char * name )
	: QFrame( parent, name ),
	  mDown(false),
	  mNumProcessings(0),
	  mNormalSize(0,0),
	  mMinimized(false)
{	
	setFrameStyle( QFrame::Box | QFrame::Raised );
	setLineWidth( 1 );
	setPalette( QPalette( QColor( 100, 100, 100 )));
	AddProcessingLabel( "Oscillator" );
	AddProcessingLabel( "Mixer 2" );
	AddProcessingLabel( "Multiplier" );
}

FactoryToolBox::~FactoryToolBox()
{	
	std::vector< ProcessingLabel* >::iterator it;
	for (it=mProcessingLabels.begin(); it!=mProcessingLabels.end(); it++)
		delete *it;
}

void FactoryToolBox::AddProcessingLabel( const std::string & name)
{
	ProcessingLabel * proc = new ProcessingLabel( name , this);
	resize (140, 30+ (mProcessingLabels.size()+1)*30 );
	mNormalSize = size();
	proc->move(5, 25 + mProcessingLabels.size()*30 );
	proc->show();
	mProcessingLabels.push_back(proc);
}

void FactoryToolBox::mousePressEvent( QMouseEvent * m )
{
	if (m->button() == RightButton )
	{
		mDown = true;
		mClickPos = m->pos();
		grabMouse();
	}
	if (m->button() == MidButton )
	{
		if (mMinimized)
		{
			mMinimized = false;
			resize(mNormalSize);
		}
		else
		{
			mMinimized = true;
			resize(140, 25);
		}
	}
}

void FactoryToolBox::mouseReleaseEvent( QMouseEvent *)
{	
	releaseMouse();
	mDown = false;
}

void FactoryToolBox::mouseMoveEvent( QMouseEvent * m )
{	
	if(mDown)
	{
		QPoint difference(QWidget::mapFromGlobal(m->globalPos()));
		difference = mapToParent(difference) - mClickPos;
		move(difference);
		QWidget * parent = parentWidget();
		parent->repaint();
	}
}

void FactoryToolBox::IncrementNumProcessings()
{
	mNumProcessings++;
}

int FactoryToolBox::GetNumProcessings()
{
	return mNumProcessings;
}
void FactoryToolBox::paintEvent( QPaintEvent * )
{
	QColor c(255, 255, 255);
	QPainter p( this );
        p.setBrush( c );
	p.drawText( QRect(0,0,140,30),
		    Qt::AlignHCenter+Qt::AlignVCenter ,	
		    QString( "CLAM Processings" ));
	
}

} // namespace NetworkGUI
