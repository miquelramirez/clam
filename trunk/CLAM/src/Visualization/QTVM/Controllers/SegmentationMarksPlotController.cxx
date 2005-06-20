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
#include "SegmentationMarksPlotController.hxx"

namespace CLAM
{
    namespace VM
    {
		SegmentationMarksPlotController::SegmentationMarksPlotController()
			: mMustProcessMarks(false) 
			, mHit(false) 
			, mCurrentElem(0)
			, mCurrentIndex(0)
			, mKeyInsertPressed(false)
			, mKeyDeletePressed(false)
			, mHasSentTag(false)
		{
		}

		SegmentationMarksPlotController::~SegmentationMarksPlotController()
		{
		}

		void SegmentationMarksPlotController::SetMarks(std::vector<unsigned>& marks)
		{
			mMarks = marks;
			sort(mMarks.begin(),mMarks.end());
			mTags.clear();
			mTags.resize(mMarks.size());
			mMustProcessMarks=true;
			emit requestRefresh();
		}

		std::vector<unsigned>& SegmentationMarksPlotController::GetMarks()
		{
			return mMarks;
		}

		void SegmentationMarksPlotController::SetMarksColor(Color c)
		{
			mMarksRenderer.SetColor(c);
		}

		void SegmentationMarksPlotController::Draw()
		{
			if(mMarks.size() > 0)
			{
				if(mMustProcessMarks) ProcessMarksData();
				mMarksRenderer.Render();
			}	
		}

		void SegmentationMarksPlotController::SurfaceDimensions(int w,int h)
		{
			PlotController::SurfaceDimensions(w,h);
			mMustProcessMarks=true;
		}

		void SegmentationMarksPlotController::SetMousePos(TData x, TData y)
		{
			mHasSentTag=false;
			PlotController::SetMousePos(x,y);
			if(IsAbleToEdit())
			{
				int index=Hit(x);
				if(index != -1) 
				{
					if(mKeyDeletePressed)
					{
						QCursor cCursor(Qt::CrossCursor);
						emit changeCursor(cCursor);
					}
					else
					{
						QCursor hcursor(Qt::SizeHorCursor);
						emit changeCursor(hcursor);
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
					if(!IsLeftButtonPressed())
					{
						QCursor acursor(Qt::ArrowCursor);
						emit changeCursor(acursor);
						mHit=false;
					}
				}
			}

			if(mHit && IsLeftButtonPressed())
			{
				if(!mKeyDeletePressed)
				{
					Update(mCurrentIndex,unsigned(x));
				}
			}
		}

		void SegmentationMarksPlotController::SetHBounds(const TData& left, const TData& right)
        {
			PlotController::SetHBounds(left,right);
			mMarksRenderer.SetHBounds(GetLeftBound(),GetRightBound());
			mMustProcessMarks=true;
		}

		void SegmentationMarksPlotController::SetVBounds(const TData& bottom, const TData& top)
		{
			PlotController::SetVBounds(bottom,top);
			mMarksRenderer.SetVBounds(GetBottomBound(),GetTopBound());
			mMustProcessMarks=true;
		}

		void SegmentationMarksPlotController::ProcessMarksData()
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

		void SegmentationMarksPlotController::InsertElem(unsigned elem)
		{
			if(HaveElem(elem)) return;
			std::vector<unsigned>::iterator pos = mMarks.begin();
			std::vector<QString>::iterator tag_pos = mTags.begin();
			for(; pos != mMarks.end(); pos++, tag_pos++)
			{
				if((*pos) > elem) break;
			} 
			mMarks.insert(pos,elem);
			mTags.insert(tag_pos,QString(""));
	    
			mMustProcessMarks=true;
			emit requestRefresh();
			emit insertedMark(elem);
		}

		void SegmentationMarksPlotController::RemoveElem(int index)
		{
			if(index < 0 || index > (int)mMarks.size()-1) return;
			unsigned elem = mMarks[index];
			std::vector<unsigned>::iterator pos = find(mMarks.begin(),mMarks.end(),elem);
			mMarks.erase(pos);

			QString tag = mTags[index];
			std::vector<QString>::iterator tag_pos = find(mTags.begin(),mTags.end(),tag);
			mTags.erase(tag_pos);

			mMustProcessMarks=true;
			emit requestRefresh();
			emit removedMark(index,elem);
		}
	    
		bool SegmentationMarksPlotController::HaveElem(unsigned elem)
		{
			return find(mMarks.begin(),mMarks.end(),elem) != mMarks.end();
		}

		void SegmentationMarksPlotController::Update(int index, unsigned elem)
		{
			mMarks[index]=elem;
	    
			mMustProcessMarks=true;
			emit requestRefresh();
			emit updatedMark(index,elem);
		}

		unsigned SegmentationMarksPlotController::GetPixel(const TData& x) const
		{
			TData w=mViewport.w;
			TData left=GetLeftBound();
			TData right=GetRightBound();
			TData xcoord=x-left;
			TData pixel=xcoord*w/(right-left);
			return (unsigned)pixel;
		}
	  
		int SegmentationMarksPlotController::Hit(const TData& x)
		{
			unsigned i;
			bool hit=false;
			unsigned selected_pixel=GetPixel(x);
			for(i=0; i < mMarks.size(); i++)
			{
				unsigned owned_pixel=GetPixel(TData(mMarks[i]));
				if(abs(int(selected_pixel-owned_pixel)) <= 1)
				{
					hit=true;
					break;
				}
			}
			return (hit) ? int(i) : -1;
		}

		void SegmentationMarksPlotController::LeaveMouse()
		{
			mHit=false;
			PlotController::LeaveMouse();
			QCursor cursor(ArrowCursor);
			emit changeCursor(cursor);
		}
	
		void SegmentationMarksPlotController::SetKeyInsertPressed(bool pressed)
		{
			mKeyInsertPressed=pressed;
		}

		void SegmentationMarksPlotController::SetKeyDeletePressed(bool pressed)
		{
			mKeyDeletePressed=pressed;
		}

		bool SegmentationMarksPlotController::CanDrawSelectedPos()
		{
			return (!mHit && !mKeyInsertPressed);
		}

		void SegmentationMarksPlotController::SetLeftButtonPressed(bool pressed)
		{
			PlotController::SetLeftButtonPressed(pressed);
			if(IsLeftButtonPressed())
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

		void SegmentationMarksPlotController::InsertMark(unsigned elem)
		{
			InsertElem(elem);
		}

		void SegmentationMarksPlotController::RemoveMark(int index, unsigned elem)
		{
			if(mMarks[index] != elem) return;
			RemoveElem(index);
		}

		void SegmentationMarksPlotController::UpdateMark(int index, unsigned elem)
		{
			if(mMarks[index] == elem) return;
			Update(index,elem);
		}

		void SegmentationMarksPlotController::OnDoubleClick()
		{
			if(mHit) emit requestSegmentationTag();
		}

		QString SegmentationMarksPlotController::GetTag() const
		{
			return mTags[mCurrentIndex];
		}

		void SegmentationMarksPlotController::SetSegmentationTag(const QString& tag)
		{
			mTags[mCurrentIndex]=tag;
			emit updatedTag(mCurrentIndex,tag);
		}

		std::vector<QString> SegmentationMarksPlotController::GetTags()
		{
			return mTags;
		}

		bool SegmentationMarksPlotController::HasSentTag() const
		{
			return mHasSentTag;
		}

		void SegmentationMarksPlotController::UpdateTag(int index, const QString& tag)
		{
			mTags[index]=tag;
		}
    }
}

// END

