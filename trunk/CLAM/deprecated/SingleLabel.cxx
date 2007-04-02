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
			this->setAlignment(Qt::AlignRight);
			this->setFont(f);
			this->setText("--");
		}

		void SingleLabel::SetUnits(const QString& text)
		{
			mUnits = text;
		}

		void SingleLabel::SetToolTip(const QString& text)
		{
			QToolTip::add(this,text);
		}

		void SingleLabel::Update(const double& value, const int& prec)
		{
			QString s = " "+QString::number(value,'f',prec)+" "+mUnits;
			this->setText(s);
		}

		void SingleLabel::Clear()
		{
			this->setText("--");
		}
	}
}

// END

