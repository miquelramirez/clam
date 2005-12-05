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
#include "SinTracksPlotController.hxx"
#include "SingleLabel.hxx"
#include "QtSinTracksPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		QtSinTracksPlot::QtSinTracksPlot(QWidget* parent, const char * name, WFlags f) 
			: SingleDisplayPlot(parent,name,f)
		{
			SetPlotController();
			InitSinTracksPlot();
			Connect();
			SwitchDisplayColors(true);
		}

		QtSinTracksPlot::~QtSinTracksPlot()
		{
		}

		void QtSinTracksPlot::SetData(const Segment& segment)
		{
			((SinTracksPlotController*)mController)->SetData(segment);
		}

		void QtSinTracksPlot::SetData(const Array< SpectralPeakArray >& peakMtx, 
									  const double& sr, const double& dur)
		{
			((SinTracksPlotController*)mController)->SetData(peakMtx,sr,dur);
		}

		void QtSinTracksPlot::SetPlotController()
		{			
			SetController(new SinTracksPlotController());		
		}		

		void QtSinTracksPlot::Connect()
		{
			// Connections
			connect(((SinTracksPlotController*)mController),SIGNAL(sendTimeFreq(double,double)),
					this,SLOT(updateLabels(double,double)));
		}

		void QtSinTracksPlot::InitSinTracksPlot()
		{
			QHBoxLayout* panel = new QHBoxLayout;
			panel->addStretch(1);
			
			mTimeLabel = new SingleLabel(this," s ","Time");
			panel->addWidget(mTimeLabel);

			mFreqLabel = new SingleLabel(this," Hz ","Frequency");
			panel->addWidget(mFreqLabel);

			QFrame* righthole = new QFrame(this);
			righthole->setFixedSize(20,mFreqLabel->height());
			panel->addWidget(righthole);

			AddToMainLayout(panel);
		}

		void QtSinTracksPlot::DisplayBackgroundBlack()
		{
			((SinTracksPlotController*)mController)->SaveScreen();
			SetDialColor(VMColor::Red());
			SetMarksColor(VMColor::Orange());
			SetBackgroundColor(VMColor::Black());
		}

		void QtSinTracksPlot::DisplayBackgroundWhite()
		{
			((SinTracksPlotController*)mController)->SaveScreen();
			SetDialColor(VMColor::Black());
			SetMarksColor(VMColor::Red());
			SetBackgroundColor(VMColor::White());
		}

		void QtSinTracksPlot::updateLabels(double time, double freq)
		{
			mTimeLabel->Update(time);
			mFreqLabel->Update(freq,0);
		}

    }
}

// END

