/*
 *	V2MAnalysisDisplay.cxx
 *	Implementation for V2MAnalysisDisplay class.
 *
 *	Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include <math.h>
#include <qframe.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlcdnumber.h>
#include <qtooltip.h>
#include "V2MDataDisplay.hxx"
#include "V2MAnalysisDisplay.hxx"


V2MAnalysisDisplay::V2MAnalysisDisplay(QWidget* parent) : QWidget(parent)
{
	setFixedSize(400,368);
	setGeometry(15,35,width(),height());
	hasAnalysis = false;
	buttonsEnabled = true;
	vline = false;
	_lastX = -1.0f;

	////////////////////// INIT DISPLAY ////////////////////////////////
	mReceivedXCoord.Wrap(this,&V2MAnalysisDisplay::UpdateAnalysisPlots);
    InitDisplay();
}

V2MAnalysisDisplay::~V2MAnalysisDisplay(){}

void V2MAnalysisDisplay::InitDisplay()
{ 
	QRect rect(25,25,350,50);
	int left = 0;
    int top = 0;
    int height = 100;
    int width = 400;
    V2MDataDisplay* dataDisplay;
    for(int i = 0;i < 3;i++)
    {
		dataDisplay = new V2MDataDisplay(this);
		dataDisplay->SetXCoordSlot(mReceivedXCoord);
		dataDisplay->setFixedSize(width,height);
		dataDisplay->setGeometry(left,top+i*height,width,height);
		if(i==0)
		{
			dataDisplay->makeCurrent();
			DataArray data(2);
			data.SetSize(2);
			dataDisplay->SetBounds(0,2);
			dataDisplay->SetData(data);
			dataDisplay->updateGL();
			QToolTip::add(dataDisplay,rect,"audio");
		}
		else
		{
			if(i==1) QToolTip::add(dataDisplay,rect,"energy");
			if(i==2) QToolTip::add(dataDisplay,rect,"fundamental");
			dataDisplay->hide();
		}

		_displayList.push_back(dataDisplay);
	}
	
	// labels and buttons for the analysis display
	infoAnalysisPanel = new QFrame(this,"info_analysis_box");
	infoAnalysisPanel->setGeometry(left,top+303,width,65); 
	infoAnalysisPanel->setFrameStyle(QFrame::Panel | QFrame::Plain);
	
	amplitudeLabel = new QLabel(infoAnalysisPanel,"amplitude_lab");
	amplitudeLabel->setGeometry(12,6,60,15);
	amplitudeLabel->setText("A: ");
	energyLabel = new QLabel(infoAnalysisPanel,"energy_label");
	energyLabel->setGeometry(amplitudeLabel->x()+amplitudeLabel->width()+8,6,60,15);
	energyLabel->setText("E: ");
	fundLabel = new QLabel(infoAnalysisPanel,"fund_label");
	fundLabel->setGeometry(energyLabel->x()+energyLabel->width()+8,6,90,15);
	fundLabel->setText("FF: ");
	timeLabel = new QLabel(infoAnalysisPanel,"time_lab");
	timeLabel->setGeometry(fundLabel->x()+fundLabel->width()+8,6,70,15);
	timeLabel->setText("t: ");
	totalTimeLabel = new QLabel(infoAnalysisPanel,"totaltime_lab");
	totalTimeLabel->setGeometry(timeLabel->x()+timeLabel->width()+8,6,70,15);
	totalTimeLabel->setText("tt: ");
	
	first = new QPushButton(infoAnalysisPanel,"first");
	first->setGeometry(125,infoAnalysisPanel->height()-33,25,25);
	first->setText("<<");
	QToolTip::add(first,"first frame");
	prior = new QPushButton(infoAnalysisPanel,"prior");
	prior->setAutoRepeat(true);
	prior->setGeometry(first->x()+first->width(),infoAnalysisPanel->height()-33,25,25);
	prior->setText("<");
	QToolTip::add(prior,"prior frame");
	next = new QPushButton(infoAnalysisPanel,"next");
	next->setGeometry(prior->x()+prior->width()+50,infoAnalysisPanel->height()-33,25,25);
	next->setAutoRepeat(true);
	next->setText(">");
	QToolTip::add(next,"next frame");
	last = new QPushButton(infoAnalysisPanel,"last");
	last->setGeometry(next->x()+next->width(),infoAnalysisPanel->height()-33,25,25);
	last->setText(">>");
	QToolTip::add(last,"last frame");
	
	// connections for the frame control buttons
	connect(first,SIGNAL(clicked()),this,SLOT(FirstFrame()));
	connect(prior,SIGNAL(clicked()),this,SLOT(PriorFrame()));
	connect(next,SIGNAL(clicked()),this,SLOT(NextFrame()));
	connect(last,SIGNAL(clicked()),this,SLOT(LastFrame()));
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	frameNum = new QLCDNumber(3,infoAnalysisPanel,"disp frameNum");
	frameNum->setGeometry(prior->x()+prior->width()+5,infoAnalysisPanel->height()-35,40,25);
	frameNum->setSegmentStyle(QLCDNumber::Flat);	
	frameNum->display(0);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
}

void V2MAnalysisDisplay::ResetAnalysisLabels()
{
	amplitudeLabel->setText("A: ");
	energyLabel->setText("E: ");
	fundLabel->setText("FF: ");
	timeLabel->setText("t: ");
	totalTimeLabel->setText("tt: "); 
}

void V2MAnalysisDisplay::InitData()
{
    _displayData.clear(); // clear
	HideEnergyAndFundDisplays(); // hide the energy and f0 displays
	hasAnalysis = false;  // has not analysis data
	frameNum->display(0); // update LCD display 
}

void V2MAnalysisDisplay::ShowAudioDisplay()
{
	DisplayList::iterator it = _displayList.begin();
	(*it)->show();
	frameNum->display(0); // update LCD display 
	SetAudioDisplayVisible(true);
	ResetLabels();
}

void V2MAnalysisDisplay::HideEnergyAndFundDisplays()
{
	DisplayList::iterator it = _displayList.begin();it++;
	for(;it != _displayList.end();it++)
	(*it)->hide();
}

void V2MAnalysisDisplay::InitFramesInfo(const DispData::iterator it)
{
    framesInfo.nSamples = it->data.Size();
    framesInfo.sampleRate = it->sampleRate;
    framesInfo.pos = 0;
    
    framesInfo.dur = TData(framesInfo.nSamples/framesInfo.sampleRate);
    int ttf = int(framesInfo.dur);
    TData frac = TData(framesInfo.dur-ttf);
    ttf = (frac > 0) ? ttf+1 : ttf;
    
    framesInfo.totalFrames = ttf;
    framesInfo.frameSize = int(framesInfo.sampleRate);
    framesInfo.lastPos = (framesInfo.totalFrames-1)*framesInfo.frameSize;
    framesInfo.rest = framesInfo.nSamples-framesInfo.lastPos;
    framesInfo.nFrame = 1;
}

void V2MAnalysisDisplay::DisplayFrames()
{
	if(!vline)
	{
		_lastX = -1.0f;
		ResetLabels();
	}
	DataArray data;
	if(!buttonsEnabled)
		data = GetXUpdated();
    DisplayList::iterator list_it = _displayList.begin();
    DispData::iterator it = _displayData.begin();
    for(;it != _displayData.end();it++,list_it++)
    {
		(*list_it)->makeCurrent();
		(*list_it)->SetSubSamplingFactor(1);
		(*list_it)->SetDrawVLineEnabled(true);
		if(vline)
		{
			if(!buttonsEnabled)
				(*list_it)->SetGlobalXCoord(data);
		}
		else
		{
			data.SetSize(0);
			(*list_it)->SetGlobalXCoord(data);
		}
		if(framesInfo.nFrame < framesInfo.totalFrames)
		{
			it->data.GiveChunk(framesInfo.pos,framesInfo.frameSize,framesInfo.data);
			(*list_it)->SetView(it->left,framesInfo.frameSize,it->bottom,it->top);
		}
		else
		{
			it->data.GiveChunk(framesInfo.pos,framesInfo.rest,framesInfo.data);
			(*list_it)->SetView(it->left,framesInfo.rest,it->bottom,it->top);
		}
		(*list_it)->SetData(framesInfo.data);
		(*list_it)->SetBounds(0,framesInfo.data.Size());
		(*list_it)->show();
		(*list_it)->updateGL();			
    }
	list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++) (*list_it)->SetDrawVLineEnabled(true);
	if(!buttonsEnabled)
	{
		EnableSpeedButtons();
	}
    frameNum->display(framesInfo.nFrame); // actualizar display lcd 
}

void V2MAnalysisDisplay::FirstFrame()
{
	if(framesInfo.nFrame != 1) 
	{
		vline = false;
		framesInfo.nFrame = 1;
		framesInfo.pos = 0;
		DisplayFrames();
	}
	SetAudioDisplayVisible(false);
}

void V2MAnalysisDisplay::PriorFrame()
{
	if(framesInfo.nFrame != 1)
	{
		vline = false;
	}
    if(framesInfo.nFrame > 1)
    {
		framesInfo.nFrame--;
		framesInfo.pos -= framesInfo.frameSize;
		DisplayFrames();
    }
	SetAudioDisplayVisible(false);
}

void V2MAnalysisDisplay::NextFrame()
{
	if(framesInfo.nFrame != framesInfo.totalFrames)
	{
		vline = false;
	}
    if(framesInfo.nFrame < framesInfo.totalFrames)
    {
		framesInfo.nFrame++;
		framesInfo.pos += framesInfo.frameSize;
		DisplayFrames();
    }
	SetAudioDisplayVisible(false);
}

void V2MAnalysisDisplay::LastFrame()
{
	if(framesInfo.nFrame != framesInfo.totalFrames) 
	{
		vline = false;
		framesInfo.nFrame = framesInfo.totalFrames;
		framesInfo.pos = framesInfo.lastPos;
		DisplayFrames();
	}
	SetAudioDisplayVisible(false);
}

void V2MAnalysisDisplay::PlotOriginalAudio()
{
	ShowAudioDisplay();
    DispData::iterator it = _displayData.begin();
    DisplayList::iterator list_it = _displayList.begin();
	(*list_it)->makeCurrent();
	(*list_it)->SetView(it->left,it->right,it->bottom,it->top);	
	(*list_it)->SetDrawVLineEnabled(false);
	(*list_it)->SetData(it->data);
	(*list_it)->SetBounds(0,TData(it->data.Size()));
	(*list_it)->updateGL();
	list_it++;
	for(;list_it != _displayList.end();list_it++)
	{	
		(*list_it)->hide();	
	}
	SetAudioDisplayVisible(true);
}

void V2MAnalysisDisplay::DisplayCurrentFrameAndVLines(float xcoord)
{
	_lastX = xcoord;
	vline = true;
	DataArray data;
	data.AddElem(xcoord);
	DisplayList::iterator it = _displayList.begin();
    for(;it != _displayList.end();it++)
    {
		(*it)->SetGlobalXCoord(data);
		if(!buttonsEnabled)
		{
		  (*it)->makeCurrent();
		  (*it)->updateGL();
		}
    }
	if(buttonsEnabled) DisplayFrames();
	SetAudioDisplayVisible(false);
}

void V2MAnalysisDisplay::UpdateAnalysisPlots(float xcoord)
{
    if(buttonsEnabled)
	{
		UpdatePanelAndVLine(xcoord);
	}
	else
	{
		UpdatePanelAndVLine(xcoord,true);
	}
}

void V2MAnalysisDisplay::UpdateDynamicDisplay(DataArray& data)
{
	if(IsAudioDisplayVisible())
	{
		DisplayList::iterator it = _displayList.begin();
		(*it)->SetData(data);
		(*it)->update();
	}
}

void V2MAnalysisDisplay::InitDisplayForDynamicBehavior()
{
	if(IsAudioDisplayVisible())
	{
		// init dynamic display	
		DisplayList::iterator it = _displayList.begin();
		(*it)->makeCurrent();
		(*it)->SetBounds(0,2048);
		(*it)->SetView(0.0,2048.0,-1.0,1.0);
		// end init dynamic display
	}
}

void V2MAnalysisDisplay::SetAudioData(std::list<DisplayData> data)
{
	_displayData = data;
	DispData::iterator it = _displayData.begin();
	InitFramesInfo(it); // init frames info
	hasAnalysis = false;
	QString s;
	QString _label("");
	TData totalTime = framesInfo.dur;
	_label += "tt:  "+(s.setNum(double(totalTime),'f',3))+" s";
	totalTimeLabel->setText(_label); 
}

void V2MAnalysisDisplay::SetAnalysisData(std::list<DisplayData> data)
{
	_lastX = -1.0f;
	DataArray empty; // init with empty data
	DisplayList::iterator list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++)
		(*list_it)->SetGlobalXCoord(empty);
	std::list<DisplayData>::iterator it = data.begin();
    for(;it != data.end();it++)
		_displayData.push_back((*it));
    hasAnalysis = true;
}

void V2MAnalysisDisplay::SetInfoAnalysisPanelEnabled(bool en)
{
	infoAnalysisPanel->setEnabled(en);
}

const std::list<DisplayData> V2MAnalysisDisplay::GetAnalysisData()
{
	return _displayData;
}

void V2MAnalysisDisplay::ShowCurrentFrame()
{
	if(framesInfo.nFrame == 0)
		FirstFrame();
	else
		DisplayFrames();
	UpdatePanelAndVLine(_lastX);
	SetAudioDisplayVisible(false);
}

void V2MAnalysisDisplay::SetAudioDisplayVisible(bool v)
{
	isAudioDisplayVisible = v;
}

bool V2MAnalysisDisplay::IsAudioDisplayVisible()
{
	return isAudioDisplayVisible;
}

void V2MAnalysisDisplay::ResetLabels()
{
	amplitudeLabel->setText("A: ");
	energyLabel->setText("E: ");
	fundLabel->setText("FF: ");
	timeLabel->setText("t: ");
}

void V2MAnalysisDisplay::hide()
{
	QWidget::hide();
	ResetLabels();
}

void V2MAnalysisDisplay::EnableSpeedButtons()
{
	first->setEnabled(true);
	last->setEnabled(true);
	prior->setEnabled(true);
	next->setEnabled(true);
	frameNum->setEnabled(true);
	buttonsEnabled = true;
}

void V2MAnalysisDisplay::DisableSpeedButtons()
{
	first->setEnabled(false);
	last->setEnabled(false);
	prior->setEnabled(false);
	next->setEnabled(false);
	frameNum->setEnabled(false);
	buttonsEnabled = false;
}

void V2MAnalysisDisplay::ShowAllData()
{
	if(!vline)
	{
		_lastX = -1.0f;
		ResetLabels();
	}
	int factor = 1;
	if(framesInfo.dur >= 4)
		factor = int(framesInfo.dur/2);
	DataArray data; 
	if(buttonsEnabled)
	  data = GetXUpdated(true);
	SetAudioDisplayVisible(false);
	DisplayList::iterator list_it = _displayList.begin();
    DispData::iterator it = _displayData.begin();
    for(;it != _displayData.end();it++,list_it++)
    {
		(*list_it)->makeCurrent();
		(*list_it)->SetSubSamplingFactor(factor);
		(*list_it)->SetDrawVLineEnabled(true);
		if(vline)
		{
			if(buttonsEnabled)
				(*list_it)->SetGlobalXCoord(data);	
		}
		else
		{
			data.SetSize(0);
			(*list_it)->SetGlobalXCoord(data);
		}
		(*list_it)->SetView(it->left,it->right,it->bottom,it->top);
		(*list_it)->SetData(it->data);
		(*list_it)->SetBounds(0,TData(it->data.Size()));
		(*list_it)->show();
		(*list_it)->updateGL();			
    }
	list_it = _displayList.begin();
	for(;list_it != _displayList.end();list_it++) (*list_it)->SetDrawVLineEnabled(true);
	if(buttonsEnabled)
	{
		DisableSpeedButtons();
	}
	frameNum->display(0); // actualizar display lcd
	UpdatePanelAndVLine(_lastX,true);
	SetAudioDisplayVisible(false);
}

void V2MAnalysisDisplay::UpdatePanelAndVLine(float xcoord,bool full)
{
	if(xcoord != -1)
	{
		DisplayList::iterator list_it = _displayList.begin();
		if((*list_it)->NeedVLines())
		{
			unsigned int index;
			TData amplitude,energy,fundFreq,time;
			if(!full)
			{
				index = int(framesInfo.pos+xcoord);
				time = TData((framesInfo.pos+xcoord)/framesInfo.sampleRate);
			}
			else
			{
				index = int(xcoord);
				time = TData(xcoord/framesInfo.sampleRate);
			}
			QString s;
			QString _label("");
			DisplayCurrentFrameAndVLines(xcoord);
			DispData::iterator it = _displayData.begin();
			amplitude = fabs(it->data[index]);it++;
			_label = "A:  "+(s.setNum(double(amplitude),'f',3));
			amplitudeLabel->setText(_label);
			energy = it->data[index];it++;
			_label = "E:  "+(s.setNum(double(energy),'f',3));
			energyLabel->setText(_label);
			fundFreq = it->data[index];
			_label = "FF:  "+(s.setNum(double(fundFreq),'f',3))+" Hz";
			fundLabel->setText(_label);
			_label = "t:  "+(s.setNum(double(time),'f',3))+" s";
			timeLabel->setText(_label);  
		}
	}
}

DataArray V2MAnalysisDisplay::GetXUpdated(bool full)
{
	DataArray data;
	if(_lastX != -1)
	{
		if(full)
		{
			_lastX += framesInfo.pos;
			data.AddElem(_lastX);
		}
		else
		{
			framesInfo.nFrame = int(_lastX/framesInfo.frameSize);
			framesInfo.pos = framesInfo.nFrame*framesInfo.frameSize;
			framesInfo.nFrame++;
			_lastX -= framesInfo.pos;
			data.AddElem(_lastX);
		}
	}
	return data;
}

// END

