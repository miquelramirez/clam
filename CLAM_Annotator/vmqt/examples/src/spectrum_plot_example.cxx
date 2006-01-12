#include <QApplication>
#include "wave_utils.hxx"
#include "analysis_utils.hxx"
#include "vm_spectrum_plot.hxx"

int main(int argc, char** argv)
{
	QApplication app( argc, argv );

	CLAM::Audio wave;
    CLAM::Segment segment;
	
    CLAM::TData frequency = 1000.0;
    CLAM::TData amplitude = 0.7;
    CLAM::TData duration = 0.5;
    CLAM::TData sampleRate = 11025.0;
	
    printf("Buidding sine wave: freq=%.0f Hz amp=%.1f duration=%.1f sec. sample rate=%.0f Hz\n", 
		   frequency, amplitude, duration, sampleRate);
    qtvm_examples_utils::sine(frequency, amplitude, duration, sampleRate, wave);

    printf("Analysing");
    qtvm_examples_utils::analyze(wave,segment);
    printf("\ndone!\n");
	
    int index = segment.GetnFrames()/2;
   
	CLAM::VM::SpectrumPlot plot;
	plot.set_title("Spectrum");
	plot.set_geometry(100,50,600,300);
	plot.set_data(segment.GetFrame(index).GetSinusoidalAnalSpectrum(),segment.GetFrame(index).GetSpectralPeakArray());
	plot.show();

// uncomment the following line to see the example on background black mode
//	plot.backgroundBlack();

	app.connect(&app,SIGNAL(lastWindowClosed()),&app,SLOT(quit()));
	return app.exec();
}

// END



