
#include "Qt_InControlPresentation.hxx"
#include <qpainter.h>
#include <qbitmap.h>
#include <qtooltip.h>

namespace NetworkGUI
{

Qt_InControlPresentation::Qt_InControlPresentation( int id,  QWidget *parent, const char *name)
	: QWidget( parent, name ),  mId(id)

{	
	move(mId*13+12, 0);
	setFixedSize(10,5);

	QPointArray points;
	points.putPoints(0,3, 0,0  , width(), 0  , width()/2, height() , 232);

	QBitmap bm( size() );
	bm.fill( color0 );			//transparent

	QPainter paint;

	paint.begin( &bm, this );
	paint.setPen( QPen(color1, 1));
	paint.setBrush( QBrush(color1));
	paint.drawPolygon(points);

	QRegion reg(bm);
	mReg += reg;
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
	QPainter p( this );
	p.setPen(QColor(0,0,0));
	p.setBrush(QColor(200,0,0));

	QPointArray points;
	points.putPoints(0,3, 0,0  , width(), 0  , width()/2, height() , 232);
	p.drawPolygon( points );

}

void Qt_InControlPresentation::mousePressEvent( QMouseEvent *m)
{
	AcquireInControlClicked.Emit(this);
}

QRegion Qt_InControlPresentation::GetRegion()
{
	QRegion reg(mReg);
	reg.translate(pos().x(), pos().y());
	return reg;

}


} // namespace NetworkGUI

