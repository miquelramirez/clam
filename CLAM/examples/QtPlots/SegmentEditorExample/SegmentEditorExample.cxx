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

	CLAM::ContiguousSegmentation* seg = new CLAM::ContiguousSegmentation(audio.GetSize());
	double step = double(audio.GetSize())/4.0;
	// make some divisions
	for(int i=1; i <= 3; i++)
	{
		seg->insert(double(i)*step);
	}
	seg->insert(step*2+1000);
	seg->current(2); // set current segment

	CLAM::VM::QtAppWrapper::Init();

	CLAM::VM::QtAudioPlot plot;
	plot.Label("Audio");
	plot.Geometry(100,100,700,350);
	plot.SetMarksEnabled(false); // disable old segments
	plot.SetData(audio);
	plot.SetSegmentation(seg);
	plot.SetForegroundColor(CLAM::VM::VMColor::Blue());
	plot.Show();
	
	return CLAM::VM::QtAppWrapper::Run();



}

// END

