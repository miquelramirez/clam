#include <QPainter>
#include <QResizeEvent>
#include "vm_color_scale.hxx"

namespace CLAM
{
    namespace VM
    {
		ColorScale::ColorScale(QWidget* parent)
			: QWidget(parent)
		{
			init_color_scale();
		}

		ColorScale::~ColorScale()
		{
		}

		void ColorScale::set_scale(const std::vector<CLAM::VM::Color>& scale)
		{
			cs_scale = scale;
			update();
		}
	    
		void ColorScale::paintEvent(QPaintEvent* e)
		{
			if(!cs_scale.size())
			{
				QWidget::paintEvent(e);
				return;
			}

			QPainter painter(this);
			for(unsigned i = 0; i < cs_scale.size(); i++)
			{
				painter.setPen(QPen(QColor(cs_scale[i].r,cs_scale[i].g,cs_scale[i].b)));
				painter.drawLine(int(i),rect().bottom(),i,rect().top());
			}
		}

		void ColorScale::resizeEvent(QResizeEvent* e)
		{
			emit widthChanged(e->size().width());
		}

		void ColorScale::init_color_scale()
		{
			setFixedHeight(20);
			setMinimumWidth(151);
			setMaximumWidth(302);
		}		
    }
}

// END
