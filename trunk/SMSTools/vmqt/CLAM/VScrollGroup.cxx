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
#include <qpushbutton.h>
#include <qtooltip.h>
#include <CLAM/IconData.hxx>
#include <CLAM/VZLabel.hxx>
#include <CLAM/ScrollBar.hxx>
#include <CLAM/VScrollGroup.hxx>

namespace CLAM
{
	namespace VM
	{
		VScrollGroup::VScrollGroup(QWidget* parent) 
			: QWidget(parent)
		{
			setFocusPolicy(QWidget::NoFocus);
			Init();
		}
		
		VScrollGroup::~VScrollGroup()
		{
		}

		void VScrollGroup::Init()
		{
			QFont f("Sans",8);
			f.setBold(true);

			QVBoxLayout* layout = new QVBoxLayout(this);
			mZOut = new QPushButton(this);
			mZOut->setAutoRepeat(true);
			mZOut->setFixedSize(20,20);
			mZOut->setFocusPolicy(QWidget::NoFocus);
			mZOut->setPixmap(QPixmap((const char**)icon_zoomout));
			QToolTip::add(mZOut,"Zoom Out");
			layout->addWidget(mZOut,0);

			mZIn = new QPushButton(this);
			mZIn->setAutoRepeat(true);
			mZIn->setFixedSize(20,20);
			mZIn->setFocusPolicy(QWidget::NoFocus);
			mZIn->setPixmap(QPixmap((const char**)icon_zoomin));
			QToolTip::add(mZIn,"Zoom In");
			layout->addWidget(mZIn,0);

			mLabel = new VZLabel(this);
			layout->addWidget(mLabel);

			mScrollBar= new ScrollBar(QScrollBar::Vertical,this);
			mScrollBar->setFixedWidth(mScrollBar->sizeHint().width());
			mScrollBar->setRange(0,0);
			mScrollBar->setSteps(20,100);
			layout->addWidget(mScrollBar,1);

			setFixedWidth(20);

			setPaletteBackgroundColor(mZIn->paletteBackgroundColor());

			// connections
			connect(mZIn,SIGNAL(clicked()),this,SIGNAL(zoomIn()));
		    connect(mZOut,SIGNAL(clicked()),this,SIGNAL(zoomOut()));
			connect(mScrollBar,SIGNAL(valueChanged(int)),this,SIGNAL(scrollValueChanged(int)));
			connect(mScrollBar,SIGNAL(clicked()),this,SIGNAL(scrollBarClicked()));
			connect(mScrollBar,SIGNAL(released()),this,SIGNAL(scrollBarReleased()));
		}

		void VScrollGroup::updateZoomRatio(double value)
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
		    mLabel->setText(s);
		    emit zoomRatio(value);
		}

		void VScrollGroup::updateScrollValue(int value)
		{
			if(mScrollBar->value() != value)
			{
				mScrollBar->setValue(value);
				emit updatedScrollValue(value);
			}
		}

		void VScrollGroup::setMaxScrollValue(int value)
		{
			if(value >= 0 && mScrollBar->maxValue() != value)
			{
				mScrollBar->setMaxValue(value);
				emit maxScrollValue(value);
			}
		}

		int VScrollGroup::GetScrollValue() const
		{
			return mScrollBar->value();
		}

		int VScrollGroup::GetMaxScrollValue() const
		{
			return mScrollBar->maxValue();
		}
	}
}

// END

