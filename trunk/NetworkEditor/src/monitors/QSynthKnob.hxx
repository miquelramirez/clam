// QSynthKnob.h
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

#ifndef __QSynthKnob_h
#define __QSynthKnob_h

#include <QtGui/QDial>
#include <QtCore/QMap>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>


//-------------------------------------------------------------------------
// QSynthKnob - A better QDial for QSynth.

class QSynthKnob : public QDial
{
	Q_OBJECT
	Q_PROPERTY( QColor knobColor READ getKnobColor WRITE setKnobColor )
	Q_PROPERTY( QColor meterColor READ getMeterColor WRITE setMeterColor )
	Q_PROPERTY( QColor pointerColor READ getPointerColor WRITE setPointerColor )
	Q_PROPERTY( QColor borderColor READ getBorderColor WRITE setBorderColor )
	Q_PROPERTY( int defaultValue READ getDefaultValue WRITE setDefaultValue )
	Q_PROPERTY( KnobMode knobMode READ knobMode WRITE setKnobMode )
	Q_ENUMS(KnobMode)
public:
	enum KnobMode
	{
		QDialMode, ///< Old QDial behaviour
		AngularMode, ///< Knob moves angularly as the mouse around the widget center
		LinearMode ///< Knob moves proportonally to drag distance in one ortogonal axis
	};

	// Constructor.
	QSynthKnob(QWidget *pParent = 0);
	// Destructor.
	~QSynthKnob();

	const QColor& getKnobColor()  const { return m_knobColor;  }
	const QColor& getMeterColor() const { return m_meterColor; }
	const QColor& getPointerColor() const { return m_pointerColor; }
	const QColor& getBorderColor() const { return m_borderColor; }

	int getDefaultValue() const { return m_iDefaultValue; }

public slots:

	// Set the colour of the knob
	void setKnobColor(const QColor& color);

	// Set the colour of the meter
	void setMeterColor(const QColor& color);

	// Set the colour of the pointer
	void setPointerColor(const QColor& color);

	// Set the colour of the border
	void setBorderColor(const QColor& color);

	// Set default (mid) value.
	void setDefaultValue(int iDefaultValue);

	void setKnobMode(KnobMode mode) { m_knobMode=mode; }
	KnobMode knobMode() const { return m_knobMode; }

protected:

	void drawTick(QPainter& paint, double angle, int size, bool internal);
	virtual void paintEvent(QPaintEvent * event);

	// Mouse angle determination.
	double mouseAngle(const QPoint& pos);

	// Alternate mouse behavior event handlers.
	virtual void mousePressEvent(QMouseEvent *pMouseEvent);
	virtual void mouseMoveEvent(QMouseEvent *pMouseEvent);
	virtual void mouseReleaseEvent(QMouseEvent *pMouseEvent);
	virtual void wheelEvent(QWheelEvent *pWheelEvent);
//	virtual void sliderChange(SliderChange change);

private:

	QColor m_knobColor;
	QColor m_meterColor;
	QColor m_pointerColor;
	QColor m_borderColor;

	// Alternate mouse behavior tracking.
	bool   m_bMousePressed;
	QPoint m_posMouse;
	KnobMode m_knobMode;

	// Default (mid) value.
	int m_iDefaultValue;
	// just for more precission on the movement
	double m_lastDragValue;
};

typedef QSynthKnob QSynthKnob;

#endif  // __QSynthKnob_h

// end of QSynthKnob.h

