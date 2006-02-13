#include "vmPlot.hxx"

namespace CLAM
{
	namespace VM
	{

		Plot::Plot(QWidget* parent)
			: QGLWidget(parent)
			, mBgColor(255,255,255,255)
		{
			setFocusPolicy(Qt::StrongFocus);
		}

		Plot::~Plot()
		{
		}

		void Plot::SetBackgroundColor(const Color& c)
		{
			mBgColor = c;
			updateGL();
		}
	}
}

// END
