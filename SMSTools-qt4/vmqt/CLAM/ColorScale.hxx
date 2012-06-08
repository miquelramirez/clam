#ifndef __COLORSCALE__
#define __COLORSCALE__

#include <vector>
#include <qwidget.h>
#include <CLAM/VMColor.hxx>

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

			void SetScale(std::vector<Color> scale);

		signals:
			void widthChanged(int);
	    
		protected:
			void paintEvent(QPaintEvent* pe);
			void resizeEvent(QResizeEvent *e);

		private:
			std::vector<Color> mColors;

			enum { ScaleWidth=151 };

			void InitWidget();

		};
    }
}

#endif

