#include "Segmentation.hxx"
#include "SegmentEditor.hxx"

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
			, mCurrentIndex(0)
			, mMustProcessData(false)
			, mMousePressed(false)
			, mIsHDragging(false)
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

			int type = NONE;
			unsigned size = mStrategy->onsets().size();
			double tolerance = double(TOLERANCE)*(mRightBound-mLeftBound)/double(mScreenWidth);
			// check for onset
			unsigned index = mStrategy->pickOnset(x,tolerance);
			if(index != size) type = ONSET;
			if(type == NONE) // check for offset
			{
				index = mStrategy->pickOffset(x,tolerance);
				if(index != size) type = OFFSET;
			}

			switch(type)
			{
				case ONSET:
					mCurrentIndex = index;
					mIsHDragging = true;
					emit working(true);
					emit cursorChanged(QCursor(Qt::SizeHorCursor));
					break;
				case OFFSET:
					mCurrentIndex = index;
					mIsHDragging = true;
					emit working(true);
					emit cursorChanged(QCursor(Qt::SizeHorCursor));
					break;
				case BODY:
					mIsHDragging = false;
					emit working(false);
					emit cursorChanged(QCursor(Qt::ArrowCursor));
					// if key alt -> set current
					break;
				default:
					mIsHDragging = false;
					emit working(false);
					emit cursorChanged(QCursor(Qt::ArrowCursor));
					break;
			}
			
			if(mIsHDragging && mMousePressed)
			{
				if(type == ONSET)
				{
					mStrategy->dragOnset(mCurrentIndex,x);
				}	
				else if(type == OFFSET)
				{
					mStrategy->dragOffset(mCurrentIndex,x);
				}
				mMustProcessData = true;
				emit requestRefresh();	
			}
		}

		void SegmentEditor::MousePressed(double xpos, double ypos)
		{
			mMousePressed = true;
			// TODO
			// if dragging or selecting return
			// if key insert -> insert 
			// if key delete -> delete
		}

		void SegmentEditor::MouseReleased(double xpos, double ypos)
		{
			mMousePressed = false;
			mIsHDragging = false;
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
