
#include "Qt_OutControlPresentation.hxx"
#include <qpainter.h>
#include <qbitmap.h>
#include <qtooltip.h>
namespace NetworkGUI
{

Qt_OutControlPresentation::Qt_OutControlPresentation( int id,  QWidget *parent, const char *name)
	: QWidget( parent, name ), mId(id)
{
	updatePosition();
	setFixedSize(10,5);


	QPointArray points;
	points.putPoints(0,3, 0,0  , width(), 0  , width()/2, height() );

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
	QPainter p( this );
	p.setPen(QColor(0,0,0));
	p.setBrush(QColor(200,0,0));

	QPointArray points;
	points.putPoints(0,3, 0,0  , width(), 0  , width()/2, height() );
	p.drawPolygon( points );
}

void Qt_OutControlPresentation::mousePressEvent( QMouseEvent *m)
{
	AcquireOutControlClicked.Emit(this);
}


QRegion Qt_OutControlPresentation::GetRegion()
{
	QRegion reg(mReg);
	reg.translate(pos().x(), pos().y());
	return reg;
}

void Qt_OutControlPresentation::updatePosition()
{
	move(mId*13+12,parentWidget()->height()-5);
}


} // namespace NetworkGUI

