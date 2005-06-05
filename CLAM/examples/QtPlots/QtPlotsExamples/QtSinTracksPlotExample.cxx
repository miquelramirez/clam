#include <stdlib.h>
#include "QtAppWrapper.hxx"
#include "QtSinTracksPlot.hxx"
#include "audio_file_utils.hxx"
#include "analysis_utils.hxx"

int main()
{
    CLAM::Audio audio;
    CLAM::Segment segment;

    if(qtvm_examples_utils::load_audio("../../data/birthday.wav",audio))
    {
	printf("File Error: \'birthday.wav\' audio file not found!\n");
	exit(1);
    }

    printf("Analysing");
    qtvm_examples_utils::analyze(audio,segment);
    printf("\ndone!\n");
    
    CLAM::VM::QtAppWrapper::Init();
    CLAM::VM::QtSinTracksPlot plot;
    plot.Label("Sinusoidal tracks");
    plot.Geometry(100,100,600,300);
    plot.SwitchDisplayColors(true);
    plot.SetData(segment);
    plot.Show();
    return CLAM::VM::QtAppWrapper::Run();
}

// END

