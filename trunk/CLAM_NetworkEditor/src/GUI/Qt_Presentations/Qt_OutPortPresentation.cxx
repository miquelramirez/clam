
#include "Qt_OutPortPresentation.hxx"
#include <qpainter.h>

#include <qtooltip.h>
namespace NetworkGUI
{

Qt_OutPortPresentation::Qt_OutPortPresentation( int id,  QWidget *parent, const char *name)
	: QWidget( parent, name ), mDown(false), mId(id)
{
	updatePosition();
	setFixedSize(10,5);
}

Qt_OutPortPresentation::~Qt_OutPortPresentation()
{
}

void Qt_OutPortPresentation::OnNewName(const std::string& name)
{
	mName = name;
	QToolTip::add( this, QString( mName.c_str() ));
}

void Qt_OutPortPresentation::Show()
{
	show();
}

void Qt_OutPortPresentation::Hide()
{
	hide();
}

void Qt_OutPortPresentation::paintEvent( QPaintEvent * )
{
	QColor c(40, 40, 200);
	QPainter p( this );
        p.setBrush( c );
	p.drawRoundRect( 0,0, width(),height()); // draw a rectangle
}

void Qt_OutPortPresentation::mousePressEvent( QMouseEvent *m)
{
	AcquireOutPortClicked.Emit(this);
}

void Qt_OutPortPresentation::updatePosition()
{
	move(parentWidget()->width()-10,mId*7+7);
}

} // namespace NetworkGUI

