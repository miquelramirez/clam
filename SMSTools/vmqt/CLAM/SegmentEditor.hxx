#ifndef __SEGMENTEDITOR__
#define __SEGMENTEDITOR__

#include <qobject.h>
#include <qcursor.h>
#include <CLAM/SegmentRenderer.hxx>

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
			void MousePressed(double x, double y);
			void MouseReleased(double x, double y);

			void ScreenArea(int w, int h);
			void SetXRange(double xmin, double xmax);
			void SetYRange(double ymin, double ymax);

			void KeyPressEvent(QKeyEvent* e);
			void KeyReleaseEvent(QKeyEvent* e);

			void LeaveMouse();

			void CheckCurrent(double xpos);
			void AllowChangeCurrent(bool allow);
			
		signals:
			void toolTip(QString);
			void cursorChanged(QCursor);
			void working(bool);
			void requestRefresh();

			// signals to tell outside what happend
			void segmentOnsetChanged(unsigned,double);
			void segmentOffsetChanged(unsigned,double);
			void segmentInserted(unsigned);
			void segmentDeleted(unsigned);
			void currentSegmentChanged(unsigned);

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
			bool            mMustProcessData;
			bool            mMousePressed;
			bool            mKeyInsertPressed;
			bool            mKeyDeletePressed;
			bool            mKeyCtrlPressed;
			bool            mAllowChangeCurrent;
			unsigned        mCurrentSegment;
		   
			enum { TOLERANCE=3, PIXMARGIN=5 };

			enum EditionMode 
			{
				Idle,
				DraggingOnset,
				DraggingOffset,
				DraggingBody,
				DraggingHeight
			};

			EditionMode mEditionMode;
			unsigned    mDraggedSegment;
		   
			void ProcessData();
		};
	}
}

#endif

