#include <qlayout.h>
#include <qpushbutton.h>
#include <qtooltip.h>
#include "IconData.hxx"
#include "VZLabel.hxx"
#include "ScrollBar.hxx"
#include "VScrollGroup.hxx"

namespace CLAM
{
	namespace VM
	{
		VScrollGroup::VScrollGroup(QWidget* parent) : QWidget(parent)
		{
			Init();
		}
		
		VScrollGroup::~VScrollGroup()
		{
		}

		void VScrollGroup::Init()
		{
			QFont f("Sans",8);
			f.setBold(true);

			QPixmap icon_zoomin((const char**)icon_zoomin);
			QPixmap icon_zoomout((const char**)icon_zoomout);

			QVBoxLayout* layout = new QVBoxLayout(this);
			_zOut = new QPushButton(this);
			_zOut->setAutoRepeat(true);
			_zOut->setFixedSize(20,20);
			_zOut->setPixmap(icon_zoomout);
			QToolTip::add(_zOut,"Zoom Out");
			layout->addWidget(_zOut,0);

			_zIn = new QPushButton(this);
			_zIn->setAutoRepeat(true);
			_zIn->setFixedSize(20,20);
			_zIn->setPixmap(icon_zoomin);
			QToolTip::add(_zIn,"Zoom In");
			layout->addWidget(_zIn,0);

			_label = new VZLabel(this);
			layout->addWidget(_label);

			_sb = new ScrollBar(QScrollBar::Vertical,this);
			_sb->setFixedWidth(_sb->sizeHint().width());
			_sb->setRange(0,0);
			_sb->setSteps(20,100);
			layout->addWidget(_sb,1);

			setFixedWidth(20);

			// connections
			connect(_zIn,SIGNAL(clicked()),this,SIGNAL(zoomIn()));
		    connect(_zOut,SIGNAL(clicked()),this,SIGNAL(zoomOut()));
			connect(_sb,SIGNAL(valueChanged(int)),this,SIGNAL(scrollValueChanged(int)));
			connect(_sb,SIGNAL(clicked()),this,SIGNAL(scrollBarClicked()));
			connect(_sb,SIGNAL(released()),this,SIGNAL(scrollBarReleased()));
		}

		void VScrollGroup::updateZoomRatio(int value)
		{
			// update zoom ratio indicator
			QString s;
			s = "1:"+(s.setNum(value));
			_label->setText(s);
			emit zoomRatio(value);
		}

		void VScrollGroup::updateScrollValue(int value)
		{
			if(_sb->value() != value)
			{
				_sb->setValue(value);
				emit updatedScrollValue(value);
			}
		}

		void VScrollGroup::setMaxScrollValue(int value)
		{
			if(value >= 0 && _sb->maxValue() != value)
			{
				_sb->setMaxValue(value);
				emit maxScrollValue(value);
			}
		}

		int VScrollGroup::GetScrollValue() const
		{
			return _sb->value();
		}

		int VScrollGroup::GetMaxScrollValue() const
		{
			return _sb->maxValue();
		}
	}
}

// END

