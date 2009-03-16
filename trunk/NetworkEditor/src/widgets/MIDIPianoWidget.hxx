/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _MIDIPianoWidget_
#define _MIDIPianoWidget_

#include <QtGui/QWidget>
#include <QtGui/QPainter>

#include <QtSvg/QSvgRenderer>

#include <CLAM/TypedInControl.hxx>
#include <CLAM/MIDIMessage.hxx>

#include <CLAM/Processing.hxx>
#include <vector>

#include <CLAM/InControlBase.hxx>
#include <CLAM/ControlPiano.hxx>


/**	\brief MIDI piano widget
*
*	Can be used as MIDI note generator and MIDI monitor.
*/
class MIDIPianoWidget: public QWidget
{	
	Q_OBJECT
public:
	MIDIPianoWidget(CLAM::Processing *processing)
		: 
		_processing(processing)
	{
		_background = new QSvgRenderer(QString(":/icons/images/piano.svg"), this);
		_controlPiano = dynamic_cast< CLAM::ControlPiano* >(_processing);

		startTimer(50);
	}

	~MIDIPianoWidget() {}

protected:
	enum TemperedNotes {
		eANote = 0,
		eASharpNote = 1,
		eBNote = 2,
		eCNote = 3,
		eCSharpNote = 4,
		eDNote = 5,
		eDSharpNote = 6,
		eENote = 7,
		eFNote = 8,
		eFSharpNote = 9,
		eGNote = 10,
		eGSharpNote = 11,
	};

	QSvgRenderer * _background;
	QImage buffer;

	void paintEvent(QPaintEvent *event);
	void timerEvent(QTimerEvent *event) { update(); }

	void pressPixmapMainKey(QPainter &painter, unsigned int keyNumber);
	void pressPixmapSharpKey(QPainter &painter, unsigned int keyNumber);

private:
	CLAM::Processing *_processing;
	CLAM::ControlPiano *_controlPiano;
};

#endif // _MIDIPianoWidget_
