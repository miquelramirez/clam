#include <stdlib.h>
#include "audio_file_utils.hxx"
#include "QtAppWrapper.hxx"
#include "QtAudioPlot.hxx"
#include "ContiguousSegmentation.hxx"

int main()
{
	CLAM::Audio audio;
	if(qtvm_examples_utils::load_audio("../data/birthday.wav",audio))
	{
	    printf("File Error: \'birthday.wav\' audio file not found!\n");
	    exit(1);
	}

	CLAM::Segmentation* seg = new CLAM::ContiguousSegmentation(audio.GetSize());

	unsigned nSegments = 15;
	double step = double(audio.GetSize())/nSegments;
	// make some divisions
	for(unsigned i=1; i < nSegments; i++)
	{
		seg->insert(double(i)*step);
	}
	seg->current(2); // set current segment

	CLAM::VM::QtAppWrapper::Init();

	CLAM::VM::QtAudioPlot plot;
	plot.Label("Audio");
	plot.Geometry(100,100,700,350);
	plot.SetData(audio);
	plot.SetSegmentation(seg);
	plot.SetBackgroundColor(CLAM::VM::VMColor::White());
	plot.SetForegroundColor(CLAM::VM::VMColor::Blue());
	plot.Show();
	
	return CLAM::VM::QtAppWrapper::Run();



}

// END

