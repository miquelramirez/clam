#include "NetPlotController.hxx"
#include "CLAMGL.hxx"
#include "VMColor.hxx"

namespace CLAM
{
    namespace VM
    {

	SView::SView()
	{
	    left = 0.0f;
	    right = 1.0f;
	    bottom = 0.0f;
	    top = 1.0f;
	}

	SView::SView(float l, float r, float t, float b)
	{
	    left=l;
	    right=r;
	    top=t;
	    bottom=b;
	}

	SView::SView(const SView& v)
	{
	    left=v.left;
	    right=v.right;
	    top=v.top;
	    bottom=v.bottom;
	}

	SView::~SView()
	{
	}
	    
	void SView::operator=(SView v)
	{
	    left=v.left;
	    right=v.right;
	    top=v.top;
	    bottom=v.bottom;
	}

	NetPlotController::NetPlotController()
	{
	    _monitorIsRunning=false;
	    _mouseOverDisplay=false;
		
	    SetnSamples(0);
	    SetvRange(TData(0.0),TData(1.0));
	    SetLeftButtonPressed(false);
	    SetRightButtonPressed(false);
	    SetFirst(true);
	    SetCanGetData(true);
	    SetCanSendData(false);

	    mStartSlot.Wrap(this,&NetPlotController::ConcreteStartMonitor);
	    mStopSlot.Wrap(this,&NetPlotController::ConcreteStopMonitor);
	}
	
	NetPlotController::~NetPlotController()
	{
	}
	        
	void NetPlotController::SetnSamples(const TSize& nSamples)
	{
	    _nSamples = nSamples;
	}

	TSize NetPlotController::GetnSamples() const
	{
	    return _nSamples;
	}

	void NetPlotController::SetvRange(const TData& vmin, const TData& vmax)
	{
	    _vmin = vmin;
	    _vmax = vmax;
	}

	TData NetPlotController::GetvMin() const
	{
	    return _vmin;
	}

	TData NetPlotController::GetvMax() const
	{
	    return _vmax;
	}
     
	void NetPlotController::ConcreteStartMonitor()
	{
	    _viewStack.clear();
	    _monitorIsRunning = true;
	}

	void NetPlotController::ConcreteStopMonitor()
	{
	    _monitorIsRunning=false;
	    SetFirst(true);
	}

	bool NetPlotController::MonitorIsRunning()
	{
	    return _monitorIsRunning;
	}

	void NetPlotController::SetFirst(bool first)
	{
	    _first=first;
	}

	bool NetPlotController::First()
	{
	    return _first;
	}

	void NetPlotController::SetCanGetData(bool canget)
	{
	    _canGetData=canget;
	}

	void NetPlotController::SetCanSendData(bool cansend)
	{
	    _canSendData=cansend;
	}

	bool NetPlotController::CanGetData()
	{
	    return _canGetData;
	}

	bool NetPlotController::CanSendData()
	{
	    return _canSendData;
	}
	
	void NetPlotController::SetPoint(const TData& x, const TData& y)
	{
	    if(_mouseOverDisplay)
	    {
		if(_leftButtonPressed)
		{
		    _corners[0].SetX(x);
		    _corners[0].SetY(y);
		    _corners[1].SetX(x);
		    _corners[1].SetY(y);
		}
		else
		{
		    _corners[1].SetX(x);
		    _corners[1].SetY(y);
		    PushView();
		}
	    }
	}

	void NetPlotController::UpdatePoint(const TData& x, const TData& y)
	{
	    if(_leftButtonPressed)
	    {
		_corners[1].SetX(x);
		_corners[1].SetY(y);
	    }
	}

	void NetPlotController::SetLeftButtonPressed(bool pressed)
	{
	    _leftButtonPressed=pressed;
	}

	void NetPlotController::SetRightButtonPressed(bool pressed)
	{
	    _rightButtonPressed=pressed;
	    if(!_rightButtonPressed)
	    {
		PopView();
	    }
	}

	void NetPlotController::Draw()
	{
	    if(_leftButtonPressed)
	    {
		Color c = VMColor::Yellow();
		glColor3ub(GLubyte(c.r),GLubyte(c.g),GLubyte(c.b));
		glBegin(GL_LINE_STRIP);
		glVertex2f((GLfloat)_corners[0].GetX(),(GLfloat)_corners[0].GetY());
		glVertex2f((GLfloat)_corners[1].GetX(),(GLfloat)_corners[0].GetY());
		glVertex2f((GLfloat)_corners[1].GetX(),(GLfloat)_corners[1].GetY());
		glVertex2f((GLfloat)_corners[0].GetX(),(GLfloat)_corners[1].GetY());
		glVertex2f((GLfloat)_corners[0].GetX(),(GLfloat)_corners[0].GetY());
		glEnd();
	    }
	}

	void NetPlotController::EnterEvent()
	{
	    _mouseOverDisplay=true;
	}

	void NetPlotController::LeaveEvent()
	{
	    _mouseOverDisplay=false;
	}

	void NetPlotController::PushView()
	{
	    SView v = _view;
	    _viewStack.push_front(v);
	    _view.left=XMin();
	    _view.right=XMax();
	    _view.top=YMax();
	    _view.bottom=YMin();
	    if((_view.left!=_view.right)&&(_view.top!=_view.bottom))
	    {
		emit sendView(_view);
	    }
	    else
	    {
		PopView();
	    }
	}

	void NetPlotController::PopView()
	{
	    if(_viewStack.size())
	    {
		_view=_viewStack.front();
		_viewStack.pop_front();
		emit sendView(_view);
	    }
	}

	float NetPlotController::XMax()
	{
	    return Max(_corners[0].GetX(), _corners[1].GetX());
	}

	float NetPlotController::XMin()
	{
	    return Min(_corners[0].GetX(), _corners[1].GetX());
	}

	float NetPlotController::YMax()
	{
	    return Max(_corners[0].GetY(), _corners[1].GetY());
	}

	float NetPlotController::YMin()
	{
	    return Min(_corners[0].GetY(), _corners[1].GetY());
	}

	float NetPlotController::Max(float a, float b)
	{
	    return (a >= b) ? a : b;
	}

	float NetPlotController::Min(float a, float b)
	{
	    return (a <= b) ? a : b;
	}
	    
    }
}

// END


