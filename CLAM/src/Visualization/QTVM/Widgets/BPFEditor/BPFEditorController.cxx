#include "BPFEditorController.hxx"

namespace CLAM
{
    namespace VM
    {
	BPFEditorController::BPFEditorController()
	    : mLeftButtonPressed(false),
	      mRightButtonPressed(false),
	      mKeyInsertPressed(false),
	      mKeyDeletePressed(false),
	      mMouseOverDisplay(false),
	      mProcessingSelection(false),
	      mHit(false),
	      mDisplayWidth(0),
	      mDisplayHeight(0)
	{
	    mData.SetSize(0);
	}

	BPFEditorController::~BPFEditorController()
	{
	}

	void BPFEditorController::SetData(const BPF& bpf)
	{
	    mData = bpf;
	    emit requestRefresh();
	}

	BPF& BPFEditorController::GetData()
	{
	    return mData;
	}

	void BPFEditorController::SetDataColor(const Color& c)
	{
	    mRenderer.SetDataColor(c);
	}

	void BPFEditorController::SetHandlersColor(const Color& c)
	{
	    mRenderer.SetHandlersColor(c);
	}

	void BPFEditorController::SetRectColor(const Color& c)
	{
	    mRectColor = c;
	}

	void BPFEditorController::SetXRange(const double& min, const double& max)
	{
	    mSettingStack.clear();
	    mXRulerRange.mMin = min;
	    mXRulerRange.mMax = max;
	    mView.mLeft = min;
	    mView.mRight = max;
	    emit viewChanged(mView);
	    emit xRulerRange(mXRulerRange.mMin, mXRulerRange.mMax);
	    emit requestRefresh();
	}

	void BPFEditorController::SetYRange(const double& min, const double& max)
	{
	    mSettingStack.clear();
	    mYRulerRange.mMin = min;
	    mYRulerRange.mMax = max;
	    mView.mBottom = min;
	    mView.mTop = max;
	    emit viewChanged(mView);
	    emit yRulerRange(mYRulerRange.mMin, mYRulerRange.mMax);
	    emit requestRefresh();
	}

	void BPFEditorController::SetKeyInsertPressed(bool pressed)
	{
	    mKeyInsertPressed = pressed;
	    mProcessingSelection = false;
	}

	void BPFEditorController::SetKeyDeletePressed(bool pressed)
	{
	    mKeyDeletePressed = pressed;
	    mProcessingSelection = false;
	}
	    
	void BPFEditorController::SetLeftButtonPressed(bool pressed)
	{
	    mLeftButtonPressed = pressed;
	    if(!mLeftButtonPressed)
	    {
		QCursor acursor(Qt::ArrowCursor);
		emit cursorChanged(acursor);
		mHit=false;
	    }
	}

	void BPFEditorController::SetRightButtonPressed(bool pressed)
	{
	    mRightButtonPressed = pressed;
	    if(!mRightButtonPressed)
	    {
		PopSettings();
	    }
	}

	void BPFEditorController::SetPoint(const TData& x, const TData& y)
	{
	    if(mMouseOverDisplay)
	    {
		int mode = GetMode();
		switch (mode)
		{
		    case Selection:
			if(mLeftButtonPressed)
			{
			    mCorners[0].SetX(x);
			    mCorners[0].SetY(y);
			    mCorners[1].SetX(x);
			    mCorners[1].SetY(y);
			    mProcessingSelection=true;
			}
			else
			{
			    if(mProcessingSelection)
			    {
				mProcessingSelection=false;
				mCorners[1].SetX(x);
				mCorners[1].SetY(y);
				PushSettings();
			    }
			}
			break;
		    case Edition:
			if(mProcessingSelection) mProcessingSelection=false;
			if(mLeftButtonPressed)
			{
			    if(!mHit && mKeyInsertPressed)
			    {
				mData.Insert(mCurrentPoint.GetX(),mCurrentPoint.GetY());
				emit requestRefresh();
			    }

			    if(mHit && mKeyDeletePressed)
			    {
				mData.DeleteIndex(mCurrentIndex);
				emit requestRefresh();
			    }
			}
			break;
		    default:
			break;
		}
	    }
	}

	void BPFEditorController::UpdatePoint(const TData& x, const TData& y)
	{
	    if(mMouseOverDisplay)
	    {
		if(mProcessingSelection)
		{
		    mCorners[1].SetX(x);
		    mCorners[1].SetY(y);
		    emit requestRefresh();
		}
		else
		{
		    int index=Hit(x,y);
		    if(index != -1) 
		    {
			if(mKeyDeletePressed)
			{
			    QCursor cCursor(Qt::CrossCursor);
			    emit cursorChanged(cCursor);
			}
			else
			{
			    QCursor sacursor(Qt::SizeAllCursor);
			    emit cursorChanged(sacursor);
			}
			mCurrentIndex=TIndex(index);
			mHit=true;
		    }
		    else
		    {
			mCurrentPoint.SetX(x);
			mCurrentPoint.SetY(y);
			if(!mLeftButtonPressed)
			{
			    QCursor acursor(Qt::ArrowCursor);
			    emit cursorChanged(acursor);
			    mHit=false;
			}
		    }

		    if(mHit && mLeftButtonPressed)
		    {
			if(!mKeyDeletePressed)
			{
			    UpdateBPF(x,y);
			}
		    }
		}

		emit labelsText(QString::number(x,'f',2),QString::number(y,'f',2));
	    }
	}

	void BPFEditorController::MouseOverDisplay(bool over)
	{
	    mMouseOverDisplay = over;
	    if(!mMouseOverDisplay)
	    {
		mHit = false;
		mProcessingSelection=false;
		QCursor acursor(Qt::ArrowCursor);
		emit cursorChanged(acursor);
		emit labelsText(QString(""),QString(""));
	    }
	    emit requestRefresh();
	}

	void BPFEditorController::Draw()
	{
	    mRenderer.SetData(mData);
	    mRenderer.Render();
	    DrawRect();
	}

	void BPFEditorController::PushSettings()
	{
	    BPFEditorSettings settings(mXRulerRange, mYRulerRange, mView);
	    mSettingStack.push_front(settings);
	    mView.mLeft = XMin();
	    mView.mRight = XMax();
	    mView.mBottom = YMin();
	    mView.mTop = YMax();
	    mXRulerRange.mMin = mView.mLeft;
	    mXRulerRange.mMax = mView.mRight;
	    mYRulerRange.mMin = mView.mBottom;
	    mYRulerRange.mMax = mView.mTop;
	    if((mView.mLeft!=mView.mRight)&&(mView.mTop!=mView.mBottom))
	    {
		emit viewChanged(mView);
		emit xRulerRange(mXRulerRange.mMin, mXRulerRange.mMax);
		emit yRulerRange(mYRulerRange.mMin, mYRulerRange.mMax);
		emit requestRefresh();
	    }
	    else
	    {
		PopSettings();
	    }
	}

	void BPFEditorController::PopSettings()
	{
	    if(mSettingStack.size())
	    {
		BPFEditorSettings settings = mSettingStack.front();
		mSettingStack.pop_front();
		mView = settings.mView;
		mXRulerRange = settings.mXRange;
		mYRulerRange = settings.mYRange;
		emit viewChanged(mView);
		emit xRulerRange(mXRulerRange.mMin, mXRulerRange.mMax);
		emit yRulerRange(mYRulerRange.mMin, mYRulerRange.mMax);
		emit requestRefresh();
	    }
	}

	void BPFEditorController::DrawRect()
	{
	    if(mProcessingSelection)
	    {
		glColor3ub(GLubyte(mRectColor.r),GLubyte(mRectColor.g),GLubyte(mRectColor.b));
		glBegin(GL_LINE_STRIP);
		glVertex2f((GLfloat)mCorners[0].GetX(),(GLfloat)mCorners[0].GetY());
		glVertex2f((GLfloat)mCorners[1].GetX(),(GLfloat)mCorners[0].GetY());
		glVertex2f((GLfloat)mCorners[1].GetX(),(GLfloat)mCorners[1].GetY());
		glVertex2f((GLfloat)mCorners[0].GetX(),(GLfloat)mCorners[1].GetY());
		glVertex2f((GLfloat)mCorners[0].GetX(),(GLfloat)mCorners[0].GetY());
		glEnd();
	    }
	}

	double BPFEditorController::XMin()
	{
	    return Min(double(mCorners[0].GetX()), double(mCorners[1].GetX()));
	}

	double BPFEditorController::XMax()
	{
	    return Max(double(mCorners[0].GetX()), double(mCorners[1].GetX()));
	}

	double BPFEditorController::YMin()
	{
	    return Min(double(mCorners[0].GetY()), double(mCorners[1].GetY()));
	}

	double BPFEditorController::YMax()
	{
	    return Max(double(mCorners[0].GetY()), double(mCorners[1].GetY()));
	}

	double BPFEditorController::Max(double a, double b)
	{
	    return (a >= b) ? a : b;
	}

	double BPFEditorController::Min(double a, double b)
	{
	    return (a <= b) ? a : b;
	}

	BPFEditorController::Pixel BPFEditorController::GetPixel(const TData& x, const TData& y)
	{
	    TData width=TData(mDisplayWidth);
	    TData left=mView.mLeft;
	    TData right=mView.mRight;
	    TData xcoord=x-left;
	    TData px=xcoord*width/(right-left);
	    
	    TData height=TData(mDisplayHeight);
	    TData bottom=mView.mBottom;
	    TData top=mView.mTop;
	    TData ycoord=y-bottom;
	    TData py=ycoord*height/(top-bottom);
	    
	    Pixel p;
	    p.x=unsigned(px);
	    p.y=unsigned(py);

	    return p;
	}

	bool BPFEditorController::Match(const Pixel& p, const Pixel& q)
	{
	    return ((abs(int(p.x-q.x))<=1)&&(abs(int(p.y-q.y))<=1));
	}

	TIndex BPFEditorController::Hit(const TData& x, const TData& y)
	{
	    TIndex i;
	    bool hit=false;
	    Pixel selected_pixel=GetPixel(x,y);
	    for(i=0; i < mData.Size(); i++)
	    {
		Pixel owned_pixel=GetPixel(mData.GetXValue(i),mData.GetValueFromIndex(i));
		if(Match(selected_pixel,owned_pixel))
		{
		    hit=true;
		    break;
		}
	    }
	    return (hit) ? i : -1;
	}

	void BPFEditorController::DisplayDimensions(const int& w, const int& h)
	{
	    mDisplayWidth = w;
	    mDisplayHeight = h;
	}

	int BPFEditorController::GetMode()
	{
	    return (!mHit && !mKeyInsertPressed && !mKeyDeletePressed) ? Selection : Edition;
	}

	void BPFEditorController::UpdateBPF(const TData& x, const TData& y)
	{
	    if(mData.Size() == 1) 
	    {
		printf("where is the problem?");
		mData.SetValue(0,y);
		mData.SetXValue(0,x);

		emit requestRefresh();

		return;
	    }

	    if(mCurrentIndex == 0)
	    {
		TData next_x = mData.GetXValue(mCurrentIndex+1);
		if(IsValid(x,next_x))
		{
		    mData.SetValue(mCurrentIndex,y);
		    mData.SetXValue(mCurrentIndex,x);
		}
		else
		{
		    mHit=false;
		    QCursor acursor(Qt::ArrowCursor);
		    emit cursorChanged(acursor);
		}

		emit requestRefresh();

		return;
	    }

	    if(mCurrentIndex==mData.Size()-1)
	    {
		TData prior_x = mData.GetXValue(mCurrentIndex-1);
		if(IsValid(prior_x,x))
		{
		    mData.SetValue(mCurrentIndex,y);
		    mData.SetXValue(mCurrentIndex,x);
		}
		else
		{
		    mHit=false;
		    QCursor acursor(Qt::ArrowCursor);
		    emit cursorChanged(acursor);
		}

		emit requestRefresh();

		return;
	    }
	    
	    TData prior_x = mData.GetXValue(mCurrentIndex-1);
	    TData next_x = mData.GetXValue(mCurrentIndex+1);
	    if(IsValid(prior_x,x) && IsValid(x,next_x))
	    {
		mData.SetValue(mCurrentIndex,y);
		mData.SetXValue(mCurrentIndex,x);
	    }
	    else
	    {
		mHit=false;
		QCursor acursor(Qt::ArrowCursor);
		emit cursorChanged(acursor);
	    }

	    emit requestRefresh();
	}

	bool BPFEditorController::IsValid(const TData& x0, const TData& x1)
	{
	    return (x0 < x1);
	}
    }
}

// END
