#include <qtooltip.h>
#include "SingleLabel.hxx"
namespace CLAM
{
	namespace VM
	{
		SingleLabel::SingleLabel(QWidget* parent,const QString& units,const QString& tooltip) 
								 : QLabel(parent)
		{
			SetUnits(units);
			SetToolTip(tooltip);
			InitLabel();
		} 

		SingleLabel::~SingleLabel()
		{
		}

		void SingleLabel::InitLabel()
		{
			QFont f("Sans",8);
			f.setBold(true);

			this->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			this->setLineWidth(2);
			this->setMinimumSize(40,20);						
			this->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
			this->setAlignment(Qt::AlignCenter);
			this->setFont(f);
			this->setText("");
		}
		void SingleLabel::SetUnits(const QString& text)
		{
			_units = text;
		}
		void SingleLabel::SetToolTip(const QString& text)
		{
			QToolTip::add(this,text);
		}
		void SingleLabel::Update(float value)
		{
			QString s("");
			s = " "+s.setNum(value,'f',3)+_units;
			this->setText(s);
		}
	}
}
// END


