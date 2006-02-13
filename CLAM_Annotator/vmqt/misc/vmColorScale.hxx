#ifndef __VMQT_COLORSCALE_H__
#define __VMQT_COLORSCALE_H__

#include <vector>
#include <QWidget>
#include "vmDataTypes.hxx"

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

			void SetScale(const std::vector<Color>& scale);

		signals:
			void widthChanged(int);
	    
		protected:
			void paintEvent(QPaintEvent* e);
			void resizeEvent(QResizeEvent* e);

		private:
			std::vector<Color> mScale;

			void InitColorScale();

		};
    }
}

#endif

