/*
 *	V2MPREditDisplay.cxx
 *	Implementation for V2MPREditDisplay class.
 *
 *	Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include "V2MPREditDisplay.hxx"

V2MPREditDisplay::V2MPREditDisplay(QWidget* parent) : V2MDataEditDisplay(parent)
{
	_dataSize = 0;
	_sampleRate = 1.0;
}

void V2MPREditDisplay::initializeGL()
{
    glClearColor(1.0,1.0,1.0,1.0); // background color: white
}

void V2MPREditDisplay::paintGL()
{
	DrawGrid();
	DrawNotes();
	DrawVLines();
}

void V2MPREditDisplay::DrawGrid()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f,0.0f,0.0f);
	TData step = mView.top/12.0;
	TData y = step;
	// horizontal lines
	glBegin(GL_LINES);
	for(int i = int(step);i < int(mView.top);i+=int(step),y+=step)
	{
		glVertex2f(0.0f,float(y));
		glVertex2f(float(bound.right-bound.left),float(y));
	}
	glEnd();
	// vertical lines
	step = _sampleRate;
	glBegin(GL_LINES);
	for(long j = int(step);j < _dataSize;j+=int(step))
	{
		if(j >= bound.left && j <= bound.right)
		{
			glVertex2f(float(j-bound.left),0.0f);
			glVertex2f(float(j-bound.left),float(mView.top));
		}
	}
	glEnd();
}

void V2MPREditDisplay::DrawNotes()
{
	glBegin(GL_QUADS);
	for(int i = 0;i < _points.Size()-1;i+=2)
	{
		// plot if available and visible
		if(availablePoint[i])
		{
			if(_points[i+1].GetX() >= bound.left && _points[i].GetX() <= bound.right)
			{
				glColor3fv(GetColor(_octave[i/2],_veloc[i/2]));
				DrawNote(i,GetDrawMode(i));
			}
		}
	}
	glEnd();
}

void V2MPREditDisplay::SetNotesInfo(const Array<Point>& points,
									const Array<int>& octave,
									const Array<int>& veloc)
{
	_points.SetSize(0);
	availablePoint.SetSize(0);
	_octave.SetSize(0);
	_veloc.SetSize(0);
	SetPoints(points);
	for(int i = 0;i < octave.Size();i++)
	{
		_octave.AddElem(octave[i]);
		_veloc.AddElem(veloc[i]);
	}
}

float* V2MPREditDisplay::GetColor(int octave,int veloc)
{
	float *v = new float[3];
	switch(octave)
	{	
		case 2:
			v = GetReed(veloc);
			break;
		case 3:
			v = GetMagenta(veloc);
			break;
		case 4:
			v = GetCyan(veloc);
			break;
		case 5:
			v = GetBlue(veloc);
			break;
		case 6:
			v = GetGrayScale(veloc);
			break;
		default:
			// out of range (yellow)
			v[0] = 1.0f;
			v[1] = 1.0f;
			v[2] = 0.0f;
	}

	return v;
}

float* V2MPREditDisplay::GetReed(int veloc)
{
	float *v = new float[3];
	float reed = float(veloc/127.0);
	v[0] = reed;
	v[1] = 0.0f;
	v[2] = 0.0f;
	return v;
}

float* V2MPREditDisplay::GetMagenta(int veloc)
{
	float *v = new float[3];
	float mag_degree = float(veloc/127.0);
	v[0] = 1.0f;
	v[1] = 0.0f;
	v[2] = mag_degree;
	return v;
}

float* V2MPREditDisplay::GetCyan(int veloc)
{
	float *v = new float[3];
	float cyan_degree = float(veloc/127.0);
	v[0] = 0.0f;
	v[1] = cyan_degree;
	v[2] = 1.0f;
	return v;
}

float* V2MPREditDisplay::GetBlue(int veloc)
{
	float *v = new float[3];
	float blue = float(veloc/127.0);
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = blue;
	return v;
}

float* V2MPREditDisplay::GetGrayScale(int veloc)
{
	float *v = new float[3];
	float gray = float(veloc/255.0);
	v[0] = gray;
	v[1] = gray;
	v[2] = gray;
	return v;
}

void V2MPREditDisplay::SetDataSizeAndSampleRate(TSize size,TData sampleRate)
{
	_dataSize = size;
	_sampleRate = sampleRate;
}

void V2MPREditDisplay::DrawNote(int pIndex,int mode)
{
	switch(mode)
	{
		case 1:
				// the note is visible but the points are not
				glVertex2f(0.0f,_points[pIndex].GetY());
				glVertex2f(float(bound.right-bound.left),_points[pIndex].GetY());
				glVertex2f(float(bound.right-bound.left),_points[pIndex+1].GetY());
				glVertex2f(0.0f,_points[pIndex+1].GetY());
				break;
		case 2:
				// the first point is visible but the second is not
				glVertex2f(_points[pIndex].GetX()-bound.left,_points[pIndex].GetY());
				glVertex2f(float(bound.right-bound.left),_points[pIndex].GetY());
				glVertex2f(float(bound.right-bound.left),_points[pIndex+1].GetY());
				glVertex2f(_points[pIndex].GetX()-bound.left,_points[pIndex+1].GetY());
				break;
		case 3:
				// the second point is visible but the first is not
				glVertex2f(0.0f,_points[pIndex].GetY());
				glVertex2f(_points[pIndex+1].GetX()-bound.left,_points[pIndex].GetY());
				glVertex2f(_points[pIndex+1].GetX()-bound.left,_points[pIndex+1].GetY());
				glVertex2f(0.0f,_points[pIndex+1].GetY());
				break;
		case 4:
				// both points are visible
				glVertex2f(_points[pIndex].GetX()-bound.left,_points[pIndex].GetY());
				glVertex2f(_points[pIndex+1].GetX()-bound.left,_points[pIndex].GetY());
				glVertex2f(_points[pIndex+1].GetX()-bound.left,_points[pIndex+1].GetY());
				glVertex2f(_points[pIndex].GetX()-bound.left,_points[pIndex+1].GetY());
				break;
		default:
			    // neither the points nor the note are visible
				break;
	}
}

// END

