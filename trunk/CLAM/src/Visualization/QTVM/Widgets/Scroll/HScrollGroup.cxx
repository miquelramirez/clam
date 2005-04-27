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
#include <qpushbutton.h>
#include <qtooltip.h>
#include "IconData.hxx"
#include "ScrollBar.hxx"
#include "HScrollGroup.hxx"

namespace CLAM
{
	namespace VM
	{
		HScrollGroup::HScrollGroup(QWidget* parent) : QWidget(parent)
		{
			Init();
		}
		
		HScrollGroup::~HScrollGroup()
		{
		}

		void HScrollGroup::Init()
		{
			QFont f("Sans",8);
			f.setBold(true);

			QPixmap icon_zoomin((const char**)icon_zoomin);
			QPixmap icon_zoomout((const char**)icon_zoomout);

			QHBoxLayout* layout = new QHBoxLayout(this);
			_sb = new ScrollBar(QScrollBar::Horizontal,this);
			_sb->setFixedHeight(_sb->sizeHint().height());
			_sb->setRange(0,0);
			_sb->setSteps(20,100);
			layout->addWidget(_sb,1);

			_label = new QLabel(this);
			_label->setMinimumSize(30,20);
			_label->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
			_label->setFont(f);
			_label->setAlignment(Qt::AlignCenter);
			QToolTip::add(_label,"Zoom Ratio");
			layout->addWidget(_label);

			_zIn = new QPushButton(this);
			_zIn->setAutoRepeat(true);
			_zIn->setFixedSize(20,20);
			_zIn->setPixmap(icon_zoomin);
			QToolTip::add(_zIn,"Zoom In");
			layout->addWidget(_zIn,0);

			_zOut = new QPushButton(this);
			_zOut->setAutoRepeat(true);
			_zOut->setFixedSize(20,20);
			_zOut->setPixmap(icon_zoomout);
			QToolTip::add(_zOut,"Zoom Out");
			layout->addWidget(_zOut,0);

			// connections
			connect(_zIn,SIGNAL(clicked()),this,SIGNAL(zoomIn()));
		    connect(_zOut,SIGNAL(clicked()),this,SIGNAL(zoomOut()));
			connect(_sb,SIGNAL(valueChanged(int)),this,SIGNAL(scrollValueChanged(int)));
			connect(_sb,SIGNAL(clicked()),this,SIGNAL(scrollBarClicked()));
			connect(_sb,SIGNAL(released()),this,SIGNAL(scrollBarReleased()));
		}

		void HScrollGroup::updateZoomRatio(double value)
		{
		    if(value < 1) value = 1.0;
		    // update zoom ratio indicator
		    QString s("1:");
		    if(value > 999999)
		    {
			s += QString::number(value,'e',1);
		    }
		    else
		    {
			s += QString::number(value,'f',0);
		    }
		    _label->setText(s);
		    emit zoomRatio(value);
		}

		void HScrollGroup::updateScrollValue(int value)
		{
			if(_sb->value() != value)
			{
				_sb->setValue(value);
				emit updatedScrollValue(value);
			}
		}

		void HScrollGroup::setMaxScrollValue(int value)
		{
			if(value >= 0 && _sb->maxValue() != value)
			{
				_sb->setMaxValue(value);
				emit maxScrollValue(value);
			}
		}

		int HScrollGroup::GetScrollValue() const
		{
			return _sb->value();
		}

		int HScrollGroup::GetMaxScrollValue() const
		{
			return _sb->maxValue();
		}
	}
}

// END

