/*
 *	V2MPianoRoll.cxx
 *	Implementation for V2MPianoRoll class.
 *
 *	Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include <q3frame.h>
#include <qscrollbar.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qfont.h>
#include <qcursor.h>
#include <q3popupmenu.h>
#include <qtooltip.h>
#include <CLAM/List.hxx>
#include "V2MPianoRoll.hxx"

using CLAM::List;
using CLAM::MIDINote;
using CLAM::MediaTime;

V2MPianoRoll::V2MPianoRoll(QWidget* parent) : QWidget(parent)
{
	setFixedSize(400,376);
    setGeometry(15,35,width(),height());
	mReceivedXCoord.Wrap(this,&V2MPianoRoll::UpdateVLine);
	mRightMousePressed.Wrap(this,&V2MPianoRoll::ShowPopupMenu);
	_modified = false;
	_current = -1;
	_hit = false;
	_left_scroll_x = 0;
	InitBounds();
	InitDisplayContainer(360,298);
	InitLeftPanel();
	InitDisplayList();	
	InitBottomPanel();
	InitPopupMenu();
	connect(scrollBar,SIGNAL(valueChanged(int)),this,SLOT(updateViewport(int)));
}

void V2MPianoRoll::InitDisplayList()
{
    V2MDataEditDisplay* dataDisplay;
    for(int i = 0;i < 3;i++)
    {
		dataDisplay = new V2MDataEditDisplay(displayContainer);
		dataDisplay->SetXCoordSlot(mReceivedXCoord);
		dataDisplay->RightButtonMousePressedSlot(mRightMousePressed);
		_displayList.push_back(dataDisplay);
	}
	// init piano roll
	pianoRoll = new V2MPREditDisplay(displayContainer);
	pianoRoll->SetXCoordSlot(mReceivedXCoord);
	pianoRoll->RightButtonMousePressedSlot(mRightMousePressed);
}

void V2MPianoRoll::SetData(const Melody& melody,
						   const MIDIMelody& midiMelody,
						   DispData analysisData,
						   bool first)
{
	if(first) _left_scroll_x = 0;
	int width = 360;
	int height = 100;
	_melody = melody;
	_midiMelody = midiMelody;
	SetAnalysisData(analysisData);
	// fill structure
	FillDataStructure(360,90);
	TData vwidth = TData(width*dataInfo.size/dataInfo.width);
	// horizontal bounds 
	SetBounds();
	// displays
	DispData::iterator it = _displayData.begin();
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++,it++)
	{
		(*list_it)->makeCurrent();
		(*list_it)->SetGlobalXCoord(GetSegmentationInfo());
		(*list_it)->setFixedSize(360,height);
		(*list_it)->SetBounds(bound.left,bound.right);
		(*list_it)->setGeometry(0,0,width,height);
		(*list_it)->SetView(0.0,vwidth,it->bottom,it->top);
		(*list_it)->SetData(it->data);
		(*list_it)->updateGL();
	}
	// Piano Roll
	pianoRoll->makeCurrent();
	pianoRoll->SetDataSizeAndSampleRate(dataInfo.size,dataInfo.sampleRate);
	pianoRoll->SetGlobalXCoord(GetSegmentationInfo());
	pianoRoll->setFixedSize(width,height*2-2);
	pianoRoll->setGeometry(0,height,width,height*2-2);
	pianoRoll->SetBounds(bound.left,bound.right);
	pianoRoll->SetView(0.0,vwidth,0.0,TData(height*2-2));
	pianoRoll->SetNotesInfo(dataInfo.notePoints,dataInfo.octave,dataInfo.veloc);
	pianoRoll->updateGL();
	
	if(first) 
	{
		scrollBar->setValue(0);
		ShowAudioDisplay(dtAudio);
	}
	InitState();
}

const DataArray V2MPianoRoll::GetSegmentationInfo()
{
	DataArray segData;
	int nNotes = _melody.GetNumberOfNotes();
	int nSamples = dataInfo.size;
	TData dur = TData(nSamples/dataInfo.sampleRate);
	for(int i = 0;i < nNotes;i++)
	{	
		TData value = _melody.GetNoteArray()[i].GetTime().GetBegin();
		value *= nSamples;
		value /= dur;
		segData.AddElem(value); // add always the beginning time of the note

		/* 
		 * add end time of the note only if is not linked with the next  
		*/
		if((i < nNotes-1) 
			&& (_melody.GetNoteArray()[i+1].GetTime().GetBegin() 
			!=  _melody.GetNoteArray()[i].GetTime().GetEnd()))
		{
			value = _melody.GetNoteArray()[i].GetTime().GetEnd();
			value *= nSamples;
			value /= dur;
			segData.AddElem(value);
		}
		
		// put the last end note value 
		if(i == nNotes-1)
		{
			value = _melody.GetNoteArray()[i].GetTime().GetEnd();
			value *= nSamples;
			value /= dur;
			segData.AddElem(value);
		}
	}
	return segData;
}

void V2MPianoRoll::InitDisplayContainer(int w,int h)
{
	displayContainer = new Q3Frame(this);
	displayContainer->setFrameStyle(Q3Frame::Panel | Q3Frame::Sunken);
    displayContainer->setFixedSize(w,h+20);
	displayContainer->setGeometry(40,0,w,h+20);
	scrollBar = new QScrollBar(displayContainer);
	scrollBar->setFixedSize(displayContainer->width(),20);
	scrollBar->setGeometry(0,displayContainer->height()-19,scrollBar->width(),scrollBar->height());
}

void V2MPianoRoll::SetAnalysisData(DispData analysisData)
{
	_displayData.clear();
	DispData::iterator it = analysisData.begin();
    dataInfo.size = it->data.Size();
	dataInfo.sampleRate = it->sampleRate;
	for(;it != analysisData.end();it++)
		_displayData.push_back((*it));
}

int V2MPianoRoll::GetFrameWidth(int wt,int wref)
{
	int frameWidth;
	TData w = TData(dataInfo.size/dataInfo.sampleRate);
	w /= 4.0;
	frameWidth = int(w);
	TData frac = TData(w-frameWidth);
    if(frac > 0.0) frameWidth++;
	frameWidth = (frameWidth == 1) ? wt : int(w*4.0*wref);
	return frameWidth;
}

void V2MPianoRoll::FillDataStructure(int wt,int wref)
{
	ComputeNotesInfo();
	dataInfo.width = GetFrameWidth(wt,wref); 
	scrollBar->setMinValue(0);
	scrollBar->setMaxValue(dataInfo.width-scrollBar->width());
	scrollBar->setOrientation(Qt::Horizontal);
	scrollBar->setSteps(20,100);
}

void V2MPianoRoll::ComputeNotesInfo()
{
	TData maxEnergy = 0.0;
	TData maxFreq = 0.0;
	TData sampleRate = dataInfo.sampleRate;
	TData step = TData(198.0/12.0);
	TData yCoord,energy_freq;
	dataInfo.notePoints.SetSize(0);
	dataInfo.octave.SetSize(0);
	dataInfo.veloc.SetSize(0);
	dataInfo.availablePoints.SetSize(0);
    int nNotes = _melody.GetNumberOfNotes();
	for(int i = 0;i < nNotes;i++)
	{
		dataInfo.octave.AddElem(_melody.GetNoteArray()[i].GetPitchNote().GetOctave());
		dataInfo.veloc.AddElem(_midiMelody.GetNoteArray()[i].GetVelocity());
		yCoord = GetYCoord(_melody.GetNoteArray()[i].GetPitchNote().GetPitch(),step);
		if(yCoord != -1)
		{
			Point p1(_midiMelody.GetNoteArray()[i].GetTime().GetBegin()*sampleRate,yCoord);
			dataInfo.notePoints.AddElem(p1);
			energy_freq = _melody.GetNoteArray()[i].GetEnergy();
			if(energy_freq > maxEnergy) maxEnergy = energy_freq;
			energy_freq = _melody.GetNoteArray()[i].GetFundFreq();
			if(energy_freq > maxFreq) maxFreq = energy_freq;
			dataInfo.availablePoints.AddElem(true);
			yCoord += step;
			Point p2(_midiMelody.GetNoteArray()[i].GetTime().GetEnd()*sampleRate,yCoord);
			dataInfo.notePoints.AddElem(p2);
			dataInfo.availablePoints.AddElem(true);
		}
	}
	dataInfo.energyMax = maxEnergy*2.0;
	dataInfo.freqMax = maxFreq*2.0;
	AddXCoordInfo();
}

TData V2MPianoRoll::GetYCoord(const EPitch& pitch,TData step)
{
	TData ycoord = 0.0;
	switch(pitch)
	{
				case EPitch::eC:
							ycoord = 0.0;
							break;
				case EPitch::eDb:
							ycoord = step;
							break;
				case EPitch::eD:
							ycoord = 2.0*step;
							break;
				case EPitch::eEb:
							ycoord = 3.0*step;
							break;
				case EPitch::eE:
							ycoord = 4.0*step;
							break;
				case EPitch::eF:
							ycoord = 5.0*step;
							break;
				case EPitch::eGb:
							ycoord = 6.0*step;
							break;
				case EPitch::eG:
							ycoord = 7.0*step;
							break;
				case EPitch::eAb:
							ycoord = 8.0*step;
							break;
				case EPitch::eA:
							ycoord = 9.0*step;
							break;
				case EPitch::eBb:
							ycoord = 10.0*step;
							break;
				case EPitch::eB:
							ycoord = 11.0*step;
							break;
				case EPitch::eNone:
							ycoord = -1;
							break;
	}
	return ycoord;
}

void V2MPianoRoll::AddXCoordInfo()
{
	dataInfo.xInfo.SetSize(0);
	int nPoints = dataInfo.notePoints.Size();
	for(int i = 0;i < nPoints;i++)
	{
		if(i==0 || i == nPoints-1)
		{
			XCoord xcoord;
			xcoord.pointNumber = i;
			xcoord.noteNumber = int(i/2);
			xcoord.key = _midiMelody.GetNoteArray()[i/2].GetKey();
			xcoord.veloc = _midiMelody.GetNoteArray()[i/2].GetVelocity();
			xcoord.pitch = _melody.GetNoteArray()[i/2].GetPitchNote();
			xcoord.shared = false;
			dataInfo.xInfo.AddElem(xcoord);
		}
		else
		{
			XCoord xcoord;
			if(dataInfo.notePoints[i].GetX() == 
			   dataInfo.notePoints[i+1].GetX())
			{
				// if shared, mark and go to the next
				xcoord.pointNumber = i+1;
				xcoord.noteNumber = int((i+1)/2);
				xcoord.key = _midiMelody.GetNoteArray()[(i+1)/2].GetKey();
				xcoord.veloc = _midiMelody.GetNoteArray()[(i+1)/2].GetVelocity();
				xcoord.pitch = _melody.GetNoteArray()[(i+1)/2].GetPitchNote();
				xcoord.shared = true;
				dataInfo.xInfo.AddElem(xcoord);
				i++;
			}
			else
			{
				xcoord.pointNumber = i;
				xcoord.noteNumber = int(i/2);
				xcoord.key = _midiMelody.GetNoteArray()[i/2].GetKey();
				xcoord.veloc = _midiMelody.GetNoteArray()[i/2].GetVelocity();
				xcoord.pitch = _melody.GetNoteArray()[i/2].GetPitchNote();
				xcoord.shared = false;
				dataInfo.xInfo.AddElem(xcoord);
			}
		}

	}
}

void V2MPianoRoll::InitLeftPanel()
{
	//////// frame ////////////////////////////////////////
	leftPanel = new Q3Frame(this,"left_panel");
	leftPanel->setGeometry(0,0,37,displayContainer->height()-2); 
	leftPanel->setFrameStyle(Q3Frame::Panel | Q3Frame::Plain);
	////////////////////////////////////////////////////////

	/////// buttons ////////////////////////////////////////
	showSignalButton = new QPushButton(leftPanel,"show_signal");
	showSignalButton->setGeometry(6,7,25,25);
	showSignalButton->setText("S");
	QToolTip::add(showSignalButton,"show audio");

	showEnergyButton = new QPushButton(leftPanel,"show_energy");
	showEnergyButton->setGeometry(6,38,25,25);
	showEnergyButton->setText("E");
	QToolTip::add(showEnergyButton,"show energy");

	showFundButton = new QPushButton(leftPanel,"show_fund");
	showFundButton->setGeometry(6,69,25,25);
	showFundButton->setText("P");
	QToolTip::add(showFundButton,"show fundamental");

	connect(showSignalButton,SIGNAL(clicked()),this,SLOT(show_signal()));
	connect(showEnergyButton,SIGNAL(clicked()),this,SLOT(show_energy()));
	connect(showFundButton,SIGNAL(clicked()),this,SLOT(show_fund()));
	//////////////////////////////////////////////////////////////////////

	/////// labels /////////////////////////////////////////
	QFont f("Helvetica",10,QFont::Bold);

	C = new QLabel(leftPanel);
	C->setGeometry(6,285,25,12);
	C->setFont(f);
	C->setText("C");

	Db = new QLabel(leftPanel);
	Db->setGeometry(6,268,25,12);
	Db->setFont(f);
	Db->setText("Db");

	D = new QLabel(leftPanel);
	D->setGeometry(6,251,25,12);
	D->setFont(f);
	D->setText("D");

	Eb = new QLabel(leftPanel);
	Eb->setGeometry(6,234,25,12);
	Eb->setFont(f);
	Eb->setText("Eb");

	E = new QLabel(leftPanel);
	E->setGeometry(6,218,25,12);
	E->setFont(f);
	E->setText("E");

	F = new QLabel(leftPanel);
	F->setGeometry(6,202,25,12);
	F->setFont(f);
	F->setText("F");

	Gb = new QLabel(leftPanel);
	Gb->setGeometry(6,185,25,12);
	Gb->setFont(f);
	Gb->setText("Gb");

	G = new QLabel(leftPanel);
	G->setGeometry(6,169,25,12);
	G->setFont(f);
	G->setText("G");

	Ab = new QLabel(leftPanel);
	Ab->setGeometry(6,154,25,12);
	Ab->setFont(f);
	Ab->setText("Ab");

	A = new QLabel(leftPanel);
	A->setGeometry(6,137,25,12);
	A->setFont(f);
	A->setText("A");

	Bb = new QLabel(leftPanel);
	Bb->setGeometry(6,120,25,12);
	Bb->setFont(f);
	Bb->setText("Bb");

	B = new QLabel(leftPanel);
	B->setGeometry(6,103,25,12);
	B->setFont(f);
	B->setText("B");
	////////////////////////////////////////////////////////
}

void V2MPianoRoll::InitBottomPanel()
{
	bottomPanel = new Q3Frame(this,"bottom_panel");
	bottomPanel->setGeometry(0,320,width(),56); 
	bottomPanel->setFrameStyle(Q3Frame::Panel | Q3Frame::Plain);
	
	midiKeyLabel = new QLabel(bottomPanel,"midikey_tag");
	midiKeyLabel->setGeometry(16,6,80,15);
	midiKeyLabel->setText("Key: ");
	
	velocityLabel = new QLabel(bottomPanel,"velocity_tag");
	velocityLabel->setGeometry(midiKeyLabel->x()+midiKeyLabel->width()+8,6,90,15);
	velocityLabel->setText("Veloc: ");

	midiPitchLabel = new QLabel(bottomPanel,"octave_tag");
	midiPitchLabel->setGeometry(velocityLabel->x()+velocityLabel->width()+8,6,80,15);
	midiPitchLabel->setText("Pitch: ");

	amplitudeLabel = new QLabel(bottomPanel,"amplitude_lab");
	amplitudeLabel->setGeometry(midiKeyLabel->x(),midiKeyLabel->y()+15,80,15);
	amplitudeLabel->setText("Amp: ");
	energyLabel = new QLabel(bottomPanel,"energy_label");
	energyLabel->setGeometry(amplitudeLabel->x()+amplitudeLabel->width()+8,amplitudeLabel->y(),90,15);
	energyLabel->setText("Energy: ");
	pitchLabel = new QLabel(bottomPanel,"pitch_label");
	pitchLabel->setGeometry(energyLabel->x()+energyLabel->width()+8,amplitudeLabel->y(),110,15);
	pitchLabel->setText("Pitch: ");

	time1Label = new QLabel(bottomPanel,"time1_lab");
	time1Label->setGeometry(amplitudeLabel->x(),amplitudeLabel->y()+15,70,15);
	time1Label->setText("t1: ");
	time2Label = new QLabel(bottomPanel,"time2_lab");
	time2Label->setGeometry(energyLabel->x(),energyLabel->y()+15,70,15);
	time2Label->setText("t2: ");
	durationLabel = new QLabel(bottomPanel,"duration_lab");
	durationLabel->setGeometry(pitchLabel->x(),pitchLabel->y()+15,70,15);
	durationLabel->setText("dur: ");

	saveButton = new QPushButton(bottomPanel,"save_changes");
	saveButton->setGeometry(338,15,50,26);
	saveButton->setText("Save");
	saveButton->setEnabled(false);
	
	// connection for the button
	connect(saveButton,SIGNAL(clicked()),this,SLOT(save()));
}

void V2MPianoRoll::ShowAudioDisplay(dType d)
{
	int displayID = -1;
	switch(d)
	{
		case dtAudio:
			displayID = 0;
			showSignalButton->setEnabled(false);
			showEnergyButton->setEnabled(true);
			showFundButton->setEnabled(true);
			break;
		case dtEnergy:
			displayID = 1;
			showSignalButton->setEnabled(true);
			showEnergyButton->setEnabled(false);
			showFundButton->setEnabled(true);
			break;
		case dtFund:
			displayID = 2;
			showSignalButton->setEnabled(true);
			showEnergyButton->setEnabled(true);
			showFundButton->setEnabled(false);
			break;
		default:
			break;
	}
	if(displayID != -1)
	{
		int k = 0;
		DisplayList::iterator list_it = _displayList.begin();
		for(;list_it != _displayList.end();list_it++,k++)
		{
			if(k == displayID)
				(*list_it)->show();
			else
				(*list_it)->hide();
		}
	}
}

void V2MPianoRoll::show_signal()
{
	ShowAudioDisplay(dtAudio);
}

void V2MPianoRoll::show_energy()
{
	ShowAudioDisplay(dtEnergy);
}

void V2MPianoRoll::show_fund()
{
	ShowAudioDisplay(dtFund);
}

void V2MPianoRoll::SetSizeHorCursor()
{
	QCursor cursor(Qt::SizeHorCursor);
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++)
		(*list_it)->setCursor(cursor);
	pianoRoll->setCursor(cursor);
}

void V2MPianoRoll::SetArrowCursor()
{
	QCursor cursor(Qt::ArrowCursor);
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++)
		(*list_it)->setCursor(cursor);
	pianoRoll->setCursor(cursor);
}

void V2MPianoRoll::UpdateVLine(float xcoord)
{
	if(isVisible()) emit sendXCoord(xcoord);
	int i = Hit(xcoord);
	if(xcoord == -1)
	{
		_current = -1;
		SetArrowCursor();
		SetDisplaysMouseTracking(false);
		_hit = false;
		ResetLabels();
	}
	if(i!=-1)
	{
		_modified = true;
		SetDisplaysMouseTracking(true);
		_hit = true;
	}
	if(_hit)
	{
		if((_current != -1) && (_current >=0 && _current < dataInfo.xInfo.Size()))
		{
			DisplayList::iterator list_it = _displayList.begin();
			if((GetPixel(xcoord) >= 0) && (GetPixel(xcoord) < dataInfo.width) 
				&& (*list_it)->IsAvailableXCoord(_current))
			{
				SetSizeHorCursor();
				UpdateHit(_current,xcoord);
				UpdatePanel(xcoord,_current);
			}
			else
			{
				_current = -1;
				SetArrowCursor();
				SetDisplaysMouseTracking(false);
				_hit = false;
			}
		}
	}
}

unsigned V2MPianoRoll::GetPixel(float xcoord)
{
	TData aux = TData(xcoord*TData(dataInfo.width)); 
	aux /= TData(dataInfo.size);
	unsigned pixel = unsigned(aux);
	return pixel;
}

int V2MPianoRoll::Hit(float xcoord)
{
	bool hit = false;
	int i;
	TData current;
	unsigned pixel = GetPixel(xcoord);
	DisplayList::iterator list_it = _displayList.begin();
	int nPoints = (*list_it)->GetnXCoords();
	for(i = 0;i < nPoints;i++)
	{
		if((*list_it)->IsAvailableXCoord(i))
		{
			current = (*list_it)->GetXCoord(i);
			unsigned rpixel = GetPixel(current);
			if(abs(int(pixel-rpixel))<=1) // hit
			{
				SetCurrent(i,xcoord); // set the actual coord hitted
				hit = true; 
				break; 
			}
		}
	}
	return (hit) ? i : -1;
}

void V2MPianoRoll::UpdateHit(int index,float xcoord)
{
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++)
	{
		(*list_it)->makeCurrent();
		(*list_it)->UpdateXCoord(index,xcoord);
		(*list_it)->updateGL();
	}
	pianoRoll->makeCurrent();
	pianoRoll->UpdateXCoord(index,xcoord);
	if(dataInfo.xInfo[index].shared)
	{
		int cp = dataInfo.xInfo[index].pointNumber;
		int shared = GetSharedPointIndex(cp);
		if(shared != -1)
		{
			pianoRoll->UpdatePoint(shared,xcoord);
		}
	}
	if(dataInfo.availablePoints[dataInfo.xInfo[index].pointNumber])
		pianoRoll->UpdatePoint(dataInfo.xInfo[index].pointNumber,xcoord);
	pianoRoll->updateGL();
}

int V2MPianoRoll::FindIndex(float xcoord)
{
	int i = 0;
	bool present = false;
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++,i++)
	{
		if(xcoord < (*list_it)->GetXCoord(i))
		{
			present = true;
			break;	
		}
	}

	return (present) ? i : -1;
}

void V2MPianoRoll::SetCurrent(int index,float xcoord)
{
	DisplayList::iterator list_it = _displayList.begin();
	if(_current == -1)
	{
		_current = index; // actual index of the coord to modify 
		PushState(_current);
	}
	else
	{
		int next_x = (*list_it)->GetNextXCoordAvailableIndex(_current);
		int prior_x = (*list_it)->GetPriorXCoordAvailableIndex(_current);
		int nXCoords = (*list_it)->GetnXCoords();
		if(_current == 0)
		{
			// first
			if(next_x != -1)
			{
				if(xcoord >= (*list_it)->GetXCoord(next_x))
					Update();
			}
		}
		else
		if(_current == nXCoords-1)
		{
			// last
			if(prior_x != -1)
			{
				if(xcoord <= (*list_it)->GetXCoord(prior_x))
					Update(true);
			}
		}
		else
		{
				if(next_x != -1)
				{
					if(xcoord > (*list_it)->GetXCoord(next_x))
						Update();
				}
				if(prior_x != -1)
				{
					if(xcoord < (*list_it)->GetXCoord(prior_x))
						Update(true);
				}	
		}
	}
}

int V2MPianoRoll::GetSharedPointIndex(int index)
{
	int i = index-1;
	bool find = false;
	for(;i >= 0;i--)
	{
		if(dataInfo.availablePoints[i])
		{
			find = true; break;
		}
	}
	return (find) ? i : -1;
}

void V2MPianoRoll::PurgeXCoord(int index)
{  
	if(index < 0 || index > dataInfo.xInfo.Size()-1) return;
	DisplayList::iterator list_it = _displayList.begin();
	if(!(*list_it)->IsAvailableXCoord(index)) return;
	if(!dataInfo.xInfo[index].shared && !dataInfo.availablePoints[dataInfo.xInfo[index].pointNumber]) 
	{
		for(;list_it != _displayList.end();list_it++)
		{
			(*list_it)->makeCurrent();
			(*list_it)->DeleteXCoord(index);
			(*list_it)->updateGL();
		}
		pianoRoll->makeCurrent();
		pianoRoll->DeleteXCoord(index);
		pianoRoll->updateGL();
	}
}

void V2MPianoRoll::SetDisplaysMouseTracking(bool mt)
{
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++)
	{
		(*list_it)->setMouseTracking(mt);
	}
	pianoRoll->setMouseTracking(mt);

}

void V2MPianoRoll::Update(bool prior)
{

	bool deletePoints = true;
	bool deleteBoth = false;
	bool flag_s = false;
	bool flag_n = false;
	int current,next;
	DisplayList::iterator list_it = _displayList.begin();
	if(prior)
	{
		current = (*list_it)->GetPriorXCoordAvailableIndex(_current);
		if(current == -1) return;
		next = _current;
		if(_state.size() > 0) _state.front().flag = true;
	}
	else
	{
		next = (*list_it)->GetNextXCoordAvailableIndex(_current);
		if(next == -1) return;
		current = _current;
	}
	bool currentShared = dataInfo.xInfo[current].shared;
	bool nextShared = dataInfo.xInfo[next].shared;
	for(;list_it != _displayList.end();list_it++)
	{
		(*list_it)->makeCurrent();
		if((!currentShared)&&(!nextShared))
		{
			if(dataInfo.xInfo[current].noteNumber != dataInfo.xInfo[next].noteNumber) 
			{
				(*list_it)->DeleteXCoord(current);
				flag_s = true;
				deletePoints = false;
			}
			else
			{
				(*list_it)->DeleteXCoord(current,true);
				deleteBoth = true;
			}
		}
		else
		{
			bool anxt = dataInfo.availablePoints[dataInfo.xInfo[next].pointNumber];
			bool diff = dataInfo.xInfo[current].noteNumber != dataInfo.xInfo[next].noteNumber;
			if(dataInfo.xInfo[current].shared && !dataInfo.xInfo[next].shared)
			{
				if(diff) deletePoints = false;
				flag_s = true;
			}
			else
			{
				if(!dataInfo.xInfo[current].shared && dataInfo.xInfo[next].shared)
				{
					if(!anxt) 
						deleteBoth = true;
					else
						flag_n = true;
				}
			}
			if(deleteBoth) 
				(*list_it)->DeleteXCoord(current,true);
			else
				(*list_it)->DeleteXCoord(current);
		}
		(*list_it)->updateGL();
	}

	pianoRoll->makeCurrent();
	if(deleteBoth) 
		pianoRoll->DeleteXCoord(current,true);
	else
		pianoRoll->DeleteXCoord(current);
	pianoRoll->updateGL();

	if(deletePoints)
	{
		pianoRoll->DeletePoint(dataInfo.xInfo[current].pointNumber);
		dataInfo.availablePoints[dataInfo.xInfo[current].pointNumber] = false;
		dataInfo.availablePoints[dataInfo.xInfo[current].pointNumber+1] = false;
	}
	if(flag_s) dataInfo.xInfo[next].shared = true;
	if(flag_n) dataInfo.xInfo[next].shared = false;
	PurgeXCoord(next);
}

void V2MPianoRoll::Synchronize(float xcoord)
{
	if(!isVisible()) UpdateVLine(xcoord);
}

void V2MPianoRoll::InitPopupMenu()
{
	popupMenu = new Q3PopupMenu();
	popupMenu->insertItem("Undo",this,SLOT(undo()),0,0);
	popupMenu->insertItem("Save",this,SLOT(save()),0,1);
	popupMenu->insertItem("Discard",this,SLOT(discard()),0,2);
	DisablePopupMenuItems();
}

void V2MPianoRoll::ShowPopupMenu()
{
	popupMenu->exec(QCursor::pos());
}

void V2MPianoRoll::EnablePopupMenuItems()
{
	for(int i = 0;i < 3;i++)
		popupMenu->setItemEnabled(i,true);
}

void V2MPianoRoll::DisablePopupMenuItems()
{
	for(int i = 0;i < 3;i++)
		popupMenu->setItemEnabled(i,false);
}

void V2MPianoRoll::undo()
{
	PopState();
	if(_state.empty())
		InitState();
	if(isVisible()) emit sendUndo();
}

void V2MPianoRoll::save()
{
	emit sendSave();
}

void V2MPianoRoll::discard()
{
	DiscardChanges();
	if(isVisible()) emit sendDiscard();
}

void V2MPianoRoll::InitState()
{
	_state.clear();
	_modified = false;
	saveButton->setEnabled(false);
	DisablePopupMenuItems();
}

void V2MPianoRoll::PushState(int index)
{
	State state;
	DisplayList::iterator list_it = _displayList.begin();
	int prior_x = (*list_it)->GetPriorXCoordAvailableIndex(index); 
	int next_x = (*list_it)->GetNextXCoordAvailableIndex(index);
	TData x;
	if(prior_x != -1)
	{
		x = (*list_it)->GetXCoord(prior_x);
		state.availX[0] = (*list_it)->IsAvailableXCoord(prior_x);
	}
	else
	{
		x = -1.0;
		state.availX[0] = false;
	}
	state.xCoords[0] = x;
	x = (*list_it)->GetXCoord(index);
    state.xCoords[1] = x;
	state.availX[1] = (*list_it)->IsAvailableXCoord(index);
	if(next_x != -1)
	{
		x = (*list_it)->GetXCoord(next_x);
		state.availX[2] = (*list_it)->IsAvailableXCoord(next_x);
	}
	else
	{
		x = -1.0;
		state.availX[2] = false;
	}
	state.xCoords[2] = x;	
	int cp = dataInfo.xInfo[index].pointNumber;
	int spoint1 = GetSharedPointIndex(cp);
	int spoint2 = GetSharedPointIndex(spoint1);
	int cpoint = cp;
	int npoint;
	if(cpoint < dataInfo.availablePoints.Size()-1)
		npoint = cpoint+1;
	else
		npoint = -1;
	state.pIndex[0] = spoint2;
	state.pIndex[1] = spoint1;
	state.pIndex[2] = cpoint;
	state.pIndex[3] = npoint;
	for(int i = 0;i < 4;i++)
	{
		state.pX[i] = pianoRoll->GetPointX(state.pIndex[i]);
		if(state.pIndex[i] != -1)
			state.availP[i] = dataInfo.availablePoints[state.pIndex[i]];
		else
			state.availP[i] = false;
	}
	bool sh = false;
	if(prior_x != -1)
		sh = dataInfo.xInfo[prior_x].shared;
	state.shared[0] = sh;
	sh = dataInfo.xInfo[index].shared; 
	state.shared[1] = sh; sh = false;
	if(next_x != -1)
		sh = dataInfo.xInfo[next_x].shared;
	state.shared[2] = sh;
	state.xIndex[0] = prior_x;
	state.xIndex[1] = index;
	state.xIndex[2] = next_x;
	state.flag = false;
	_state.push_front(state);  // stack behavior
	if(_state.size() > 20) _state.pop_back(); // if there are more than 20 elements delete back
	if(!popupMenu->isItemEnabled(0))
		EnablePopupMenuItems();
	if(!saveButton->isEnabled()) saveButton->setEnabled(true);
}

void V2MPianoRoll::PopState()
{
	if(!_state.empty())
	{
		State state = _state.front();	
		for(int i = 0;i < 4;i++)
		{
			if(i < 3)
			{
				if(state.xIndex[i] != -1)
					dataInfo.xInfo[state.xIndex[i]].shared = state.shared[i];
			}
			if(state.pIndex[i] != -1)
				dataInfo.availablePoints[state.pIndex[i]] = state.availP[i];
		}

		DisplayList::iterator list_it = _displayList.begin();
		for(;list_it != _displayList.end();list_it++)
		{
			(*list_it)->makeCurrent();
			for(int j = 0;j < 3;j++)
			{
				if(state.xIndex[j] != -1)
				{
					if(state.availX[j])
					{
						(*list_it)->WakeUpXCoord(state.xIndex[j]);
						(*list_it)->UpdateXCoord(state.xIndex[j],state.xCoords[j]);
					}
				}
			}
			(*list_it)->updateGL();
		}
		
		pianoRoll->makeCurrent();
		for(int k = 0;k < 4;k++)
		{
			if(k < 3)
			{
				if(state.xIndex[k] != -1)
				{
					if(state.availX[k])
					{
						pianoRoll->WakeUpXCoord(state.xIndex[k]);
						pianoRoll->UpdateXCoord(state.xIndex[k],state.xCoords[k]);
					}
				}
			}
			if(state.availP[k])
			{
				pianoRoll->WakeUpPoint(state.pIndex[k]);
				pianoRoll->UpdatePoint(state.pIndex[k],state.pX[k]);
			}
		}
		pianoRoll->updateGL();

		_state.pop_front();
	}
}

void V2MPianoRoll::DiscardChanges()
{
	SetData(_melody,_midiMelody,_displayData);
}

void V2MPianoRoll::SyncUndo()
{
	if(!isVisible()) undo();
}

void V2MPianoRoll::SyncSave()
{
	if(!isVisible()) save();
}

void V2MPianoRoll::SyncDiscard()
{
	if(!isVisible()) discard();
}

void V2MPianoRoll::UpdatePanel(TData xcoord,int index)
{
	int point = dataInfo.xInfo[index].pointNumber;
	bool hasPoint = dataInfo.availablePoints[point];
	if(!hasPoint || point%2!=0) 
		UpdateLabels(xcoord,index,true);
	else
		UpdateLabels(xcoord,index);
}

void V2MPianoRoll::UpdateLabels(TData xcoord,int index,bool back)
{
	if(index < 0 || index > dataInfo.xInfo.Size()-1) return;
	int _index = index;
	QString s;
	QString _label("");
	TData amplitude,energy,fund,t1,t2;
	EPitch pitch;
	int key,velocity,octave;
	unsigned nindex = int(xcoord);
	int point = dataInfo.xInfo[index].pointNumber;
	if(point == -1) return;
	if(!dataInfo.availablePoints[point])
	{
		int tmp = pianoRoll->GetPriorXCoordAvailableIndex(index);
		_index = (tmp != -1) ? tmp : _index;
		point = dataInfo.xInfo[_index].pointNumber;

	}
	DispData::iterator it = _displayData.begin();
    amplitude = fabs(it->data[nindex]);it++;
	_label = "A:  "+(s.setNum(double(amplitude),'f',3));
	amplitudeLabel->setText(_label);
	energy = fabs(it->data[nindex]);it++;
	_label = "Energy: "+(s.setNum(double(energy),'f',3));
	energyLabel->setText(_label);
	fund = it->data[nindex];
	_label = "Pitch: "+(s.setNum(double(fund),'f',3)+" Hz");
	pitchLabel->setText(_label);
	key = dataInfo.xInfo[_index].key;
	_label = "Key: "+(s.setNum(key));
	midiKeyLabel->setText(_label);
	velocity = dataInfo.xInfo[_index].veloc;
	_label = "Veloc: "+(s.setNum(velocity));
	velocityLabel->setText(_label);
	pitch = dataInfo.xInfo[_index].pitch.GetPitch();
	octave = dataInfo.octave[point/2];
	QString str = GetPitchStr(pitch);
	_label = "Pitch: "+str;
	str=(s.setNum(octave));
	_label += str;
	midiPitchLabel->setText(_label);
	DisplayList::iterator list_it = _displayList.begin();
	if(back)
	{
		int indext1 = (*list_it)->GetPriorXCoordAvailableIndex(index);
		t1 = (*list_it)->GetXCoord(indext1);
		t1 /= dataInfo.sampleRate;
		t2 = xcoord/dataInfo.sampleRate; 
	}
	else
	{
		int indext2 = (*list_it)->GetNextXCoordAvailableIndex(index);
		t1 = xcoord/dataInfo.sampleRate;
		t2 = (*list_it)->GetXCoord(indext2);
		t2 /= dataInfo.sampleRate;
	}
	_label = "t1: "+(s.setNum(double(t1),'f',3))+" s";
	time1Label->setText(_label);
	_label = "t2: "+(s.setNum(double(t2),'f',3))+" s";
	time2Label->setText(_label);
	_label = "dur: "+(s.setNum(double(t2-t1),'f',3))+" s";
	durationLabel->setText(_label);
}

void V2MPianoRoll::ResetLabels()
{
	time1Label->setText("t1: ");
	time2Label->setText("t2: ");
	amplitudeLabel->setText("A: ");
	energyLabel->setText("Energy: ");
	pitchLabel->setText("Pitch: ");
	midiKeyLabel->setText("Key: ");
	velocityLabel->setText("Veloc: ");
	midiPitchLabel->setText("Pitch: ");
	durationLabel->setText("dur: ");
}

const QString V2MPianoRoll::GetPitchStr(const EPitch& pitch)
{
	QString s("");
	switch(pitch)
	{
				case EPitch::eC:
							s += "C";
							break;
				case EPitch::eDb:
							s += "Db";
							break;
				case EPitch::eD:
							s += "D";
							break;
				case EPitch::eEb:
							s += "Eb";
							break;
				case EPitch::eE:
							s += "E";
							break;
				case EPitch::eF:
							s += "F";
							break;
				case EPitch::eGb:
						    s += "Gb";
							break;
				case EPitch::eG:
							s += "G";
							break;
				case EPitch::eAb:
							s += "Ab";
							break;
				case EPitch::eA:
							s += "A";
							break;
				case EPitch::eBb:
							s += "Bb";
							break;
				case EPitch::eB:
							s += "B";
							break;
				case EPitch::eNone:
							break;
	}
	return s;
}

bool V2MPianoRoll::IsModified()
{
	return _modified;
}

MIDIMelody V2MPianoRoll::GetMIDIMelody()
{
	Array<Point> points = pianoRoll->GetPoints();
	Array<int> points_index = pianoRoll->GetPointsIndex();
	List<MIDINote> array;
	MIDINote myNote;
	MediaTime time;
	for(int i = 0;i < points.Size();i+=2)
	{
		myNote.SetKey(_midiMelody.GetNoteArray()[points_index[i]/2].GetKey());
		myNote.SetVelocity(_midiMelody.GetNoteArray()[points_index[i]/2].GetVelocity());
		time.SetBegin(points[i].GetX()/dataInfo.sampleRate);
		time.SetEnd(points[i+1].GetX()/dataInfo.sampleRate);
		myNote.SetTime(time);
		array.AddElem(myNote);
	}
	MIDIMelody midi_melody;
	midi_melody.AddNumberOfNotes();
	midi_melody.UpdateData();
	midi_melody.SetNoteArray(array);
	midi_melody.SetNumberOfNotes(array.Size());
	return midi_melody;
}

void V2MPianoRoll::InitBounds()
{
	bound.left = 0.0;
	bound.right = 0.0;
}

void V2MPianoRoll::SetBounds()
{
	bound.left = TData(_left_scroll_x*TData(dataInfo.size)/TData(dataInfo.width));
	TData right = TData(bound.left+(360.0*TData(dataInfo.size)/TData(dataInfo.width)));
	right -= TData(dataInfo.size);
	bound.right = (right >= 0) ? 0.0 : fabs(right);
	bound.right = TData(dataInfo.size-bound.right);
}

void V2MPianoRoll::updateViewport(int x)
{
	_left_scroll_x = x;
	TData value = TData(TData(_left_scroll_x)*400.0/360.0);
	if(isVisible()) emit scrollValue(int(value));
	SetBounds();
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++)
	{
		(*list_it)->makeCurrent();
		(*list_it)->SetBounds(bound.left,bound.right);
		(*list_it)->updateGL();
	}
	pianoRoll->makeCurrent();
	pianoRoll->SetBounds(bound.left,bound.right);
	pianoRoll->updateGL();
}

void V2MPianoRoll::SyncScroll(int value)
{
	if(!isVisible()) scrollBar->setValue(value); 
}
	
// END

