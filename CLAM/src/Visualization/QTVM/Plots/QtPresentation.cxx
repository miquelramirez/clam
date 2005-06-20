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
#include "Ruler.hxx"
#include "HScrollGroup.hxx"
#include "VScrollGroup.hxx"
#include "QtPresentation.hxx"
#include "EditTagDialog.hxx"

namespace CLAM
{
    namespace VM
    {
		QtPresentation::QtPresentation(QWidget* parent, const char * name, WFlags f) 
			: QtPlot(parent,name,f)
			, mController(0)
			, mFlag(false)
			, editTagDlg(0)
		{
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

			mDisplaySurface->SetBackgroundColor(r,g,b);
		}

		void QtPresentation::SetController(PlotController* controller)
		{
			mController = controller;
			mDisplaySurface->SetController(mController);
			
			// vertical scroll and zoom ratio management 
			connect(mController,SIGNAL(vZoomRatio(double)),this,SLOT(receivedVZoomRatio(double)));
			connect(mController,SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));
			connect(mController,SIGNAL(vScrollValue(int)),this,SLOT(updateVScroll(int)));

			// horizontal scroll and zoom ratio management
			connect(mController,SIGNAL(hZoomRatio(double)),this,SLOT(receivedHZoomRatio(double)));
			connect(mController,SIGNAL(hScrollMaxValue(int)),this,SLOT(setMaxHScroll(int)));
			connect(mController,SIGNAL(hScrollValue(int)),this,SLOT(updateHScroll(int)));

			// connections to emit sel pos
			connect(mController,SIGNAL(selPos(TData)),this,SIGNAL(selPos(TData)));

			// connections to emit signals for segmentation marks management
			connect(mController,SIGNAL(insertedMark(unsigned)),this,SIGNAL(insertedMark(unsigned)));
			connect(mController,SIGNAL(removedMark(int,unsigned)),this,SIGNAL(removedMark(int,unsigned)));
			connect(mController,SIGNAL(updatedMark(int,unsigned)),this,SIGNAL(updatedMark(int,unsigned)));

			// connect rulers
			connect(mController,SIGNAL(xRulerRange(double,double)),mXRuler,SLOT(updateRange(double,double)));
			connect(mController,SIGNAL(yRulerRange(double,double)),mYRuler,SLOT(updateRange(double,double)));

			// selected x pos
			connect(mController,SIGNAL(selectedXPos(double)),this,SIGNAL(selectedXPos(double)));

			// to show edit tag dialog
			connect(mController,SIGNAL(requestSegmentationTag()),this,SLOT(showEditTagDialog()));

			// signal updated tag 
			connect(mController,SIGNAL(updatedTag(int,QString)),this,SIGNAL(updatedTag(int,QString)));
		}

		void QtPresentation::Init()
		{
			QFont f("Sans",8);
			f.setBold(true);

			mMainLayout = new QVBoxLayout(this);
			mMainLayout->setMargin(2);
			mMainLayout->setSpacing(0);

			// top area (x ruler)
			mXRuler = new Ruler(this,CLAM::VM::Top);

			QFont ref = mXRuler->Font();
			QFontMetrics fm(ref);
			int initial_yruler_width=fm.width("X:-0.0e+00");
	    
			mTopLayout = new QHBoxLayout(mMainLayout);
			topLeftHole = new QFrame(this);
			topLeftHole->setFixedSize(initial_yruler_width,40);
			mToggleColor = new QPushButton(topLeftHole);
			mToggleColor->setFont(f);
			mToggleColor->setFixedSize(25,25);
			mToggleColor->setGeometry(12,8,25,25);
			mToggleColor->setToggleButton(true);
			mToggleColor->setText("CM");
			QToolTip::add(mToggleColor,"switch display colors");
			SwitchDisplayColors(false);

			mXRuler->setFixedHeight(topLeftHole->height());
	    
			topRightHole = new QFrame(this);
			topRightHole->setFixedSize(20,40);

			mTopLayout->addWidget(topLeftHole);
			mTopLayout->addWidget(mXRuler);
			mTopLayout->addWidget(topRightHole);
			
			// middle area (y ruler, display area, vertical scroll and zoom group)
			mMiddleLayout = new QHBoxLayout(mMainLayout);
			mYRuler = new Ruler(this,CLAM::VM::Left);
			mYRuler->setFixedWidth(initial_yruler_width);
			mMiddleLayout->addWidget(mYRuler);
			
			mDisplaySurface = new DisplaySurface(this);
			mDisplaySurface->setMinimumSize(200,20);
			mMiddleLayout->addWidget(mDisplaySurface);
			
			mVScrollBar = new VScrollGroup(this);
			mMiddleLayout->addWidget(mVScrollBar);

			// bottom area (horizontal scroll and zoom group)
			mBottomLayout = new QHBoxLayout(mMainLayout);
			bottomLeftHole = new QFrame(this);
			bottomLeftHole->setFixedSize(initial_yruler_width,20);
			mBottomLayout->addWidget(bottomLeftHole);

			mHScrollBar = new HScrollGroup(this);
			mBottomLayout->addWidget(mHScrollBar);

			bottomRightHole = new QFrame(this);
			bottomRightHole->setFixedSize(20,20);
			mBottomLayout->addWidget(bottomRightHole);

			// Connections vertical scroll and zoom
			connect(mVScrollBar,SIGNAL(zoomIn()),this,SLOT(vZoomIn()));
			connect(mVScrollBar,SIGNAL(zoomOut()),this,SLOT(vZoomOut()));
			connect(mVScrollBar,SIGNAL(scrollValueChanged(int)),this,SLOT(vScrollValue(int)));

			// Connections horizontal scroll and zoom group
			connect(mHScrollBar,SIGNAL(zoomIn()),this,SLOT(hZoomIn()));
			connect(mHScrollBar,SIGNAL(zoomOut()),this,SLOT(hZoomOut()));
			connect(mHScrollBar,SIGNAL(scrollValueChanged(int)),this,SLOT(hScrollValue(int)));

			// scroll clicked/released 
			connect(mHScrollBar,SIGNAL(scrollBarClicked()),this,SIGNAL(HScrollClicked()));
			connect(mVScrollBar,SIGNAL(scrollBarClicked()),this,SIGNAL(VScrollClicked()));
			connect(mHScrollBar,SIGNAL(scrollBarReleased()),this,SIGNAL(HScrollReleased()));
			connect(mVScrollBar,SIGNAL(scrollBarReleased()),this,SIGNAL(VScrollReleased()));

			// toggle button to change display colors automatically
			connect(mToggleColor,SIGNAL(clicked()),this,SLOT(switchColors()));

			// connect ruler signals
			connect(mXRuler,SIGNAL(valueChanged(double,double)),this,SIGNAL(xRulerRange(double,double)));
			connect(mYRuler,SIGNAL(valueChanged(double,double)),this,SIGNAL(yRulerRange(double,double)));

		}

		void QtPresentation::hZoomIn()
		{
			if(mController) 
			{
				mController->HZoomIn();
				emit HZoomIn();
			}
		}

		void QtPresentation::hZoomOut()
		{
			if(mController) 
			{
				mController->HZoomOut();	
				emit HZoomOut();
			}
		}

		void QtPresentation::receivedHZoomRatio(double zr)
		{
			// update horizontal zoom ratio indicator
			mHScrollBar->updateZoomRatio(zr);
			emit HZoomRatio(zr);
		}

		void QtPresentation::hScrollValue(int value)
		{
			if(mController)
			{
				if(mFlag)
				{
					if(mHScrollBar->GetScrollValue()!=value)
					{
						mController->UpdateHViewport(value);
					}
				}
				else
				{
					mController->UpdateHViewport(value);
				}
				emit HScrollValue(value);
			}
		}

		void QtPresentation::setMaxHScroll(int value)
		{
			int max = value-mDisplaySurface->width();
			if(max < 0) max=0;;
			if(max >= 0 && mHScrollBar->GetMaxScrollValue() != max)
			{
				mHScrollBar->setMaxScrollValue(max);
				emit HMaxScroll(max);
			}
		}

		void QtPresentation::updateHScroll(int value)
		{
			if(value > 0 && mHScrollBar->GetScrollValue() != value)
			{
				mHScrollBar->updateScrollValue(value);
				emit UpdatedHScroll(value);
			}
		}

		void QtPresentation::vZoomIn()
		{
			if(mController) mController->VZoomIn();
			emit VZoomIn();
		}

		void QtPresentation::vZoomOut()
		{
			if(mController) mController->VZoomOut();
			emit VZoomOut();
		}

		void QtPresentation::receivedVZoomRatio(double zr)
		{
			// update vertical zoom ratio indicator
			mVScrollBar->updateZoomRatio(zr);
			emit VZoomRatio(zr);
		}

		void QtPresentation::setMaxVScroll(int value)
		{
			int max = value-mDisplaySurface->height();
			if(max < 0) max=0;
			if(max >= 0 && mVScrollBar->GetMaxScrollValue() != max)
			{
				mVScrollBar->setMaxScrollValue(max);
				emit VMaxScroll(max);
			}
		}

		void QtPresentation::vScrollValue(int value)
		{
			if(mController)
			{
				mController->UpdateVViewport(value);
				emit VScrollValue(value);
			}
		}

		void QtPresentation::updateVScroll(int value)
		{
			if(mVScrollBar->GetScrollValue() != value)
			{
				mVScrollBar->updateScrollValue(value);
				emit UpdatedVScroll(value);
			}
		}

		void QtPresentation::AddToMainLayout(QLayout* layout)
		{
			mMainLayout->addLayout(layout);
		}

		void QtPresentation::setXRulerRange(double min, double max)
		{
			mXRuler->updateRange(min,max);
		}

		void QtPresentation::setYRulerRange(double min, double max)
		{
			mYRuler->updateRange(min,max);
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
			(b) ? mToggleColor->show() : mToggleColor->hide();
		}

		void QtPresentation::switchColors()
		{
			if(mToggleColor->isOn())
			{
				DisplayBackgroundBlack();
			}
			else
			{
				DisplayBackgroundWhite();	
			}
			mDisplaySurface->updateGL();
			emit switchColorsRequested();
		}

		void QtPresentation::RemoveXRuler()
		{
			mMainLayout->removeItem(mTopLayout);
			topLeftHole->hide();
			mXRuler->hide();
			topLeftHole->hide();
			topRightHole->hide();
		}

		void QtPresentation::RemoveYRuler()
		{
			mMiddleLayout->remove(mYRuler);
			mYRuler->hide();
			topLeftHole->hide();
			bottomLeftHole->hide();
		}

		void QtPresentation::RemoveHScrollGroup()
		{
			mMainLayout->removeItem(mBottomLayout);
			bottomLeftHole->hide();
			mHScrollBar->hide();
			bottomRightHole->hide();
		}

		void QtPresentation::RemoveVScrollGroup()
		{
			topRightHole->hide();
			mVScrollBar->hide();
			bottomRightHole->hide();
		}

		void QtPresentation::RemoveFromMainLayout(QLayout* layout)
		{
			mMainLayout->removeItem(layout);
		}

		void QtPresentation::setSelPos(TData pos)
		{
			mController->SetSelPos(pos);
		}

		void QtPresentation::SetToggleColorOn(bool b)
		{
			mToggleColor->setOn(b);
		}

		void QtPresentation::SetFlag(bool f)
		{
			mFlag=f;
		}

		void QtPresentation::insertMark(unsigned elem)
		{
			mController->InsertMark(elem);
		}
	    
		void QtPresentation::removeMark(int index, unsigned elem)
		{
			mController->RemoveMark(index,elem);
		}

		void QtPresentation::updateMark(int index, unsigned elem)
		{
			mController->UpdateMark(index,elem);
		}

		int QtPresentation::YRulerWidth()
		{
			return mYRuler->width();
		}
    
		void QtPresentation::initialYRulerRange(double min, double max)
		{
			int length_min = QString::number(min,'f',2).length();
			int length_max = QString::number(max,'f',2).length();

			QFontMetrics fm(mYRuler->Font());

			int width = (length_min > length_max) ? fm.width(QString::number(min,'f',2)) : fm.width(QString::number(max,'f',2));
			width += 10;

			if(width > YRulerWidth())
			{
				int bt_x = (width-mToggleColor->width())/2;
				topLeftHole->setFixedSize(width,topLeftHole->height());
				mToggleColor->setGeometry(bt_x,mToggleColor->y(),mToggleColor->width(),mToggleColor->height());
				mYRuler->setFixedWidth(width);
				bottomLeftHole->setFixedSize(width,bottomLeftHole->height());
			}
		}

		QFont& QtPresentation::RulerFont()
		{
			return mYRuler->Font();
		}

		void QtPresentation::SetYRulerWidth(int w)
		{
			mYRuler->setFixedWidth(w);
		}

		void QtPresentation::showEditTagDialog()
		{
			if(editTagDlg)
			{
				delete editTagDlg;
				editTagDlg=0;    
			}

			editTagDlg = new EditTagDialog(mController->GetTag(),this);
			if( editTagDlg->exec() == QDialog::Accepted )
			{
				mController->SetSegmentationTag(editTagDlg->text());
			}
		}

		void QtPresentation::updateTag(int index, QString tag)
		{
			mController->UpdateTag(index, tag);
		}
    }
}

// END

