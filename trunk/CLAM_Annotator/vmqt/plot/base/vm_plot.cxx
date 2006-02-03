#include "vm_plot.hxx"

namespace CLAM
{
	namespace VM
	{

		Plot::Plot(QWidget* parent)
			: QGLWidget(parent)
			, cp_bg_color(255,255,255,255)
		{
			setFocusPolicy(Qt::StrongFocus);
		}

		Plot::~Plot()
		{
		}

		void Plot::set_bg_color(const Color& c)
		{
			cp_bg_color = c;
			updateGL();
		}
	}
}

// END
