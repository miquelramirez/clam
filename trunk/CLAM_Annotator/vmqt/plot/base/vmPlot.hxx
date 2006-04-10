#ifndef __VMQT_PLOT_H__
#define __VMQT_PLOT_H__

#include <QtOpenGL/QGLWidget>
#include "vmDataTypes.hxx"

namespace CLAM
{
	namespace VM
	{
 
		class Plot : public QGLWidget
		{
		public:
			Plot(QWidget* parent=0);
			virtual ~Plot();

			void SetBackgroundColor(const Color& c);

		protected:
			Color mBgColor;

			virtual void Draw()=0;
		};
	}
}

#endif

