#include <CLAM/CLAMGL.hxx>
#include <CLAM/Segmentation.hxx>
#include <CLAM/SegmentEditor.hxx>
#include <iostream>

/*
#define __SEGMENT_EDITOR__DEBUG__
 */

namespace CLAM
{
	namespace VM
	{
		SegmentEditor::SegmentEditor()
			: mStrategy(0)
			, mMinX(0.0)
			, mMaxX(100.0)
			, mMinY(0.0)
			, mMaxY(100.0)
			, mLeftBound(0.0)
			, mRightBound(100.0)
			, mBottomBound(0.0)
			, mTopBound(100.0)
			, mScreenWidth(0)
			, mScreenHeight(100)
			, mMustProcessData(false)
			, mMousePressed(false)
			, mKeyInsertPressed(false)
			, mKeyDeletePressed(false)
			, mKeyCtrlPressed(false)
			, mAllowChangeCurrent(false)
			, mCurrentSegment(0)
			, mEditionMode(Idle)
			, mDraggedSegment(0)
		{
		}

		SegmentEditor::~SegmentEditor()
		{
		}

		void SegmentEditor::SetSegmentation(Segmentation* s)
		{
			mStrategy = s;
			if(!mStrategy) return;
			mCurrentSegment = mStrategy->current();
			mMustProcessData = true;
			emit requestRefresh();
		}

		void SegmentEditor::SetHBounds(double left, double right)
		{
			mLeftBound = left;
			mRightBound = right;
			mRenderer.SetHBounds(mLeftBound,mRightBound);
		}

		void SegmentEditor::SetVBounds(double bottom, double top)
		{
			mBottomBound = bottom;
			mTopBound = top;
			mRenderer.SetVBounds(mBottomBound,mTopBound);
			mRenderer.SetMargin(double(PIXMARGIN)*(mTopBound-mBottomBound)/double(mScreenHeight));
		}

		void SegmentEditor::Draw()
		{
			if(!mStrategy) return;
			if(mMustProcessData) ProcessData();
			mRenderer.Render();
		}

		void SegmentEditor::ScreenArea(int w, int h)
		{
			mScreenWidth = w;
			mScreenHeight = h;
			mRenderer.SetMargin(double(PIXMARGIN)*(mTopBound-mBottomBound)/double(mScreenHeight));
		}

		void SegmentEditor::SetXRange(double xmin, double xmax)
		{
			mMinX = xmin;
			mMaxX = xmax;
		}

		void SegmentEditor::SetYRange(double ymin, double ymax)
		{
			mMinY = ymin;
			mMaxY = ymax;
		}

		void SegmentEditor::MousePos(double x, double y)
		{
			if(!mStrategy) return;
			QString ttip("");
			switch (mEditionMode)
			{
				case DraggingOnset:
					mStrategy->dragOnset(mDraggedSegment,x);
					mMustProcessData = true;
					ttip = "samples:"+QString::number(mStrategy->onsets()[mDraggedSegment],'f',0);
					emit toolTip(ttip);
					emit requestRefresh();	
					return;
				case DraggingOffset:
					mStrategy->dragOffset(mDraggedSegment,x);
					mMustProcessData = true;
					ttip = "samples:"+QString::number(mStrategy->offsets()[mDraggedSegment],'f',0);
					emit toolTip(ttip);
					emit requestRefresh();	
					return;
				case DraggingBody:
					emit working(true);
					mRenderer.SetVHighlighted(-1);
					emit toolTip(ttip);
					return;
				default:
					// Just continue below
					mRenderer.SetVHighlighted(-1);
					emit working(false);
					emit toolTip(ttip);
					break;
			}

			unsigned size = mStrategy->onsets().size();
			double tolerance = double(TOLERANCE)*(mRightBound-mLeftBound)/double(mScreenWidth);

			unsigned index;
			index = mStrategy->pickOnset(x,tolerance);
			if (index != size) 
			{
				emit working(true);
				mRenderer.SetVHighlighted(index,LEFT_BOUND);
				ttip = "samples:"+QString::number(mStrategy->onsets()[index],'f',0);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			index = mStrategy->pickOffset(x,tolerance);
			if (index != size) 
			{
				emit working(true);
				mRenderer.SetVHighlighted(index,RIGHT_BOUND);
				ttip = "samples:"+QString::number(mStrategy->offsets()[index],'f',0);
				emit toolTip(ttip);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			if(mKeyInsertPressed || mKeyCtrlPressed)
			{
				emit working(true);	
			}
		}

		void SegmentEditor::MousePressed(double x, double y)
		{
			if(!mStrategy) return;
			if(mKeyInsertPressed)
			{
				unsigned index = mStrategy->insert(x);
				mMustProcessData = true;
				emit requestRefresh();
				emit segmentInserted(index);
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Inserted segment at index  " << index << std::endl;
#endif
				return;
			}
			unsigned index;
			unsigned nSegments = mStrategy->onsets().size();
			double tolerance = double(TOLERANCE)*(mRightBound-mLeftBound)/double(mScreenWidth);
			index = mStrategy->pickOnset(x,tolerance);
			if (index!=nSegments)
			{
				mEditionMode=DraggingOnset;
				mDraggedSegment=index;
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Dragging onset " << index << std::endl;
#endif
				return;
			}
			index = mStrategy->pickOffset(x,tolerance);
			if (index!=nSegments)
			{
				mEditionMode=DraggingOffset;
				mDraggedSegment=index;
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Dragging offset " << index << std::endl;
#endif
				return;
			}
			index = mStrategy->pickSegmentBody(x);
			{
				mEditionMode=DraggingBody;
				mDraggedSegment=index;
				if(mKeyCtrlPressed)
				{
					(mStrategy->selections()[index]) ? mStrategy->deselect(index) : mStrategy->select(index);
				}
				else
				{
					mStrategy->current(index);
					mStrategy->clearSelection();
					emit currentSegmentChanged(index);
					mCurrentSegment = mStrategy->current();
#ifdef __SEGMENT_EDITOR__DEBUG__
					std::cout << "Current segment is " << index << std::endl;
#endif
				}
				mMustProcessData = true;
				emit requestRefresh();	
				return;
			}
		}

		void SegmentEditor::MouseReleased(double x, double y)
		{
			if(!mStrategy) return;
			int mode = mEditionMode;
			mEditionMode=Idle;
			mMousePressed = false;
			if(!mKeyCtrlPressed) emit working(false);
			mRenderer.SetVHighlighted(-1);
			emit toolTip("");
			emit cursorChanged(QCursor(Qt::ArrowCursor));
			if(mode == DraggingOnset) 
			{
				emit segmentOnsetChanged(mDraggedSegment,mStrategy->onsets()[mDraggedSegment]);
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Onset changed at index " << mDraggedSegment << ", new value " 
						  << mStrategy->onsets()[mDraggedSegment] << std::endl;
#endif
				return;
			}
			if(mode == DraggingOffset) 
			{
				emit segmentOffsetChanged(mDraggedSegment,mStrategy->offsets()[mDraggedSegment]);
#ifdef __SEGMENT_EDITOR__DEBUG__
				std::cout << "Offset changed at index " << mDraggedSegment << ", new value " 
						  << mStrategy->offsets()[mDraggedSegment] << std::endl;
#endif
				return;
			}
		}

		void SegmentEditor::KeyPressEvent(QKeyEvent* e)
		{
			if(!mStrategy) return;
			switch(e->key())
			{
				case Qt::Key_Insert:
				{
					emit working(true);
					mKeyInsertPressed = true; 
				}
				break;
						
				case Qt::Key_Delete:
				{
					mKeyDeletePressed = true; 
					unsigned nSegments = mStrategy->onsets().size();
					unsigned index = mStrategy->current();
					mStrategy->remove(index);
					if (mStrategy->onsets().size() == nSegments) return; // Remove was ignored
					mMustProcessData = true;
					emit requestRefresh();
					emit segmentDeleted(index);
					emit currentSegmentChanged(mStrategy->current());
					mCurrentSegment = mStrategy->current();
#ifdef __SEGMENT_EDITOR__DEBUG__
					std::cout << "Segment deleted  " << index << std::endl;
					std::cout << "Current segment is " << mStrategy->current() << std::endl;
#endif
				}
				break;

				case Qt::Key_Control:
				{
					emit working(true);
					mKeyCtrlPressed = true;
				}
				break;
			}
		}

		void SegmentEditor::KeyReleaseEvent(QKeyEvent* e)
		{
			if(!mStrategy) return;
			switch(e->key())
			{
				case Qt::Key_Insert:
					emit working(false);
					mKeyInsertPressed = false; 
					break;
						
				case Qt::Key_Delete:
					mKeyDeletePressed = false; 
					break;

				case Qt::Key_Control:
					emit working(false);
					mKeyCtrlPressed = false;
					break;

				default:
					break;
			}
		}

		void SegmentEditor::ProcessData()
		{
			mRenderer.SetData(*mStrategy);
			mMustProcessData = false;
		}

		void SegmentEditor::LeaveMouse()
		{
			mRenderer.SetVHighlighted(-1);
			emit toolTip("");
		}

		void SegmentEditor::CheckCurrent(double xpos)
		{
			if(!mStrategy) return;
			if(!mAllowChangeCurrent) return;
			unsigned index = mStrategy->pickSegmentBody(xpos);
			if(index == mCurrentSegment || index == mStrategy->offsets().size()) return;
			mStrategy->current(index);
			mStrategy->clearSelection();
			emit currentSegmentChanged(index);
			mCurrentSegment = mStrategy->current();
		}

		void SegmentEditor::AllowChangeCurrent(bool allow)
		{
			mAllowChangeCurrent = allow;
		}
	}
}

// END

