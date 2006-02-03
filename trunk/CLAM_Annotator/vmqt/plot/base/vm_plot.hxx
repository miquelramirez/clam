#ifndef __VMQT_PLOT_H__
#define __VMQT_PLOT_H__

#include <QGLWidget>
#include "vm_data_types.hxx"

namespace CLAM
{
	namespace VM
	{
 
		class Plot : public QGLWidget
		{
		public:
			Plot(QWidget* parent=0);
			virtual ~Plot();

			void set_bg_color(const Color& c);

		protected:
			Color cp_bg_color;

			virtual void draw()=0;
		};
	}
}

#endif

