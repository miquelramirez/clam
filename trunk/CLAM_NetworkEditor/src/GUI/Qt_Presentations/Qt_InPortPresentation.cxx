
#include "Qt_InPortPresentation.hxx"
#include <qpainter.h>

#include <qtooltip.h>

namespace NetworkGUI
{

Qt_InPortPresentation::Qt_InPortPresentation( int id,  QWidget *parent, const char *name)
	: QWidget( parent, name ),  mId(id)

{	
	move(0,mId*7+3);
	setFixedSize(10,5);
}

Qt_InPortPresentation::~Qt_InPortPresentation()
{
}

void Qt_InPortPresentation::OnNewName(const std::string& name)
{
	mName = name;
	QToolTip::add( this, QString( mName.c_str() ));
}

void Qt_InPortPresentation::Show()
{
	show();
}

void Qt_InPortPresentation::Hide()
{
	hide();
}

void Qt_InPortPresentation::paintEvent( QPaintEvent * )
{
	QColor c(40, 40, 200);
	QPainter p( this );
        p.setBrush( c );
	p.drawRoundRect( 0,0, width(),height()); // draw a rectangle
}

void Qt_InPortPresentation::mousePressEvent( QMouseEvent *m)
{
	AcquireInPortClicked.Emit(this);
}

} // namespace NetworkGUI

