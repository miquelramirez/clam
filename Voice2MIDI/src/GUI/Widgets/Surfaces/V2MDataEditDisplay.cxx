/*
 *	V2MDataEditDisplay.cxx
 *	Implementation for V2MDataEditDisplay class.
 *
 *	Ismael Mosquera Rivera PFC Voice2MIDI UPF 2004
*/
#include "V2MDataEditDisplay.hxx"
#include <qevent.h>

V2MDataEditDisplay::V2MDataEditDisplay(QWidget* parent) : V2MDataDisplay(parent)
{
	setMouseTracking(false);
	SetSubSamplingFactor(2);
	SetDrawVLineEnabled(true);
	_dataType = dtAudio;
}

V2MDataEditDisplay::~V2MDataEditDisplay(){}

void V2MDataEditDisplay::SetPoints(const Array<Point>& points)
{
	_points = points;
	availablePoint.Resize(_points.Size());
	availablePoint.SetSize(_points.Size());
	for(int i = 0;i < _points.Size();i++)
	{
		availablePoint[i]=true; // the point is available
	}
}

void V2MDataEditDisplay::SetDataType(dataType dt)
{
	_dataType = dt;
}

void V2MDataEditDisplay::paintGL()
{
	if(_dataType == dtAudio)
		V2MDataDisplay::paintGL();
	else
	{
		DrawDataLines();
	    DrawVLines();
	}
}

void V2MDataEditDisplay::DrawDataLines()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINES);
	for(int i = 0;i < _points.Size()-1;i+=2)
	{
		// plot if available and visible
		if(availablePoint[i])
		{
			if(_points[i+1].GetX() >= bound.left && _points[i].GetX() <= bound.right)
			{
				DrawHLine(i,GetDrawMode(i));
			}
		}
	}
	glEnd();
}

int V2MDataEditDisplay::GetnXCoords()
{
	return globalXCoord.Size();
}

TData V2MDataEditDisplay::GetXCoord(int index)
{
	return globalXCoord[index];
}

void V2MDataEditDisplay::UpdateXCoord(int index,TData value)
{
	globalXCoord[index] = value;
}

void V2MDataEditDisplay::UpdatePoint(int pointNumber,TData xcoord)
{
	_points[pointNumber].SetX(xcoord);
}

void V2MDataEditDisplay::RightButtonMousePressedSlot(Slotv0& slot)
{
	_sendRightButtonPressed.Connect(slot);
}

void V2MDataEditDisplay::mousePressEvent(QMouseEvent* e)
{
	if(e->button() == Qt::RightButton)
		_sendRightButtonPressed.Emit();
}

void V2MDataEditDisplay::mouseMoveEvent(QMouseEvent* e)
{
	xcoord = e->x();
	xcoord *= float(mView.right);
	xcoord /= float(width());
	xcoord += float(bound.left);
	if(xcoord >= bound.left && xcoord <= bound.right)
	{
		mSendXCoord.Emit(xcoord);  // send coord
	}
}

void V2MDataEditDisplay::mouseReleaseEvent(QMouseEvent * e)
{
	mSendXCoord.Emit(-1.0);
}

void V2MDataEditDisplay::DeleteXCoord(int index,bool dd)
{
	availableXCoord[index] = false;
	if(dd) availableXCoord[GetNextXCoordAvailableIndex(index)] = false;
}

void V2MDataEditDisplay::DeletePoint(int index)
{
	availablePoint[index] = false;
	availablePoint[index+1] = false;
}

bool V2MDataEditDisplay::IsAvailableXCoord(int index)
{
	if(index < 0 || index >= availableXCoord.Size()) return false;
	return availableXCoord[index];
}

int V2MDataEditDisplay::GetNextXCoordAvailableIndex(int index)
{
	bool find = false;
	int i = index+1;
	for(;i < availableXCoord.Size();i++)
	{
		if(availableXCoord[i])
		{
			find = true; break;
		}
	}
	return (find) ? i : -1;
}

int V2MDataEditDisplay::GetPriorXCoordAvailableIndex(int index)
{
	int i = index-1;
	bool find = false;
	for(;i >= 0;i--)
	{
		if(availableXCoord[i])
		{
			find = true; break;
		}
	}
	return (find) ? i : -1;
}

void V2MDataEditDisplay::WakeUpXCoord(int index)
{
	availableXCoord[index] = true;
}

void V2MDataEditDisplay::WakeUpPoint(int pointNumber)
{
	availablePoint[pointNumber] = true;
}

TData V2MDataEditDisplay::GetPointX(int index)
{
	
	if(index >= 0 && index < _points.Size())
		return _points[index].GetX();
	else return -1;
}

const Array<Point> V2MDataEditDisplay::GetPoints()
{
	Array<Point> avPoints;
	for(int i = 0;i < availablePoint.Size();i++)
	{
		if(availablePoint[i])
			avPoints.AddElem(_points[i]);
	}
	return avPoints;
}

const Array<int> V2MDataEditDisplay::GetPointsIndex()
{
	Array<int> points_index;
	for(int i = 0;i < availablePoint.Size();i++)
	{
		if(availablePoint[i])
			points_index.AddElem(i);
	}
	return points_index;
}

int V2MDataEditDisplay::GetDrawMode(int pIndex)
{
	// 0 -> neither the points nor the note are visible
	// 1 -> the note is visible but the points are not  
	// 2 -> the first pont is visible but the second is not
	// 3 -> the second point is visible but the first is not
	// 4 -> both points are visible
	TData p1 = _points[pIndex].GetX();
    TData p2 = _points[pIndex+1].GetX();
	if(p1 < bound.left && p2 > bound.right) return 1;
	if(p1 >= bound.left && p2 > bound.right) return 2;
	if(p1 < bound.left && p2 <= bound.right) return 3;
	if(p1 >= bound.left && p2 <= bound.right) return 4;
	return 0;
}

void V2MDataEditDisplay::DrawHLine(int pIndex,int mode)
{
	switch(mode)
	{
		case 1:
				// the note is visible but the points are not
				glVertex2f(0.0f,_points[pIndex].GetY());
				glVertex2f(float(bound.right-bound.left),_points[pIndex+1].GetY());
				break;
		case 2:
				// the first pont is visible but the second is not
				glVertex2f(_points[pIndex].GetX()-bound.left,_points[pIndex].GetY());
				glVertex2f(float(bound.right-bound.left),_points[pIndex+1].GetY());
				break;
		case 3:
				// the second point is visible but the first is not
				glVertex2f(0.0f,_points[pIndex].GetY());
				glVertex2f(_points[pIndex+1].GetX()-bound.left,_points[pIndex+1].GetY());
				break;
		case 4:
				// both points are visible
				glVertex2f(_points[pIndex].GetX()-bound.left,_points[pIndex].GetY());
				glVertex2f(_points[pIndex+1].GetX()-bound.left,_points[pIndex+1].GetY());
				break;
		default:
			    // neither the points nor the note are visible
				break;
	}
}

// END

