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
#include <CLAM/TimeSegmentLabelsGroup.hxx>

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

			mBeginTimeLab = new QLabel(this);
			mBeginTimeLab->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			mBeginTimeLab->setLineWidth(2);
			mBeginTimeLab->setFixedHeight(20);
			mBeginTimeLab->setFont(f);
			mBeginTimeLab->setText("00:00,000");
			mBeginTimeLab->setAlignment(Qt::AlignCenter);
			QToolTip::add(mBeginTimeLab,"Begin Time");
			layout->addWidget(mBeginTimeLab);

			mEndTimeLab = new QLabel(this);
			mEndTimeLab->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			mEndTimeLab->setLineWidth(2);
			mEndTimeLab->setFixedHeight(20);
			mEndTimeLab->setFont(f);
			mEndTimeLab->setText("");
			mEndTimeLab->setAlignment(Qt::AlignCenter);
			QToolTip::add(mEndTimeLab,"End Time");
			layout->addWidget(mEndTimeLab);

			mDurTimeLab = new QLabel(this);
			mDurTimeLab->setFrameStyle(QFrame::Panel | QFrame::Sunken);
			mDurTimeLab->setLineWidth(2);
			mDurTimeLab->setFixedHeight(20);
			mDurTimeLab->setFont(f);
			mDurTimeLab->setText("");
			mDurTimeLab->setAlignment(Qt::AlignCenter);
			QToolTip::add(mDurTimeLab,"Duration");
			layout->addWidget(mDurTimeLab);
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
			    mBeginTimeLab->setText(GetText(time.GetBegin()));
				mEndTimeLab->setText(GetText(time.GetEnd()));
				mDurTimeLab->setText(GetText(time.GetDuration()));
			}
			else
			{
				mBeginTimeLab->setText(GetText(time.GetBegin()));
				mEndTimeLab->clear();
				mDurTimeLab->clear();
			}
		}
		
	}
}

//END

