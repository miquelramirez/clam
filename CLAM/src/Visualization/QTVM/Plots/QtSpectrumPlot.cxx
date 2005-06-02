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
	    : QtPresentation(parent,name,f)
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
			
	    _magLabel = new SingleLabel(this," dB ","Magnitude");
	    panel->addWidget(_magLabel);

	    _freqLabel = new SingleLabel(this," Hz ","Frequency");
	    panel->addWidget(_freqLabel);

	    QFrame* righthole = new QFrame(this);
	    righthole->setFixedSize(20,_freqLabel->height());
	    panel->addWidget(righthole);

	    AddToMainLayout(panel);
	}

	void QtSpectrumPlot::SetData(const Spectrum& spec)
	{
	    ((SpectrumPlotController*)_controller)->SetData(spec);
	}

	void QtSpectrumPlot::SetForegroundColor(Color c)
	{
	    ((SpectrumPlotController*)_controller)->SetDataColor(c);
	}

	void QtSpectrumPlot::SetVLineColor(Color c)
	{
	    ((SpectrumPlotController*)_controller)->SetDialColor(c);
	}

	void QtSpectrumPlot::updateMagLabel(TData value)
	{
	    _magLabel->Update(value);
	}

	void QtSpectrumPlot::updateFreqLabel(TData value)
	{
	    _freqLabel->Update(value);
	}

	void QtSpectrumPlot::SetPlotController()
	{			
	    SetController(new SpectrumPlotController());
	}

	void QtSpectrumPlot::Connect()
	{
	    // Connections
	    connect(((SpectrumPlotController*)_controller),SIGNAL(mag(TData)),this,SLOT(updateMagLabel(TData)));
	    connect(((SpectrumPlotController*)_controller),SIGNAL(freq(TData)),this,SLOT(updateFreqLabel(TData)));
	}

	void QtSpectrumPlot::DisplayBackgroundBlack()
	{
	    SetBackgroundColor(VMColor::Black());
	    SetForegroundColor(VMColor::Green());
	    SetVLineColor(VMColor::Red());
	    SetMarksColor(VMColor::Orange());
	}

	void QtSpectrumPlot::DisplayBackgroundWhite()
	{
	    SetBackgroundColor(VMColor::White());
	    SetForegroundColor(VMColor::Blue());
	    SetVLineColor(VMColor::Black());
	    SetMarksColor(VMColor::Red());
	}

	void QtSpectrumPlot::SetMarks(std::vector<unsigned>& marks)
	{			
	    ((SpectrumPlotController*)_controller)->SetMarks(marks);
	}

	std::vector<unsigned>& QtSpectrumPlot::GetMarks()
	{
	    return ((SpectrumPlotController*)_controller)->GetMarks();
	}
	
	void QtSpectrumPlot::SetMarksColor(Color c)
	{
	    ((SpectrumPlotController*)_controller)->SetMarksColor(c);
	}

	void QtSpectrumPlot::keyPressEvent(QKeyEvent* e)
	{
	    switch(e->key())
	    {
		case Qt::Key_Insert:
		    ((SpectrumPlotController*)_controller)->SetKeyInsertPressed(true); 
		    break;
						
		case Qt::Key_Delete:
		    ((SpectrumPlotController*)_controller)->SetKeyDeletePressed(true); 
		    break;
				    
		default:
		    break;

	    }
	}

	void QtSpectrumPlot::keyReleaseEvent(QKeyEvent* e)
	{
	    switch(e->key())
	    {
		case Qt::Key_Insert:
		    ((SpectrumPlotController*)_controller)->SetKeyInsertPressed(false); 
		    break;
						
		case Qt::Key_Delete:
		    ((SpectrumPlotController*)_controller)->SetKeyDeletePressed(false); 
		    break;
				    
		default:
		    break;
	    }
	}

	std::vector<QString> QtSpectrumPlot::GetSegmentationTags()
	{
	    return  ((SpectrumPlotController*)_controller)->GetTags();
	}

    }
}

// END


