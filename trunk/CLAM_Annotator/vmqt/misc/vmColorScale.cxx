#include <QPainter>
#include <QResizeEvent>
#include "vmColorScale.hxx"

namespace CLAM
{
    namespace VM
    {
		ColorScale::ColorScale(QWidget* parent)
			: QWidget(parent)
		{
			InitColorScale();
		}

		ColorScale::~ColorScale()
		{
		}

		void ColorScale::SetScale(const std::vector<Color>& scale)
		{
			mScale = scale;
			update();
		}
	    
		void ColorScale::paintEvent(QPaintEvent* e)
		{
			if(!mScale.size())
			{
				QWidget::paintEvent(e);
				return;
			}

			QPainter painter(this);
			for(unsigned i = 0; i < mScale.size(); i++)
			{
				painter.setPen(QPen(QColor(mScale[i].r,mScale[i].g,mScale[i].b)));
				painter.drawLine(int(i),rect().bottom(),i,rect().top());
			}
		}

		void ColorScale::resizeEvent(QResizeEvent* e)
		{
			emit widthChanged(e->size().width());
		}

		void ColorScale::InitColorScale()
		{
			setFixedHeight(20);
			setMinimumWidth(151);
			setMaximumWidth(302);
		}		
    }
}

// END
