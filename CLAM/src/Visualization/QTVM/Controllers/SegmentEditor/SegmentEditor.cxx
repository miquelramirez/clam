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
			, mMustProcessData(false)
			, mMousePressed(false)
			, mHit(false)
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
			// TODO
/*
			unsigned size = mStrategy->onsets().size();
			// check for begin
			unsigned index = mStrategy->pickOnset(x,3.0);
			if(index != size) 
			{
				printf("begin found at index %d\n",index);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			// check for end
			index = mStrategy->pickOffset(x,3.0);
			if(index != size) 
			{
				printf("end found at index %d\n",index);
				emit cursorChanged(QCursor(Qt::SizeHorCursor));
				return;
			}
			emit cursorChanged(QCursor(Qt::ArrowCursor));
*/

			std::pair<unsigned,unsigned> pickInfo = PickHBound(x);
			switch(pickInfo.second)
			{
				case BEGIN:
					printf("begin found at index %d\n",pickInfo.first);
					emit cursorChanged(QCursor(Qt::SizeHorCursor));
					break;
				case END:
					printf("end found at index %d\n",pickInfo.first);
					emit cursorChanged(QCursor(Qt::SizeHorCursor));
					break;
				default:
					emit cursorChanged(QCursor(Qt::ArrowCursor));
					break;
			}

		}

		void SegmentEditor::MousePressed(double xpos, double ypos)
		{
			mMousePressed = true;
			// TODO
		}

		void SegmentEditor::MouseReleased(double xpos, double ypos)
		{
			mMousePressed = false;
			// TODO
		}

		void SegmentEditor::ProcessData()
		{
			mRenderer.SetData(*mStrategy);
			mMustProcessData = false;
		}

		std::pair<unsigned, unsigned> SegmentEditor::PickHBound(double x)
		{
			unsigned type = NONE;
			if(!mStrategy) return std::make_pair(1000000,type);
			unsigned selected_pixel=GetPixel(x);
			Segmentation::TimePositions begin = mStrategy->onsets();
			Segmentation::TimePositions end = mStrategy->offsets();
			unsigned nSegments = begin.size();
			unsigned index = nSegments;			
			for(unsigned i=0; i < nSegments; i++)
			{
				unsigned owned_pixel=GetPixel(begin[i]);
				if(abs(int(selected_pixel-owned_pixel)) <= TOLERANCE)
				{
					index = i;
					type = BEGIN;
					break;
				}
				owned_pixel=GetPixel(end[i]);
				if(abs(int(selected_pixel-owned_pixel)) <= TOLERANCE)
				{
					index = i;
					type = END;
					break;
				}
			}
			return std::make_pair(index,type);
		}

		unsigned SegmentEditor::GetPixel(double x) const
		{
			double w = double(mScreenWidth);
			double left = mLeftBound;
			double right = mRightBound;
			double xcoord = x-left;
			double pixel = xcoord*w/(right-left);
			return unsigned(pixel);
		}

	}
}

// END
