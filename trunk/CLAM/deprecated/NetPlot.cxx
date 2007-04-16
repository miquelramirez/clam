#include <qlayout.h>
#include "GLContext.hxx"
#include "NetPlotController.hxx"
#include "NetDisplaySurface.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		NetPlot::NetPlot(QWidget* parent, const char * name)
			: QWidget(parent,name)
			, mController(0)
			, mIsClosed(false)
		{
			InitNetPlot();
		}
		
		NetPlot::~NetPlot()
		{
		}

		void NetPlot::SetBackgroundColor(Color c)
		{
			double r = double(c.r)/255.0;
			double g = double(c.g)/255.0;
			double b = double(c.b)/255.0;

			mDisplaySurface->SetBackgroundColor(r,g,b);
		}
        
		void NetPlot::Label(const std::string& label)
		{
			setCaption(label.c_str());
		}

		void NetPlot::Geometry(int x,int y,int w,int h)
		{
			setGeometry(x,y+18,w,h);
		}

		void NetPlot::show()
		{
		    if(!mIsClosed)
		    {
				QWidget::show();
				mDisplaySurface->startRendering();
		    }
		}

		void NetPlot::hide()
		{
			mDisplaySurface->stopRendering();
			QWidget::hide();
		}

		void NetPlot::SetController(NetPlotController* controller)
		{
			mController = controller;
			mDisplaySurface->SetController(mController);
		}

		void NetPlot::InitNetPlot()
		{
			QBoxLayout* layout = new QHBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(0);

			mDisplaySurface = new NetDisplaySurface(this);
			GLContext* glc = new GLContext(mDisplaySurface->format(), mDisplaySurface);
			mDisplaySurface->setContext(glc);
			mDisplaySurface->setMinimumSize(100,20);
			layout->addWidget(mDisplaySurface);
		}

		void NetPlot::closeEvent(QCloseEvent* ce)
		{
		    mIsClosed= true;
		    QWidget::closeEvent(ce);
		}

		void NetPlot::StopRendering()
		{
			mDisplaySurface->stopRendering();
		}
	     
	}
}

// END

