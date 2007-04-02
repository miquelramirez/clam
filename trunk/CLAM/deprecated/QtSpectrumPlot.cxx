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
#include "SingleLabel.hxx"
#include "SpectrumPlotController.hxx"
#include "QtSpectrumPlot.hxx"

namespace CLAM
{
    namespace VM
    {
		QtSpectrumPlot::QtSpectrumPlot(QWidget* parent, const char * name, WFlags f) 
			: SingleDisplayPlot(parent,name,f)
		{
			SetPlotController();
			InitSpectrumPlot();
			Connect();
		}

		QtSpectrumPlot::~QtSpectrumPlot()
		{
		}

		void QtSpectrumPlot::InitSpectrumPlot()
		{   
			QHBoxLayout* panel = new QHBoxLayout;
			panel->addStretch(1);
			
			mMagLabel = new SingleLabel(this," dB ","Magnitude");
			panel->addWidget(mMagLabel);

			mFreqLabel = new SingleLabel(this," Hz ","Frequency");
			panel->addWidget(mFreqLabel);

			QFrame* righthole = new QFrame(this);
			righthole->setFixedSize(20,mFreqLabel->height());
			panel->addWidget(righthole);

			AddToMainLayout(panel);
		}

		void QtSpectrumPlot::SetData(const Spectrum& spec)
		{
			((SpectrumPlotController*)mController)->SetData(spec);
		}

		void QtSpectrumPlot::SetData(const Spectrum& spec, const SpectralPeakArray& peaks)		
		{			
		    ((SpectrumPlotController*)mController)->SetData(spec,peaks);
		}

		void QtSpectrumPlot::UpdateData(const Spectrum& spec)
		{
			((SpectrumPlotController*)mController)->UpdateData(spec);
		}

		void QtSpectrumPlot::UpdateData(const Spectrum& spec, const SpectralPeakArray& peaks)		
		{			
		    ((SpectrumPlotController*)mController)->UpdateData(spec,peaks);
		}

		void QtSpectrumPlot::SetForegroundColor(Color c)
		{
			((SpectrumPlotController*)mController)->SetDataColor(c);
		}

		void QtSpectrumPlot::SetPeaksColor(Color cline,Color cpoint)
		{
			((SpectrumPlotController*)mController)->SetPeaksColor(cline,cpoint);
		}

		void QtSpectrumPlot::updateLabels(double mag, double freq)
		{
			mMagLabel->Update(mag,0);
			mFreqLabel->Update(freq,0);
		}

		void QtSpectrumPlot::SetPlotController()
		{			
			SetController(new SpectrumPlotController());
		}

		void QtSpectrumPlot::Connect()
		{
			// Connections
			connect(((SpectrumPlotController*)mController),SIGNAL(sendMagFreq(double,double)),
					this,SLOT(updateLabels(double,double)));
		}

		void QtSpectrumPlot::DisplayBackgroundBlack()
		{
			SetForegroundColor(VMColor::Green());
			SetDialColor(VMColor::Red());
			SetMarksColor(VMColor::Orange());
			SetPeaksColor(VMColor::Yellow(),VMColor::Red());
			SetBackgroundColor(VMColor::Black());
		}

		void QtSpectrumPlot::DisplayBackgroundWhite()
		{
			SetForegroundColor(VMColor::Blue());
			SetDialColor(VMColor::Black());
			SetMarksColor(VMColor::Red());
			SetPeaksColor(VMColor::Cyan(),VMColor::Red());
			SetBackgroundColor(VMColor::White());
		}

    }
}

// END


