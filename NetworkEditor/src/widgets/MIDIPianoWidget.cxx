/*
 *
 *
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

#include "MIDIPianoWidget.hxx"
#include <CLAM/ProcessingFactory.hxx>
#include <CLAM/CLAM_Math.hxx>

#include <QtGui/QMouseEvent>

void MIDIPianoWidget::paintEvent(QPaintEvent *event)
{
	if ( buffer.size() != size() ) {
		buffer = QImage(size(), QImage::Format_ARGB32_Premultiplied);
		QPainter p(&buffer);
		p.setViewport(0, 0, width(), height());
		p.eraseRect(0, 0, width(), height());
		_background->render(&p);
	}
	QPainter painter(this);
	painter.drawImage(0, 0, buffer);

	//pressed keys
	for(TSize midiNote=0;midiNote<12;midiNote++) {
		if (_controlPiano->GetNoteStatus(midiNote)) {
			switch(	midiNote ) {
			case eANote:
			case eBNote:
			case eCNote:
			case eDNote:
			case eENote:
			case eFNote:
			case eGNote:
				pressPixmapMainKey(painter, midiNote); break;
			case eASharpNote:
			case eCSharpNote:
			case eDSharpNote:
			case eFSharpNote:
			case eGSharpNote:
				pressPixmapSharpKey(painter, midiNote); break;
			}
		}
	}

	event->accept();
}

void MIDIPianoWidget::pressPixmapMainKey(QPainter &painter, TSize keyNumber)
{
	TSize PIXMAP_KEY_WIDTH = width()/7.;
	TSize STEP = PIXMAP_KEY_WIDTH/52.;

	painter.setPen(QColor("black")); //edge
	painter.setBrush(palette().highlight()); //fill

	TSize xpos = 0.;
	switch( keyNumber )
	{
	case eCNote: //C
		xpos = 0.; break;
	case eDNote:
		xpos = STEP + PIXMAP_KEY_WIDTH; break;
	case eENote:
		xpos = 2.*STEP + PIXMAP_KEY_WIDTH*2.; break;
	case eFNote:
		xpos = 3.*STEP + PIXMAP_KEY_WIDTH*3.; break;
	case eGNote:
		xpos = 3.*STEP + PIXMAP_KEY_WIDTH*4.; break;
	case eANote:
		xpos = 3.*STEP + PIXMAP_KEY_WIDTH*5.; break;
	case eBNote:
		xpos = 3.*STEP + PIXMAP_KEY_WIDTH*6.; break;
	default: return; //keyNumber error
	}
	painter.drawRect(PIXMAP_KEY_WIDTH/4.+xpos, height()*3./4., PIXMAP_KEY_WIDTH/2., PIXMAP_KEY_WIDTH/2.);
}

void MIDIPianoWidget::pressPixmapSharpKey(QPainter &painter, TSize keyNumber)
{
	TSize PIXMAP_KEY_WIDTH = width()/7.;
	TSize STEP = PIXMAP_KEY_WIDTH/52.;
	TSize PIXMAP_SHARP_KEY_WIDTH = PIXMAP_KEY_WIDTH*.7;

	painter.setPen(QColor("black")); //edge
	painter.setBrush(palette().highlight()); //fill

	TSize xpos = 0.;
	switch( keyNumber )
	{
	case eCSharpNote:
		xpos = PIXMAP_KEY_WIDTH*3./4.; break;
	case eDSharpNote:
		xpos = 2.*STEP+PIXMAP_KEY_WIDTH*7./4.; break;
	case eFSharpNote:
		xpos = 3.*STEP+PIXMAP_KEY_WIDTH*15./4.; break;
	case eGSharpNote:
		xpos = 4.*STEP+PIXMAP_KEY_WIDTH*19./4.; break;
	case eASharpNote:
		xpos = 5.*STEP+PIXMAP_KEY_WIDTH*23./4.; break;
	default: return; //keyNumber error
	}
	painter.drawRect(xpos, height()/2.2, PIXMAP_KEY_WIDTH/2., PIXMAP_KEY_WIDTH/2.);
}

void MIDIPianoWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		TSize x=event->x(), y=event->y();

		TSize note = identifyMidiByPosition(x,y);
		_controlPiano->SetNoteStatus(note%12,true);
		note += 21 + (_controlPiano->GetOctave()-1)*12;

		MIDI::Message tmpMessage(144, note, _controlPiano->GetVelocity(), 0); //144 NoteOff, note, velocity
		_controlPiano->SendMIDIMessage(tmpMessage);
		update();
		event->accept();
	}
}

void MIDIPianoWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		TSize x=event->x(), y=event->y();

//FIXME: there is a bug if you press in one note, move the mouse and release the click in a different (note) one

		TSize note = identifyMidiByPosition(x,y);
		_controlPiano->SetNoteStatus(note%12,false);
		note += 21 + (_controlPiano->GetOctave()-1)*12;
		MIDI::Message tmpMessage(128, note, _controlPiano->GetVelocity(), 0); //128 NoteOff, note, velocity
		_controlPiano->SendMIDIMessage(tmpMessage);
		update();
		event->accept();
	}
}

TSize MIDIPianoWidget::identifyMidiByPosition(TSize x, TSize y)
{
	TSize SHARP_BREAK = height()*.5;
	TSize PIXMAP_KEY_WIDTH = width()/7.;
	TSize note = 0;
	const TSize octaveConst = 12;

	if (y>SHARP_BREAK) {
		//below: only main keys
		switch ((int)round(float(x)/PIXMAP_KEY_WIDTH))
		{
		case 0:
			note = eCNote; break;
		case 1:
			note = eDNote; break;
		case 2:
			note = eENote; break;
		case 3:
			note = eFNote; break;
		case 4:
			note = eGNote; break;
		case 5:
			note = eANote+octaveConst; break;
		case 6:
			note = eBNote+octaveConst; break;
		}
		return note;
	}
	else {
		//above: main and sharp keys
		TSize SHARP_WIDTH = PIXMAP_KEY_WIDTH/2.;

		if (x<PIXMAP_KEY_WIDTH*3./4.)
		{
			note = eCNote;
		}
		else if (x>PIXMAP_KEY_WIDTH*3./4. && x<PIXMAP_KEY_WIDTH*3./4.+SHARP_WIDTH)
		{
			note = eCSharpNote;
		}
		else if (x>PIXMAP_KEY_WIDTH*(1.+1./4.) && x<PIXMAP_KEY_WIDTH*(1.+3./4.))
		{
			note = eDNote;
		}
		else if (x>PIXMAP_KEY_WIDTH*(1.+3./4.) && x<PIXMAP_KEY_WIDTH*(1.+3./4.)+SHARP_WIDTH)
		{
			note = eDSharpNote;
		}
		else if (x>PIXMAP_KEY_WIDTH*(2.+1./4.) && x<PIXMAP_KEY_WIDTH*3.)
		{
			note = eENote;
		}
		else if (x>PIXMAP_KEY_WIDTH*3. && x<PIXMAP_KEY_WIDTH*(3.+3./4.))
		{
			note = eFNote;
		}
		else if (x>PIXMAP_KEY_WIDTH*(3.+3./4.) && x<PIXMAP_KEY_WIDTH*(3.+3./4.)+SHARP_WIDTH)
		{
			note = eFSharpNote;
		}
		else if (x>PIXMAP_KEY_WIDTH*(4.+1./4.) && x<PIXMAP_KEY_WIDTH*(4.+3./4.))
		{
			note = eGNote;
		}
		else if (x>PIXMAP_KEY_WIDTH*(4.+3./4.) && x<PIXMAP_KEY_WIDTH*(4.+3./4.)+SHARP_WIDTH)
		{
			note = eGSharpNote;
		}
		else if (x>PIXMAP_KEY_WIDTH*(5.+1./4.) && x<PIXMAP_KEY_WIDTH*(5.+3./4.))
		{
			note = eANote+octaveConst;
		}
		else if (x>PIXMAP_KEY_WIDTH*(5.+3./4.) && x<PIXMAP_KEY_WIDTH*(5.+3./4.)+SHARP_WIDTH)
		{
			note = eASharpNote+octaveConst;
		}
		else if (x>PIXMAP_KEY_WIDTH*(6.+1./4.) && x<PIXMAP_KEY_WIDTH*7.)
		{
			note = eBNote+octaveConst;
		}
		return note;
	}
}
