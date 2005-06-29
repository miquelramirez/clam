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

#include <stdlib.h>
#include "QtPlotter.hxx"
#include "audio_file_utils.hxx"
#include "analysis_utils.hxx"

using CLAM::TData;
using CLAM::DataArray;
using CLAM::VM::QtPlotter;

int main()
{
	// make functions for multiplot 
	DataArray sig0;
	DataArray sig1;
	sig0.Resize(11025);
	sig1.Resize(11025);
	sig0.SetSize(11025);
	sig1.SetSize(11025);
	double f=20.0;
	double fs=11025.0;
	double t=0.0;
	double x0,x1;
	int j=0;
	while(t <= 1)
	{
		x0=cos(2.0*PI*f*t);
		x1=sin(2.0*PI*f*t);
		sig0[j]=TData(x0);
		sig1[j]=TData(x1);
		t+=1.0/fs;
		j++;
	}

	std::vector<Audio> channels;
	if(qtvm_examples_utils::load_audio_st("../data/imagine.mp3",channels))
	{
	    printf("File Error: \'imagine.mp3\' audio file not found!\n");
	    exit(1);
	}

	CLAM::Audio audio;
	if(qtvm_examples_utils::load_audio("../data/birthday.wav",audio))
	{
	    printf("File Error: \'birthday.wav\' audio file not found!\n");
	    exit(1);
	}

	CLAM::Segment seg;
    
	printf("Analysing audio");
	qtvm_examples_utils::analyze(audio,seg);
	printf("\ndone!\n");
	
	// add plots to plotter
	QtPlotter::Add("audio_plot",audio);
	QtPlotter::Add("fundamental_plot",seg,CLAM::VM::FundamentalPlot);
	QtPlotter::Add("multi_plot","data0",sig0);
	QtPlotter::Add("audiost_plot",channels[0],channels[1]);

	// assign labels to plots 
	QtPlotter::SetLabel("audio_plot","Audio");
	QtPlotter::SetLabel("fundamental_plot","Fundamental");
	QtPlotter::SetLabel("multi_plot","MultiPlot");
	QtPlotter::SetLabel("audiost_plot","Audio Stereo");

	// add data to multiplot
	QtPlotter::AddData("multi_plot","data1",sig1);
	
	// set color to multiplot data
	QtPlotter::SetDataColor("multi_plot","data0",CLAM::VM::VMColor::Cyan());
	QtPlotter::SetDataColor("multi_plot","data1",CLAM::VM::VMColor::Magenta());

	// set units and tooltips to multiplot
	QtPlotter::SetUnits("multi_plot","s","");
	QtPlotter::SetToolTips("multi_plot","Instant Time","Amplitude");

	// set x y range to multiplot
	QtPlotter::SetXRange("multi_plot",0.0,1.0);
	QtPlotter::SetYRange("multi_plot",-1.5,1.5);
	
	// Display plots
	QtPlotter::Run();
	
	return 0;
}

// END

