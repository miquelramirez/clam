#include <qpainter.h>
#include <qtooltip.h>
#include "VZLabel.hxx"
namespace CLAM
{
	namespace VM
	{
		VZLabel::VZLabel(QWidget* parent) : QFrame(parent)
		{
			_align = AlignCenter;
			InitLabel();
		} 

		VZLabel::~VZLabel()
		{
		}

		void VZLabel::InitLabel()
		{
			_f.setFamily("Sans");
			_f.setPointSize(8);
			_f.setBold(true);

			this->setMinimumSize(20,25);
			this->setFixedWidth(20);
			this->setMaximumHeight(70);
			QToolTip::add(this,"Zoom Ratio");
			this->setText("");
		}
		void VZLabel::drawContents(QPainter* p)
		{
			QRect r = rect();
			p->setFont(_f);
			p->translate(r.x(),r.y()+r.height());
			p->rotate(-90.0);
			p->drawText(0,0,r.height(),r.width(),_align,_text);
			p->resetXForm();
		}

		void VZLabel::setText(const QString& text)
		{
			_text = text;
			int h = _text.length()*_f.pointSize();
			this->setFixedHeight(h);
			update();
		}

	}
}
// END


