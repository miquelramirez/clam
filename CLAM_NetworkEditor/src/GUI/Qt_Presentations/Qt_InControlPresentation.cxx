
#include "Qt_InControlPresentation.hxx"
#include <qpainter.h>

#include <qtooltip.h>

namespace NetworkGUI
{

Qt_InControlPresentation::Qt_InControlPresentation( int id,  QWidget *parent, const char *name)
	: QWidget( parent, name ),  mId(id)

{	
//	move(0,mId*7+3);
	move(mId*13+12, 0);
	setFixedSize(10,5);
}

Qt_InControlPresentation::~Qt_InControlPresentation()
{
}

void Qt_InControlPresentation::OnNewName(const std::string& name)
{
	mName = name;
	QToolTip::add( this, QString( mName.c_str() ));
}

void Qt_InControlPresentation::Show()
{
	show();
}

void Qt_InControlPresentation::Hide()
{
	hide();
}

void Qt_InControlPresentation::paintEvent( QPaintEvent * )
{
	QColor c(200, 0, 0);
	QPainter p( this );
        p.setBrush( c );

	QPointArray points;
//	points.putPoints(0,3, 0,height()  , width(), height()  , width()/2, 0 );
	points.putPoints(0,3, 0,0  , width(), 0  , width()/2, height() , 232);
	p.drawPolygon( points );
}

void Qt_InControlPresentation::mousePressEvent( QMouseEvent *m)
{
	AcquireInControlClicked.Emit(this);
}

} // namespace NetworkGUI

