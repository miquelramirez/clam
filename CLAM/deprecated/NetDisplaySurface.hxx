#ifndef __NETDISPLAYSURFACE__
#define __NETDISPLAYSURFACE__

#include <qgl.h>
#include <qtimer.h>
#include "NetPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetDisplaySurface : public QGLWidget
		{
			Q_OBJECT

		public:
			NetDisplaySurface(QWidget* parent = 0);
			~NetDisplaySurface();

			void SetBackgroundColor(double r, double g, double b);
			void SetController(NetPlotController* controller);

			void startRendering();
			void stopRendering();

		private slots:
			void receivedView(SView);
			void updateToolTip(QString);

		protected:
			void paintGL();
			void resizeEvent(QResizeEvent* re);
			void mousePressEvent(QMouseEvent* e);
			void mouseReleaseEvent(QMouseEvent* e);
			void mouseMoveEvent(QMouseEvent* e);

		private:
			NetPlotController* mController;
			QTimer&            mTimer;
			int                mWidth;
			int                mHeight;
			double             mLeft;
			double             mRight;
			double             mBottom;
			double             mTop;
			double             mRed;
			double             mGreen;
			double             mBlue;
			volatile bool      mDoResize;
			unsigned           mRefreshInterval;
				
			void InitView();
		};
	}
}

#endif

