#include "BPFEditorController.hxx"

namespace CLAM
{
	namespace VM
	{
		BPFEditorController::BPFEditorController(int eFlags)
			: mEFlags(eFlags) 
			, mXScale(EScale(EScale::eLinear))
			, mYScale(EScale(EScale::eLinear))
			, mLeftButtonPressed(false)
			, mRightButtonPressed(false)
			, mKeyInsertPressed(false)
			, mKeyDeletePressed(false)
			, mProcessingSelection(false)
			, mHit(false)
			, mDisplayWidth(0)
			, mDisplayHeight(0)
			, mCurrentIndex(0)
			, mXModified(false)
			, mYModified(false)
			, mSelectPoint(false)
			, mSpanX(0.0), mSpanY(0.0)
			, mMinSpanX(1.0), mMinSpanY(1.0)
			, mMinX(0.0), mMaxX(0.0)
			, mMinY(0.0), mMaxY(0.0)
			, mVCurrent(0.0)
			, mVZoomRatio(1.0)
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

		void BPFEditorController::SetXRange(const double& min, const double& max, const EScale& scale)
		{
			mXScale = scale;
			if(min >= max) return;
			mSpanX = max-min;
			mMinX = min;
			mMaxX = max;
			mSettingStack.clear();
			mXRulerRange.mMin = min;
			mXRulerRange.mMax = max;
			mView.mLeft = min;
			mView.mRight = max;
			emit viewChanged(mView);
			emit xRulerRange(mXRulerRange.mMin, mXRulerRange.mMax);
			emit requestRefresh();
		}

		void BPFEditorController::SetYRange(const double& min, const double& max, const EScale& scale)
		{
			mYScale = scale;
			if(min >= max) return;
			mSpanY = max-min;
			mMinY = min;
			mMaxY = max;
			mSettingStack.clear();
			mYRulerRange.mMin = min;
			mYRulerRange.mMax = max;
			mView.mBottom = min;
			mView.mTop = max;
			emit viewChanged(mView);
			emit yRulerRange(mYRulerRange.mMin, mYRulerRange.mMax);
			emit requestRefresh();
			InitVScroll();
		}

		void BPFEditorController::SetXScale(const EScale& scale)
		{
			mXScale = scale;
		}

		void BPFEditorController::SetYScale(const EScale& scale)
		{
			mYScale = scale;
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
				if(mXModified)
				{
					emit xValueChanged(mCurrentIndex, mData.GetXValue(mCurrentIndex));
					mXModified = false;
				}
				if(mYModified)
				{
					emit yValueChanged(mCurrentIndex, mData.GetValueFromIndex(mCurrentIndex));
					mYModified = false;
				}

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
				if(mEFlags & CLAM::VM::AllowZoomByMouse)
				{
					PopSettings();
				}
			}
			emit rightButtonPressed();
		}

		void BPFEditorController::SetPoint(const TData& x, const TData& y)
		{
			if(mLeftButtonPressed)
			{
				if(mSelectPoint)
				{
					mRenderer.SetSelectedIndex(mCurrentIndex);
					mSelectPoint=false;
					emit requestRefresh();
					emit selectedXPos(double(mData.GetXValue(mCurrentIndex)));
				}
			}
			int mode = GetMode();
			switch (mode)
			{
				case Selection:
					if(!(mEFlags & CLAM::VM::AllowZoomByMouse)) break;
					if(mLeftButtonPressed)
					{
						mCorners[0].SetX(x);
						mCorners[0].SetY(y);
						mCorners[1].SetX(x);
						mCorners[1].SetY(y);
						mProcessingSelection=true;
					}
					else if(mProcessingSelection)
					{
						mProcessingSelection=false;
						mCorners[1].SetX(x);
						mCorners[1].SetY(y);
						PushSettings();
					}
					break;
				case Edition:
					mProcessingSelection=false;
					if(!mLeftButtonPressed) break;
					if(!mHit && mKeyInsertPressed)
					{
						TIndex index = mData.Insert(mCurrentPoint.GetX(),mCurrentPoint.GetY());
						emit elementAdded(int(index),float(mCurrentPoint.GetX()), float(mCurrentPoint.GetY()));
						ChooseCurrentPoint(index);
					}
					if(mHit && mKeyDeletePressed)
					{
						mData.DeleteIndex(mCurrentIndex);
						emit elementRemoved(int(mCurrentIndex));
						ChooseCurrentPointByJumping(-1);
					}
					break;
				default:
					break;
			}
		}

		void BPFEditorController::MoveCurrentPointDelta(int stepX, int stepY)
		{
			TData stepXSize = (mMaxX - mMinX) / 100;
			TData stepYSize = (mMaxY - mMinY) / 100;
			TData x = mData.GetXValue(mCurrentIndex);
			TData y = mData.GetValueFromIndex(mCurrentIndex);
			UpdateBPF(x + stepX*stepXSize, y + stepY*stepYSize);
			emit requestRefresh();
			UpdateXYLabels(x,y);
		}

		void BPFEditorController::ChooseCurrentPointByJumping(int step)
		{
			ChooseCurrentPoint(mCurrentIndex+step);
		}

		void BPFEditorController::ChooseCurrentPoint(int index)
		{
			mCurrentIndex=index;
			if (mCurrentIndex<0) mCurrentIndex=0;
			if (mCurrentIndex>=mData.Size()) mCurrentIndex=mData.Size()-1;

			mRenderer.SetSelectedIndex(mCurrentIndex);
			if (mCurrentIndex>=0)
				emit selectedXPos(double(mData.GetXValue(mCurrentIndex)));
			emit requestRefresh();
		}

		void BPFEditorController::UpdatePoint(const TData& x, const TData& y)
		{
			if(mProcessingSelection)
			{
				mCorners[1].SetX(x);
				mCorners[1].SetY(y);
				emit requestRefresh();
				UpdateXYLabels(x,y);
				return;
			}
			int index=Hit(x,y);
			if(index != -1) 
			{
				ChooseCurrentPoint(index);
				if(mKeyDeletePressed)
				{
					mSelectPoint=false;
					QCursor cCursor(Qt::CrossCursor);
					emit cursorChanged(cCursor);
				}
				else if(
					(mEFlags & CLAM::VM::AllowVerticalEdition) && 
					(mEFlags & CLAM::VM::AllowHorizontalEdition))
				{
					QCursor sacursor(Qt::SizeAllCursor);
					emit cursorChanged(sacursor);
				}
				else if(mEFlags & CLAM::VM::AllowVerticalEdition)
				{
					QCursor vcursor(Qt::SizeVerCursor);
					emit cursorChanged(vcursor);
				}
				else if(mEFlags & CLAM::VM::AllowHorizontalEdition)
				{
					QCursor hcursor(Qt::SizeHorCursor);
					emit cursorChanged(hcursor);
				}
				mSelectPoint=true;
				mHit=true;
			}
			else
			{
				mCurrentPoint.SetX(x);
				mCurrentPoint.SetY(y);
				if(!mLeftButtonPressed)
				{
					mHit=false;
					mSelectPoint=false;
					QCursor acursor(Qt::ArrowCursor);
					emit cursorChanged(acursor);
				}
			}
			
			if(mHit && mLeftButtonPressed)
			{
				if(!mKeyDeletePressed)
				{
					UpdateBPF(x,y);
				}
			}
			
			emit requestRefresh();
			UpdateXYLabels(x,y);
		}

		void BPFEditorController::UpdateXYLabels(TData x, TData y)
		{
			QString xlabel("");
			QString ylabel("");

			if(mXScale==EScale::eLinear)
			{
				xlabel = QString::number(x,'f',2);
			}
			else if(mXScale==EScale::eLog)
			{
				xlabel = QString::number(x,'e',1);
			}

			if(mYScale==EScale::eLinear)
			{
				ylabel = QString::number(y,'f',2);
			}
			else if(mYScale==EScale::eLog)
			{
				ylabel = QString::number(y,'e',1);
			}

			emit labelsText(xlabel,ylabel);
		}

		void BPFEditorController::Draw()
		{
			mRenderer.SetData(mData);
			mRenderer.SetBounds(GetLeftBound(),GetRightBound());
			mRenderer.Render();
			if(mEFlags & CLAM::VM::AllowZoomByMouse)
			{
				DrawRect();
			}
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
			Pixel selected_pixel=GetPixel(x,y);
			for(TIndex i=0; i < mData.Size(); i++)
			{
				Pixel owned_pixel=GetPixel(mData.GetXValue(i),mData.GetValueFromIndex(i));
				if(Match(selected_pixel,owned_pixel))
					return i;
			}
			return -1;
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

		void BPFEditorController::UpdateBPF(TData x, TData y)
		{
			// Bound movement
			if (mCurrentIndex!=0)
			{
				TData prior_x = mData.GetXValue(mCurrentIndex-1);
				if(x<prior_x) x=prior_x;
			}
			if(mCurrentIndex!=mData.Size()-1)
			{
				TData next_x = mData.GetXValue(mCurrentIndex+1);
				if(x>next_x) x=next_x;
			}
			if (x<mMinX) x=mMinX;
			if (x>mMaxX) x=mMaxX;
			if (y<mMinY) y=mMinY;
			if (y>mMaxY) y=mMaxY;

			if(mEFlags & CLAM::VM::AllowVerticalEdition)
			{
				mData.SetValue(mCurrentIndex,y);
				mYModified = true;
			}
			if(mEFlags & CLAM::VM::AllowHorizontalEdition)
			{
				mData.SetXValue(mCurrentIndex,x);
				mXModified = true;
			}
		}

		void BPFEditorController::SetHBounds(double left, double right)
		{
			if(left==mView.mLeft && right==mView.mRight) return;
			mView.mLeft = left;
			mView.mRight = right;
			mXRulerRange.mMin = mView.mLeft;
			mXRulerRange.mMax = mView.mRight;
			emit viewChanged(mView);
			emit xRulerRange(mXRulerRange.mMin, mXRulerRange.mMax);
			emit requestRefresh();
		}

		void BPFEditorController::SetVBounds(double bottom, double top)
		{
			if(bottom==mView.mBottom && top==mView.mTop) return;
			mView.mBottom = bottom;
			mView.mTop = top;
			mYRulerRange.mMin = mView.mBottom;
			mYRulerRange.mMax = mView.mTop;
			emit viewChanged(mView);
			emit yRulerRange(mYRulerRange.mMin, mYRulerRange.mMax);
			emit requestRefresh();
		}

		void BPFEditorController::SelectPointFromXCoord(double xcoord)
		{
			if(!mData.Size()) return;
			if(mData.Size()==1)
			{
				mRenderer.SetSelectedIndex(0);
				emit requestRefresh();
				return;
			}
			TIndex index = -1;
			for(TIndex i=0; i < mData.Size(); i++)
			{
				if(TData(xcoord)<=mData.GetXValue(i))
				{
					index=i;
					break;
				}
			}
			if(index != -1)
			{
				if(xcoord==mData.GetXValue(index))
				{
					mRenderer.SetSelectedIndex(index);
					emit requestRefresh();
					return;
				}
				double x0 = mData.GetXValue(index-1);
				double x1 = mData.GetXValue(index);
				if((xcoord-x0)<(x1-xcoord)) index--;
				mRenderer.SetSelectedIndex(index);
				emit requestRefresh();
			}
		}

		void BPFEditorController::vZoomIn()
		{
			if(mVCurrent/2.0 > mMinSpanY)
			{
				mVCurrent /= 2.0;
				UpdateVBounds(true);
				mVZoomRatio /= 2.0;
				emit vZoomRatio(mVZoomRatio);
				emit vScrollMaxValue(GetnyPixels());
				emit vScrollValue(GetVScrollValue());
			}
		}

		void BPFEditorController::vZoomOut()
		{
			if(mVCurrent*2.0 <= mSpanY)
			{
				mVCurrent *= 2.0;
				UpdateVBounds(false);
				mVZoomRatio *= 2.0;
				emit vZoomRatio(mVZoomRatio);
				emit vScrollValue(GetVScrollValue());
				emit vScrollMaxValue(GetnyPixels());
			} 
		}

		void BPFEditorController::updateVScrollValue(int value)
		{
			double bottom = mSpanY/double(GetnyPixels())*double(value)-fabs(mMinY);
			double top = bottom+mVCurrent;
			SetVBounds(bottom,top);
		}

		int BPFEditorController::GetnyPixels() const
		{
			double value = mSpanY*double(mDisplayHeight)/mVCurrent;
			return int(value);
		}

		void BPFEditorController::InitVZoomRatio()
		{
			double n = mMinSpanY;
			double r = 1.0;
			while(n < mSpanY)
			{
				n *= 2.0;
				r *= 2.0;
			}
			mVZoomRatio = r/2.0;
		}

		void BPFEditorController::InitVScroll()
		{
			mVCurrent = mSpanY;
			InitVZoomRatio();
			emit vZoomRatio(mVZoomRatio);
			int vsv=GetVScrollValue();
			emit vScrollMaxValue(GetnyPixels());
			emit vScrollValue(vsv);
		}

		int BPFEditorController::GetVScrollValue() const
		{
			double value = (mView.mBottom+fabs(mMinY))*double(GetnyPixels())/mSpanY;
			return int(value);
		}

		void BPFEditorController::UpdateVBounds(bool zin)
		{
			double bottom,top;
			bottom = mView.mBottom;
			top = mView.mTop;
			if(zin)
			{
				bottom += mVCurrent/2.0;
				top -= mVCurrent/2.0;
			}
			else
			{
				bottom -= mVCurrent/4.0;
				top += mVCurrent/4.0;
				if(bottom < mMinY)
				{
					bottom = mMinY;
					top = bottom+mVCurrent;
				}
				if(top > mMaxY)
				{
					top = mMaxY;
					bottom = top-mVCurrent;
				}
			}
			SetVBounds(bottom,top);
		}

		TIndex BPFEditorController::GetRightBound()
		{
			int nPoints = mData.Size();
			if(nPoints==0) return 0;

			TData searchValue = TData(mView.mRight);

			if(searchValue <= mData.GetXValue(0)) return 0;
			if(searchValue >= mData.GetXValue(nPoints-1)) return nPoints;

			TIndex left_index = 0;
			TIndex right_index = nPoints-1;
			while(left_index + 1 < right_index)
			{
				TIndex currentIndex = (left_index+right_index)/2;
				if (searchValue > mData.GetXValue(currentIndex))
				{
					left_index = currentIndex;
				}
				if(searchValue < mData.GetXValue(currentIndex))
				{
					right_index = currentIndex;
				}
			}
			return right_index+1;
		}

		TIndex BPFEditorController::GetLeftBound()
		{
			int nPoints = mData.Size();
			if(!nPoints) return 0;
			if(nPoints==1) return 0;

			TData searchValue = TData(mView.mLeft);

			if(searchValue <= mData.GetXValue(0)) return 0;
			if(searchValue >= mData.GetXValue(nPoints-1)) return nPoints-1;
			
			TIndex left_index = 0;
			TIndex right_index = nPoints-1;
			while(left_index + 1 < right_index)
			{
				TIndex currentIndex = (left_index+right_index)/2;
				if (searchValue > mData.GetXValue(currentIndex))
				{
					left_index = currentIndex;
				}
				if(searchValue < mData.GetXValue(currentIndex))
				{
					right_index = currentIndex;
				}
			}
			return left_index;

		}

	}
}

// END
