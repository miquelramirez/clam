#include "PcpTorus.hxx"
#include <QtGui/QPixmap>
#include <QtCore/QTimer>
#include <iostream>
#include "FrameDivision.hxx"

CLAM::VM::PcpTorus::~PcpTorus()
{
}
CLAM::VM::PcpTorus::PcpTorus(QWidget * parent) :
	QGLWidget(parent)
{
	_font.setFamily("sans-serif");
	_font.setPointSize(11);
	_gradient = bindTexture(QPixmap(":/gradients/gradient.png"), GL_TEXTURE_2D);
	_updatePending=0;
	_currentFrame=0;
	_pcps = 0;
	_pcp = 0;
	_currentFrame = 0;
	initData(1);
	_frameDivision=0;
	_samplingRate=44100;
}

void CLAM::VM::PcpTorus::initializeGL()
{
	glShadeModel(GL_FLAT);
	glClearColor(0,0,0,0); // rgba
	glEnable(GL_BLEND);
	glEnable (GL_LINE_SMOOTH);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_CULL_FACE);
}
void CLAM::VM::PcpTorus::resizeGL(int width, int height)
{
	int side = qMin(width, height);
//	glViewport((width - side) / 2, (height - side) / 2, side, side); // This is to have a square
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
void CLAM::VM::PcpTorus::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	Draw();
	swapBuffers(); // TODO: This should not be needed
	_updatePending=0;

}
void CLAM::VM::PcpTorus::Draw()
{
	_maxValue*=0.95;
	if (_maxValue<1e-10) _maxValue=1;
	for (unsigned i = 0; i < _nBins; i++)
		if (_pcp[i]>=_maxValue) _maxValue=_pcp[i];

	for (int x = -10; x<10; x++)
		for (int y = 0-x; y<10-x; y++)
			DrawTile(x,y);
}

void CLAM::VM::PcpTorus::DrawTile(int x, int y)
{
	const double sin30 = .5;
	const double cos30 = .8660254;
	const double posx = x*2*cos30+y*cos30;
	const double posy = y*(1+sin30);
//	unsigned pitch = (x*7+y*4+_nBins*400)%_nBins; // For pitches
	bool isMinor = y&1;
	unsigned pitch =  ((x*7)%(_nBins/2) + 11*(y/2) + (isMinor?4:0)  + _nBins*400)%(_nBins/2) + (isMinor?_nBins/2:0);
	double pitchLevel = _pcp[pitch]/_maxValue;
	double hexsize=pitchLevel;
	if (hexsize>1) hexsize = 1;
	glPushMatrix();
	glTranslatef(posx,posy,0);
	glScalef(hexsize, hexsize, 1.);
	glColor4f(pitchLevel,pitchLevel/3,pitchLevel/3,1);
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, _gradient);
//	glTexCoord2d(pitchLevel, pitchLevel);
	glBegin(GL_POLYGON);
	glVertex2f(0,1);
	glVertex2f(-cos30,sin30);
	glVertex2f(-cos30,-sin30);
	glVertex2f(0,-1);
	glVertex2f(cos30,-sin30);
	glVertex2f(cos30,sin30);
	glEnd();
	glPopMatrix();
	glColor4f(1,1,.5,1);
	renderText(posx, posy, -1, getLabel(pitch).c_str(), _font);
}



void CLAM::VM::PcpTorus::setCurrentTime(double timeMiliseconds)
{
	if (!_frameDivision) return;
	unsigned newFrame = _frameDivision->GetItem(timeMiliseconds*_samplingRate);
	if (newFrame>=_nPcps) newFrame=0;
	if (newFrame == _currentFrame) return;
	_currentFrame = newFrame;
	_pcp = _pcps + _nBins * _currentFrame;
	if (!_updatePending++) update();
}

const std::string & CLAM::VM::PcpTorus::getLabel(unsigned pitch)
{
	return _binLabels[pitch];
}

void CLAM::VM::PcpTorus::initData(const CLAM_Annotator::FrameDivision & frameDivision, const CLAM::DataArray * arrays, unsigned nFrames, const std::list<std::string> & binLabels, CLAM::TData samplingRate)
{
	_binLabels.assign(binLabels.begin(), binLabels.end());
	_nBins = binLabels.size();
	_frameDivision = & frameDivision;
	_samplingRate = samplingRate;
	if (_pcps) delete[] _pcps;
	_nPcps = nFrames;
	_pcps = new double[_nPcps*_nBins];
	_pcp=_pcps;
	double maxValue = 0;
	double minValue = 0;
	for (unsigned frame =0; frame < _nPcps; frame++)
		for (unsigned i=0; i<_nBins; i++)
		{
			double value = arrays[frame][i]*_nBins;
			if (value>10)
				std::cout << "Weird bin at frame " << frame << " bin " << i << "/" << _nBins << " value " << value << std::endl;
			if (value>maxValue) maxValue = value;
			if (value<minValue) minValue = value;
			_pcps[frame*_nBins+i] = value;
		}
}

void CLAM::VM::PcpTorus::initData(unsigned nFrames)
{
	if (_pcps) delete _pcps;
	_nPcps = nFrames;
	_pcps = new double[_nPcps*_nBins];
	for (unsigned i=0; i<_nBins; i++)
		_pcps[i]=0;

	for (unsigned frame =0; frame < _nPcps-1; frame++)
		for (unsigned i=0; i<_nBins; i++)
		{
			double newValue;
			while (true)
			{
				newValue = _pcps[frame*_nBins+i] + (rand()*2.0/double(RAND_MAX) - 1)/5.0 ;
				if (newValue<0) continue;
				if (newValue>=1) continue;
				break;
			}
			_pcps[(frame+1)*_nBins+i] = newValue;;
		}
	_pcp=_pcps;
}


