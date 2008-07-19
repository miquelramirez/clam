/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include "CLAMGL.hxx"
#include <CLAM/Segmentation.hxx>
#include "vmSegmentEditor.hxx"
#include "vmPlot2D.hxx"

//#define __SEGMENT_EDITOR__DEBUG__

#ifdef __SEGMENT_EDITOR__DEBUG__
#include <iostream>
#endif
#include <QtGui/QColor>

namespace CLAM
{
	namespace VM
	{
		SegmentEditor::SegmentEditor()
			: mSegmentation(0)
			, mAllowChangeCurrent(false)
			, mCurrentSegment(0)
			, mEditionMode(Idle)
			, mDraggedSegment(0)
			, mColors(5)
			, mHighlighted(-1)
			, mVMargin(0.0)
			, mBoundType(LEFT_BOUND)
		{
			Colorize();
		}

		SegmentEditor::~SegmentEditor()
		{
		}

		void SegmentEditor::SetSegmentation(Segmentation* s)
		{
			mSegmentation = s;
			if(!mSegmentation) return;
			mCurrentSegment = mSegmentation->current();
			_container->updateRenderers();
		}

		void SegmentEditor::checkCurrent(double xpos)
		{
			if(!mSegmentation) return;
			if(!mEnabled || !mAllowChangeCurrent) return;
			unsigned index = mSegmentation->pickSegmentBody(xpos);
			if(index == mCurrentSegment || index == mSegmentation->offsets().size()) return;
			mSegmentation->current(index);
			mCurrentSegment = mSegmentation->current();
			emit currentSegmentChanged();
			emit requestRefresh();	
		}

		void SegmentEditor::allowChangeCurrent(bool allow)
		{
			mAllowChangeCurrent = allow;
		}

		void SegmentEditor::SetHBounds(double left, double right)
		{
			Renderer2D::SetHBounds(left,right);
		}

		void SegmentEditor::SetVBounds(double bottom, double top)
		{
			Renderer2D::SetVBounds(bottom,top);
			mVMargin = double(PIXMARGIN)*(mView.top-mView.bottom)/double(mViewport.h);
		}

		void SegmentEditor::Render()
		{
			if(!mSegmentation) return;
			if(!mEnabled) return;
			unsigned nElems = mSegmentation->onsets().size();
			if(!nElems) return;
			unsigned current = mSegmentation->current();
			const Segmentation::TimePositions & beginnings = mSegmentation->onsets();
			const Segmentation::TimePositions & endings = mSegmentation->offsets();
			glLineWidth(2);
			for(unsigned i=0;  i < nElems; i++)
			{
				if(beginnings[i] > mView.right) break;
				if(IsVisible(beginnings[i],endings[i],mView.left,mView.right)) 
				{
					DrawSegment(beginnings[i],
								endings[i],
								mYRange.min,
								mYRange.max,
								i==current,
								mSegmentation->selections()[i]);
				}				
			}
			DrawVHighlighted(mYRange.min,mYRange.max);
			if (current>=nElems) return; // no current
			if (IsVisible(beginnings[current],endings[current],mView.left,mView.right)) 
				StippledRect(beginnings[current],endings[current],mYRange.min,mYRange.max);
			glLineWidth(1);
		}

		void SegmentEditor::MouseMoveEvent(double x, double y)
		{
			if(!mEnabled) return;
			if(!mCatchEvents) return;
			if(!mSegmentation) return;
			QString ttip("");
			QString units = mSegmentation->xUnits().c_str();
			switch (mEditionMode)
			{
				case DraggingOnset:
					mSegmentation->dragOnset(mDraggedSegment,x);
					ttip = QString("Begin (%1): %2%3")
						.arg(mDraggedSegment)
						.arg(mSegmentation->onsets()[mDraggedSegment],0,'g')
						.arg(units)
						;
					_container->setToolTip(ttip);
					emit requestRefresh();	
					return;
				case DraggingOffset:
					mSegmentation->dragOffset(mDraggedSegment,x);
					ttip = QString("End (%1): %2%3")
						.arg(mDraggedSegment)
						.arg(mSegmentation->offsets()[mDraggedSegment],0,'g')
						.arg(units)
						;
					_container->setToolTip(ttip);
					emit requestRefresh();	
					return;
				case DraggingBody:
					_container->rendererWorking(mKey,true);
					mHighlighted = -1;
					_container->setToolTip(ttip);
					return;
				default:
					// Just continue below
					mHighlighted = -1;
					_container->rendererWorking(mKey,false);
					_container->setToolTip(ttip);
					_container->setCursor(Qt::ArrowCursor);
					break;
			}

			unsigned size = mSegmentation->onsets().size();
			double tolerance = double(TOLERANCE)*(mView.right-mView.left)/double(mViewport.w);

			unsigned index;
			index = mSegmentation->pickOnset(x,tolerance);
			if(index != size) 
			{
				_container->rendererWorking(mKey,true);
				mHighlighted = index;
				mBoundType = LEFT_BOUND;
				ttip = QString("Begin (%1): %2%3")
						.arg(index)
						.arg(mSegmentation->onsets()[index],0,'g')
						.arg(units)
						;
				_container->setToolTip(ttip);
				_container->setCursor(Qt::SizeHorCursor);
				return;
			}
			index = mSegmentation->pickOffset(x,tolerance);
			if(index != size) 
			{
				_container->rendererWorking(mKey,true);
				mHighlighted = index;
				mBoundType = RIGHT_BOUND;
				ttip = QString("End (%1): %2%3")
						.arg(index)
						.arg(mSegmentation->offsets()[index],0,'g')
						.arg(units)
						;
				_container->setToolTip(ttip);
				_container->setCursor(Qt::SizeHorCursor);
				return;
			}
			if(mKeyboard.key_insert || mKeyboard.key_ctrl)
			{
				_container->rendererWorking(mKey,true);	
			}
		}

		void SegmentEditor::MousePressEvent(double x, double y)
		{
			if(!mEnabled) return;
			if(!mCatchEvents) return;
			if(!mSegmentation) return;
			if(mKeyboard.key_insert)
			{
				unsigned index = mSegmentation->insert(x);
				emit requestRefresh();
				emit segmentInserted(index);
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Inserted segment at index  " << index << std::endl;
#endif
				return;
			}
			unsigned index;
			unsigned nSegments = mSegmentation->onsets().size();
			double tolerance = double(TOLERANCE)*(mView.right-mView.left)/double(mViewport.w);
			index = mSegmentation->pickOnset(x,tolerance);
			if(index!=nSegments)
			{
				mEditionMode=DraggingOnset;
				mDraggedSegment=index;
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Dragging onset " << index << std::endl;
#endif
				return;
			}
			index = mSegmentation->pickOffset(x,tolerance);
			if(index!=nSegments)
			{
				mEditionMode=DraggingOffset;
				mDraggedSegment=index;
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Dragging offset " << index << std::endl;
#endif
				return;
			}
			index = mSegmentation->pickSegmentBody(x);
			if(index!=nSegments)
			{
				mEditionMode=DraggingBody;
				mDraggedSegment=index;
				if(mKeyboard.key_ctrl)
				{
					(mSegmentation->selections()[index]) 
						? mSegmentation->deselect(index) 
						: mSegmentation->select(index);
				}
				else
				{
					mSegmentation->current(index);
					mSegmentation->clearSelection();
					mCurrentSegment = mSegmentation->current();
					emit currentSegmentChanged();
#ifdef __SEGMENT_EDITOR__DEBUG__
					std::cout << "Current segment is " << index << std::endl;
#endif
				}
				emit requestRefresh();	
				return;
			}
		}

		void SegmentEditor::MouseReleaseEvent(double x, double y)
		{
			if(!mEnabled) return;
			if(!mCatchEvents) return;
			if(!mSegmentation) return;
			int mode = mEditionMode;
			mEditionMode=Idle;
			if(!mKeyboard.key_ctrl) _container->rendererWorking(mKey,false);
			mHighlighted = -1;
			_container->setToolTip("");
			_container->setCursor(Qt::ArrowCursor);
			if(mode == DraggingOnset) 
			{
				emit segmentOnsetChanged(mDraggedSegment,mSegmentation->onsets()[mDraggedSegment]);
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Onset changed at index " << mDraggedSegment << ", new value " 
						  << mSegmentation->onsets()[mDraggedSegment] << std::endl;
#endif
				return;
			}
			if(mode == DraggingOffset) 
			{
				emit segmentOffsetChanged(mDraggedSegment,mSegmentation->offsets()[mDraggedSegment]);
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Offset changed at index " << mDraggedSegment << ", new value " 
						  << mSegmentation->offsets()[mDraggedSegment] << std::endl;
#endif
				return;
			}
		}

		void SegmentEditor::KeyPressEvent(int key)
		{
			if(!mEnabled) return;
			if(!mCatchEvents) return;
			if(!mSegmentation) return;
			switch(key)
			{
				case Qt::Key_Insert:
				{
					_container->rendererWorking(mKey,true);
					mKeyboard.key_insert = true; 
				}
				break;
						
				case Qt::Key_Delete:
				{
					mKeyboard.key_delete = true; 
					unsigned nSegments = mSegmentation->onsets().size();
					unsigned index = mSegmentation->current();
					mSegmentation->remove(index);
					if(mSegmentation->onsets().size() == nSegments) return; // Remove was ignored
					emit requestRefresh();
					emit segmentDeleted(index);
					emit currentSegmentChanged();
					mCurrentSegment = mSegmentation->current();
#ifdef __SEGMENT_EDITOR__DEBUG__
					std::cout << "Segment deleted  " << index << std::endl;
					std::cout << "Current segment is " << mSegmentation->current() << std::endl;
#endif
				}
				break;

				case Qt::Key_Control:
				{
					_container->rendererWorking(mKey,true);
					mKeyboard.key_ctrl = true;
				}
				break;
			}
		}

		void SegmentEditor::KeyReleaseEvent(int key)
		{
			if(!mEnabled) return;
			if(!mCatchEvents) return;
			if(!mSegmentation) return;
			switch(key)
			{
				case Qt::Key_Insert:
					_container->rendererWorking(mKey,false);
					mKeyboard.key_insert = false; 
					break;
						
				case Qt::Key_Delete:
					mKeyboard.key_delete = false; 
					break;

				case Qt::Key_Control:
					_container->rendererWorking(mKey,false);
					mKeyboard.key_ctrl = false;
					break;

				default:
					break;
			}
		}

		void SegmentEditor::LeaveEvent()
		{
			mHighlighted = -1;
			_container->setToolTip("");
			_container->rendererWorking(mKey,false);
		}

		void SegmentEditor::SetViewport(const GLViewport& v)
		{
			Renderer2D::SetViewport(v);
			mVMargin = double(PIXMARGIN)*(mView.top-mView.bottom)/double(mViewport.h);
		}

		void SegmentEditor::Colorize()
		{
			mColors[NORMAL] = QColor(100,200,20);
			mColors[CURRENT] = QColor(100,200,20);
			mColors[SELECTED] = QColor(140,240,60); 
			mColors[HIGHLIGHTED] = QColor(255,255,0);
			mColors[STIPPLED] = QColor(255,90,60);
		}

		void SegmentEditor::PlainRect(double left, double right, double bottom, double top)
		{
			glVertex2d(left,bottom+mVMargin);
			glVertex2d(left,top-mVMargin);
			glVertex2d(right,top-mVMargin);
			glVertex2d(right,bottom+mVMargin);
			glVertex2d(left,bottom+mVMargin);
		}

		void SegmentEditor::StippledRect(double left, double right, double bottom, double top)
		{
			float vPadding = mVMargin-(2.0*(mView.top-mView.bottom)/double(mViewport.h));
			float hPadding = 3.0*(mView.right-mView.left)/double(mViewport.w);
			// draw stippled rect
			glColor3ub(mColors[STIPPLED].red(),mColors[STIPPLED].green(),mColors[STIPPLED].blue());
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(FACTOR,PATTERN);
			glBegin(GL_LINE_STRIP);
			PlainRect(left-hPadding,right+hPadding,bottom-vPadding,top+vPadding);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
		}

		bool SegmentEditor::IsVisible(double left, double right, double lBound, double rBound)
		{
			if(left >= lBound  && left <= rBound) return true;
			if(right >= lBound && right <= rBound) return true;
			if(left <= lBound && right >= rBound) return true;
			return false;
		}

		void SegmentEditor::DrawSegment(double left, double right, 
										double bottom, double top, 
										bool isCurrent, 
										bool isSelected)
		{
			int cindex = isSelected ? SELECTED : NORMAL;
			// fill rec
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glColor4ub(mColors[cindex].red(),mColors[cindex].green(),mColors[cindex].blue(),125);
			glBegin(GL_QUADS);
			PlainRect(left,right,bottom,top);
			glEnd();
			// draw plain
			glColor4ub(mColors[cindex].red(),mColors[cindex].green(),mColors[cindex].blue(),200);
			glBegin(GL_LINE_STRIP);
			PlainRect(left,right,bottom,top);
			glEnd();
		}

		void SegmentEditor::DrawVHighlighted(double bottom,  double top)
		{
			if(mHighlighted < 0 || mHighlighted >= (int)mSegmentation->onsets().size()) return;
			float x = 
				(mBoundType==LEFT_BOUND) 
				? mSegmentation->onsets()[mHighlighted] 
				: mSegmentation->offsets()[mHighlighted];
			bool isCurrent = (mHighlighted == (int)mSegmentation->current());
		   
			// draw vertical highlighted 
			glColor3ub(mColors[HIGHLIGHTED].red(),mColors[HIGHLIGHTED].green(),mColors[HIGHLIGHTED].blue());
			glBegin(GL_LINES);
			glVertex2d(x,top-mVMargin); 
			glVertex2d(x,bottom+mVMargin);
			glEnd();
		}
	}
}

// END

