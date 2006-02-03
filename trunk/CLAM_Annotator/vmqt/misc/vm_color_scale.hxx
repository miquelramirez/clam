#ifndef __VMQT_COLORSCALE_H__
#define __VMQT_COLORSCALE_H__

#include <vector>
#include <QWidget>
#include "vm_data_types.hxx"

namespace CLAM
{
    namespace VM
    {
		class ColorScale : public QWidget
		{
			Q_OBJECT
		public:
			ColorScale(QWidget* parent=0);
			~ColorScale();

			void set_scale(const std::vector<Color>& scale);

		signals:
			void widthChanged(int);
	    
		protected:
			void paintEvent(QPaintEvent* e);
			void resizeEvent(QResizeEvent* e);

		private:
			std::vector<Color> cs_scale;

			void init_color_scale();

		};
    }
}

#endif

