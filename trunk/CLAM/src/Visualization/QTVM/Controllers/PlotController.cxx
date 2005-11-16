/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <algorithm>
#include "PlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		PlotController::PlotController()
			: mDisplayWidth(1)
			, mDisplayHeight(1)
			, mLeftBound(0.0)
			, mRightBound(1.0)
			, mBottomBound(0.0)
			, mTopBound(1.0)
			, mSelPos(0.0)
			, mMinSpanX(1.0)
			, mMinSpanY(1.0)
			, mSamples(1.0) 
			, mSpanX(1.0)
			, mMaxSpanY(1.0)
			, mSpanY(1.0)
			, mMinY(0.0)
			, mMaxY(0.0)
			, mHZRatio(1.0)
			, mVZRatio(1.0)
			, mIsLeftButtonPressed(false)
			, mIsAbleToEdit(false)
			, mMustProcessMarks(false) 
			, mHit(false) 
			, mCurrentElem(0)
			, mCurrentIndex(0)
			, mKeyInsertPressed(false)
			, mKeyDeletePressed(false)
			, mKeyShiftPressed(false)
			, mHasSentTag(false)
			, mSegmentationMarksEnabled(true)
			, mIsRenderingEnabled(true)
		{	
		}
	
		PlotController::~PlotController()
		{
		}

		void PlotController::SetHBounds(const double& left, const double& right)
		{
			mLeftBound = left;
			mRightBound = right;
			mView.left = 0.0;
			mView.right = mRightBound-mLeftBound;
			mMarksRenderer.SetHBounds(mLeftBound, mRightBound);
			mDial.SetHBounds(mLeftBound, mRightBound);
			mMustProcessMarks = true;
			emit viewChanged(mView);
		}

		void PlotController::SetVBounds(const double& bottom, const double& top)
		{
			mBottomBound = bottom;
			mTopBound = top;
			mView.bottom = mBottomBound;
			mView.top = mTopBound;
			mMarksRenderer.SetVBounds(mBottomBound, mTopBound);
			mDial.SetVBounds(mBottomBound, mTopBound);
			mMustProcessMarks=true;
			emit viewChanged(mView);
		}

		const double& PlotController::GetLeftBound() const
		{
			return mLeftBound;
		}

	    const double& PlotController::GetRightBound() const
		{
			return mRightBound;
		}

		const double& PlotController::GetBottomBound() const
		{
			return mBottomBound;
		}

		const double& PlotController::GetTopBound() const
		{
			return mTopBound;
		}

		void PlotController::SetSelPos(const double& value, bool render)
		{
			mSelPos = value;
			if(CanDrawSelectedPos() && render)
			{
				mDial.Update(mSelPos);
			}
		}

		const double& PlotController::GetSelPos() const
		{
			return mSelPos;
		}

		void PlotController::SetnSamples(const double& samples)
		{
			mSamples = samples;
			mSpanX = mSamples;
			InitHRatio();
			emit hZoomRatio(mHZRatio);
			int hsv=GetHScrollValue();
			emit hScrollMaxValue(GetnxPixels());
			emit hScrollValue(hsv);
		}

		const double& PlotController::GetnSamples() const
		{
			return mSamples;
		}

		void PlotController::SetYRange(const double& min, const double& max)
		{
			mMinY = min;
			mMaxY = max;
			mMaxSpanY = mMaxY-mMinY;
			mSpanY = mMaxSpanY;
			InitVRatio();
			emit vZoomRatio(mVZRatio);
			int vsv=GetVScrollValue();
			emit vScrollMaxValue(GetnyPixels());
			emit vScrollValue(vsv);
		}

		const double& PlotController::GetMaxSpanY() const
		{
			return mMaxSpanY;
		}

		void PlotController::hZoomIn()
		{
			if(mSpanX/2.0 > mMinSpanX)
			{
				mSpanX /= 2.0;
				UpdateHBounds(true);
				mHZRatio /= 2.0;
				emit hZoomRatio(mHZRatio);
				emit hScrollMaxValue(GetnxPixels());
				emit hScrollValue(GetHScrollValue());
			}
		}

		void PlotController::hZoomOut()
		{
			if(mSpanX*2.0 <= mSamples)
			{
				mSpanX *= 2.0;
				UpdateHBounds(false);
				mHZRatio *= 2.0;
				emit hZoomRatio(mHZRatio);
				emit hScrollValue(GetHScrollValue());
				emit hScrollMaxValue(GetnxPixels());
			} 
		}

		void PlotController::vZoomIn()
		{
			if(mSpanY/2.0 > mMinSpanY)
			{
				mSpanY /= 2.0;
				UpdateVBounds(true);
				mVZRatio /= 2.0;
				emit vZoomRatio(mVZRatio);
				emit vScrollMaxValue(GetnyPixels());
				emit vScrollValue(GetVScrollValue());
			}
		}

		void PlotController::vZoomOut()
		{
			if(mSpanY*2.0 <= mMaxSpanY)
			{
				mSpanY *= 2.0;
				UpdateVBounds(false);
				mVZRatio *= 2.0;
				emit vZoomRatio(mVZRatio);
				emit vScrollValue(GetVScrollValue());
				emit vScrollMaxValue(GetnyPixels());
			} 
		}

		void PlotController::SetMinSpanX(const double& min)
		{
			mMinSpanX = min;
		}

		const double& PlotController::GetMinSpanX() const
		{
			return mMinSpanX;
		}	
	  
		void PlotController::SetMinSpanY(const double& min)
		{
			mMinSpanY = min;
		}

		const double& PlotController::GetMinSpanY() const
		{
			return mMinSpanY;
		}

		void PlotController::UpdateHBounds(bool zin)
		{
			double left,right;
			left = mLeftBound;
			right = mRightBound;
			if(zin)
			{
			    if(ReferenceIsVisible())
			    {
					double ref = GetReference();
					if(ref-mLeftBound > mSpanX/2.0)
					{
						left = (ref+mSpanX/2.0 < mSamples) ? ref-mSpanX/2.0 : ref-(mSpanX-(mSamples-ref));
					}
					right = left+mSpanX;
			    }
			    else
			    {
					left += mSpanX/2.0;
					right -= mSpanX/2.0;
			    }
			}
			else
			{
				left -= mSpanX/4.0;
				right += mSpanX/4.0;
				if(left < 0)
				{
					left = 0;
					right = mSpanX;
				}
				if(right > mSamples)
				{
					right = double(mSamples);
					left = right-mSpanX;
				}
			}
			SetHBounds(left,right);
		}

		void PlotController::UpdateVBounds(bool zin)
		{
			double bottom,top;
			bottom = mView.bottom;
			top = mView.top;
			if(zin)
			{
				bottom += mSpanY/2.0;
				top -= mSpanY/2.0;
			}
			else
			{
				bottom -= mSpanY/4.0;
				top += mSpanY/4.0;
				if(bottom < mMinY)
				{
					bottom = mMinY;
					top = bottom+mSpanY;
				}
				if(top > mMaxY)
				{
					top = mMaxY;
					bottom = top-mSpanY;
				}
			}
			SetVBounds(bottom,top);
		}

		void PlotController::InitHRatio()
		{
			double n = mMinSpanX;
			double r = 1.0;
			while(n < mSamples)
			{
				n *= 2.0;
				r *= 2.0;
			}
			mHZRatio = r/2.0;
		}

		void PlotController::InitVRatio()
		{
			double n = mMinSpanY;
			double r = 1.0;
			while(n < mMaxSpanY)
			{
				n *= 2.0;
				r *= 2.0;
			}
			mVZRatio = r/2.0;
		}

		void PlotController::DisplayDimensions(const int& w, const int& h)
		{
			mDisplayWidth = w;
			mDisplayHeight = h;

			int hsv = GetHScrollValue();
			emit hScrollMaxValue(GetnxPixels());
			emit hScrollValue(hsv);

			int vsv = GetVScrollValue();
			emit vScrollMaxValue(GetnyPixels());
			emit vScrollValue(vsv);

			mMustProcessMarks = true;
		}

		void PlotController::updateHScrollValue(int value)
		{
			double left = mSamples/double(GetnxPixels())*double(value);
			double right = left+mSpanX;
			SetHBounds(left,right);
		}

		void PlotController::updateVScrollValue(int value)
		{
			double bottom = mMaxSpanY/double(GetnyPixels())*double(value)+mMinY;
			double top = bottom+mSpanY;
			double tmp = bottom;
			bottom = mMinY+(mMaxY-top);
			top = mMaxY-(tmp-mMinY);
			SetVBounds(bottom,top);
		}

		int PlotController::GetnxPixels() const
		{
			return int(double(mSamples)*double(mDisplayWidth)/mSpanX);
		}

		int PlotController::GetnyPixels() const
		{
			return int(mMaxSpanY*double(mDisplayHeight)/mSpanY);
		}

		const double& PlotController::GetSpanX() const
		{
			return mSpanX; 
		}

		const double& PlotController::GetSpanY() const
		{
			return mSpanY;
		}

		int PlotController::GetHScrollValue() const
		{
			return int(mLeftBound*double(GetnxPixels())/double(mSamples));
		}

		int PlotController::GetVScrollValue() const
		{
			return int((mMaxY-mView.top)*double(GetnyPixels())/mMaxSpanY);
		}

		bool PlotController::ReferenceIsVisible()
		{
			// TODO: revise when incorpore Dial rendering
			return (mSelPos >= mLeftBound && mSelPos <= mRightBound);
		}

		const double& PlotController::GetReference() const 
		{
			// TODO: revise when incorporate Dial rendering
			return mSelPos;
		}

		void PlotController::SetMousePos(const double& x, const double& y)
		{
			mHasSentTag=false;
			mMouseXPos=x;
			mMouseYPos=y;
			if(IsAbleToEdit())
			{
				int index=Hit(x);
				if(index != -1) 
				{
					if(mKeyDeletePressed)
					{
						QCursor cCursor(Qt::CrossCursor);
						emit cursorChanged(cCursor);
					}
					else
					{
						QCursor hcursor(Qt::SizeHorCursor);
						emit cursorChanged(hcursor);
					}
					mCurrentIndex=index;
					mHit=true;
					if(!mTags[mCurrentIndex].isEmpty())
					{
						mHasSentTag=true;
						emit toolTip(mTags[mCurrentIndex]);
					}
				}
				else
				{
					mCurrentElem = unsigned(x);
					if(!mIsLeftButtonPressed)
					{
						QCursor acursor(Qt::ArrowCursor);
						emit cursorChanged(acursor);
						mHit=false;
					}
				}
			}

			if(mHit && mIsLeftButtonPressed)
			{
				if(!mKeyDeletePressed)
				{
					Update(mCurrentIndex,unsigned(x));
				}
			}
		}

		const double& PlotController::GetMouseXPos() const
		{
			return mMouseXPos;
		}

		const double& PlotController::GetMouseYPos() const
		{
			return mMouseYPos;
		}

		void PlotController::SetLeftButtonPressed(bool pressed)
		{
		    mIsLeftButtonPressed=pressed;
			if(mIsLeftButtonPressed)
			{
				if(!mHit && mKeyInsertPressed)
				{
					InsertElem(mCurrentElem);
				}

				if(mHit && mKeyDeletePressed)
				{
					RemoveElem(mCurrentIndex);
				}
			}
		}

		bool PlotController::IsLeftButtonPressed()
		{
		    return mIsLeftButtonPressed;
		}

		bool PlotController::IsKeyShiftPressed()
		{
			return mKeyShiftPressed;
		}

		void PlotController::LeaveMouse()
		{
			mHit=false;
		    mIsAbleToEdit=false;
			QCursor cursor(ArrowCursor);
			emit cursorChanged(cursor);
			emit mouseOverDisplay(false);
		}

		void PlotController::EnterMouse()
		{
		    mIsAbleToEdit=true;
			emit mouseOverDisplay(true);
		}

		bool PlotController::IsAbleToEdit()
		{
		    return mIsAbleToEdit;
		}

		bool PlotController::IsPlayable()
		{
		    return false;
		}

		const int& PlotController::GetDisplayWidth() const
		{
			return mDisplayWidth;
		}

		const int& PlotController::GetDisplayHeight() const
		{
			return mDisplayHeight;
		}

		void PlotController::Draw()
		{
			if(mMarks.size() > 0)
			{
				if(mMustProcessMarks) ProcessMarks();
				mMarksRenderer.Render();
			}
			mDial.Render();
		}

		void PlotController::SetMarks(std::vector<unsigned>& marks)
		{
			mMarks = marks;
			sort(mMarks.begin(),mMarks.end());
			mTags.clear();
			mTags.resize(mMarks.size());
			mMustProcessMarks=true;
			emit requestRefresh();
		}

		std::vector<unsigned>& PlotController::GetMarks()
		{
			return mMarks;
		}

		void PlotController::SetMarksColor(Color c)
		{
			mMarksRenderer.SetColor(c);
		}

		void PlotController::KeyPressEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
					mKeyShiftPressed = true;;
					break;

				case Qt::Key_Insert:
					mKeyInsertPressed = true;; 
					break;
						
				case Qt::Key_Delete:
					mKeyDeletePressed = true;; 
					break;

				default:
					break;
			}
		}

		void PlotController::KeyReleaseEvent(QKeyEvent* e)
		{
			switch(e->key())
			{
				case Qt::Key_Shift:
					mKeyShiftPressed = false;
					break;

				case Qt::Key_Insert:
					mKeyInsertPressed = false;; 
					break;
						
				case Qt::Key_Delete:
					mKeyDeletePressed = false; 
					break;

				default:
					break;
			}
		}

		void PlotController::insertMark(unsigned elem)
		{
			InsertElem(elem);
		}

		void PlotController::removeMark(int index, unsigned elem)
		{
			if(mMarks[index] != elem) return;
			RemoveElem(index);
		}

		void PlotController::updateMark(int index, unsigned elem)
		{
			if(mMarks[index] == elem) return;
			Update(index,elem);
		}

		void PlotController::updateTag(int index, QString tag)
		{
			mTags[index]=tag;
		}

		void PlotController::OnDoubleClick()
		{
			if(mHit) emit requestSegmentationTag();
		}

		void PlotController::SetSegmentationTag(const QString& tag)
		{
			mTags[mCurrentIndex]=tag;
			emit updatedTag(mCurrentIndex,tag);
		}

		QString PlotController::GetTag() const
		{
			return mTags[mCurrentIndex];
		}

		unsigned PlotController::GetMarkIndex() const
		{
			return mCurrentIndex;
		}

		std::vector<QString> PlotController::GetTags()
		{
			return mTags;
		}

		bool PlotController::CanDrawSelectedPos()
		{
			return (!mHit && !mKeyInsertPressed);
		}

		bool PlotController::HasSentTag() const
		{
			return mHasSentTag;
		}

		void PlotController::ProcessMarks()
		{
			unsigned left = unsigned(GetLeftBound());
			unsigned right = unsigned(GetRightBound());

			Array<unsigned> processedMarks;

			std::vector<unsigned>::iterator it = mMarks.begin();
			for(;it != mMarks.end();it++)
			{
				if((*it) > right) break;
				if((*it) >= left) processedMarks.AddElem((*it));
			}

			mMarksRenderer.SetData(processedMarks);
			mMustProcessMarks=false;
		}

		bool PlotController::HaveElem(unsigned elem)
		{
			return find(mMarks.begin(),mMarks.end(),elem) != mMarks.end();
		}

		void PlotController::InsertElem(unsigned elem)
		{
			if(!mSegmentationMarksEnabled) return;
			if(HaveElem(elem)) return;
			std::vector<unsigned>::iterator pos = mMarks.begin();
			std::vector<QString>::iterator tag_pos = mTags.begin();
			for(; pos != mMarks.end(); pos++, tag_pos++)
			{
				if((*pos) > elem) break;
			} 
			mMarks.insert(pos,elem);
			mTags.insert(tag_pos,QString(""));
//			mMustProcessMarks=true;
			ProcessMarks();
			if(mIsRenderingEnabled) emit requestRefresh();
			emit insertedMark(elem);
		}

		void PlotController::RemoveElem(int index)
		{
			if(index < 0 || index > (int)mMarks.size()-1) return;
			unsigned elem = mMarks[index];
			std::vector<unsigned>::iterator pos = find(mMarks.begin(),mMarks.end(),elem);
			mMarks.erase(pos);

			QString tag = mTags[index];
			std::vector<QString>::iterator tag_pos = find(mTags.begin(),mTags.end(),tag);
			mTags.erase(tag_pos);
//			mMustProcessMarks=true;
			ProcessMarks();
			if(mIsRenderingEnabled) emit requestRefresh();
			emit removedMark(index,elem);
		}

		void PlotController::Update(int index, unsigned elem)
		{
			mMarks[index]=elem;
//			mMustProcessMarks=true;
			ProcessMarks();
			if(mIsRenderingEnabled) emit requestRefresh();
			emit updatedMark(index,elem);
		}

		unsigned PlotController::GetPixel(const double& x) const
		{
			double w = double(GetDisplayWidth());
			double left = GetLeftBound();
			double right = GetRightBound();
			double xcoord = x-left;
			double pixel = xcoord*w/(right-left);
			return unsigned(pixel);
		}

		int PlotController::Hit(const double& x)
		{
			unsigned i;
			bool hit=false;
			unsigned selected_pixel=GetPixel(x);
			for(i=0; i < mMarks.size(); i++)
			{
				unsigned owned_pixel=GetPixel(double(mMarks[i]));
				if(abs(int(selected_pixel-owned_pixel)) <= 1)
				{
					hit=true;
					break;
				}
			}
			return (hit) ? int(i) : -1;
		}

		void PlotController::SetDialColor(Color c)
		{
			mDial.SetColor(c);
		}

		void PlotController::UpdateDial(const double& value)
		{
			mDial.Update(value);
		}

		const double& PlotController::GetDialPos() const
		{
			return mDial.GetPos();
		}

		const double& PlotController::GetMinY() const
		{
			return mMinY;
		}

		const double& PlotController::GetMaxY() const
		{
			return mMaxY;
		}

		void PlotController::SetSegmentationMarksEnabled(bool e)
		{
			mSegmentationMarksEnabled = e;
		}

		void PlotController::enableRendering()
		{
			mIsRenderingEnabled = true;
		}

		void PlotController::disableRendering()
		{
			mIsRenderingEnabled = false;
		}

		bool PlotController::IsRenderingEnabled() const
		{
			return mIsRenderingEnabled;
		}

		void PlotController::ClearMarks()
		{
			mMarks.clear();
			mTags.clear();
			ProcessMarks();

		}
	}
}

// END


