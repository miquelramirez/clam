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
#include <qframe.h>
#include <qtooltip.h>
#include "PlotController.hxx"
#include "DisplaySurface.hxx"
#include "XRuler.hxx"
#include "YRuler.hxx"
#include "HScrollGroup.hxx"
#include "VScrollGroup.hxx"
#include "QtPresentation.hxx"

namespace CLAM
{
	namespace VM
	{
		QtPresentation::QtPresentation(QWidget* parent) : QtPlot(parent)
		{
			flag=false;
			Init();
		}
		
		QtPresentation::~QtPresentation()
		{
		}

		void QtPresentation::SetBackgroundColor(Color c)
		{
			double r = double(c.r)/255.0;
			double g = double(c.g)/255.0;
			double b = double(c.b)/255.0;

			_surf->SetBackgroundColor(r,g,b);
		}

		void QtPresentation::SetController(PlotController* controller)
		{
			
			_controller = controller;
			_surf->SetController(_controller);
			
			// vertical scroll and zoom ratio management 
			connect(_controller,SIGNAL(vZoomRatio(int)),this,SLOT(receivedVZoomRatio(int)));
			connect(_controller,SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));
			connect(_controller,SIGNAL(vScrollValue(int)),this,SLOT(updateVScroll(int)));

			// horizontal scroll and zoom ratio management
			connect(_controller,SIGNAL(hZoomRatio(int)),this,SLOT(receivedHZoomRatio(int)));
			connect(_controller,SIGNAL(hScrollMaxValue(int)),this,SLOT(setMaxHScroll(int)));
			connect(_controller,SIGNAL(hScrollValue(int)),this,SLOT(updateHScroll(int)));

			// connections to emit sel pos
			connect(_controller,SIGNAL(selPos(TData)),this,SIGNAL(selPos(TData)));
		}

		void QtPresentation::Init()
		{
			_controller = NULL;

			_mainLayout = new QVBoxLayout(this);
			_mainLayout->setMargin(2);
			_mainLayout->setSpacing(0);

			// top area (x ruler)
			_top = new QHBoxLayout(_mainLayout);
			topLeftHole = new QFrame(this);
			topLeftHole->setFixedSize(50,40);
			_btoggle_color = new QPushButton(topLeftHole);
			_btoggle_color->setFixedSize(25,25);
			_btoggle_color->setGeometry(12,8,25,25);
			_btoggle_color->setToggleButton(true);
			_btoggle_color->setText("CM");
			QToolTip::add(_btoggle_color,"switch display colors");
			SwitchDisplayColors(false);
			_top->addWidget(topLeftHole,0);

			_xRuler = new XRuler(this);
			_top->addWidget(_xRuler);

			topRightHole = new QFrame(this);
			topRightHole->setFixedSize(20,40);
			_top->addWidget(topRightHole,0);
			
			// middle area (y ruler, display area, vertical scroll and zoom group)
			_middle = new QHBoxLayout(_mainLayout);
			_yRuler = new YRuler(this);
			_middle->addWidget(_yRuler,0);
			
			_surf = new DisplaySurface(this);
			_surf->setMinimumSize(200,20);
			_middle->addWidget(_surf);
			
			_vs = new VScrollGroup(this);
			_middle->addWidget(_vs,0);

			// bottom area (horizontal scroll and zoom group)
			_bottom = new QHBoxLayout(_mainLayout);
			bottomLeftHole = new QFrame(this);
			bottomLeftHole->setFixedSize(50,20);
			_bottom->addWidget(bottomLeftHole,0);

			_hs = new HScrollGroup(this);
			_bottom->addWidget(_hs);

			bottomRightHole = new QFrame(this);
			bottomRightHole->setFixedSize(20,20);
			_bottom->addWidget(bottomRightHole,0);

			// Connections vertical scroll and zoom
			connect(_vs,SIGNAL(zoomIn()),this,SLOT(vZoomIn()));
		    connect(_vs,SIGNAL(zoomOut()),this,SLOT(vZoomOut()));
			connect(_vs,SIGNAL(scrollValueChanged(int)),this,SLOT(vScrollValue(int)));

			// Connections horizontal scroll and zoom group
			connect(_hs,SIGNAL(zoomIn()),this,SLOT(hZoomIn()));
		    connect(_hs,SIGNAL(zoomOut()),this,SLOT(hZoomOut()));
			connect(_hs,SIGNAL(scrollValueChanged(int)),this,SLOT(hScrollValue(int)));

			// scroll clicked/released 
			connect(_hs,SIGNAL(scrollBarClicked()),this,SIGNAL(HScrollClicked()));
			connect(_vs,SIGNAL(scrollBarClicked()),this,SIGNAL(VScrollClicked()));
			connect(_hs,SIGNAL(scrollBarReleased()),this,SIGNAL(HScrollReleased()));
			connect(_vs,SIGNAL(scrollBarReleased()),this,SIGNAL(VScrollReleased()));

			// toggle button to change display colors automatically
			connect(_btoggle_color,SIGNAL(clicked()),this,SLOT(switchColors()));
		}

		void QtPresentation::hZoomIn()
		{
			if(_controller) 
			{
				_controller->HZoomIn();
				emit HZoomIn();
			}
		}

		void QtPresentation::hZoomOut()
		{
			if(_controller) 
			{
				_controller->HZoomOut();	
				emit HZoomOut();
			}
		}

		void QtPresentation::receivedHZoomRatio(int zr)
		{
			// update horizontal zoom ratio indicator
			_hs->updateZoomRatio(zr);
			emit HZoomRatio(zr);
		}

		void QtPresentation::hScrollValue(int value)
		{
			if(_controller)
			{
				if(flag)
				{
					if(_hs->GetScrollValue()!=value)
					{
						_controller->UpdateHViewport(value);
					}
				}
				else
				{
					_controller->UpdateHViewport(value);
				}
				emit HScrollValue(value);
			}
		}

		void QtPresentation::setMaxHScroll(int value)
		{
			int max = value-_surf->width();
			if(max < 0) max=0;;
			if(max >= 0 && _hs->GetMaxScrollValue() != max)
			{
				_hs->setMaxScrollValue(max);
				emit HMaxScroll(max);
			}
		}

		void QtPresentation::updateHScroll(int value)
		{
			if(value > 0 && _hs->GetScrollValue() != value)
			{
				_hs->updateScrollValue(value);
				emit UpdatedHScroll(value);
			}
		}

		void QtPresentation::vZoomIn()
		{
			if(_controller) _controller->VZoomIn();
			emit VZoomIn();
		}

		void QtPresentation::vZoomOut()
		{
			if(_controller) _controller->VZoomOut();
			emit VZoomOut();
		}

		void QtPresentation::receivedVZoomRatio(int zr)
		{
			// update vertical zoom ratio indicator
			_vs->updateZoomRatio(zr);
			emit VZoomRatio(zr);
		}

		void QtPresentation::setMaxVScroll(int value)
		{
			int max = value-_surf->height();
			if(max < 0) max=0;
			if(max >= 0 && _vs->GetMaxScrollValue() != max)
			{
				_vs->setMaxScrollValue(max);
				emit VMaxScroll(max);
			}
		}

		void QtPresentation::vScrollValue(int value)
		{
			if(_controller)
			{
				_controller->UpdateVViewport(value);
				emit VScrollValue(value);
			}
		}

		void QtPresentation::updateVScroll(int value)
		{
			if(_vs->GetScrollValue() != value)
			{
				_vs->updateScrollValue(value);
				emit UpdatedVScroll(value);
			}
		}

		void QtPresentation::AddToMainLayout(QLayout* layout)
		{
			_mainLayout->addLayout(layout);
		}

		void QtPresentation::setXRulerScaleDiv(QwtScaleDiv div)
		{
			_xRuler->SetScaleDiv(div);
			emit xRulerScaleDiv(div);
		}

		void QtPresentation::setYRulerScaleDiv(QwtScaleDiv div)
		{
			_yRuler->SetScaleDiv(div);
			emit yRulerScaleDiv(div);
		}

		void QtPresentation::SetXRulerLabelFormat(char f,int prec,int fieldWidth)
		{
			_xRuler->SetLabelFormat(f,prec,fieldWidth);
		}

		void QtPresentation::SetYRulerLabelFormat(char f,int prec,int fieldWidth)
		{
			_yRuler->SetLabelFormat(f,prec,fieldWidth);
		}

		void QtPresentation::Label(const std::string& label)
		{
			setCaption(label.c_str());
		}

		void QtPresentation::Geometry(int x,int y,int w,int h)
		{
			setGeometry(x,y+18,w,h);
		}

		void QtPresentation::Show()
		{
			show();
		}

		void QtPresentation::Hide()
		{
			hide();
		}

		void QtPresentation::SwitchDisplayColors(bool b)
		{
			(b) ? _btoggle_color->show() : _btoggle_color->hide();
		}

		void QtPresentation::switchColors()
		{
			if(_btoggle_color->isOn())
			{
				DisplayBackgroundBlack();
			}
			else
			{
				DisplayBackgroundWhite();	
			}
			_surf->updateGL();
			emit switchColorsRequested();
		}

		void QtPresentation::RemoveXRuler()
		{
			_mainLayout->removeItem(_top);
			topLeftHole->hide();
			_xRuler->hide();
			topLeftHole->hide();
			topRightHole->hide();
		}

		void QtPresentation::RemoveYRuler()
		{
			_middle->remove(_yRuler);
			_yRuler->hide();
			topLeftHole->hide();
			bottomLeftHole->hide();
		}

		void QtPresentation::RemoveHScrollGroup()
		{
			_mainLayout->removeItem(_bottom);
			bottomLeftHole->hide();
			_hs->hide();
			bottomRightHole->hide();
		}

		void QtPresentation::RemoveVScrollGroup()
		{
			topRightHole->hide();
			_vs->hide();
			bottomRightHole->hide();
		}

		void QtPresentation::RemoveFromMainLayout(QLayout* layout)
		{
			_mainLayout->removeItem(layout);
		}

		void QtPresentation::setSelPos(TData pos)
		{
			_controller->SetSelPos(pos);
		}

		void QtPresentation::SetToggleColorOn(bool b)
		{
			_btoggle_color->setOn(b);
		}

		void QtPresentation::SetFlag(bool f)
		{
			flag=f;
		}
	}
}

// END
