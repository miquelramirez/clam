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
				virtual ~NetDisplaySurface();

				void SetBackgroundColor(double r, double g, double b);
				void SetController(NetPlotController* controller);

				void startRendering();
				void stopRendering();

			private slots:
				void receivedView(SView);

			protected slots:
				void paintGL();

			protected:
		                void resizeEvent(QResizeEvent* re);

			private:
				NetPlotController* _controller;

				QTimer & _timer;
				int _w, _h;
				double _left, _right, _bottom, _top;
				double _r, _g, _b;
				volatile bool _doResize;
				unsigned _refreshInterval;
				
				void InitView();
		};
	}
}

#endif

