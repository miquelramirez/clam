// QSynthKnob.cxx
//
/****************************************************************************
   Copyright (C) 2005-2006, rncbc aka Rui Nuno Capela. All rights reserved.

   This widget is based on a design by Thorsten Wilms, 
   implemented by Chris Cannam in Rosegarden,
   adapted for QSynth by Pedro Lopez-Cabanillas

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#include <cmath>
#include <QtCore/QTimer>
#include <QtGui/QToolTip>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtGui/QWheelEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QColormap>
#include <QtGui/QRadialGradient>

#include "QSynthKnob.hxx"


struct QSynthKnobCacheIndex
{
	// Constructor.
	QSynthKnobCacheIndex(int s = 0, int kc = 0, int mc = 0,
		int a = 0, int n = 0, int c = 0) :
			size(s), knobColor(kc), meterColor(mc),
			angle(a), numTicks(n), centered(c) {}

	bool operator<(const QSynthKnobCacheIndex &i) const {
		// woo!
		if (size < i.size) return true;
		else if (size > i.size) return false;
		else if (knobColor < i.knobColor) return true;
		else if (knobColor > i.knobColor) return false;
		else if (meterColor < i.meterColor) return true;
		else if (meterColor > i.meterColor) return false;
		else if (angle < i.angle) return true;
		else if (angle > i.angle) return false;
		else if (numTicks < i.numTicks) return true;
		else if (numTicks > i.numTicks) return false;
		else if (centered == i.centered) return false;
		else if (!centered) return true;
		return false;
	}

	int          size;
	unsigned int knobColor;
	unsigned int meterColor;
	int          angle;
	int          numTicks;
	bool         centered;
};

typedef QMap<QSynthKnobCacheIndex, QPixmap> QSynthKnobPixmapCache;


//-------------------------------------------------------------------------
// QSynthKnob - Instance knob widget class.
//

#define QSYNTHKNOB_MIN      (0.25 * M_PI)
#define QSYNTHKNOB_MAX      (1.75 * M_PI)
#define QSYNTHKNOB_RANGE    (QSYNTHKNOB_MAX - QSYNTHKNOB_MIN)


// Constructor.
QSynthKnob::QSynthKnob ( QWidget *pParent)
	: QDial(pParent),
	m_knobColor(Qt::black), m_meterColor(Qt::white),
	m_bMouseDial(false), m_bMousePressed(false),
	m_iDefaultValue(-1)
{
}


// Destructor.
QSynthKnob::~QSynthKnob (void)
{
}


void QSynthKnob::paintEvent ( QPaintEvent * event )
{
	static QSynthKnobPixmapCache s_pixmaps;

	double angle = QSYNTHKNOB_MIN // offset
		+ (QSYNTHKNOB_RANGE *
			(double(value() - minimum()) /
			(double(maximum() - minimum()))));
	int degrees = int(angle * 180.0 / M_PI);

	int side = width() < height() ? width() : height();
	int xcenter = width()/2;
	int ycenter = height()/2;
	int notchWidth = 2+side/400;
	int pointerWidth = 2+side/30;
	int scaleShadowWidth = 1+side/100;
	int knobBorderWidth = 4+side/400;

	int ns = notchSize();
	int numTicks = 1 + (maximum() + ns - minimum()) / ns;
	int indent = side * 0.15 + 2;
	int knobWidth = side-2*indent;
	int darkWidth = knobWidth * 3 / 4;
	int meterWidth = side-2*scaleShadowWidth;
	
	QColor knobColor(m_knobColor);
	if (knobColor == Qt::black)
		knobColor =  palette().mid().color();

	QColor meterColor(m_meterColor);
	if (!isEnabled())
		meterColor = palette().mid().color();
	else if (m_meterColor == Qt::white)
		meterColor = palette().highlight().color();
	QColor background = palette().window().color();

	QPainter paint(this);
	paint.setRenderHint(QPainter::Antialiasing, true);

	// The bright metering bit...

	QConicalGradient meterShadow(xcenter,ycenter,-90);
	meterShadow.setColorAt(0,meterColor.dark().dark());
	meterShadow.setColorAt(0.5,meterColor);
	meterShadow.setColorAt(1,meterColor.light().light());
	paint.setBrush(meterShadow);
	paint.setPen(Qt::transparent);
	paint.drawPie(xcenter-meterWidth/2, ycenter-meterWidth/2,
		meterWidth, meterWidth, (180 + 45) * 16, -(degrees - 45) * 16);

	// Knob body and face...

	QPen pen;
	pen.setColor(knobColor);
	pen.setWidth(knobBorderWidth);
	paint.setPen(pen);

	QRadialGradient gradient(xcenter,ycenter,side/2,xcenter-darkWidth/3,ycenter-darkWidth/3);
	gradient.setColorAt(0,knobColor.light().light());
	gradient.setColorAt(1,knobColor.dark());
	QBrush knobBrush(gradient);
	paint.setBrush(knobBrush);
	paint.drawEllipse(xcenter-knobWidth/2, ycenter-knobWidth/2, knobWidth, knobWidth);

	// Tick notches...

	paint.setBrush(Qt::NoBrush);

	if ( notchesVisible() ) {
		pen.setColor(palette().dark().color());
		pen.setWidth(notchWidth);
		paint.setPen(pen);
		double hyp = double(side-scaleShadowWidth) / 2.0;
		double len = hyp / 4;
		for (int i = 0; i < numTicks; ++i) {
			int div = numTicks;
			if (div > 1) --div;
			bool internal = i != 0 && i != numTicks-1;
			double angle = QSYNTHKNOB_MIN + (QSYNTHKNOB_MAX - QSYNTHKNOB_MIN) * i / div;
			double dir = internal? -1 : len;
			double sinAngle = sin(angle);
			double cosAngle = cos(angle);
			double x0 = xcenter - (hyp - len) * sinAngle;
			double y0 = ycenter + (hyp - len) * cosAngle;
			double x1 = xcenter - (hyp + dir) * sinAngle;
			double y1 = ycenter + (hyp + dir) * cosAngle;
			paint.drawLine(QLineF(x0,y0,x1,y1));
		}
	}

	// Shadowing...

	// Knob shadow...
	QLinearGradient inShadow(xcenter-side/4,ycenter-side/4,xcenter+side/4,ycenter+side/4);
	inShadow.setColorAt(0,knobColor.light().light());
	inShadow.setColorAt(1,knobColor.dark());
	paint.setPen(QPen(QBrush(inShadow),knobBorderWidth*3/4));
	paint.drawEllipse(xcenter-side/2+indent, ycenter-side/2+indent,
		side-2*indent, side-2*indent);

	// Scale shadow...
	QLinearGradient outShadow(xcenter-side/4,ycenter-side/4,xcenter+side/4,ycenter+side/4);
	outShadow.setColorAt(0,background.dark());
	outShadow.setColorAt(1,background.light().light());
	paint.setPen(QPen(QBrush(outShadow),scaleShadowWidth));
	paint.drawArc(xcenter-side/2+scaleShadowWidth/2, ycenter-side/2+scaleShadowWidth/2,
		side-scaleShadowWidth, side-scaleShadowWidth, -45 * 16, 270 * 16);

	// Pointer notch...

	double hyp = double(side) / 2.0;
	double len = hyp - indent -1;

	double x = xcenter - len * sin(angle);
	double y = ycenter + len * cos(angle);

	QColor c = palette().dark().color();
	pen.setColor(isEnabled() ? c.dark(130) : c);
	pen.setWidth(pointerWidth);
	paint.setPen(pen);
	paint.drawLine(QLineF(xcenter, ycenter, x, y));
}


void QSynthKnob::drawTick ( QPainter& paint,
	double angle, int size, bool internal )
{
	double xcenter = width()/2;
	double ycenter = height()/2;
	double hyp = double(size) / 2.0;
	double len = hyp / 4;
	double dir = internal? -1 : len;
	double sinAngle = sin(angle);
	double cosAngle = cos(angle);
	double x0 = xcenter - (hyp - len) * sinAngle;
	double y0 = ycenter + (hyp - len) * cosAngle;
	double x1 = xcenter - (hyp + dir) * sinAngle;
	double y1 = ycenter + (hyp + dir) * cosAngle;
	paint.drawLine(QPointF(x0,y0), QPointF(x1,y1));
}


void QSynthKnob::setKnobColor ( const QColor& color )
{
	m_knobColor = color;
	repaint();
}


void QSynthKnob::setMeterColor ( const QColor& color )
{
	m_meterColor = color;
	repaint();
}


void QSynthKnob::setMouseDial ( bool bMouseDial )
{
	m_bMouseDial = bMouseDial;
}


void QSynthKnob::setDefaultValue ( int iDefaultValue )
{
	m_iDefaultValue = iDefaultValue;
}


// Mouse angle determination.
double QSynthKnob::mouseAngle ( const QPoint& pos )
{
	int dx = pos.x() - (QDial::width() / 2);
	int dy = (QDial::height() / 2) - pos.y();
	return (dx ? atan((double) dy / (double) dx) : 0.0);
}


// Alternate mouse behavior event handlers.
void QSynthKnob::mousePressEvent ( QMouseEvent *pMouseEvent )
{
	if (m_bMouseDial) {
		QDial::mousePressEvent(pMouseEvent);
	} else if (pMouseEvent->button() == Qt::LeftButton) {
		m_bMousePressed = true;
		m_posMouse = pMouseEvent->pos();
		emit sliderPressed();
	} else if (pMouseEvent->button() == Qt::MidButton) {
		// Reset to default value...
		if (m_iDefaultValue < minimum() || m_iDefaultValue > maximum())
			m_iDefaultValue = (maximum() + minimum()) / 2;
		setValue(m_iDefaultValue);
	}
}


void QSynthKnob::mouseMoveEvent ( QMouseEvent *pMouseEvent )
{
	if (m_bMouseDial) {
		QDial::mouseMoveEvent(pMouseEvent);
	} else if (m_bMousePressed) {
		// Dragging by x or y axis when clicked modifies value.
		const QPoint& posMouse = pMouseEvent->pos();
		int iValue = value()
			+ (mouseAngle(m_posMouse) < mouseAngle(posMouse) ? -1 : +1)
			* singleStep();
		m_posMouse = posMouse;
		if (iValue > maximum())
			iValue = maximum();
		else
		if (iValue < minimum())
			iValue = minimum();
		setValue(iValue);
		emit sliderMoved(value());
	}
}

void QSynthKnob::mouseReleaseEvent ( QMouseEvent *pMouseEvent )
{
	if (m_bMouseDial) {
		QDial::mouseReleaseEvent(pMouseEvent);
	} else if (m_bMousePressed) {
		m_bMousePressed = false;
	}
}


void QSynthKnob::wheelEvent ( QWheelEvent *pWheelEvent )
{
	if (m_bMouseDial) {
		QDial::wheelEvent(pWheelEvent);
	} else {
		int iValue = value();
		if (pWheelEvent->delta() > 0)
			iValue -= pageStep();
		else
			iValue += pageStep();
		if (iValue > maximum())
			iValue = maximum();
		else
		if (iValue < minimum())
			iValue = minimum();
		setValue(iValue);
	}
}

/*
void QSynthKnob::sliderChange (SliderChange change)
{
	repaintScreen();
	emit valueChanged(value());
#if defined(QT_ACCESSIBILITY_SUPPORT)
	QAccessible::updateAccessibility(this, 0, QAccessible::ValueChanged);
#endif
}
*/

// end of QSynthKnob.cpp
