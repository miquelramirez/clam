/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#include <QtGui/QGridLayout>
#include "Assert.hxx"
#include "vmRuler.hxx"
#include "vmScrollGroup.hxx"
#include "vmPlot2D.hxx"
#include "vmGrid.hxx"
#include "vmLocatorRenderer.hxx"
#include "vmDataArrayRenderer.hxx"
#include "vmDataTypes.hxx"
#include "vmAudioStereoPlot.hxx"

namespace CLAM
{
	namespace VM
	{
		AudioStereoPlot::AudioStereoPlot(QWidget* parent)
			: MultiDisplayPlot(parent)
		{
			InitAudioStereoPlot();
		}

		AudioStereoPlot::~AudioStereoPlot()
		{
		}

		void AudioStereoPlot::SetData(const Audio& channel0, const Audio& channel1, bool update)
		{
			CLAM_ASSERT(channel0.GetSize() == channel1.GetSize(), "Size of channels 0 and 1 must be the same!");
			
			if(!update)
			{
				mDisplay[MASTER]->SetXRange(0.0,channel0.GetDuration()/1000.0);
				mDisplay[SLAVE]->SetXRange(0.0,channel0.GetDuration()/1000.0);
				mDisplay[MASTER]->SetYRange(-1.0,1.0);
				mDisplay[SLAVE]->SetYRange(-1.0,1.0);
				std::pair<int, int> zoom_steps = GetZoomSteps(channel0.GetBuffer().Size());
				mDisplay[MASTER]->SetZoomSteps(zoom_steps.first,zoom_steps.second);
			}
			_grid0->SetGridSteps(channel0.GetDuration()/1000.0,1.0);
			_channel0->SetData(channel0.GetBuffer());
			_grid1->SetGridSteps(channel0.GetDuration()/1000.0,1.0);
			_channel1->SetData(channel1.GetBuffer());
		}

		void AudioStereoPlot::backgroundWhite()
		{
			setPalette(Qt::white);
			mXRuler->SetBackgroundColor(QColor(255,255,255));
			mXRuler->SetForegroundColor(QColor(0,0,0));
			mYRuler0->SetBackgroundColor(QColor(255,255,255));
			mYRuler0->SetForegroundColor(QColor(0,0,0));
			mYRuler1->SetBackgroundColor(QColor(255,255,255));
			mYRuler1->SetForegroundColor(QColor(0,0,0));
			mHScroll->setPalette(Qt::white);
			mVScroll->setPalette(Qt::white);
			mDisplay[MASTER]->SetBackgroundColor(QColor(255,255,255));
			mDisplay[SLAVE]->SetBackgroundColor(QColor(255,255,255));
			_grid0->SetGridColor(QColor(0,0,255));
			_grid1->SetGridColor(QColor(0,0,255));
			_channel0->SetDataColor(QColor(0,0,255));
			_channel1->SetDataColor(QColor(0,0,255));
			_locator0->SetLocatorColor(QColor(250,160,30));
			_locator1->SetLocatorColor(QColor(250,160,30));
		}

		void AudioStereoPlot::backgroundBlack()
		{
			setPalette(Qt::black);
			mXRuler->SetBackgroundColor(QColor(0,0,0));
			mXRuler->SetForegroundColor(QColor(255,255,255));
			mYRuler0->SetBackgroundColor(QColor(0,0,0));
			mYRuler0->SetForegroundColor(QColor(255,255,255));
			mYRuler1->SetBackgroundColor(QColor(0,0,0));
			mYRuler1->SetForegroundColor(QColor(255,255,255));
			mHScroll->setPalette(Qt::darkGreen);
			mVScroll->setPalette(Qt::darkGreen);
			mDisplay[MASTER]->SetBackgroundColor(QColor(0,0,0));
			mDisplay[SLAVE]->SetBackgroundColor(QColor(0,0,0));
			_grid0->SetGridColor(QColor(0,255,0));
			_grid1->SetGridColor(QColor(0,255,0));
			_channel0->SetDataColor(QColor(0,255,0));
			_channel1->SetDataColor(QColor(0,255,0));
			_locator0->SetLocatorColor(QColor(255,0,0));
			_locator1->SetLocatorColor(QColor(255,0,0));
		}

		void AudioStereoPlot::updateLocator(double value)
		{
			_locator0->updateLocator(value);
			_locator1->updateLocator(value);
		}

		void AudioStereoPlot::updateLocator(double value,bool flag)
		{
			_locator0->updateLocator(value,flag);
			_locator1->updateLocator(value,flag);
		}

		void AudioStereoPlot::setMaxVScroll(int value)
		{
			int max = value-mDisplay[MASTER]->height();
			if(max < 0) max=0;
			if(mVScroll->GetMaxScrollValue() == max) return;
			mVScroll->setMaxScrollValue(max);

		}

		void AudioStereoPlot::CreateDisplay()
		{
			mDisplay.resize(2);
			for(unsigned i=0; i < mDisplay.size(); i++) 
			{
				mDisplay[i] = new Plot2D(this);
			}
			_grid0 = new Grid;
			_grid1 = new Grid;
			_locator0 = new Locator;
			_locator1 = new Locator;
			_channel0 = new DataArrayRenderer;
			_channel1 = new DataArrayRenderer;
			mDisplay[MASTER]->AddRenderer("grid0", _grid0);
			mDisplay[SLAVE]->AddRenderer("grid1", _grid1);
			mDisplay[MASTER]->AddRenderer("locator0", _locator0);
			mDisplay[SLAVE]->AddRenderer("locator1", _locator1);
			mDisplay[MASTER]->AddRenderer("channel0", _channel0);
			mDisplay[SLAVE]->AddRenderer("channel1", _channel1);

			mDisplay[MASTER]->SendToBack("channel0");
			mDisplay[MASTER]->SendToBack("grid0");
			mDisplay[SLAVE]->SendToBack("channel1");
			mDisplay[SLAVE]->SendToBack("grid1");

			_grid0->ShowGrid(true);
			_grid1->ShowGrid(true);

			connect(
				_locator0, SIGNAL(regionChanged(double,double,bool)),
				_locator1, SLOT(updateRegion(double,double,bool)));
			connect(
				_locator1, SIGNAL(regionChanged(double,double,bool)),
				_locator0, SLOT(updateRegion(double,double,bool)));
			connect(
				_locator0, SIGNAL(selectedRegion(double,double)),
				this, SIGNAL(selectedRegion(double,double)));
			connect(
				_locator1, SIGNAL(selectedRegion(double,double)),
				this, SIGNAL(selectedRegion(double,double)));
		}
	
		void AudioStereoPlot::InitAudioStereoPlot()
		{
			// common inits			
			InitMultiDisplayPlot(MASTER);

			// own inits
			mYRuler0 = new Ruler(this,CLAM::VM::eLeft);
			mYRuler1 = new Ruler(this,CLAM::VM::eLeft);
			mVScroll = new ScrollGroup(CLAM::VM::eVertical,this);

			// layout
			mLayout = new QGridLayout(this);
			mLayout->setMargin(0);
			mLayout->setSpacing(1);
			
			mLayout->addWidget(mXRuler,0,1);
			mLayout->addWidget(mYRuler0,1,0);
			mLayout->addWidget(mYRuler1,2,0);
			mLayout->addWidget(mDisplay[MASTER],1,1);
			mLayout->addWidget(mDisplay[SLAVE],2,1);
			mLayout->addWidget(mVScroll,1,2,2,1);
			mLayout->addWidget(mHScroll,3,1);

//			mXRuler->SetStep(0.01);
//			mYRuler0->SetStep(0.05);
//			mYRuler1->SetStep(0.05);
		   
			// connect yrulers and vertical scroll group
			mDisplay[MASTER]->setYRuler(mYRuler0);
			mDisplay[SLAVE]->setYRuler(mYRuler1);
			
			connect(mVScroll,SIGNAL(zoomIn()),mDisplay[MASTER],SLOT(vZoomIn()));
			connect(mVScroll,SIGNAL(zoomOut()),mDisplay[MASTER],SLOT(vZoomOut()));
			connect(mVScroll,SIGNAL(scrollValueChanged(int)),mDisplay[MASTER],SLOT(updateVScrollValue(int)));

			connect(mDisplay[MASTER],SIGNAL(vZoomRatio(QString)),mVScroll,SLOT(updateZoomRatio(QString)));
			connect(mDisplay[MASTER],SIGNAL(vScrollValue(int)),mVScroll,SLOT(updateScrollValue(int)));
			connect(mDisplay[MASTER],SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));
			
			// synchronize vertical scrolling
			connect(mDisplay[MASTER],SIGNAL(yRangeChanged(double,double)),
					mDisplay[SLAVE],SLOT(setVBounds(double,double)));

			backgroundWhite();
		}

		std::pair<int,int> AudioStereoPlot::GetZoomSteps(TSize size)
		{
			double n = 100.0;
			int xratio = 0;
			while(n < size)
			{
				n *= 2.0;
				xratio++;
			}
			
			n = 0.2;
			int yratio = 0;
			while(n < 2.0)
			{
				n *= 2.0;
				yratio++;
			}
			return std::make_pair(--xratio,--yratio);
		}
	}
}

// END

