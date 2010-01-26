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
#include "EmbededWidgets.hxx"


namespace CLAM {

static EmbededWidgetCreator <MIDIPianoWidget> reg("ControlPiano");

MIDIPianoWidget::~MIDIPianoWidget()
{
}

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

	if (_dataSource) processData();

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
	TSize whiteKeyWidth = width()/7.;
	TSize STEP = whiteKeyWidth/52.;

	painter.setPen(QColor("black")); //edge
	painter.setBrush(palette().highlight()); //fill

	TSize xpos = 0.;
	switch( keyNumber )
	{
	case eCNote: //C
		xpos = 0.; break;
	case eDNote:
		xpos = STEP + whiteKeyWidth; break;
	case eENote:
		xpos = 2.*STEP + whiteKeyWidth*2.; break;
	case eFNote:
		xpos = 3.*STEP + whiteKeyWidth*3.; break;
	case eGNote:
		xpos = 3.*STEP + whiteKeyWidth*4.; break;
	case eANote:
		xpos = 3.*STEP + whiteKeyWidth*5.; break;
	case eBNote:
		xpos = 3.*STEP + whiteKeyWidth*6.; break;
	default: return; //keyNumber error
	}
	if ( width()>5.*height() )
		painter.drawRect(whiteKeyWidth/4.+xpos, height()*3./4., whiteKeyWidth/2., height()/6.);
	else
		painter.drawRect(whiteKeyWidth/4.+xpos, height()*3./4., whiteKeyWidth/2., whiteKeyWidth/2.);
}

void MIDIPianoWidget::pressPixmapSharpKey(QPainter &painter, TSize keyNumber)
{
	TSize whiteKeyWidth = width()/7.;
	TSize STEP = whiteKeyWidth/52.;

	painter.setPen(QColor("black")); //edge
	painter.setBrush(palette().highlight()); //fill

	TSize xpos = 0.;
	switch( keyNumber )
	{
	case eCSharpNote:
		xpos = whiteKeyWidth*3./4.; break;
	case eDSharpNote:
		xpos = 2.*STEP+whiteKeyWidth*7./4.; break;
	case eFSharpNote:
		xpos = 3.*STEP+whiteKeyWidth*15./4.; break;
	case eGSharpNote:
		xpos = 4.*STEP+whiteKeyWidth*19./4.; break;
	case eASharpNote:
		xpos = 5.*STEP+whiteKeyWidth*23./4.; break;
	default: return; //keyNumber error
	}
	if ( width()>5.*height() )
		painter.drawRect(xpos, height()/2.2, whiteKeyWidth/2., height()/6.);
	else
		painter.drawRect(xpos, height()/2.2, whiteKeyWidth/2., whiteKeyWidth/2.);
}

void MIDIPianoWidget::mousePressEvent(QMouseEvent *event)
{
	if (!_clickEnabled) return;
	if (event->button() == Qt::LeftButton)
	{
		TSize x=event->x(), y=event->y();

		unsigned note = identifyMidiByPosition(x,y);
		noteOn(note);
		update();
		event->accept();
	}
}

void MIDIPianoWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (!_clickEnabled) return;
	if (event->button() == Qt::LeftButton)
	{
		TSize x=event->x(), y=event->y();

//FIXME: there is a bug if you press in one note, move the mouse and release the click in a different (note) one
// DGG: Suggestion: keep the state of a pressed key, and here, just release the one we pressed

		unsigned note = identifyMidiByPosition(x,y);
		noteOff(note);
		update();
		event->accept();
	}
}

void MIDIPianoWidget::noteOn(unsigned note)
{
	_controlPiano->SetNoteStatus(note%12,true);

	// TODO: Shouldn't the controlPiano be responsible of that?
	unsigned midiNote = note + 21 + (_controlPiano->GetOctave()-1)*12;
	//144 NoteOn, note, velocity
	MIDI::Message tmpMessage(144, note, _controlPiano->GetVelocity(), 0);
	_controlPiano->SendMIDIMessage(tmpMessage);
}

void MIDIPianoWidget::noteOff(unsigned note)
{
	_controlPiano->SetNoteStatus(note%12,false);

	// TODO: Shouldn't the controlPiano be responsible of that?
	unsigned midiNote = note + 21 + (_controlPiano->GetOctave()-1)*12;
	//128 NoteOff, note, velocity
	MIDI::Message tmpMessage(128, note, _controlPiano->GetVelocity(), 0);
	_controlPiano->SendMIDIMessage(tmpMessage);
}

unsigned MIDIPianoWidget::identifyMidiByPosition(TSize x, TSize y)
{
	unsigned whiteKeyWidth = width()/7.;
	if (not  whiteKeyWidth) return eCNote; // TODO: return eNoNote?
	unsigned sharpHeight = height()/2;
	unsigned sharpWidth = whiteKeyWidth/2;
	const TSize octaveConst = 12;

	bool isSharpZone = (y<=sharpHeight) and (x+whiteKeyWidth/4)%whiteKeyWidth <= sharpWidth;
	if (isSharpZone)
	{
		unsigned sharpNote = (x+whiteKeyWidth/2)/whiteKeyWidth-1;
		switch (sharpNote)
		{
			case 0: return eCSharpNote;
			case 1: return eDSharpNote;
			case 2: break; // E
			case 3: return eFSharpNote;
			case 4: return eGSharpNote;
			case 5: return eASharpNote+octaveConst;
			case 6: break; // B
		}
	}
	unsigned flatNote = x*7/width();
	switch (flatNote)
	{
		case 0: return eCNote;
		case 1: return eDNote;
		case 2: return eENote;
		case 3: return eFNote;
		case 4: return eGNote;
		case 5: return eANote+octaveConst;
		case 6: return eBNote+octaveConst;
	}
	return eCNote; // TODO: return eNoNote?
}

void MIDIPianoWidget::setDataSource(VM::FloatArrayDataSource & dataSource )
{
	_dataSource = &dataSource;
	_nBins = _dataSource->nBins();
}

void MIDIPianoWidget::noDataSource()
{
	_dataSource = 0;
	_nBins = 0;
}

void MIDIPianoWidget::processData()
{
	_data = _dataSource->frameData();
	float max = 0;
	for (unsigned bin = 0; bin < _nBins; bin++)
		if (max<_data[bin]) max = _data[bin];

	for (unsigned bin = 0; bin < _nBins; bin++)
	{
		//(bin+10)%12 since dataSource array starts in 'G' and NoteStatus one in 'A'
		if (_data[bin]>0.5*max)
			_controlPiano->SetNoteStatus((bin+10)%12,true);
		else
			_controlPiano->SetNoteStatus((bin+10)%12,false);
	}
}

} //namespace CLAM
