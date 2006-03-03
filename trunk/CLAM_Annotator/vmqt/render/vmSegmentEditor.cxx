#include "CLAMGL.hxx"
#include "vmSegmentation.hxx"
#include "vmSegmentEditor.hxx"

//#define __SEGMENT_EDITOR__DEBUG__

#ifdef __SEGMENT_EDITOR__DEBUG__
#include <iostream>
#endif

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
			emit requestUpdate();
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
		}

		void SegmentEditor::MouseMoveEvent(double x, double y)
		{
			if(!mEnabled) return;
			if(!mSegmentation || !mCatchEvents) return;
			QString ttip("");
			switch (mEditionMode)
			{
				case DraggingOnset:
					mSegmentation->dragOnset(mDraggedSegment,x);
					ttip = "x:"+QString::number(mSegmentation->onsets()[mDraggedSegment],'f',2);
					emit toolTip(ttip);
					emit requestRefresh();	
					return;
				case DraggingOffset:
					mSegmentation->dragOffset(mDraggedSegment,x);
					ttip = "x:"+QString::number(mSegmentation->offsets()[mDraggedSegment],'f',2);
					emit toolTip(ttip);
					emit requestRefresh();	
					return;
				case DraggingBody:
					emit working(mKey,true);
					mHighlighted = -1;
					emit toolTip(ttip);
					return;
				default:
					// Just continue below
					mHighlighted = -1;
					emit working(mKey,false);
					emit toolTip(ttip);
					emit cursorChanged(QCursor(Qt::ArrowCursor));
					break;
			}

			unsigned size = mSegmentation->onsets().size();
			double tolerance = double(TOLERANCE)*(mView.right-mView.left)/double(mViewport.w);

			unsigned index;
			index = mSegmentation->pickOnset(x,tolerance);
			if(index != size) 
			{
				emit working(mKey,true);
				mHighlighted = index;
				mBoundType = LEFT_BOUND;
				ttip = "x:"+QString::number(mSegmentation->onsets()[index],'f',2);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			index = mSegmentation->pickOffset(x,tolerance);
			if(index != size) 
			{
				emit working(mKey,true);
				mHighlighted = index;
				mBoundType = RIGHT_BOUND;
				ttip = "x:"+QString::number(mSegmentation->offsets()[index],'f',2);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			if(mKeyboard.key_insert || mKeyboard.key_ctrl)
			{
				emit working(mKey,true);	
			}
		}

		void SegmentEditor::MousePressEvent(double x, double y)
		{
			if(!mEnabled) return;
			if(!mSegmentation || !mCatchEvents) return;
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
			if(!mSegmentation || !mCatchEvents) return;
			int mode = mEditionMode;
			mEditionMode=Idle;
			if(!mKeyboard.key_ctrl) emit working(mKey,false);
			mHighlighted = -1;
			emit toolTip("");
			emit cursorChanged(QCursor(Qt::ArrowCursor));
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
			if(!mSegmentation || !mCatchEvents) return;
			switch(key)
			{
				case Qt::Key_Insert:
				{
					emit working(mKey,true);
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
					emit working(mKey,true);
					mKeyboard.key_ctrl = true;
				}
				break;
			}
		}

		void SegmentEditor::KeyReleaseEvent(int key)
		{
			if(!mEnabled) return;
			if(!mSegmentation || !mCatchEvents) return;
			switch(key)
			{
				case Qt::Key_Insert:
					emit working(mKey,false);
					mKeyboard.key_insert = false; 
					break;
						
				case Qt::Key_Delete:
					mKeyboard.key_delete = false; 
					break;

				case Qt::Key_Control:
					emit working(mKey,false);
					mKeyboard.key_ctrl = false;
					break;

				default:
					break;
			}
		}

		void SegmentEditor::LeaveEvent()
		{
			mHighlighted = -1;
			emit toolTip("");
			emit working(mKey,false);
		}

		void SegmentEditor::SetViewport(const GLViewport& v)
		{
			Renderer2D::SetViewport(v);
			mVMargin = double(PIXMARGIN)*(mView.top-mView.bottom)/double(mViewport.h);
		}

		void SegmentEditor::Colorize()
		{
			mColors[NORMAL] = Color(100,200,20);
			mColors[CURRENT] = Color(100,200,20);
			mColors[SELECTED] = Color(140,240,60); 
			mColors[HIGHLIGHTED] = Color(255,255,0);
			mColors[STIPPLED] = Color(255,90,60);
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
			glLineWidth(2);
			// draw stippled rect
			glColor3ub(mColors[STIPPLED].r,mColors[STIPPLED].g,mColors[STIPPLED].b);
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(FACTOR,PATTERN);
			glBegin(GL_LINE_STRIP);
			PlainRect(left-hPadding,right+hPadding,bottom-vPadding,top+vPadding);
			glEnd();
			glDisable(GL_LINE_STIPPLE);
			glLineWidth(1);
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
			int lineWidth = isCurrent ? CLINEWIDTH : NLINEWIDTH;
			glLineWidth(lineWidth);
			// fill rec
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			glColor4ub(mColors[cindex].r,mColors[cindex].g,mColors[cindex].b,125);
			glBegin(GL_QUADS);
			PlainRect(left,right,bottom,top);
			glEnd();
			glDisable(GL_BLEND);
			// draw plain
			glColor3ub(mColors[cindex].r,mColors[cindex].g,mColors[cindex].b);
			glBegin(GL_LINE_STRIP);
			PlainRect(left,right,bottom,top);
			glEnd();
			glLineWidth(1);
		}

		void SegmentEditor::DrawVHighlighted(double bottom,  double top)
		{
			if(mHighlighted < 0 || mHighlighted >= (int)mSegmentation->onsets().size()) return;
			float x = 
				(mBoundType==LEFT_BOUND) 
				? mSegmentation->onsets()[mHighlighted] 
				: mSegmentation->offsets()[mHighlighted];
			bool isCurrent = (mHighlighted == (int)mSegmentation->current());
			int lineWidth = isCurrent ? CLINEWIDTH : NLINEWIDTH;
		   
			glLineWidth(lineWidth);
			// draw vertical highlighted 
			glColor3ub(mColors[HIGHLIGHTED].r,mColors[HIGHLIGHTED].g,mColors[HIGHLIGHTED].b);
			glBegin(GL_LINES);
			glVertex2d(x,top-mVMargin); 
			glVertex2d(x,bottom+mVMargin);
			glEnd();
			glLineWidth(1);
		}
	}
}

// END
