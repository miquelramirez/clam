#include <qlayout.h>
#include "NetPlotController.hxx"
#include "NetDisplaySurface.hxx"
#include "NetPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		NetPlot::NetPlot(QWidget* parent) : QWidget(parent)
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

			_surf->SetBackgroundColor(r,g,b);
		}
        
		void NetPlot::Label(const std::string& label)
		{
			setCaption(label.c_str());
		}

		void NetPlot::Geometry(int x,int y,int w,int h)
		{
			setGeometry(x,y+18,w,h);
		}

		void NetPlot::Show()
		{
			show();
		}

		void NetPlot::Hide()
		{
			hide();
		}

		void NetPlot::SetController(NetPlotController* controller)
		{
			_controller = controller;
			_surf->SetController(_controller);
		}

		void NetPlot::InitNetPlot()
		{
			QBoxLayout* layout = new QHBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(0);

			_surf = new NetDisplaySurface(this);
			_surf->setMinimumSize(100,20);
			layout->addWidget(_surf);
		}
	}
}

// END
