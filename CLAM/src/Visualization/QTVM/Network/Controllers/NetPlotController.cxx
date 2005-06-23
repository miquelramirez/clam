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
			: mSamples(0)
			, mVMin(TData(0.0))
			, mVMax(TData(1.0))
			, mFirst(true)
			, mMonitorIsRunning(false)
			, mCanGetData(true)
			, mCanSendData(false)
			, mLeftButtonPressed(false)
			, mRightButtonPressed(false)
		{
			mStartSlot.Wrap(this,&NetPlotController::ConcreteStartMonitor);
			mStopSlot.Wrap(this,&NetPlotController::ConcreteStopMonitor);
		}
	
		NetPlotController::~NetPlotController()
		{
		}
	        
		void NetPlotController::SetnSamples(const TSize& nSamples)
		{
			mSamples = nSamples;
		}

		TSize NetPlotController::GetnSamples() const
		{
			return mSamples;
		}

		void NetPlotController::SetvRange(const TData& vmin, const TData& vmax)
		{
			mVMin = vmin;
			mVMax = vmax;
		}

		TData NetPlotController::GetvMin() const
		{
			return mVMin;
		}

		TData NetPlotController::GetvMax() const
		{
			return mVMax;
		}
     
		void NetPlotController::ConcreteStartMonitor()
		{
			mViewStack.clear();
			mMonitorIsRunning = true;
		}

		void NetPlotController::ConcreteStopMonitor()
		{
			mMonitorIsRunning=false;
			SetFirst(true);
		}

		bool NetPlotController::MonitorIsRunning()
		{
			return mMonitorIsRunning;
		}

		void NetPlotController::SetFirst(bool first)
		{
			mFirst=first;
		}

		bool NetPlotController::First()
		{
			return mFirst;
		}

		void NetPlotController::SetCanGetData(bool canget)
		{
			mCanGetData=canget;
		}

		void NetPlotController::SetCanSendData(bool cansend)
		{
			mCanSendData=cansend;
		}

		bool NetPlotController::CanGetData()
		{
			return mCanGetData;
		}

		bool NetPlotController::CanSendData()
		{
			return mCanSendData;
		}
	
		void NetPlotController::SetPoint(const TData& x, const TData& y)
		{
			if(mLeftButtonPressed)
			{
				mCorners[0].SetX(AdjustX(x));
				mCorners[0].SetY(AdjustY(y));
				mCorners[1].SetX(AdjustX(x));
				mCorners[1].SetY(AdjustY(y));
			}
			else
			{
				mCorners[1].SetX(AdjustX(x));
				mCorners[1].SetY(AdjustY(y));
				PushView();
			}
		}

		void NetPlotController::UpdatePoint(const TData& x, const TData& y)
		{
			if(mLeftButtonPressed)
			{
				mCorners[1].SetX(AdjustX(x));
				mCorners[1].SetY(AdjustY(y));
			}
		}

		void NetPlotController::SetLeftButtonPressed(bool pressed)
		{
			mLeftButtonPressed=pressed;
		}

		void NetPlotController::SetRightButtonPressed(bool pressed)
		{
			mRightButtonPressed=pressed;
			if(!mRightButtonPressed)
			{
				PopView();
			}
		}

		void NetPlotController::Draw()
		{
			if(mLeftButtonPressed)
			{
				Color c = VMColor::Yellow();
				glColor3ub(GLubyte(c.r),GLubyte(c.g),GLubyte(c.b));
				glBegin(GL_LINE_STRIP);
				glVertex2f((GLfloat)mCorners[0].GetX(),(GLfloat)mCorners[0].GetY());
				glVertex2f((GLfloat)mCorners[1].GetX(),(GLfloat)mCorners[0].GetY());
				glVertex2f((GLfloat)mCorners[1].GetX(),(GLfloat)mCorners[1].GetY());
				glVertex2f((GLfloat)mCorners[0].GetX(),(GLfloat)mCorners[1].GetY());
				glVertex2f((GLfloat)mCorners[0].GetX(),(GLfloat)mCorners[0].GetY());
				glEnd();
			}
		}

		void NetPlotController::PushView()
		{
			SView v = mView;
			mViewStack.push_front(v);
			mView.left=XMin();
			mView.right=XMax();
			mView.top=YMax();
			mView.bottom=YMin();
			if((mView.left!=mView.right)&&(mView.top!=mView.bottom))
			{
				emit sendView(mView);
			}
			else
			{
				PopView();
			}
		}

		void NetPlotController::PopView()
		{
			if(mViewStack.size())
			{
				mView=mViewStack.front();
				mViewStack.pop_front();
				emit sendView(mView);
			}
		}

		float NetPlotController::XMax()
		{
			return Max(mCorners[0].GetX(), mCorners[1].GetX());
		}

		float NetPlotController::XMin()
		{
			return Min(mCorners[0].GetX(), mCorners[1].GetX());
		}

		float NetPlotController::YMax()
		{
			return Max(mCorners[0].GetY(), mCorners[1].GetY());
		}

		float NetPlotController::YMin()
		{
			return Min(mCorners[0].GetY(), mCorners[1].GetY());
		}

		float NetPlotController::Max(float a, float b)
		{
			return (a >= b) ? a : b;
		}

		float NetPlotController::Min(float a, float b)
		{
			return (a <= b) ? a : b;
		}

		TData NetPlotController::AdjustX(const TData& value)
		{
			TData x = value;
			if(x < 0) x = 0;
			if(x > GetnSamples()) x = TData(GetnSamples());
			return x;
		}

       	TData NetPlotController::AdjustY(const TData& value)
		{
			TData y = value;
			if(y < GetvMin()) y = GetvMin();
			if(y > GetvMax()) y = GetvMax();
			return y;
		}
	    
    }
}

// END


