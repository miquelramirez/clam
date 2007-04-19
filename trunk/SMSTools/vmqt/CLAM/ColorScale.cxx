#include <qpainter.h>
#include <CLAM/ColorScale.hxx>

namespace CLAM
{
    namespace VM
    {
		ColorScale::ColorScale(QWidget* parent)
			: QWidget(parent)
		{
			InitWidget();
		}

		ColorScale::~ColorScale()
		{
		}

		void ColorScale::SetScale(std::vector<Color> scale)
		{
			mColors = scale;
			update();
		}
	    
		void ColorScale::paintEvent(QPaintEvent* pe)
		{
			if(!mColors.size())
			{
				QWidget::paintEvent(pe);
				return;
			}

			QPainter painter(this);
			for(unsigned i = 0; i < mColors.size(); i++)
			{
				painter.setPen(QPen(QColor(mColors[i].r,mColors[i].g,mColors[i].b)));
				painter.drawLine(int(i),rect().bottom(),i,rect().top());
			}
		}

		void ColorScale::InitWidget()
		{
			setFixedHeight(20);
			setMinimumWidth(151);
			setMaximumWidth(302);
		}

		void ColorScale::resizeEvent(QResizeEvent *e)
		{
			emit widthChanged(e->size().width());
		}
    }
}

// END

