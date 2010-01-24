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

#include <vector>

#include "FloatArrayDataSource.hxx"
#include <CLAM/MIDIMessage.hxx>
#include <CLAM/Processing.hxx>
#include <CLAM/InControlBase.hxx>
#include <CLAM/ControlPiano.hxx>

typedef unsigned int TSize;

namespace CLAM {

/**	\brief MIDI piano widget
*
*	Can be used as MIDI note generator and MIDI monitor.
*/
class MIDIPianoWidget: public QWidget
{	
	Q_OBJECT

public:
	MIDIPianoWidget(Processing *processing, QWidget * parent=NULL)
		: QWidget(parent)
		, _processing(processing)
	{
		_background = new QSvgRenderer(QString(":/icons/images/piano.svg"), this);
		_controlPiano = dynamic_cast< ControlPiano* >(_processing);

		noDataSource();

		startTimer(50);
	}

	~MIDIPianoWidget();

	void setDataSource( VM::FloatArrayDataSource & dataSource );
	void noDataSource();

	void setClickEnabled(bool clickEnabled) { _clickEnabled = clickEnabled; }

protected:
	unsigned _nBins;
	VM::FloatArrayDataSource * _dataSource;
	const TData * _data;

	bool _clickEnabled; ///< the option to enable/disable the mouse events / keys pressed. Chordata is the use case for this.

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
		eGSharpNote = 11
	};

	QSvgRenderer * _background;
	QImage buffer;

	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void timerEvent(QTimerEvent *event) { update(); }

	void pressPixmapMainKey(QPainter &painter, TSize keyNumber);
	void pressPixmapSharpKey(QPainter &painter, TSize keyNumber);

	unsigned identifyMidiByPosition(TSize x, TSize y);
	void processData();

private:
	void noteOn(unsigned note);
	void noteOff(unsigned note);
private:
	Processing *_processing;
	ControlPiano *_controlPiano;
};

} //namespace CLAM

#endif // _MIDIPianoWidget_

