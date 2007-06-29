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

		void AudioStereoPlot::SetData(const Audio& chn0, const Audio& chn1, bool update)
		{
			CLAM_ASSERT(chn0.GetSize() == chn1.GetSize(), "Size of channels 0 and 1 must be the same!");
			
			if(!update)
			{
				mDisplay[MASTER]->SetXRange(0.0,chn0.GetDuration()/1000.0);
				mDisplay[SLAVE]->SetXRange(0.0,chn0.GetDuration()/1000.0);
				mDisplay[MASTER]->SetYRange(-1.0,1.0);
				mDisplay[SLAVE]->SetYRange(-1.0,1.0);
				std::pair<int, int> zoom_steps = GetZoomSteps(chn0.GetBuffer().Size());
				mDisplay[MASTER]->SetZoomSteps(zoom_steps.first,zoom_steps.second);
			}
			static_cast<Grid*>(mDisplay[MASTER]->GetRenderer("grid0"))->SetGridSteps(chn0.GetDuration()/1000.0,1.0);
			static_cast<DataArrayRenderer*>(mDisplay[MASTER]->GetRenderer("chn0"))->SetData(chn0.GetBuffer());
			static_cast<Grid*>(mDisplay[SLAVE]->GetRenderer("grid1"))->SetGridSteps(chn0.GetDuration()/1000.0,1.0);
			static_cast<DataArrayRenderer*>(mDisplay[SLAVE]->GetRenderer("chn1"))->SetData(chn1.GetBuffer());
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
			static_cast<Grid*>(mDisplay[MASTER]->GetRenderer("grid0"))->SetGridColor(QColor(0,0,255));
			static_cast<Grid*>(mDisplay[SLAVE]->GetRenderer("grid1"))->SetGridColor(QColor(0,0,255));
			static_cast<DataArrayRenderer*>(mDisplay[MASTER]->GetRenderer("chn0"))->SetDataColor(QColor(0,0,255));
			static_cast<DataArrayRenderer*>(mDisplay[SLAVE]->GetRenderer("chn1"))->SetDataColor(QColor(0,0,255));
			static_cast<Locator*>(mDisplay[MASTER]->GetRenderer("locator0"))->SetLocatorColor(QColor(250,160,30));
			static_cast<Locator*>(mDisplay[SLAVE]->GetRenderer("locator1"))->SetLocatorColor(QColor(250,160,30));
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
			static_cast<Grid*>(mDisplay[MASTER]->GetRenderer("grid0"))->SetGridColor(QColor(0,255,0));
			static_cast<Grid*>(mDisplay[SLAVE]->GetRenderer("grid1"))->SetGridColor(QColor(0,255,0));
			static_cast<DataArrayRenderer*>(mDisplay[MASTER]->GetRenderer("chn0"))->SetDataColor(QColor(0,255,0));
			static_cast<DataArrayRenderer*>(mDisplay[SLAVE]->GetRenderer("chn1"))->SetDataColor(QColor(0,255,0));
			static_cast<Locator*>(mDisplay[MASTER]->GetRenderer("locator0"))->SetLocatorColor(QColor(255,0,0));
			static_cast<Locator*>(mDisplay[SLAVE]->GetRenderer("locator1"))->SetLocatorColor(QColor(255,0,0));
		}

		void AudioStereoPlot::updateLocator(double value)
		{
			static_cast<Locator*>(mDisplay[MASTER]->GetRenderer("locator0"))->updateLocator(value);
			static_cast<Locator*>(mDisplay[SLAVE]->GetRenderer("locator1"))->updateLocator(value);
		}

		void AudioStereoPlot::updateLocator(double value,bool flag)
		{
			static_cast<Locator*>(mDisplay[MASTER]->GetRenderer("locator0"))->updateLocator(value,flag);
			static_cast<Locator*>(mDisplay[SLAVE]->GetRenderer("locator1"))->updateLocator(value,flag);
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
			mDisplay[MASTER]->AddRenderer("grid0", new Grid());
			mDisplay[MASTER]->AddRenderer("locator0", new Locator());
			mDisplay[MASTER]->AddRenderer("chn0", new DataArrayRenderer());
			mDisplay[SLAVE]->AddRenderer("grid1", new Grid());
			mDisplay[SLAVE]->AddRenderer("locator1", new Locator());
			mDisplay[SLAVE]->AddRenderer("chn1", new DataArrayRenderer());

			mDisplay[MASTER]->SendToBack("chn0");
			mDisplay[MASTER]->SendToBack("grid0");
			mDisplay[SLAVE]->SendToBack("chn1");
			mDisplay[SLAVE]->SendToBack("grid1");

			static_cast<Grid*>(mDisplay[MASTER]->GetRenderer("grid0"))->ShowGrid(true);
			static_cast<Grid*>(mDisplay[SLAVE]->GetRenderer("grid1"))->ShowGrid(true);

			connect(static_cast<Locator*>(mDisplay[MASTER]->GetRenderer("locator0")),
					SIGNAL(regionChanged(double,double,bool)),
					static_cast<Locator*>(mDisplay[SLAVE]->GetRenderer("locator1")),
					SLOT(updateRegion(double,double,bool)));
			connect(static_cast<Locator*>(mDisplay[SLAVE]->GetRenderer("locator1")),
					SIGNAL(regionChanged(double,double,bool)),
					static_cast<Locator*>(mDisplay[MASTER]->GetRenderer("locator0")),
					SLOT(updateRegion(double,double,bool)));
			connect(static_cast<Locator*>(mDisplay[MASTER]->GetRenderer("locator0")),
					SIGNAL(selectedRegion(double,double)),this,SIGNAL(selectedRegion(double,double)));
			connect(static_cast<Locator*>(mDisplay[SLAVE]->GetRenderer("locator1")),
					SIGNAL(selectedRegion(double,double)),this,SIGNAL(selectedRegion(double,double)));
		}
	
		void AudioStereoPlot::InitAudioStereoPlot()
		{
			// common inits			
			InitMultiDisplayPlot(MASTER);

			// own inits
			mYRuler0 = new Ruler(this,CLAM::VM::eLeft);
			mYRuler1 = new Ruler(this,CLAM::VM::eLeft);
			mVScroll = new ScrollGroup(CLAM::VM::eVertical,this);
			QFontMetrics fm(mYRuler0->GetFont());
			int yruler_width = fm.width("-0.0e+00")+12;
			mYRuler0->setFixedWidth(yruler_width);
			mYRuler1->setFixedWidth(yruler_width);

			mVScroll->setFixedWidth(20);

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

			mXRuler->SetStep(0.01);
			mYRuler0->SetStep(0.05);
			mYRuler1->SetStep(0.05);
		   
			// connect yrulers and vertical scroll group
			connect(mDisplay[MASTER],SIGNAL(yRulerRange(double,double)),
					mYRuler0,SLOT(updateRange(double,double)));
			connect(mDisplay[MASTER],SIGNAL(yRulerRange(double,double)),
					mYRuler1,SLOT(updateRange(double,double)));
			
			connect(mVScroll,SIGNAL(zoomIn()),mDisplay[MASTER],SLOT(vZoomIn()));
			connect(mVScroll,SIGNAL(zoomOut()),mDisplay[MASTER],SLOT(vZoomOut()));
			connect(mVScroll,SIGNAL(scrollValueChanged(int)),mDisplay[MASTER],SLOT(updateVScrollValue(int)));

			connect(mDisplay[MASTER],SIGNAL(vZoomRatio(QString)),mVScroll,SLOT(updateZoomRatio(QString)));
			connect(mDisplay[MASTER],SIGNAL(vScrollValue(int)),mVScroll,SLOT(updateScrollValue(int)));
			connect(mDisplay[MASTER],SIGNAL(vScrollMaxValue(int)),this,SLOT(setMaxVScroll(int)));
			
			// synchronize vertical scrolling
			connect(mDisplay[MASTER],SIGNAL(yRulerRange(double,double)),
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

