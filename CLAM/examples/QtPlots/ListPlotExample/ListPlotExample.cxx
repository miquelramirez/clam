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
#include "XMLStorage.hxx"
#include "AudioFileLoader.hxx"
#include "PlotFactory.hxx"
#include "QtPlotter.hxx"

using CLAM::VM::QtAudioPlot;
using CLAM::VM::QtFundFreqPlot;
using CLAM::VM::PlotFactory;
using CLAM::VM::QtPlotter;

int main()
{
	Audio audio;
	AudioFileLoader fLoader;
	
	// get audio data
	int err = fLoader.Load("../data/birthday.wav",audio);
	if(err)
	{
		printf("\'birthday.wav\' audio file not found!\n");
		exit(1);
	}

	CLAM::Segment seg;
	// get fundamental frequency data from XML file
	CLAM::XMLStorage::Restore(seg,"../data/fundfreq_data.xml");
	
	// get plots from the factory
	QtAudioPlot* aPlot = PlotFactory::GetAudioPlot(audio,"Audio",50,50);
	QtFundFreqPlot* fPlot = PlotFactory::GetFundFreqPlot(seg,"Fundamental",50,300); 
	
	// add plots to plotter
	QtPlotter::Add(aPlot);
	QtPlotter::Add(fPlot);
	
	// Display plots
	QtPlotter::ShowAll();

	return 0;
}


// END
