#include "QtAppWrapper.hxx"
#include "QtSpectrogram.hxx"
#include "wave_utils.hxx"
#include "analysis_utils.hxx"

int main()
{
    CLAM::Audio audio;
    CLAM::Segment segment;

    double freq0 = 15000.0;
    double freq1 = 300.0;
    double duration = 12.0;
    double sampleRate = 8000.0;

    printf("Building a chirp signal freq=%.0lf..%.0lf Hz duration=%.0lf sec. sample rate=%.0lf Hz\n",
	   freq0,freq1,duration,sampleRate);
    qtvm_examples_utils::chirp(freq0,freq1,duration,sampleRate,audio);

    printf("Analysing");
    qtvm_examples_utils::analyze(audio,segment);
    
    CLAM::Array<CLAM::Spectrum> specMtx;
    specMtx.Resize(segment.GetnFrames());
    specMtx.SetSize(segment.GetnFrames());
    printf("\nBuilding spectrum matrix");
    for(CLAM::TIndex i=0; i < segment.GetnFrames(); i++)
    {
	specMtx[i]=segment.GetFrame(i).GetSinusoidalAnalSpectrum();
	printf(".");
    }
    printf("\ndone!\n");

    CLAM::VM::QtAppWrapper::Init();
    CLAM::VM::QtSpectrogram plot;
    plot.Label("Sonogram");
    plot.Geometry(100,100,600,300);
    plot.SetData(specMtx,duration);
    plot.Show();
    return CLAM::VM::QtAppWrapper::Run();

}

// END

