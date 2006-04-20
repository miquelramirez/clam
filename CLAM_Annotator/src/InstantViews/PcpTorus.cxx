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
	unsigned i=0;
	for (int x = -10; x<10; x++)
	{
		for (int y = -10; y<10; y++)
		{
			DrawTile(x,y);
			i++;
		}
	}
}

void CLAM::VM::PcpTorus::DrawTile(int x, int y)
{
	const double sin30 = .5;
	const double cos30 = .8660254;
	const double posx = x*2*cos30+y*cos30;
	const double posy = y*(1+sin30);
	unsigned pitch = (x*7+y*4+_nBins*400)%_nBins;
	double pitchLevel = _pcp[pitch];
	glPushMatrix();
	glTranslatef(posx,posy,0);
	glScalef(pitchLevel, pitchLevel, 1.);
	glColor4f(pitchLevel,pitchLevel/3,pitchLevel/3,1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _gradient);
	glTexCoord2d(pitchLevel, pitchLevel);
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
	unsigned newFrame = _frameDivision->GetItem(timeMiliseconds*44100);
	if (_currentFrame>=_nPcps) _currentFrame=0;
	if (newFrame == _currentFrame) return;
	_currentFrame = newFrame;
	_pcp = _pcps + _nBins * _currentFrame;
	if (!_updatePending++) update();
}

const std::string & CLAM::VM::PcpTorus::getLabel(unsigned pitch)
{
	return _binLabels[pitch];
}

void CLAM::VM::PcpTorus::initData(const CLAM_Annotator::FrameDivision & frameDivision, const CLAM::DataArray * arrays, unsigned nFrames, const std::list<std::string> & binLabels)
{
	_binLabels.assign(binLabels.begin(), binLabels.end());
	_nBins = binLabels.size();
	_frameDivision = & frameDivision;
	if (_pcps) delete _pcps;
	_nPcps = nFrames;
	_pcps = new double[_nPcps*_nBins];
	for (unsigned frame =0; frame < _nPcps; frame++)
		for (unsigned i=0; i<_nBins; i++)
		{
			_pcps[(frame)*_nBins+i] = arrays[frame][i]*_nBins;
		}
	_pcp=_pcps;
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


