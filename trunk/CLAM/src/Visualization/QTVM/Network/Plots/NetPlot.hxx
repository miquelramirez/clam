#ifndef __NETPLOT__
#define __NETPLOT__

#include <string>
#include <qwidget.h>
#include "VMColor.hxx"

namespace CLAM
{
	namespace VM
	{
		class NetPlotController;
		class NetDisplaySurface;

		class NetPlot : public QWidget
		{	
			public:
				NetPlot(QWidget* parent = 0);
				virtual ~NetPlot();
			
				void SetBackgroundColor(Color c);
				void Label(const std::string& label);
				void Geometry(int x,int y,int w,int h);
				void Show();
				void Hide();

			protected:
				NetPlotController* _controller;

				void closeEvent(QCloseEvent* ce);

				void SetController(NetPlotController* controller);

				virtual void SetPlotController()=0;

			private:
				NetDisplaySurface* _surf;

				void InitNetPlot();
		};
	}
}

#endif
