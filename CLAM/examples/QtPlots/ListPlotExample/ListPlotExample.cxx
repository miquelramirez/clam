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
