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

///********** uncomment this to test it *******************************

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

//*********** end uncomment ***********************/
/*
			///////// coment this to test the above /////////////////
			std::pair<unsigned,unsigned> pixel = PickHBound(x);
			int index = pixel.first;
			int type = pixel.second;
			////////  end comment  //////////////////////////////////           
			*/
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

		std::pair<unsigned, unsigned> SegmentEditor::PickHBound(double x)
		{
			unsigned selected_pixel=GetPixel(x);
			Segmentation::TimePositions onsets = mStrategy->onsets();
			Segmentation::TimePositions offsets = mStrategy->offsets();
			unsigned nSegments = onsets.size();
			unsigned index = nSegments;	
			unsigned type = NONE;
			for(unsigned i=0; i < nSegments; i++)
			{
				unsigned owned_pixel=GetPixel(onsets[i]);
				if(abs(int(selected_pixel-owned_pixel)) <= TOLERANCE)
				{
					index = i;
					type = ONSET;
					break;
				}
				owned_pixel=GetPixel(onsets[i]);
				if(abs(int(selected_pixel-owned_pixel)) <= TOLERANCE)
				{
					index = i;
					type = OFFSET;
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
