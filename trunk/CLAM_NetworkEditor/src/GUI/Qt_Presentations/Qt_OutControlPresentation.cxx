
#include "Qt_OutControlPresentation.hxx"
#include <qpainter.h>

#include <qtooltip.h>
namespace NetworkGUI
{

Qt_OutControlPresentation::Qt_OutControlPresentation( int id,  QWidget *parent, const char *name)
	: QWidget( parent, name ), mDown(false), mId(id)
{
	updatePosition();
	setFixedSize(10,5);
}

Qt_OutControlPresentation::~Qt_OutControlPresentation()
{
}

void Qt_OutControlPresentation::OnNewName(const std::string& name)
{
	mName = name;
	QToolTip::add( this, QString( mName.c_str() ));
}

void Qt_OutControlPresentation::Show()
{
	show();
}

void Qt_OutControlPresentation::Hide()
{
	hide();
}

void Qt_OutControlPresentation::paintEvent( QPaintEvent * )
{
	QColor c(200, 0, 0);
	QPainter p( this );
        p.setBrush( c );

	QPointArray points;
	points.putPoints(0,3, 0,0  , width(), 0  , width()/2, height() );
	p.drawPolygon( points );
}

void Qt_OutControlPresentation::mousePressEvent( QMouseEvent *m)
{
	AcquireOutControlClicked.Emit(this);
}

void Qt_OutControlPresentation::updatePosition()
{
	move(mId*13+12,parentWidget()->height()-5);
}

} // namespace NetworkGUI

