/*
 * Copyright (c) 2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <qlayout.h>
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
		        QBoxLayout* layout = new QHBoxLayout(this);
			layout->setMargin(0);
			layout->setSpacing(0);
			
			QFont f("Sans",8);
			f.setBold(true);

			_beginTimeLab = new QLabel(this);
			_beginTimeLab->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			_beginTimeLab->setLineWidth(2);
			_beginTimeLab->setFixedHeight(20);
			_beginTimeLab->setFont(f);
			_beginTimeLab->setText("00:00,000");
			_beginTimeLab->setAlignment(Qt::AlignCenter);
			QToolTip::add(_beginTimeLab,"Begin Time");
			layout->addWidget(_beginTimeLab);

			_endTimeLab = new QLabel(this);
			_endTimeLab->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			_endTimeLab->setLineWidth(2);
			_endTimeLab->setFixedHeight(20);
			_endTimeLab->setFont(f);
			_endTimeLab->setText("");
			_endTimeLab->setAlignment(Qt::AlignCenter);
			QToolTip::add(_endTimeLab,"End Time");
			layout->addWidget(_endTimeLab);

			_durTimeLab = new QLabel(this);
			_durTimeLab->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			_durTimeLab->setLineWidth(2);
			_durTimeLab->setFixedHeight(20);
			_durTimeLab->setFont(f);
			_durTimeLab->setText("");
			_durTimeLab->setAlignment(Qt::AlignCenter);
			QToolTip::add(_durTimeLab,"Duration");
			layout->addWidget(_durTimeLab);
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
