
#include "KeySpace.hxx"
#include <cmath>

struct TKeyNode
{
	float x;
	float y;
};

TKeyNode * getKeyNodes()
{
	static TKeyNode keyNodes[] = 
	{
		{.50,.65}, {.91,.40}, {.33,.15}, {.74,.91}, {.16,.65}, {.60,.40},
		{.01,.15}, {.42,.91}, {.83,.65}, {.25,.40}, {.67,.15}, {.08,.91},
		{.66,.73}, {.08,.50}, {.50,.22}, {.91,.01}, {.33,.73}, {.74,.50},
		{.15,.22}, {.58,.01}, {.01,.73}, {.42,.50}, {.83,.22}, {.25,.01}
	};
	return keyNodes;
}
unsigned nKeyNodes=24;
// The number of 'pixels'
unsigned nX = 150;
unsigned nY = 100;
std::vector<float> weights;


inline void Rectangle(float x, float y, float w, float h)
{
	glBegin(GL_QUADS);
		glVertex2f( x,   y+h );
		glVertex2f( x+w, y+h );
		glVertex2f( x+w, y );
		glVertex2f( x,   y );
	glEnd();
}



CLAM::VM::KeySpace::KeySpace(QWidget * parent) 
	: PcpTorus(parent)
{
	x = y = w = h = 0;
	x_res = 1;
	y_res = 1;

	centroidx_ = 0;
	centroidy_ = 0; 

	ColorsIndex[0] = 0; 
	ColorsIndex[1] = 30;
	ColorsIndex[2] = 70; 
	ColorsIndex[3] = 110;
	ColorsIndex[4] = 145;
	ColorsIndex[5] = 200;
	pRColor[ColorsIndex[0]] = 0;//RGB(0,0,0);
	pRColor[ColorsIndex[1]] = 58/4;//RGB(58/4,68/4,65/4);
	pRColor[ColorsIndex[2]] = 80;//RGB(80,100,153);
	pRColor[ColorsIndex[3]] = 90;//RGB(90,180,100);
	pRColor[ColorsIndex[4]] = 224;//RGB(224,224,44);
	pRColor[ColorsIndex[5]] = 255;//RGB(255,60,30);//RGB(255,155,80);
	pGColor[ColorsIndex[0]] = 0;//RGB(0,0,0);
	pGColor[ColorsIndex[1]] = 68/4;//RGB(58/4,68/4,65/4);
	pGColor[ColorsIndex[2]] = 100;//RGB(80,100,153);
	pGColor[ColorsIndex[3]] = 180;//RGB(90,180,100);
	pGColor[ColorsIndex[4]] = 224;//RGB(224,224,44);
	pGColor[ColorsIndex[5]] = 60;//RGB(255,60,30);//RGB(255,155,80);
	pBColor[ColorsIndex[0]] = 0;//RGB(0,0,0);
	pBColor[ColorsIndex[1]] = 65/4;//RGB(58/4,68/4,65/4);
	pBColor[ColorsIndex[2]] = 153;//RGB(80,100,153);
	pBColor[ColorsIndex[3]] = 100;//RGB(90,180,100);
	pBColor[ColorsIndex[4]] = 44;//RGB(224,224,44);
	pBColor[ColorsIndex[5]] = 30;//RGB(255,60,30);//RGB(255,155,80);
	int n = 0;
	int nDif = 0;
	int i,k;
	for(k=0; k<200; k+=nDif,n++)
	{
		nDif = ColorsIndex[n+1] - ColorsIndex[n];
		for(i=1; i<nDif; i++)
		{
			float Factor = (float)i / nDif;
			/*unsigned char R = GetRValue(pColor[k]) + (GetRValue(pColor[k+nDif]) - GetRValue(pColor[k]))*Factor;
			unsigned char G = GetGValue(pColor[k]) + (GetGValue(pColor[k+nDif]) - GetGValue(pColor[k]))*Factor;
			unsigned char B = GetBValue(pColor[k]) + (GetBValue(pColor[k+nDif]) - GetBValue(pColor[k]))*Factor;*/
			float R = pRColor[k] + (pRColor[k+nDif] - pRColor[k])*Factor;
			float G = pGColor[k] + (pGColor[k+nDif] - pGColor[k])*Factor;
			float B = pBColor[k] + (pBColor[k+nDif] - pBColor[k])*Factor;
			pRColor[k+i] = R;
			pGColor[k+i] = G;
			pBColor[k+i] = B;
		}
	}
	for(k=0; k<=200; k++)
	{
		pRColor[k] /= 255.;
		pGColor[k] /= 255.;
		pBColor[k] /= 255.;
	}
	_maxValue = 1;
	setWhatsThis(tr(
				"<p>The <b>Key Space view</b> represents the probability of each key/chord to be the one played.</p>\n"
				"<p>Mayor chords/keys are displayed with uppercase letters and minor chords/keys are displayed with lowercase letters.\n"
				"Tonally close key/chords are displayed closer so normally you have a color stain covering several chords\n"
				"with the most probable chord as a central color spot.</p>\n"
				));
}

void CLAM::VM::KeySpace::initializeGL()
{
	glShadeModel(GL_FLAT);
	glClearColor(0,0,0,0); // rgba
	glEnable(GL_CULL_FACE);
}
void CLAM::VM::KeySpace::resizeGL(int width, int height)
{
	x = 0;
	y = 0;
	w = width;
	h = height;
	glViewport(x,y,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,x_res,y_res,0,-2,2);
	glMatrixMode(GL_MODELVIEW);
}
void CLAM::VM::KeySpace::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (_frameData) DrawTiles();
	DrawLabels();

	swapBuffers();
	_updatePending=0;
}

void CLAM::VM::KeySpace::DrawTiles()
{
	TKeyNode *pKeyNodes = getKeyNodes();
	if (weights.size()!=nX*nY*nKeyNodes)
	{
		std::cout << "Precomputing KeySpace weights" << std::endl;
		weights.resize(nX*nY*nKeyNodes);
		for(unsigned i=0; i<nX; i++)
		{
			float x1 = i / float(nX) * x_res;
			for(unsigned k=0; k<nY; k++)
			{
				float y1 = k / float(nY) * y_res;
				for(unsigned m=0; m<nKeyNodes; m++)
				{
					double d1 = wdist(x1,pKeyNodes[m].x);
					double d2 = wdist(y1,pKeyNodes[m].y);
					double dist = d1*d1+d2*d2;											   
					double g = dist*dist;
					if (g < 1E-5)
						g = 1E-5;
					weights[m+nKeyNodes*(k+nY*i)] = 1. / g;
				}
			}
		}
	}
	_maxValue*=.5;
	for (unsigned i=0; i<_nBins; i++)
	{
		if (_maxValue<_frameData[i]) _maxValue=_frameData[i];
	}
	if (_maxValue<1e-10) _maxValue=1e-10;

	if (_nBins!=nKeyNodes) return;
	float xStep = x_res/nX;
	float yStep = y_res/nY;
	for(unsigned i=0; i<nX; i++)
	{
		float x1 = i*xStep;
		for(unsigned k=0; k<nY; k++)
		{
			float y1 = k*yStep;
			double num = 0.;
			double den = 0.;
			for(unsigned m=0; m<nKeyNodes; m++)
			{
				unsigned weightIndex = m+nKeyNodes*(k+nY*i);
				num += _frameData[m] * weights[weightIndex] /_maxValue;
				den += weights[weightIndex];
			}
			double value = (den != 0.) ? num / den : 0;

			float ColorIndex = value;
			if (ColorIndex > 1.0)
				ColorIndex = 1.0;	
			ColorIndex *= ColorIndex;
			ColorIndex *= 200.f;
			int cidx = floorf(ColorIndex);
			glColor3d(pRColor[cidx],pGColor[cidx],pBColor[cidx]);
			Rectangle(x1,y1,xStep,yStep);
		}
	}
}

void CLAM::VM::KeySpace::DrawLabels()
{
	TKeyNode *pKeyNodes = getKeyNodes();
	for(unsigned i=0; i<nKeyNodes; i++)
	{
		float x1 = pKeyNodes[i].x * x_res;
		float y1 = pKeyNodes[i].y * y_res;

		if (y1 < 4.*y_res/nY)
			y1 = 4.*y_res/nY;

		float value = _frameData ? _frameData[i]/_maxValue : 0; 
		if (value>.6) glColor3d(0,0,0);
		else          glColor3d(1,1,1);

		renderText(x1, y1, -1, _binLabels[i].c_str());
	}
}
