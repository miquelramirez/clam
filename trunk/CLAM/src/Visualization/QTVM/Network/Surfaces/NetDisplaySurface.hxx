#ifndef __NETDISPLAYSURFACE__
#define __NETDISPLAYSURFACE__

#include <qgl.h>
#include "DataTypes.hxx"
#include "NetPlotController.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetDisplaySurface : public QGLWidget
		{
			typedef struct
			{
				double r;
				double g;
				double b;
			} BkColor;

			Q_OBJECT

			public:
				NetDisplaySurface(QWidget* parent = 0);
				virtual ~NetDisplaySurface();

				void SetBackgroundColor(double r, double g, double b);
				void SetController(NetPlotController* controller);

			private slots:
				void receivedView(SView);
				void refresh();

			protected:
				virtual void initializeGL(); 
				virtual void resizeGL(int w,int h); 
				virtual void paintGL(); 

			private:
				SView _view;
				NetPlotController* _controller;
				BkColor _bkColor;
				
				void InitView();
		};
	}
}

#endif

