/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
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

#include "Tonnetz.hxx"
#include <QtGui/QPixmap>
#include <QtCore/QTimer>
#include <iostream>
#include "FrameDivision.hxx"
#include <CLAM/Pool.hxx>
#include <CLAM/Array.hxx>

CLAM::VM::Tonnetz::~Tonnetz()
{
	if (_dataSource) delete _dataSource;
}
CLAM::VM::Tonnetz::Tonnetz(QWidget * parent) :
	InstantView(parent)
{
	_dataSource = new FloatArrayDataSource; //TODO remove
	_font.setFamily("sans-serif");
	_font.setPointSize(11);
	_updatePending=0;
	_nBins=0;
	_maxValue=1;
	setWhatsThis(tr(
				"<p>The <b>Tonnetz view</b> represents the intensity of each note played.</p>\n"
				"<p>Notes are displayed in a toroidal surface so that harmonic notes are painted closer.\n"
				"Thus, when the played notes are a chord, a mode dependant shape is displayed.\n"
				"Typical shapes are:</p>\n"
				"<ul>\n"
				"<li>Major: Pointing up triangle. The root is the lefter note. (C-E-G)</li>\n"
				"<li>Minor: Pointing down triangle. The root is the lefter note. (C-Eb-G)</li>\n"
				"<li>Fifth: Horitzontal pair. The root is the lefter note. (C-G)</li>\n"
				"<li>Augmented: Forward slash. (C-E-G#)</li>\n"
				"<li>Diminished: Backward slash. (C-Eb-A-F#)</li>\n"
				"<li>Major7: Forward Paralelogram. The root is the lefter note. (C-E-G-B)</li>\n"
				"<li>Minor7: Backward Paralelogram. The root is the lefter note. (C-Eb-G-Bb</li>\n"
				"<li>Dominant7: Pointing up triangle with a tail down. The root is the lefter note. (C-E-G-Bb)</li>\n"
				"<li>MajorMinor7: Pointing down triangle with a tail up. The root is the lefter note. (C-Eb-G-B)</li>\n"
				"<li>Diminished7: Triplet down and one up. The root is the lefter note. (C-Eb-F#-Bb)</li>\n"
				"<li>Minor6: Down triangle plus a head up. The root is the second lefter note. (C-G-A-Eb)</li>\n"
				"<li>Major6: Up triangle plust a head up. The root is the lefter note. (C-G-A-E)</li>\n"
				"<li>Suspended4: Three horizontal. The root is the lefter note. (C-F-G)</li>\n"
				"<li>Suspended2: Three horizontal. The root is the lefter note. (C-D-G)</li>\n"
				"<li>6/9: Five horizontal. The root is the lefter note.</li>\n"
				"</ul>\n"
				));
}

void CLAM::VM::Tonnetz::initializeGL()
{
	glShadeModel(GL_FLAT);
	glClearColor(0,0,0,0); // rgba
	glEnable(GL_BLEND);
	glEnable (GL_LINE_SMOOTH);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_CULL_FACE);
}
void CLAM::VM::Tonnetz::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	glViewport(0 , 0, width, height); // This is to have a square
	double left = 0;
	double bottom = 0;
	double right = 15;
	double top = 5;
	double front = -1;
	double back = 1;
	// keep the aspect ratio
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left,right,bottom,top,front,back);
	glMatrixMode(GL_MODELVIEW);
}
void CLAM::VM::Tonnetz::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Draw();
	swapBuffers(); // TODO: This should not be needed
	_updatePending=0;

}
void CLAM::VM::Tonnetz::Draw()
{
	if (!_nBins) return;
	_maxValue*=0.95;
	if (_maxValue<1e-5) _maxValue=1;
	if (frameData())
		for (unsigned i = 0; i < _nBins; i++)
			if (frameData()[i]>=_maxValue) _maxValue=frameData()[i];
	for (int y = 0; y<4; y++)
		for (int x = 0-y/2; x<10; x++)
			DrawTile(x,y);
	DrawChordsShapes();
	glColor4f(1,1,.5,1);
	for (int y = 0; y<4; y++)
		for (int x = 0-y/2; x<10; x++)
			DrawLabel(x,y);
}

unsigned CLAM::VM::Tonnetz::BinAtPosition(int x, int y)
{
	if (_nBins==12)
		return (x*7+y*4+_nBins*1000)%_nBins; // for pitches
	if (_nBins==24) // for tonality
	{
		bool isminor = y&1;
		return  ((x*7)%(_nBins/2) + 11*(y/2) + (isminor?4:0)  + _nBins*1000)%(_nBins/2) + (isminor?_nBins/2:0);
	}
	return 0;
}


void CLAM::VM::Tonnetz::DrawLabel(int x, int y)
{
	const double sin30 = .5;
	const double cos30 = .8660254;
	const double posx = x*2*cos30+y*cos30;
	const double posy = y*(1+sin30);
	unsigned bin=BinAtPosition(x,y);
	renderText(posx, posy, .6, getLabel(bin).c_str(), _font);
}
void CLAM::VM::Tonnetz::DrawTile(int x, int y)
{
	const double sin30 = .5;
	const double cos30 = .8660254;
	const double posx = x*2*cos30+y*cos30;
	const double posy = y*(1+sin30);
	unsigned bin=BinAtPosition(x,y);
	double pitchLevel = frameData()? frameData()[bin]/_maxValue: 0;
	double hexsize=pitchLevel;
	if (hexsize>1) hexsize = 1;
	glPushMatrix();
		glTranslatef(posx,posy,0);
		glPushMatrix();
			glScalef(hexsize, hexsize, 1.);
			glColor4f(pitchLevel,pitchLevel/3,pitchLevel/3,1);
			glBegin(GL_POLYGON);
				glVertex2f(0,1);
				glVertex2f(-cos30,sin30);
				glVertex2f(-cos30,-sin30);
				glVertex2f(0,-1);
				glVertex2f(cos30,-sin30);
				glVertex2f(cos30,sin30);
			glEnd();
			glLineWidth(3.);
			glColor4f(pitchLevel,pitchLevel/4,pitchLevel/4,1);
			glBegin(GL_LINE_LOOP);
				glVertex3f(0,1,.1);
				glVertex3f(-cos30,sin30,.1);
				glVertex3f(-cos30,-sin30,.1);
				glVertex3f(0,-1,.1);
				glVertex3f(cos30,-sin30,.1);
				glVertex3f(cos30,sin30,.1);
			glEnd();
		glPopMatrix();
		/*
		// This one requires glShademodel(GL_SMOOTH)
		glBegin(GL_TRIANGLE_FAN);
			glColor4f(pitchLevel,pitchLevel,pitchLevel/4,1);
			glVertex3f(0,0,0);
			glColor4f(0,0,0,0);
			glVertex3f(0,1,.1);
			glVertex3f(-cos30,sin30,.1);
			glVertex3f(-cos30,-sin30,.1);
			glVertex3f(0,-1,.1);
			glVertex3f(cos30,-sin30,.1);
			glVertex3f(cos30,sin30,.1);
			glVertex3f(0,1,.1);
		glEnd();
		*/
	glPopMatrix();
}

void CLAM::VM::Tonnetz::DrawChordsShapes()
{
	return;
	const double sin30 = .5;
	const double cos30 = .8660254;
//	const double posx = x*2*cos30+y*cos30;
//	const double posy = y*(1+sin30);
	glPushMatrix();
	glTranslatef(4*cos30+2*cos30,2*1.5,0);
	glLineWidth(5);
	glColor4f(0,.5,1,1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0,0,.2);
	glVertex3f(cos30,1+sin30,.2);
	glVertex3f(2*cos30,0,.2);
	glVertex3f(0,0,.2);
	glEnd();
	glTranslatef(.05,.05,0);
	glColor4f(.5,0,1,1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(0,0,.2);
	glVertex3f(cos30,-1-sin30,.2);
	glVertex3f(2*cos30,0,.2);
	glVertex3f(0,0,.2);
	glEnd();
	glTranslatef(.05,.05,0);
	glColor4f(.5,.5,0,1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(2*cos30,0,.2);
	glVertex3f(0,0,.2);
	glVertex3f(cos30,+1+sin30,.2);
	glVertex3f(2*cos30,0,.2);
	glVertex3f(3*cos30,-1-sin30,.2);
	glEnd();
	glPopMatrix();
}

void CLAM::VM::Tonnetz::setCurrentTime(double timeMiliseconds)
{
	bool mustUpdate = _dataSource->setCurrentTime(timeMiliseconds);
	if (!mustUpdate) return;
	if (!_updatePending++) update();
}

const std::string & CLAM::VM::Tonnetz::getLabel(unsigned bin) const
{
	return _dataSource->getLabel(bin);
}

void CLAM::VM::Tonnetz::setSource(const CLAM_Annotator::Project & project, const std::string & scope, const std::string & name)
{
	_dataSource->setSource(project, scope, name);
	_nBins = _dataSource->nBins();
}

void CLAM::VM::Tonnetz::setSource( FloatArrayDataSource * dataSource )
{
	if (_dataSource) delete _dataSource;
	_dataSource = dataSource;
	_nBins = _dataSource->nBins();
}

void CLAM::VM::Tonnetz::updateData(const CLAM::DescriptionDataPool & data, CLAM::TData samplingRate)
{
	_dataSource->updateData(data, samplingRate);
}


void CLAM::VM::Tonnetz::clearData()
{
	_dataSource->clearData();
	_maxValue=1;
}

CLAM::VM::FloatArrayDataSource::FloatArrayDataSource()
	: _nFrames(0)
	, _frameDivision(0)
	, _samplingRate(44100)
	, _frameData(0)
	, _currentFrame(0)
{
}

void CLAM::VM::FloatArrayDataSource::clearData()
{
	_data.resize(0);
	_nFrames=0;
	_frameDivision=0;
	_frameData=0;
	_currentFrame=0;
}

void CLAM::VM::FloatArrayDataSource::setSource(const CLAM_Annotator::Project & project, const std::string & scope, const std::string & name)
{
	_name = name;
	_scope = scope;
	_project = & project;
	const std::list<std::string> & binLabels=
		project.GetAttributeScheme(scope,name).GetBinLabels();
	_binLabels.assign(binLabels.begin(), binLabels.end());
}

void CLAM::VM::FloatArrayDataSource::updateData(const CLAM::DescriptionDataPool & data, CLAM::TData samplingRate)
{
	_frameData = 0;
	_samplingRate = samplingRate;
	_nFrames = data.GetNumberOfContexts(_scope);
	const CLAM_Annotator::SchemaAttribute & parent =
		_project->GetParentAttribute(_scope);
	_frameDivision = 
		data.GetReadPool<CLAM_Annotator::FrameDivision>(
			parent.GetScope(),
			parent.GetName()
		);
	const CLAM::DataArray * arrays =
		data.GetReadPool<CLAM::DataArray>(_scope,_name);
	unsigned nBins = _binLabels.size();
	_data.resize(_nFrames*nBins);
	for (unsigned frame =0; frame < _nFrames; frame++)
	{
		const CLAM::DataArray & array = arrays[frame];
		for (unsigned i=0; i<nBins; i++)
		{
			// TODO: This nBins is and adhoc hack for normalization
			double value = array[i]*nBins;
			_data[frame*nBins+i] = value;
		}
	}
	_frameData = &_data[0];
}

bool CLAM::VM::FloatArrayDataSource::setCurrentTime(double timeMiliseconds)
{
	unsigned newFrame = _frameDivision ? _frameDivision->GetItem(timeMiliseconds*_samplingRate): 0;
	if (_nFrames==0) newFrame = 0;
	else if (newFrame>=_nFrames) newFrame=_nFrames-1;
	_frameData = getData()? getData()+_binLabels.size()*newFrame : 0;
	if (newFrame == _currentFrame) return false;
	_currentFrame = newFrame;
	return true;
}


