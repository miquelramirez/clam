/*
 *	V2MSegmentationEditor.cxx
 *	Implementation for V2MSegmentationEditor class.
 *
 *	Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include <q3frame.h>
#include <qscrollbar.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <q3popupmenu.h>
#include <qcursor.h>
#include <qtooltip.h>
#include <CLAM/List.hxx>
#include "V2MSegmentationEditor.hxx"

using CLAM::Note;
using CLAM::MediaTime;
using CLAM::List;

V2MSegmentationEditor::V2MSegmentationEditor(QWidget* parent) : QWidget(parent)
{
	setFixedSize(400,376);
    setGeometry(15,35,width(),height());
	mReceivedXCoord.Wrap(this,&V2MSegmentationEditor::UpdateVLine);
	mRightMousePressed.Wrap(this,&V2MSegmentationEditor::ShowPopupMenu);
	_modified = false;
	_current = -1;
	_hit = false;
	_left_scroll_x;
	InitBounds();
	InitDisplayContainer(400,300);
	InitDisplayList();
	InitPanel();
	InitPopupMenu();
	connect(scrollBar,SIGNAL(valueChanged(int)),this,SLOT(updateViewport(int)));
}

V2MSegmentationEditor::~V2MSegmentationEditor(){}

void V2MSegmentationEditor::InitDisplayList()
{
    V2MDataEditDisplay* dataDisplay;
    for(int i = 0;i < 5;i++)
    {
		dataDisplay = new V2MDataEditDisplay(displayContainer);
		dataDisplay->SetXCoordSlot(mReceivedXCoord);
		dataDisplay->RightButtonMousePressedSlot(mRightMousePressed);
		_displayList.push_back(dataDisplay);
	}
}

void V2MSegmentationEditor::SetData(const Melody& melody,DispData analysisData,bool first)
{
	// tooltips area ///////
	QRect arect(25,25,350,50);
	QRect rect(25,10,350,30);
	////////////////////////////
	if(first) _left_scroll_x = 0;
	int width = 400;
	int top = 50;
	int height = 50;
	_melody = melody;
	SetAnalysisData(analysisData);
	// fill structure
	FillDataStructure(400,100);
	TData vwidth = TData(width*dataInfo.size/dataInfo.width);
	// horizontal bounds
	SetBounds();
	// displays
	int i = 0;
	DispData::iterator it = _displayData.begin();
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++,i++)
	{
		(*list_it)->makeCurrent();
		(*list_it)->SetBounds(bound.left,bound.right);
		// common
		(*list_it)->SetGlobalXCoord(GetSegmentationInfo());
		if(i == 0)
		{
			(*list_it)->setFixedSize(width,height*2);
			(*list_it)->setGeometry(0,0,width,height*2);
			(*list_it)->SetView(0.0,vwidth,it->bottom,it->top);
			(*list_it)->SetData(it->data);
			QToolTip::add((*list_it),arect,"audio");
			it++;
		}
		else
		{
            (*list_it)->setFixedSize(width,height);
			(*list_it)->setGeometry(0,top+i*height,width,height);
		}
		
		// data for the analysis display
		if((i % 2) != 0)
		{
			if(i != 0) (*list_it)->SetColor(1.0f,1.0f,0.0f);
			(*list_it)->SetView(0.0,vwidth,it->bottom,it->top);
			(*list_it)->SetData(it->data);
			if(i==1) QToolTip::add((*list_it),rect,"energy");
			if(i==3) QToolTip::add((*list_it),rect,"fundamental");
			it++;
		}
		// data for the energy display
		if(i == 2)
		{
			(*list_it)->SetView(0.0,vwidth,0.0,dataInfo.energyMax);
			(*list_it)->SetDataType(dtFundEnergy);
			(*list_it)->SetPoints(dataInfo.EnergyPoints);
			QToolTip::add((*list_it),rect,"note energy");
		}
		// data for the f0 display
		if(i == 4)
		{
			(*list_it)->SetView(0.0,vwidth,0.0,dataInfo.freqMax);
			(*list_it)->SetDataType(dtFundEnergy);
			(*list_it)->SetPoints(dataInfo.FundPoints);
			QToolTip::add((*list_it),rect,"pitch note");
		}
		(*list_it)->updateGL();
	}
	if(first) scrollBar->setValue(0);
	InitState();
}

const DataArray V2MSegmentationEditor::GetSegmentationInfo()
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
		segData.AddElem(value); // always add the begin time of the note

		/* 
		 * add the end time of the note only if is not linked with the next 
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
		
		// put the last end time
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

void V2MSegmentationEditor::InitPanel()
{
	panel = new Q3Frame(this,"panel");
	panel->setGeometry(0,320,width(),56); 
	panel->setFrameStyle(Q3Frame::Panel | Q3Frame::Plain);

	amplitudeLabel = new QLabel(panel,"amplitude_lab");
	amplitudeLabel->setGeometry(12,6,80,15);
	amplitudeLabel->setText("Amp: ");
	energy1Label = new QLabel(panel,"energy1_label");
	energy1Label->setGeometry(amplitudeLabel->x()+amplitudeLabel->width()+8,6,90,15);
	energy1Label->setText("Energy: ");
	pitch1Label = new QLabel(panel,"pitch1_label");
	pitch1Label->setGeometry(energy1Label->x()+energy1Label->width()+8,6,110,15);
	pitch1Label->setText("Pitch: ");

	segLabel = new QLabel(panel,"seg_tag");
	segLabel->setGeometry(amplitudeLabel->x(),amplitudeLabel->y()+15,90,15);
	segLabel->setText("Segmentation --->");

    energy2Label = new QLabel(panel,"energy2_label");
	energy2Label->setGeometry(energy1Label->x(),energy1Label->y()+15,90,15);
	energy2Label->setText("Energy: ");
	pitch2Label = new QLabel(panel,"pitch2_label");
	pitch2Label->setGeometry(pitch1Label->x(),pitch1Label->y()+15,85,15);
	pitch2Label->setText("Pitch: ");
	
	time1Label = new QLabel(panel,"time1_lab");
	time1Label->setGeometry(segLabel->x(),segLabel->y()+15,70,15);
	time1Label->setText("t1: ");
	time2Label = new QLabel(panel,"time2_lab");
	time2Label->setGeometry(energy1Label->x(),energy2Label->y()+15,70,15);
	time2Label->setText("t2: ");
	durationLabel = new QLabel(panel,"duration_lab");
	durationLabel->setGeometry(pitch1Label->x(),pitch2Label->y()+15,70,15);
	durationLabel->setText("dur: ");

	saveButton = new QPushButton(panel,"save_changes");
	saveButton->setGeometry(338,15,50,26);
	saveButton->setText("Save");
	saveButton->setEnabled(false);
	
	// connections for the button
	connect(saveButton,SIGNAL(clicked()),this,SLOT(save()));
}

void V2MSegmentationEditor::InitPopupMenu()
{
	popupMenu = new Q3PopupMenu();
	popupMenu->insertItem("Undo",this,SLOT(undo()),0,0);
	popupMenu->insertItem("Save",this,SLOT(save()),0,1);
	popupMenu->insertItem("Discard",this,SLOT(discard()),0,2);
	DisablePopupMenuItems();
}

void V2MSegmentationEditor::ShowPopupMenu()
{
	popupMenu->exec(QCursor::pos());
}

void V2MSegmentationEditor::FillDataStructure(int wt,int wref)
{
	ComputeFundAndEnergyPoints();
	dataInfo.width = GetFrameWidth(wt,wref); 
	scrollBar->setMinValue(0);
	scrollBar->setMaxValue(dataInfo.width-scrollBar->width());
	scrollBar->setOrientation(Qt::Horizontal);
	scrollBar->setSteps(20,100);
}

int V2MSegmentationEditor::GetFrameWidth(int wt,int wref)
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

void V2MSegmentationEditor::ComputeFundAndEnergyPoints()
{
	TData maxEnergy = 0.0;
	TData maxFreq = 0.0;
	TData sampleRate = dataInfo.sampleRate;
	dataInfo.EnergyPoints.SetSize(0);
	dataInfo.FundPoints.SetSize(0);
	dataInfo.availablePoints.SetSize(0);
    int nNotes = _melody.GetNumberOfNotes();
	for(int i = 0;i < nNotes;i++)
	{
		Point p1(_melody.GetNoteArray()[i].GetTime().GetBegin()*sampleRate,
			     _melody.GetNoteArray()[i].GetEnergy());
		dataInfo.EnergyPoints.AddElem(p1);
		if(p1.GetY() > maxEnergy) maxEnergy = p1.GetY();
		p1.SetY(_melody.GetNoteArray()[i].GetFundFreq());
		dataInfo.FundPoints.AddElem(p1);
		dataInfo.availablePoints.AddElem(true);
		if(p1.GetY() > maxFreq) maxFreq = p1.GetY();
		Point p2(_melody.GetNoteArray()[i].GetTime().GetEnd()*sampleRate,
			     _melody.GetNoteArray()[i].GetEnergy());
		dataInfo.EnergyPoints.AddElem(p2);
		p2.SetY(_melody.GetNoteArray()[i].GetFundFreq());
		dataInfo.FundPoints.AddElem(p2);
		dataInfo.availablePoints.AddElem(true);
	}
	dataInfo.energyMax = maxEnergy*2.0;
	dataInfo.freqMax = maxFreq*2.0;
	AddXCoordInfo();
}

void V2MSegmentationEditor::InitDisplayContainer(int w,int h)
{
	displayContainer = new Q3Frame(this);
    displayContainer->setFixedSize(w,h+20);
	scrollBar = new QScrollBar(displayContainer);
	scrollBar->setFixedSize(displayContainer->width(),20);
	scrollBar->setGeometry(0,displayContainer->height()-20,scrollBar->width(),scrollBar->height());
}

void V2MSegmentationEditor::SetAnalysisData(DispData analysisData)
{
	_displayData.clear();
	DispData::iterator it = analysisData.begin();
    dataInfo.size = it->data.Size();
	dataInfo.sampleRate = it->sampleRate;
	for(;it != analysisData.end();it++)
		_displayData.push_back((*it));
}

unsigned V2MSegmentationEditor::GetPixel(float xcoord)
{
	TData aux = TData(xcoord*TData(dataInfo.width)); 
	aux /= TData(dataInfo.size);
	unsigned pixel = unsigned(aux);
	return pixel;
}

void V2MSegmentationEditor::SetDisplaysMouseTracking(bool mt)
{
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++)
	{
		(*list_it)->setMouseTracking(mt);
	}
}

void V2MSegmentationEditor::UpdateVLine(float xcoord)
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

void V2MSegmentationEditor::undo()
{
	PopState();
	if(_state.empty())
		InitState();
	if(isVisible()) emit sendUndo();
}

void V2MSegmentationEditor::save()
{
	UpdateChanges();
	emit requestSaveMelody();
	emit sendSave();
}

void V2MSegmentationEditor::SaveMelody()
{
	save();
}

void V2MSegmentationEditor::discard()
{
	DiscardChanges();
	if(isVisible()) emit sendDiscard();
}

void V2MSegmentationEditor::UpdateChanges()
{
	_melody = GetMelody();
	SetData(_melody,_displayData);
}

void V2MSegmentationEditor::DiscardChanges()
{
	SetData(_melody,_displayData);
}

void V2MSegmentationEditor::InitState()
{
	_state.clear();
	_modified = false;
	saveButton->setEnabled(false);
	DisablePopupMenuItems();
}

void V2MSegmentationEditor::EnablePopupMenuItems()
{
	for(int i = 0;i < 3;i++)
		popupMenu->setItemEnabled(i,true);
}

void V2MSegmentationEditor::DisablePopupMenuItems()
{
	for(int i = 0;i < 3;i++)
		popupMenu->setItemEnabled(i,false);
}

int V2MSegmentationEditor::Hit(float xcoord)
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
				SetCurrent(i,xcoord); // actual xcoord 
				hit = true; 
				break; 
			}
		}
	}
	return (hit) ? i : -1;
}

void V2MSegmentationEditor::SetSizeHorCursor()
{
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++)
	{
		QCursor cursor(Qt::SizeHorCursor);
		(*list_it)->setCursor(cursor);
	}
}

void V2MSegmentationEditor::SetArrowCursor()
{
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++)
	{
		QCursor cursor(Qt::ArrowCursor);
		(*list_it)->setCursor(cursor);
	}
	
}

void V2MSegmentationEditor::UpdateHit(int index,float xcoord)
{
	int i = 0;
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++,i++)
	{
		(*list_it)->makeCurrent();
		(*list_it)->UpdateXCoord(index,xcoord);
		if(i == 2 || i == 4)
		{
			if(dataInfo.xInfo[index].shared)
			{
				int cp = dataInfo.xInfo[index].pointNumber;
				int shared = GetSharedPointIndex(cp);
				if(shared != -1)
				{
					(*list_it)->UpdatePoint(shared,xcoord);
				}
			}
			if(dataInfo.availablePoints[dataInfo.xInfo[index].pointNumber])
				(*list_it)->UpdatePoint(dataInfo.xInfo[index].pointNumber,xcoord);
		}
		(*list_it)->updateGL();
	}
}

int V2MSegmentationEditor::FindIndex(float xcoord)
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

void V2MSegmentationEditor::AddXCoordInfo()
{
	dataInfo.xInfo.SetSize(0);
	int nPoints = dataInfo.EnergyPoints.Size();
	for(int i = 0;i < nPoints;i++)
	{
		if(i==0 || i == nPoints-1)
		{
			XCoord xcoord;
			xcoord.pointNumber = i;
			xcoord.noteNumber = int(i/2);
			xcoord.energy = dataInfo.EnergyPoints[i].GetY();
			xcoord.freq = dataInfo.FundPoints[i].GetY();
			xcoord.shared = false;
			dataInfo.xInfo.AddElem(xcoord);
		}
		else
		{
			XCoord xcoord;
			if(dataInfo.EnergyPoints[i].GetX() == 
			   dataInfo.EnergyPoints[i+1].GetX())
			{
				// if is not shared mark and go to the next
				xcoord.pointNumber = i+1;
				xcoord.noteNumber = int((i+1)/2);
				xcoord.energy = dataInfo.EnergyPoints[i+1].GetY();
				xcoord.freq = dataInfo.FundPoints[i].GetY();
				xcoord.shared = true;
				dataInfo.xInfo.AddElem(xcoord);
				i++;
			}
			else
			{
				xcoord.pointNumber = i;
				xcoord.noteNumber = int(i/2);
				xcoord.energy = dataInfo.EnergyPoints[i].GetY();
				xcoord.freq = dataInfo.FundPoints[i].GetY();
				xcoord.shared = false;
				dataInfo.xInfo.AddElem(xcoord);
			}
		}

	}
}

void V2MSegmentationEditor::Update(bool prior)
{
	int i = 0;
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
	for(;list_it != _displayList.end();list_it++,i++)
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
		if(i == 2 || i == 4)
		{
			if(deletePoints) 
				(*list_it)->DeletePoint(dataInfo.xInfo[current].pointNumber);
		}
		(*list_it)->updateGL();
	}
	if(deletePoints)
	{
		dataInfo.availablePoints[dataInfo.xInfo[current].pointNumber] = false;
		dataInfo.availablePoints[dataInfo.xInfo[current].pointNumber+1] = false;
	}
	if(flag_s) dataInfo.xInfo[next].shared = true;
	if(flag_n) dataInfo.xInfo[next].shared = false;
	PurgeXCoord(next);
}

void V2MSegmentationEditor::SetCurrent(int index,float xcoord)
{
	DisplayList::iterator list_it = _displayList.begin();
	if(_current == -1)
	{
		_current = index; // index for actual coord to modify
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

int V2MSegmentationEditor::GetSharedPointIndex(int index)
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

void V2MSegmentationEditor::PurgeXCoord(int index)
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
	}
}

void V2MSegmentationEditor::UpdateLabels(TData xcoord,int index,bool back)
{
	if(index < 0 || index > dataInfo.xInfo.Size()-1) return;
	int _index = index;
	QString s;
	QString  _label("");
	TData amplitude,energy1,fund1,energy2,fund2,t1,t2;
	unsigned nindex = int(xcoord);
	int point = dataInfo.xInfo[index].pointNumber;
	if(point == -1) return;
	DisplayList::iterator list_it = _displayList.begin();
	if(!dataInfo.availablePoints[point])
	{
		int tmp = (*list_it)->GetPriorXCoordAvailableIndex(index);
		_index = (tmp != -1) ? tmp : _index;
		point = dataInfo.xInfo[_index].pointNumber;

	}
	DispData::iterator it = _displayData.begin();
    amplitude = fabs(it->data[nindex]);it++;
	_label = "A:  "+(s.setNum(double(amplitude),'f',3));
	amplitudeLabel->setText(_label);
	energy1 = fabs(it->data[nindex]);it++;
	_label = "Energy: "+(s.setNum(double(energy1),'f',3));
	energy1Label->setText(_label);
	fund1 = it->data[nindex];
	_label = "Pitch: "+(s.setNum(double(fund1),'f',3)+" Hz");
	pitch1Label->setText(_label);
	energy2 = fabs(dataInfo.EnergyPoints[point].GetY());
	_label = "Energy: "+(s.setNum(double(energy2),'f',3));
	energy2Label->setText(_label);
	fund2 = dataInfo.FundPoints[point].GetY();
	_label = "Pitch: "+(s.setNum(double(fund2),'f',3)+" Hz");
	pitch2Label->setText(_label);
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

void V2MSegmentationEditor::UpdatePanel(TData xcoord,int index)
{
	int point = dataInfo.xInfo[index].pointNumber;
	bool hasPoint = dataInfo.availablePoints[point];
	if(!hasPoint || point%2!=0) 
		UpdateLabels(xcoord,index,true);
	else
		UpdateLabels(xcoord,index);
}

void V2MSegmentationEditor::ResetLabels()
{
	time1Label->setText("t1: ");
	time2Label->setText("t2: ");
	amplitudeLabel->setText("A: ");
	energy1Label->setText("Energy: ");
	pitch1Label->setText("Pitch: ");
	energy2Label->setText("Energy: ");
	pitch2Label->setText("Pitch: ");
	durationLabel->setText("dur: ");
}

void V2MSegmentationEditor::PushState(int index)
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
	list_it++;list_it++;
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
		state.pX[i] = (*list_it)->GetPointX(state.pIndex[i]);
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

void V2MSegmentationEditor::PopState()
{
	if(!_state.empty())
	{
		State state = _state.front();	
		int i;
		for(i = 0;i < 4;i++)
		{
			if(i < 3)
			{
				if(state.xIndex[i] != -1)
					dataInfo.xInfo[state.xIndex[i]].shared = state.shared[i];
			}
			if(state.pIndex[i] != -1)
				dataInfo.availablePoints[state.pIndex[i]] = state.availP[i];
		}
		i = 0;
		DisplayList::iterator list_it = _displayList.begin();
		for(;list_it != _displayList.end();list_it++,i++)
		{
			(*list_it)->makeCurrent();
			for(int j = 0;j < 4;j++)
			{
				if(j < 3)
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
				if(i == 2 || i ==4)
				{
					if(state.pIndex[j] != -1)
					{	
						if(state.availP[j])
						{
							(*list_it)->WakeUpPoint(state.pIndex[j]);
							(*list_it)->UpdatePoint(state.pIndex[j],state.pX[j]);
						}
					}
				}
			}
			(*list_it)->updateGL();
		}
		_state.pop_front();
	}
}

bool V2MSegmentationEditor::IsModified()
{
	return _modified;
}

Melody V2MSegmentationEditor::GetMelody()
{
	DisplayList::iterator list_it = _displayList.begin();
    list_it++;list_it++;
	Array<Point> energy_points = (*list_it)->GetPoints(); 
	list_it++;list_it++;
	Array<Point> fund_points = (*list_it)->GetPoints();
	Array<int> points_index = (*list_it)->GetPointsIndex();
	List<Note> array;
	Note myNote;
	myNote.AddPitchNote();
	myNote.UpdateData();
	MediaTime time;
	for(int i = 0;i < energy_points.Size();i+=2)
	{
		myNote.SetPitchNote(_melody.GetNoteArray()[points_index[i]/2].GetPitchNote());
		myNote.SetFundFreq(_melody.GetNoteArray()[points_index[i]/2].GetFundFreq());
		myNote.SetEnergy(_melody.GetNoteArray()[points_index[i]/2].GetEnergy());
		time.SetBegin(energy_points[i].GetX()/dataInfo.sampleRate);
		time.SetEnd(energy_points[i+1].GetX()/dataInfo.sampleRate);
		myNote.SetTime(time);
		array.AddElem(myNote);
	}
	Melody melody;
	melody.AddNumberOfNotes();
	melody.UpdateData();
	melody.SetNoteArray(array);
	melody.SetNumberOfNotes(array.Size());
	return melody;
}

void V2MSegmentationEditor::Synchronize(float xcoord)
{
	if(!isVisible()) UpdateVLine(xcoord);
}

void V2MSegmentationEditor::SyncUndo()
{
	if(!isVisible()) undo();
}

void V2MSegmentationEditor::SyncSave()
{
	if(!isVisible()) save();
}

void V2MSegmentationEditor::SyncDiscard()
{
	if(!isVisible()) discard();
}

void V2MSegmentationEditor::InitBounds()
{
	bound.left = 0.0;
	bound.right = 0.0;
}

void V2MSegmentationEditor::SetBounds()
{
	bound.left = TData(_left_scroll_x*TData(dataInfo.size)/TData(dataInfo.width));
	TData right = TData(bound.left+(400.0*TData(dataInfo.size)/TData(dataInfo.width)));
	right -= TData(dataInfo.size);
	bound.right = (right >= 0) ? 0.0 : fabs(right);
	bound.right = TData(dataInfo.size-bound.right);
}

void V2MSegmentationEditor::updateViewport(int x)
{
	_left_scroll_x = x;
	TData value = TData(TData(_left_scroll_x)*360.0/400.0);
	if(isVisible()) emit scrollValue(int(value));
	SetBounds();	
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++)
	{
		(*list_it)->makeCurrent();
		(*list_it)->SetBounds(bound.left,bound.right);
		(*list_it)->updateGL();
	}
}

void V2MSegmentationEditor::SyncScroll(int value)
{
	if(!isVisible()) scrollBar->setValue(value); 
}

// END

