#ifndef __DISPLAYSURFACE__
#define __DISPLAYSURFACE__

#include <qgl.h>
#include "DataTypes.hxx"
#include "PlotController.hxx"

class QMouseEvent;

namespace CLAM
{
	namespace VM
	{
		class DisplaySurface : public QGLWidget
		{
			typedef struct
			{
				double r;
				double g;
				double b;
			} BkColor;

			Q_OBJECT

			public:
				DisplaySurface(QWidget* parent = 0);
				virtual ~DisplaySurface();

				void SetBackgroundColor(double r, double g, double b);
				void SetController(PlotController* controller);

			signals:
				void leavingMouse();

			private slots:
				void receivedView(View);
				void updateToolTip(QString);

			protected:
				virtual void initializeGL(); 
				virtual void resizeGL(int w,int h); 
				virtual void paintGL(); 

				virtual void mousePressEvent(QMouseEvent* e);
				virtual void mouseMoveEvent(QMouseEvent* e);

				virtual void resizeEvent(QResizeEvent *e);

				virtual void leaveEvent(QEvent* e);

			private:
				View _view;
				PlotController* _controller;
				BkColor _bkColor;
				
				void InitView();
		};
	}
}

#endif

