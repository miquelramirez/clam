/*
 *	V2MDataDisplay.cxx
 *	Implementation for V2MDataDisplay class.
 *
 *	Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include <qevent.h>
#include "V2MDataDisplay.hxx"

V2MDataDisplay::V2MDataDisplay(QWidget* parent) : QGLWidget(parent)
{
	InitBounds();
    InitView();
    SetDefaultColor();
	needVLine = false;
	SetSubSamplingFactor(1);
	xcoord = 0.0f;
}

V2MDataDisplay::~V2MDataDisplay(){}

void V2MDataDisplay::InitBounds()
{
	bound.left = 0.0;
	bound.right = 0.0;
}

void V2MDataDisplay::SetBounds(TData left,TData right)
{
	bound.left = left;
	bound.right = right;
}

void V2MDataDisplay::InitView()
{
    mView.top = 1.0;
    mView.bottom = -1.0;
    mView.left = 0.0;
    mView.right = 1.0;
}

void V2MDataDisplay::SetView(TData left,TData right,TData bottom,TData top)
{
    mView.left = left;
    mView.right = right;
    mView.bottom = bottom;
    mView.top = top;
    resizeGL(width(),height());
}

void V2MDataDisplay::SetData(const DataArray& data)
{
	mData = data;
}

void V2MDataDisplay::initializeGL()
{
    glClearColor(0.0,0.0,0.0,1.0); // background color: black
}

void V2MDataDisplay::resizeGL(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(mView.left,mView.right,mView.bottom,mView.top,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
}

void V2MDataDisplay::paintGL()
{
	unsigned left = int(bound.left);
	unsigned right = int(bound.right);
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(color.r,color.g,color.b);
	glBegin(GL_LINE_STRIP);
	for(unsigned i = left;i < right;i+=subSamplingFactor)
		glVertex2f(float(i)-bound.left,float(mData[i]));
	glEnd();
	if(needVLine)
		DrawVLines();
}

void V2MDataDisplay::SetSubSamplingFactor(int factor)
{
	subSamplingFactor = factor;
}

void V2MDataDisplay::mousePressEvent(QMouseEvent * e) 
{
	xcoord = e->x();
	xcoord *= float(mView.right);
	xcoord /= float(width());
	mSendXCoord.Emit(xcoord);  // send coord
}

void V2MDataDisplay::SetDrawVLineEnabled(bool s)
{
	needVLine = s;
}

bool V2MDataDisplay::NeedVLines()
{
    return needVLine;
}

void V2MDataDisplay::SetXCoordSlot(SigSlot::Slotv1<float>& slot)
{
	mSendXCoord.Connect(slot);	
}

void V2MDataDisplay::DrawVLines()
{
	float left = float(bound.left);
	float right = float(bound.right);
	glColor3f(1.0f,0.0f,0.0f);
	glBegin(GL_LINES);
	for(int i = 0;i < globalXCoord.Size();i++)
	{
		// plot if available and visible
		if(availableXCoord[i])
		{
			if(globalXCoord[i] >= left && globalXCoord[i] <= right)
			{
				glVertex2f(float(globalXCoord[i]-bound.left),float(mView.top));
				glVertex2f(float(globalXCoord[i]-bound.left),float(mView.bottom));
			}
		}
	}
	glEnd();
	glFlush();
}

void V2MDataDisplay::SetGlobalXCoord(const DataArray& xcoords)
{
	globalXCoord.SetSize(0);
	availableXCoord.SetSize(0);
	for(int i = 0;i < xcoords.Size();i++)
	{
		globalXCoord.AddElem(xcoords[i]);  // anyadir coordenada
		availableXCoord.AddElem(true);    // coordenada disponible
	}
}

void V2MDataDisplay::SetColor(float r,float g,float b)
{
	color.r = r;
	color.g = g;
	color.b = b;
}

void V2MDataDisplay::SetDefaultColor()
{
	color.r = 0.0f;
	color.g = 1.0f;
	color.b = 0.0f;
}

// END

