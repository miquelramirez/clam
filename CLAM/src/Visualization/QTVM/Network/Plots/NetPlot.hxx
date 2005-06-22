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
			Q_OBJECT
		public:
			NetPlot(QWidget* parent = 0, const char * name = 0);
			virtual ~NetPlot();
		
			void SetBackgroundColor(Color c);
			void Label(const std::string& label);
			void Geometry(int x,int y,int w,int h);
			void show();
			void hide();

			void StopRendering();

		protected:
			NetPlotController* mController;

			void closeEvent(QCloseEvent* ce);
			void SetController(NetPlotController* controller);
			virtual void SetPlotController()=0;

		private:
			NetDisplaySurface* mDisplaySurface;
			bool               mIsActive;
			bool               mIsClosed;

			void InitNetPlot();
		};
	}
}

#endif

