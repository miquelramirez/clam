#include "Segmentation.hxx"
#include "SegmentEditor.hxx"
#include <iostream>
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
			, mEditionMode(Idle)
			, mDraggedSegment(0)
		{
		}

		SegmentEditor::~SegmentEditor()
		{
			if(mStrategy) delete mStrategy;
		}

		void SegmentEditor::SetSegmentation(Segmentation* s)
		{
			mStrategy = s;
			mMustProcessData = true;
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

			switch (mEditionMode)
			{
				case DraggingOnset:
					mStrategy->dragOnset(mDraggedSegment,x);
					mMustProcessData = true;
					emit requestRefresh();	
					return;
				case DraggingOffset:
					mStrategy->dragOffset(mDraggedSegment,x);
					mMustProcessData = true;
					emit requestRefresh();	
					return;
				case DraggingBody:
					return;
				default:
					// Just continue below
					break;
			}

			unsigned size = mStrategy->onsets().size();
			double tolerance = double(TOLERANCE)*(mRightBound-mLeftBound)/double(mScreenWidth);

			unsigned index;
			index = mStrategy->pickOnset(x,tolerance);
			if (index != size) 
			{
				emit working(true);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			index = mStrategy->pickOffset(x,tolerance);
			if (index != size) 
			{
				emit working(true);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
		}

		void SegmentEditor::MousePressed(double xpos, double ypos)
		{
			unsigned nSegments = mStrategy->onsets().size();
			double tolerance = double(TOLERANCE)*(mRightBound-mLeftBound)/double(mScreenWidth);
			unsigned index;
			index = mStrategy->pickOnset(xpos,tolerance);
			if (index!=nSegments)
			{
				mEditionMode=DraggingOnset;
				mDraggedSegment=index;
				std::cout << "Dragging onset " << index << std::endl;
				return;
			}
			index = mStrategy->pickOffset(xpos,tolerance);
			if (index!=nSegments)
			{
				mEditionMode=DraggingOffset;
				mDraggedSegment=index;
				std::cout << "Dragging offset " << index << std::endl;
				return;
			}
			index = mStrategy->pickSegmentBody(xpos);
			{
				mEditionMode=DraggingBody;
				mDraggedSegment=index;
				// This two lines maybe only when Alt is pressed
				mStrategy->current(index);
				emit requestRefresh();	
				std::cout << "Current segment is " << index << std::endl;
				return;
			}
			// TODO
			// if dragging or selecting return
			// if key insert -> insert 
			// if key delete -> delete
		}

		void SegmentEditor::MouseReleased(double xpos, double ypos)
		{
			mEditionMode=Idle;
			mMousePressed = false;
			emit working(false);
			emit cursorChanged(QCursor(Qt::ArrowCursor));
		}

		void SegmentEditor::ProcessData()
		{
			mRenderer.SetData(*mStrategy);
			mMustProcessData = false;
		}
	}
}

// END
