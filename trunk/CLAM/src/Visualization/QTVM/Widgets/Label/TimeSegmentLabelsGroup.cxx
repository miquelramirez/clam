#include <qlabel.h>
#include <qtooltip.h>
#include "TimeSegmentLabelsGroup.hxx"

namespace CLAM
{
	namespace VM
	{
		TimeSegmentLabelsGroup::TimeSegmentLabelsGroup(QWidget* parent) 
			: QWidget(parent)
		{
			InitLabelsGroup();
		}

		TimeSegmentLabelsGroup::~TimeSegmentLabelsGroup()
		{
		}

		void TimeSegmentLabelsGroup::InitLabelsGroup()
		{
			QFrame* container = new QFrame(this);
			container->setFixedSize(186,30);

			QFont f("Sans",8);
			f.setBold(true);

			_beginTimeLab = new QLabel(container);
			_beginTimeLab->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			_beginTimeLab->setLineWidth(2);
			_beginTimeLab->setFixedSize(62,20);
			_beginTimeLab->setGeometry(0,3,62,20);
			_beginTimeLab->setFont(f);
			_beginTimeLab->setText("00:00,000");
			_beginTimeLab->setAlignment(Qt::AlignCenter);
			QToolTip::add(_beginTimeLab,"Begin Time");

			_endTimeLab = new QLabel(container);
			_endTimeLab->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			_endTimeLab->setLineWidth(2);
			_endTimeLab->setFixedSize(62,20);
			_endTimeLab->setGeometry(_beginTimeLab->width(),_beginTimeLab->y(),62,20);
			_endTimeLab->setFont(f);
			_endTimeLab->setText("");
			_endTimeLab->setAlignment(Qt::AlignCenter);
			QToolTip::add(_endTimeLab,"End Time");

			_durTimeLab = new QLabel(container);
			_durTimeLab->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			_durTimeLab->setLineWidth(2);
			_durTimeLab->setFixedSize(62,20);
			_durTimeLab->setGeometry(_endTimeLab->x()+_endTimeLab->width(),_beginTimeLab->y(),62,20);
			_durTimeLab->setFont(f);
			_durTimeLab->setText("");
			_durTimeLab->setAlignment(Qt::AlignCenter);
			QToolTip::add(_durTimeLab,"Duration");
		}

		QString TimeSegmentLabelsGroup::GetText(const TData& value)
		{
			QString s("");
			int tmp = int(value*1000);
			int sec = tmp/1000;
			int min = sec/60;
			sec %= 60;
			int msec = tmp%1000;
			s = s.sprintf("%02d:%02d,%03d",min,sec,msec);
			return s;
		}
				
		void TimeSegmentLabelsGroup::UpdateLabels(MediaTime time)
		{
			if(time.HasDuration())
			{
			    _beginTimeLab->setText(GetText(time.GetBegin()));
				_endTimeLab->setText(GetText(time.GetEnd()));
				_durTimeLab->setText(GetText(time.GetDuration()));
			}
			else
			{
				_beginTimeLab->setText(GetText(time.GetBegin()));
				_endTimeLab->clear();
				_durTimeLab->clear();
			}
		}
		
	}
}

//END
