#ifndef __SEGMENTEDITOR__
#define __SEGMENTEDITOR__

#include <qobject.h>
#include <qcursor.h>
#include "SegmentRenderer.hxx"

namespace CLAM
{
	class Segmentation;
	
	namespace VM
	{
		class SegmentEditor : public QObject
		{
			Q_OBJECT
		public:
			SegmentEditor();
			~SegmentEditor();

			void SetSegmentation(Segmentation* s);
			void SetHBounds(double left, double right);
			void SetVBounds(double bottom, double top);
			void Draw();

			void MousePos(double x, double y);
			void MousePressed(double xpos, double ypos);
			void MouseReleased(double xpos, double ypos);

			void ScreenArea(int w, int h);
			void SetXRange(double xmin, double xmax);
			void SetYRange(double ymin, double ymax);
			
		signals:
			void cursorChanged(QCursor);
			void working(bool);
			void requestRefresh();

		private:
			Segmentation*   mStrategy;
			SegmentRenderer mRenderer;
			double          mMinX;
			double          mMaxX;
			double          mMinY;
			double          mMaxY;
			double          mLeftBound;
			double          mRightBound;
			double          mBottomBound;
			double          mTopBound;
			int             mScreenWidth;
			int             mScreenHeight;
			int             mCurrentIndex;
			bool            mMustProcessData;
			bool            mMousePressed;
			bool            mIsHDragging;

			enum { ONSET=0, OFFSET=1, NONE=2, TOLERANCE=3, BODY=10 /* Bo Derek, but I preffer Jenifer Lopez */ };

			void ProcessData();
			std::pair<unsigned, unsigned> PickHBound(double x); // firs=index second=type[BEGIN/END/NONE]
			unsigned GetPixel(double x) const;
		};
	}
}

#endif

